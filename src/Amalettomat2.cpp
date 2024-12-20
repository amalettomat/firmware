/**
 * Amalettomat2 firmware main module
 */
#include "Arduino.h"
#include "ScraperControl.h"
#include "Adafruit_ILI9486_Teensy.h"
#include <SPI.h>
#include "Splashscreen.h"
#include "images/icon_choc.bmp.h"
#include "images/icon_jam.bmp.h"
//#include "images/test.bmp.h"
#include "states/StatePreheat.h"
#include "states/StateIdle.h"
#include "layout.h"
#include "config.h"
#include "LedControl.h"
#include "Gui.h"
#include <EEPROM.h>


#define SERIAL_DEBUG false

// #include <SD.h>
// #include "BmpFile.h"

// extern const GFXfont FreeSans18pt7b;


Adafruit_ILI9486_Teensy tftDisplay;
Adafruit_ILI9486_Teensy* g_display = &tftDisplay;

// #define NUM_AVG_TEMP_VALUES 32
// RunningAverage<float> g_plateTempAverage(NUM_AVG_TEMP_VALUES);
float g_plateTemp = 0.0F;

// #define NUM_AVG_PRESSURE_VALUES 200
// RunningAverage<float> g_pressureAverage(NUM_AVG_PRESSURE_VALUES);
float g_pressure = 0.0F;

LedControl g_ledController;

ScraperControl g_scraperControl;


// process state variables
// -------------------------------------

// heating
bool g_heating = false;
bool g_heatingEnabled = true;
float g_setTemp = 160.0;

// pneumatics
bool g_compressor = false;
bool g_fillingValve1 = false;
bool g_fillingValve2 = false;

// touch screen position
int g_touchX = -1;
int g_touchY = -1;
bool g_touchPressed = false;

// L298 PWM outputs
bool g_plateMotor = false;
int g_plateMotorSpeed = 150;

// relay board
bool g_batterValve = false;
bool g_oilerSolenoid = false;

float g_credit = 0;
bool g_showMaint = true;
bool g_maintButton = false;

// processing params
float g_batterAmount = 1.7;
float g_spreadTime = 0.9; // orig: 3.5
float g_bakingTime = 65.0;
float g_amountFilling1 = 1.3;
float g_amountFilling2 = 1.2;
float g_fillingOverrunTime = 2.9; // 2.15
int g_selectedFilling = 0;

// pancake price
float g_price = 0.0F;

// rozel state
bool g_rozelDown = false;
bool g_rozelIsDown = false;
bool g_rozelIsUp = false;

// ===========================
// persistent values in EEPROM

// marker to detect if EEPROM is initialized
uint16_t g_eeprom_marker = 0;

// pancake counter
uint32_t g_counter = 0;


// commands received from serial
enum eCommand {
	COMMAND_NONE,
	COMMAND_MOVE_TO_POS,
	COMMAND_MOVE_STEP_FWD,
	COMMAND_MOVE_STEP_BACK,
	COMMAND_HEAT_ON,
	COMMAND_HEAT_OFF,
	COMMAND_PLATE_SPEED_UP,
	COMMAND_PLATE_SPEED_DOWN,
	COMMAND_TOGGLE_BATTER_VALVE,
	COMMAND_TOGGLE_OILER_SOLENOID,
	COMMAND_ROZEL_UP,
	COMMAND_ROZEL_DOWN
};

#define MAX_PARAMS 5

// screens from SD card
// #define SCREEN_START "/IMG/START.BMP"


float R1 = 9370; // 49100; // series resistor for temp sensor
float c1 = 0.96e-3; // 1.009249522e-03; // coefficents for Steinhard-Hart equation
float c2 = 2.00378405444e-04;

int command = COMMAND_NONE;
String commandParams[MAX_PARAMS];
int numOfParams = 0;
unsigned long displayTime;
unsigned long ledUpdateTime;


// ====================================================================

