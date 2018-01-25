/**
 * Amalettomat2 firmware main module
 */

#include <AccelStepper.h>

// pin numbers
// -----------------------------------
#define PIN_STEPPER1_DIR 31
#define PIN_STEPPER1_STEP 32
#define PIN_PLATE_TEMP A0


AccelStepper rozelStepper(AccelStepper::DRIVER, PIN_STEPPER1_STEP, PIN_STEPPER1_DIR);


// process state variables
// -------------------------------------

float plateTemp = 0.0F;
float prevPlateTemp = 0.0F;

String command = NULL;


void setup() {
	Serial.begin(9600);
	while( !Serial ) {};

	Serial.println("# Amalettomat V2");
	Serial.println("# READY.");
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

/**
 * Read commands from serial port
 */
void serialComm() {
	int ch;
	static String buffer;

	while( Serial.available() ) {
		ch = Serial.read();
		if(ch == '\n') {
			// end of line, process command
			buffer.trim();
			command = buffer;
			buffer = "";
		}
		buffer.append((char)ch);
	}
}

void handleCommand() {
	if( !command || command.length() == 0 )
		return; // no command received

	if( command.startsWith("#") ) {
		Serial.print("// ");
		Serial.println(command);
	} else {
		// parse command

	}

	command = "";
}

void loop() {
	serialComm();
	handleCommand();
}
