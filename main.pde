/**
 * Amalettomat2 firmware main module
 */

#include <AccelStepper.h>

// pin numbers
// -----------------------------------
#define PIN_STEPPER1_DIR 28
#define PIN_STEPPER1_STEP 29
#define PIN_STEPPER1_ENABLE 30
#define PIN_PLATE_TEMP A9
#define PIN_PLATE_RELAY 22


AccelStepper rozelStepper(AccelStepper::DRIVER, PIN_STEPPER1_STEP, PIN_STEPPER1_DIR);

// process state variables
// -------------------------------------

float plateTemp = 0.0F;
float prevPlateTemp = 0.0F;
bool heating = false;

// commands received from serial

enum CommandType {
	COMMAND_NONE,
	COMMAND_MOVE_TO_POS,
	COMMAND_MOVE_STEP_FWD,
	COMMAND_MOVE_STEP_BACK,
	COMMAND_HEAT_ON,
	COMMAND_HEAT_OFF
};

#define MAX_PARAMS 5
#define DISPLAY_INTERVAL 500

float R1 = 49100; // series resistor for temp sensor
float c1 = 1.009249522e-03; // coefficents for Steinhard-Hart equation
float c2 = 2.00378405444e-04;

int command = COMMAND_NONE;
String commandParams[MAX_PARAMS];
int numOfParams = 0;
unsigned long displayTime;

// ====================================================================

void writeOutputs();

// ====================================================================

void initRozel() {
	pinMode(PIN_STEPPER1_ENABLE, OUTPUT);
	pinMode(PIN_STEPPER1_DIR, OUTPUT);
	pinMode(PIN_STEPPER1_STEP, OUTPUT);
	rozelStepper.setMaxSpeed(2000.0);
	rozelStepper.setSpeed(1500.0);
	rozelStepper.setAcceleration(2000.0);
	digitalWrite(PIN_STEPPER1_ENABLE, HIGH);
}

// =============================================================================

void setup() {
	initRozel();

	pinMode(PIN_PLATE_RELAY, OUTPUT);
	pinMode(PIN_PLATE_TEMP, INPUT);

	writeOutputs();

	Serial.begin(9600);
	while( !Serial ) {};

	Serial.println("# Amalettomat V2");
	Serial.println("# READY.");

	displayTime = millis() + DISPLAY_INTERVAL;
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
		rozelStepper.move(800);
		break;
	case COMMAND_MOVE_STEP_BACK:
		Serial.println("step BACK");
		rozelStepper.move(-800);
		break;
	case COMMAND_HEAT_ON:
		heating = true;
		break;
	case COMMAND_HEAT_OFF:
		heating = false;
		break;


	}
	command = COMMAND_NONE;
}

void writeOutputs() {
	digitalWrite(PIN_PLATE_RELAY, heating);
}

void readInputs() {
	prevPlateTemp = plateTemp;
	int value = analogRead(PIN_PLATE_TEMP);
	float R = value * R1 / (1024.0 - value);
	float logR = log(R);
	plateTemp = (1.0 / (c1 + c2 * logR)) - 273.15;
}

void writeState() {
	Serial.print("T=");
	Serial.println(plateTemp);
}

// =============================================================================

void loop() {
	readInputs();
	serialComm();
	handleCommand();

	rozelStepper.run();

	if( millis() > displayTime ) {
		writeState();
		displayTime = millis() + DISPLAY_INTERVAL;
	}

	writeOutputs();
}