void writeOutputs();

// ====================================================================

void initScreen() {
	pinMode(PIN_BACKLIGHT, OUTPUT);
	analogWrite(PIN_BACKLIGHT, 70);

	pinMode(TP_CS, OUTPUT);
	digitalWrite(TP_CS, HIGH);

	tftDisplay.begin();
	delay(100);

	tftDisplay.setRotation(1);

	// show splash screen
	//g_display->drawRGBBitmap(0, 0, (uint16_t*)splash_image.pixel_data, 480, 320);
//	tftDisplay.setAddrWindow(0,0, 480, 320);
//	for(int i=0;i<320*480;i++){
//		tftDisplay.pushColor( makeWord(splash_image.pixel_data[2*i], splash_image.pixel_data[2*i+1]) );
//	}
	// tftDisplay.setFont(&FreeSans18pt7b);
	tftDisplay.setFont(DEFAULT_FONT);

	tftDisplay.drawRGBBitmap_fast(0, 0, (const uint8_t*)splash_image.pixel_data, 480, 320);

	delay(2000);
}

void readPlateTemp() {
	static int count = 0;
	count++;
	if( count < 200 )
		return;
	count = 0;

	int plateTempRaw;
	float tempSensorResistance;
	float logR;

	plateTempRaw = analogRead(PIN_PLATE_TEMP);
	tempSensorResistance = plateTempRaw * R1 / (1024.0 - plateTempRaw);
	logR = log(tempSensorResistance);
	float temp = (1.0 / (c1 + c2 * logR)) - 273.15;
	// g_plateTempAverage.addValue();
	g_plateTemp = g_plateTemp * 0.99 + temp * 0.01;
}

void readPressure() {
	float pressure;
	static int count = 0;

	// only read every 1000th run
	count++;
	if( count < 1000 ) {
		return;
	}
	count = 0;

	// g_pressureReadings++;

	pressure = analogRead(PIN_PRESSURE_SENSOR) * PRESSURE_SENSOR_SCALE_FACTOR;
	pressure = (pressure - PRESSURE_SENSOR_OFFSET_VOLTAGE) * PRESSURE_SENSOR_BAR_PER_VOLT;

	if( pressure < 0.0 )
		pressure = 0.0F;

	// g_pressureAverage.addValue(pressure);
	g_pressure = g_pressure * 0.99 + pressure * 0.01;
}

void initEEPROM() {
	EEPROM.get(EEPROM_IDX_MARKER, g_eeprom_marker);
	if( g_eeprom_marker != EEPROM_MARKER_VALUE ) {
		// initialize EEPROM
		g_eeprom_marker = EEPROM_MARKER_VALUE;
		EEPROM.put(EEPROM_IDX_MARKER, g_eeprom_marker);

		g_counter = 0;
		EEPROM.put(EEPROM_IDX_COUNTER, g_counter);

		EEPROM.put(EEPROM_IDX_PRICE, g_price);
		EEPROM.put(EEPROM_IDX_BAKING_TIME, g_bakingTime);
		EEPROM.put(EEPROM_IDX_BATTER_AMOUNT, g_batterAmount);
		EEPROM.put(EEPROM_IDX_FILLING1, g_amountFilling1);
		EEPROM.put(EEPROM_IDX_FILLING2, g_amountFilling2);

		return;
	} 

	EEPROM.get(EEPROM_IDX_COUNTER, g_counter);
	EEPROM.get(EEPROM_IDX_PRICE, g_price);
	if( isnan(g_price) )
		g_price = 0.0;
	EEPROM.get(EEPROM_IDX_BAKING_TIME, g_bakingTime);
	if( isnan(g_bakingTime) )
		g_bakingTime = 65.0;
	EEPROM.get(EEPROM_IDX_BATTER_AMOUNT, g_batterAmount);
	if( isnan(g_batterAmount) )
		g_batterAmount = 1.7;
	EEPROM.get(EEPROM_IDX_FILLING1, g_amountFilling1);
	if( isnan(g_amountFilling1) )
		g_amountFilling1 = 0.0;
	EEPROM.get(EEPROM_IDX_FILLING2, g_amountFilling2);
	if( isnan(g_amountFilling2) )
		g_amountFilling2 = 0.0;
}

