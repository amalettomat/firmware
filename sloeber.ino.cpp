#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2020-03-09 11:17:45

#include "Arduino.h"
#include <AccelStepper.h>
#include "RunningAverage.h"
#include "Adafruit_ILI9486_Teensy.h"
#include <SPI.h>
#include "Splashscreen.h"
#include "states/StatePreheat.h"
#include "states/StateIdle.h"
#include "layout.h"
#include "config.h"

void initRozel() ;
void initScraper() ;
void initScreen() ;
void setup() ;
void readTouchPos() ;
void parseCommand(String &cmdLine) ;
void serialComm() ;
void handleCommand() ;
void writeOutputs() ;
void readPlateTemp() ;
void readPressure() ;
void writeState() ;
void displayMessage(const char* msg) ;
void tempControl() ;
void pressureControl() ;
void rozelControl() ;
void coinControl() ;
void loop() ;


#include "main.pde"

#endif