// =============================================================================

void setup() {
	SPI.begin();

	if( SERIAL_DEBUG )
		Serial.begin(9600);

	// init I2C
	// Wire.begin();

	pinMode(PIN_PLATE_RELAY, OUTPUT);
	pinMode(PIN_PLATE_TEMP, INPUT);
	pinMode(PIN_PRESSURE_SENSOR, INPUT);
	pinMode(PIN_COMPRESSOR, OUTPUT);
	pinMode(PIN_COMPRESSOR2, OUTPUT);
	pinMode(PIN_FILLING_VALVE1, OUTPUT);
	pinMode(PIN_FILLING_VALVE2, OUTPUT);
	pinMode(PIN_PLATE_MOTOR, OUTPUT);
	pinMode(PIN_ROZEL_UP, OUTPUT);
	pinMode(PIN_ROZEL_DOWN, OUTPUT);
	pinMode(PIN_ROZEL_ENDSTOP_UP, INPUT_PULLDOWN);
	pinMode(PIN_ROZEL_ENDSTOP_DOWN, INPUT_PULLDOWN);
	pinMode(PIN_BATTER_VALVE, OUTPUT);
	pinMode(PIN_OILER_SOLENOID, OUTPUT);
	pinMode(PIN_RELAY4, OUTPUT);
	pinMode(PIN_COIN_SIGNAL, INPUT);
	pinMode(PIN_BUTTON_MAINT, INPUT_PULLUP);

	// turn off batter valve
	digitalWrite(PIN_BATTER_VALVE, LOW);

	initScreen();

	g_scraperControl.moveBack();

	g_ledController.init();

	initEEPROM();

	writeOutputs();

	for( int index=0; index < 200; index++ )
		readPlateTemp();

	for( int index=0; index < 200; index++ )
		readPressure();

	displayTime = millis() + DISPLAY_INTERVAL;

	if( SERIAL_DEBUG ) {
		Serial.println("# Amalettomat V2");
		Serial.println("# READY.");
	}

	// clear display area
	g_display->fillScreen(COL_BACKGROUND);
	g_display->drawFastHLine(0, STATUS_AREA_YPOS, 480, COL_LINES);

	AbstractState::switchState(&STATE_PREHEAT);
}

void increaseCounter() {
	g_counter++;
	EEPROM.put(EEPROM_IDX_COUNTER, g_counter);
}

void readTouchPos() {
	int x, y, z;
	static int count = 0;
	count++;
	if( count < 50 )
		return;
	count = 0;

	SPI.beginTransaction(TP_SETTING);
	digitalWrite(TP_CS, LOW);
	SPI.transfer(0xB1);

	z = 32780+SPI.transfer16(0xC1)-SPI.transfer16(0x91);
	if (z >= 8000) {
		SPI.transfer16(0x91);
		x = SPI.transfer16(0xD1);
		y = SPI.transfer16(0x91);
		x+= SPI.transfer16(0xD1);
		y+= SPI.transfer16(0x91);
		// g_touchX = (x-4700)*480/60000;    // inverted x-axis
		g_touchX = (64000-x)*480/60000;
		g_touchY = (y-5500)*320/55000;
		g_touchPressed = true;
	} else {
		g_touchPressed = false;
	}
	digitalWrite(TP_CS, HIGH);
	SPI.endTransaction();

	if(g_touchPressed){
		if( SERIAL_DEBUG )
			Serial.printf("% 5i % 8i % 8i\n",z, g_touchX, g_touchY);

//		g_display->drawFastHLine(g_touchX-20, g_touchY, 40, COL_LINES);
//		g_display->drawFastVLine(g_touchX, g_touchY-20, 40, COL_LINES);
	}
	// touch indicator
	g_display->fillRect(465, 305, 10, 10, g_touchPressed ? COL_BUTTON_OUTLINE : COL_BACKGROUND);
}

//void setup()
//{
//	pinMode(PIN_STEPPER1_DIR, OUTPUT);
//	pinMode(PIN_STEPPER1_STEP, OUTPUT);
//	rozelStepper.setMaxSpeed(2000.0);
//	rozelStepper.setSpeed(1500.0);
//	rozelStepper.setAcceleration(2000.0);
//	rozelStepper.moveTo(2000);
//}

//void loop()
//{
//    // Change direction at the limits
//    if (rozelStepper.distanceToGo() == 0)
//    	rozelStepper.moveTo(-rozelStepper.currentPosition());
//    rozelStepper.run();
//}

void parseCommand(String &cmdLine) {
	String cmd;
	numOfParams = 0;
	unsigned int prev;
	int pos;
	int paramIndex = 0;

	// tokenize cmd line
	pos = cmdLine.indexOf(' ');
	if( pos < 0 ) {
		cmd = cmdLine;
		numOfParams = 0;
	} else {
		cmd = cmdLine.substring(0, pos);
		// parse parameters
		prev = pos+1;
		pos = cmdLine.indexOf(' ', prev);
		while( paramIndex < MAX_PARAMS && prev < cmdLine.length() ) {
			if( pos < 0 )
				pos = cmdLine.length();
			commandParams[paramIndex] = cmdLine.substring(prev, pos);
			paramIndex++;
			prev = pos+1;
			pos = cmdLine.indexOf(' ', prev);
		}
		numOfParams = paramIndex;
	}

//	Serial.printf("CMD='%s', #params=%d, params='%s','%s','%s'\n", cmd.c_str(), numOfParams,
//			commandParams[0].c_str(), commandParams[1].c_str(), commandParams[2].c_str());
	cmd.toLowerCase();
	if( cmd.length() == 0 )
		command = COMMAND_NONE;
	else if( cmd.equals("move") )
		command = COMMAND_MOVE_TO_POS;
	else if( cmd.equals("f") )
		command = COMMAND_MOVE_STEP_FWD;
	else if( cmd.equals("b") )
		command = COMMAND_MOVE_STEP_BACK;
	else if( cmd.equals("h+") )
		command = COMMAND_HEAT_ON;
	else if( cmd.equals("h-") )
		command = COMMAND_HEAT_OFF;
	else if( cmd.equals("p+") )
		command = COMMAND_PLATE_SPEED_UP;
	else if( cmd.equals("p-") )
		command = COMMAND_PLATE_SPEED_DOWN;
	else if( cmd.equals("o") )
		command = COMMAND_TOGGLE_OILER_SOLENOID;
	else if( cmd.equals("bv") )
		command = COMMAND_TOGGLE_BATTER_VALVE;
	else if( cmd.equals("ru") )
		command = COMMAND_ROZEL_UP;
	else if( cmd.equals("rd") )
		command = COMMAND_ROZEL_DOWN;
	else {
		Serial.println("# unknown command!");
		command = COMMAND_NONE;
	}
}

/**
 * Read commands from serial port
 */
void serialComm() {
	int ch;
	static String buffer;

	while( Serial.available() ) {
		ch = Serial.read();
		if(ch == '\n') {
			// end of line, parse command line
			buffer.trim();
			parseCommand(buffer);
			buffer = "";
		}
		buffer.append((char)ch);
	}
}

void handleCommand() {
	switch( command ) {
	case COMMAND_MOVE_STEP_FWD:
		Serial.println("step FWD");
//		digitalWrite(PIN_STEPPER_ROZEL_ENABLE, LOW);
//		g_rozelStepper.move(100);
		break;
	case COMMAND_MOVE_STEP_BACK:
		Serial.println("step BACK");
//		digitalWrite(PIN_STEPPER_ROZEL_ENABLE, LOW);
//		g_rozelStepper.move(-100);
		break;
	case COMMAND_HEAT_ON:
		Serial.println("g_heating on");
		g_heating = true;
		break;
	case COMMAND_HEAT_OFF:
		Serial.println("g_heating off");
		g_heating = false;
		break;
	case COMMAND_PLATE_SPEED_UP:
		g_plateMotorSpeed += 10;
		if( g_plateMotorSpeed > 255 )
			g_plateMotorSpeed = 255;
		break;
	case COMMAND_PLATE_SPEED_DOWN:
		g_plateMotorSpeed -= 10;
		if( g_plateMotorSpeed < 0 )
			g_plateMotorSpeed = 0;
		break;
	case COMMAND_TOGGLE_BATTER_VALVE:
		g_batterValve = !g_batterValve;
		break;
	case COMMAND_TOGGLE_OILER_SOLENOID:
		g_oilerSolenoid = !g_oilerSolenoid;
		break;
	case COMMAND_ROZEL_UP:
		g_rozelDown = false;
		break;
	case COMMAND_ROZEL_DOWN:
		g_rozelDown = true;
		break;

	}
	command = COMMAND_NONE;
}

void writeOutputs() {
	digitalWrite(PIN_PLATE_RELAY, g_heatingEnabled && g_heating);
	digitalWrite(PIN_COMPRESSOR, g_compressor);
	digitalWrite(PIN_FILLING_VALVE1, g_fillingValve1);
	digitalWrite(PIN_FILLING_VALVE2, g_fillingValve2);

	if( g_plateMotor )
		analogWrite(PIN_PLATE_MOTOR, g_plateMotorSpeed);
	else
		analogWrite(PIN_PLATE_MOTOR, 0);

	// MOSFET boards
	digitalWrite(PIN_BATTER_VALVE, g_batterValve);     // J6-1
	digitalWrite(PIN_OILER_SOLENOID, g_oilerSolenoid); // J6-2
	digitalWrite(PIN_COMPRESSOR2, g_compressor);       // J6-3
}

void writeState() {
//	Serial.print("Raw=");
//	Serial.println(plateTempRaw);
//	Serial.print(", T=");
//	Serial.print(g_plateTemp);
//	Serial.print(", R=");
//	Serial.print(tempSensorResistance);
//	Serial.print(", Tavg=");
//	Serial.print(g_plateTemp);
//	Serial.print(", heat=");
//	Serial.println(heating);
}

//void displayState() {
//	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
//	g_display->setTextSize(3);
//
//	// show heating indicator
//	uint16_t color = g_heating ? COLOR565(255,64,64) : COLOR565(64,255,64);
//	g_display->fillCircle(15, 300, 10, color);
//
//	// write temp
//	g_display->setCursor(60, 288);
//	g_display->printf("%3.0f \xF7" "C",g_plateTempAverage.getAverage()); // degree character in code page 437 (minus 1 offset for TFT lib correction)
//
//	// write pressure
//	float pressure = g_pressureAverage.getAverage();
//	if( pressure < 0.0 )
//		pressure = 0.0F;
//	g_display->setCursor(220, 288);
//	g_display->printf("%1.2f bar  %3d ", pressure, g_pressureReadings);
//
//	g_pressureReadings = 0;
//
//	// write state
//	g_display->setCursor(5, 260);
//	g_display->setTextSize(2);
//	g_display->print(AbstractState::getCurrentState()->toString());
//}

void displayMessage(const char* msg) {
	if( SERIAL_DEBUG )
		Serial.println(msg);

	g_display->setCursor(120, 280);
	g_display->setTextColor(WHITE, BLACK);
	g_display->setTextSize(2);
	g_display->println(msg);
}

//void showScreen(const char* path) {
//	BmpFile file;
//	if( !file.open(path) ) {
//		displayMessage("ERROR: open BMP file!");
//		return;
//	}
//
//	uint16_t* imgLine = file.readLine();
//	int y=319;
//	while( imgLine && y >= 270 ) {
//		tft.drawRGBBitmap(0, y, imgLine, 480, BmpFile::NUM_LINES_IN_BUFFER);
//		y -= BmpFile::NUM_LINES_IN_BUFFER;
//		imgLine = file.readLine();
//	}
//}

void tempControl() {
	float diff = g_setTemp - g_plateTemp;

	if( diff < 0 ) {
		g_heating = false;
	} else if( diff > 1 ) {
		g_heating = true;
	}
}

void pressureControl() {
	if( g_pressure > MAX_PRESSURE )
		g_compressor = false;
	if( g_pressure < MIN_PRESSURE )
		g_compressor = true;
}

//void rozelControl() {
//	long dist = g_rozelStepper.distanceToGo();
//
//	if( dist < 0 && !digitalRead(PIN_STEPPER_ROZEL_ENDSTOP) ){
//		g_rozelStepper.stop();
//		Serial.printf("rozel endstop, pos: %ld\n", g_rozelStepper.currentPosition());
//		g_rozelStepper.setCurrentPosition(0);
//	}
//}

void coinControl() {
	static int signalCount = 0;
	static bool prevState = false;
	static long lastSignal = 0;

	if( lastSignal != 0 && millis() - lastSignal > 250 ) {
		lastSignal = 0;
		switch( signalCount ) {
			case 1: g_credit += 0.1; break;
			case 2: g_credit += 0.2; break;
			case 3: g_credit += 0.5; break;
			case 4: g_credit += 1; break;
			case 5: g_credit += 2; break;
		}
		signalCount = 0;
	}

	if( digitalRead(PIN_COIN_SIGNAL) ) {
		// NO signal
		if( prevState ) { // transition from high to low
			lastSignal = millis();
		}
		prevState = false;
	} else {
		// transition from low to high
		if( !prevState ) {
			signalCount++;

		}
		prevState = true;
	}
}

void rozelControl() {
	g_rozelIsUp = !digitalRead(PIN_ROZEL_ENDSTOP_UP);
	g_rozelIsDown = !digitalRead(PIN_ROZEL_ENDSTOP_DOWN);

	if( g_rozelDown ) {
		analogWrite(PIN_ROZEL_UP, 0);

		if( g_rozelIsDown ) {
			// endstop hit
			analogWrite(PIN_ROZEL_DOWN, 0);
		} else {
			// endstop not hit
			analogWrite(PIN_ROZEL_DOWN, ROZEL_SPEED_DOWN);
		}
	} else {
		analogWrite(PIN_ROZEL_DOWN, 0);

		if( g_rozelIsUp ) {
			// endstop hit
			analogWrite(PIN_ROZEL_UP, 0);
		} else {
			// endstop not hit
			analogWrite(PIN_ROZEL_UP, ROZEL_SPEED_UP);
		}
	}
}

// =============================================================================

void loop() {
	readPlateTemp();
	readPressure();
	tempControl();
	pressureControl();
	coinControl();
	rozelControl();

	g_maintButton = !digitalRead(PIN_BUTTON_MAINT);

	g_scraperControl.run();

	if( SERIAL_DEBUG )
		serialComm();

	handleCommand();

	// g_rozelStepper.run();

	readTouchPos();

	// state machine: calls action method of current state
	AbstractState::handleState();

	if( millis() > displayTime ) {
		writeState();
		// displayState();
		AbstractState::refreshStateDisplay();
		displayTime = millis() + DISPLAY_INTERVAL;
	}

	if( millis() > ledUpdateTime ) {
		ledUpdateTime = millis() + LEDS_INTERVAL;
		g_ledController.process();
	}

	writeOutputs();
}
