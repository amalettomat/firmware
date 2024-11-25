/*
 * StateMaintIdle.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateMaintIdle.h"
#include "StateIdle.h"
#include "../Gui.h"
#include <AccelStepper.h>
#include "../config.h"
#include "../ScraperControl.h"
#include <EEPROM.h>


extern bool g_batterValve;
extern bool g_plateMotor;
extern int g_plateMotorSpeed;
extern bool g_heatingEnabled;
extern float g_setTemp;
extern float g_batterAmount;
extern float g_bakingTime;
// extern float g_spreadTime;
extern float g_price;
extern float g_amountFilling1;
extern float g_amountFilling2;
extern bool g_fillingValve1;
extern bool g_fillingValve2;
extern ScraperControl g_scraperControl;
extern bool g_rozelDown;


StateMaintIdle STATE_MAINTENANCE_IDLE;


StateMaintIdle::StateMaintIdle() : m_rozelDown(false), m_batterStartTime(0), m_maintStartTime(0), m_fillStartTime(0) {
}

StateMaintIdle::~StateMaintIdle() {
}

#define X_POS_COL1   0
#define X_POS_COL2  82
#define X_POS_COL3 248
#define X_POS_COL4 328
#define Y_POS_LINE1   2
#define Y_POS_LINE2  58
#define Y_POS_LINE3 114
#define Y_POS_LINE4 170
#define Y_POS_LINE5 226


void StateMaintIdle::transition(AbstractState* prevState) {
	clearMainArea();

	// rozel button
	m_rozelDown = false;

	// ========== Left Column =================

	g_btnRozel.initButtonUL(g_display,
			                X_POS_COL1, Y_POS_LINE1,                            // x-pos, y-pos
							MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
							COL_BUTTON_OUTLINE,	COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
							"ROZEL", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnRozel.drawButton(false);

	// rozel pos
//	g_spinRozelPos.init(g_display,
  //			            X_POS_COL2, 2, // x-pos, y-pos
 //			            16, 1, 0, 25,     // value, step, min, max
//			            "%2.0f");         // format
//	g_spinRozelPos.draw();

	// plate motor on/off
	g_btnPlateMotor.initButtonUL(g_display,
			                     X_POS_COL1, Y_POS_LINE2,                           // x-pos, y-pos
			                     MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
								 "PLATE", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnPlateMotor.drawButton(g_plateMotor);

	// plate speed
	g_spinPlateSpeed.init(g_display,
  			              X_POS_COL2, Y_POS_LINE2, // x-pos, y-po s
						  g_plateMotorSpeed, 5, 50, 255,  // value, step, min, max
						  "%3.0f");          // format
	g_spinPlateSpeed.draw();

	// heating
	g_btnHeating.initButtonUL(g_display,        // x-pos, y-pos
			                  X_POS_COL1, Y_POS_LINE3,
							  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
							  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
							  "HEAT", TEXTSIZE_BUTTON);
	g_btnHeating.drawButton(g_heatingEnabled);

	// plate temperature
	g_spinTemp.init(g_display,
  			        X_POS_COL2, Y_POS_LINE3, // x-pos, y-po s
					g_setTemp, 2, 120, 200,   // value, step, min, max
					"%3.0f");
	g_spinTemp.draw();

	// batter dose button
	g_btnBatterDose.initButtonUL(g_display,
			                     X_POS_COL1, Y_POS_LINE4,
								 MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								 "DOSE", TEXTSIZE_BUTTON);
	g_btnBatterDose.drawButton(false);

	// batter amount
	g_spinBatterAmount.init(g_display,
  			                X_POS_COL2, Y_POS_LINE4 ,
							g_batterAmount, 0.1, 1.0, 3.0, // value, step, min, max
							"%3.1f");
	g_spinBatterAmount.draw();

	// batter button
	g_batterValve = false;
	g_btnBatterOnOff.initButtonUL(g_display,
			                      X_POS_COL1, Y_POS_LINE5,
								  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								  "BATTER", TEXTSIZE_BUTTON);
	g_btnBatterOnOff.drawButton(false);

	// ========== Right Column =================

	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(1);
	g_display->setCursor(X_POS_COL3, 32);
	g_display->print("BAKE");
	g_display->setCursor(X_POS_COL3, 88);
	g_display->print("PRICE");

	// baking time
	g_spinBakingTime.init(g_display,
			              X_POS_COL4, Y_POS_LINE1,
						  g_bakingTime  , 1, 40.0, 99.0,
						  "%2.0f" );
	g_spinBakingTime.draw();

	// spreading time
	g_spinPrice.init(g_display,
			              X_POS_COL4, Y_POS_LINE2,
						  g_price, 0.1  , 0.0, 10.0,
						  "%1.2f" );
	g_spinPrice.draw();

	// filling 1 button
	g_btnFilling1.initButtonUL(g_display,
			                   X_POS_COL3, Y_POS_LINE3,
							   MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
							   COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
							   "FILL 1", TEXTSIZE_BUTTON);
	g_btnFilling1.drawButton(false);

	// filling time 1
	g_spinFillingTime1.init(g_display,
			              X_POS_COL4, Y_POS_LINE3,
						  g_amountFilling1, 0.1, 0.0, 8.0,
						  "%1.2f" );
	g_spinFillingTime1.draw();

	// filling 2 button
	g_btnFilling2.initButtonUL(g_display,
			                   X_POS_COL3, Y_POS_LINE4,
							   MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
							   COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
							   "FILL 2", TEXTSIZE_BUTTON);
	g_btnFilling2.drawButton(false);

	// filling time 2
	g_spinFillingTime2.init(g_display,
			              X_POS_COL4, Y_POS_LINE4,
						  g_amountFilling2, 0.02, 0.0, 3.0,
						  "%1.2f" );
	g_spinFillingTime2.draw();

	// scrape button
	g_btnScrape.initButtonUL(g_display,
			               X_POS_COL3, Y_POS_LINE5,
						   MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
						   COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
						   "SCRAPE", TEXTSIZE_BUTTON);
	g_btnScrape.drawButton(false);

	// exit
	g_btnExit.initButtonUL(g_display,
			               400, Y_POS_LINE5,
						   70, BUTTON_DEFAULT_HEIGHT,
						   COL_BUTTON_OUTLINE, COL_BUTTON_EXIT, COL_BUTTON_TEXT,
						   "EXIT", TEXTSIZE_BUTTON);
	g_btnExit.drawButton(false);
}

void StateMaintIdle::action() {
//	if( g_spinRozelPos.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
//		if( m_rozelDown ) {
//			// if rozel pos changed and rozel is down, set new position
//			// g_rozelStepper.moveTo(ROZEL_ENDPOS - 5 * g_spinRozelPos.getValue());
//		}
//	}

	if( g_touchPressed ) {
		g_btnRozel.press(g_btnRozel.contains(g_touchX, g_touchY));
		g_btnBatterDose.press(g_btnBatterDose.contains(g_touchX, g_touchY));
		g_btnBatterOnOff.press(g_btnBatterOnOff.contains(g_touchX, g_touchY));
		g_btnPlateMotor.press(g_btnPlateMotor.contains(g_touchX, g_touchY));
		g_btnHeating.press(g_btnHeating.contains(g_touchX, g_touchY));
		g_btnExit.press(g_btnExit.contains(g_touchX, g_touchY));
		g_btnFilling1.press(g_btnFilling1.contains(g_touchX, g_touchY));
		g_btnFilling2.press(g_btnFilling2.contains(g_touchX, g_touchY));
		g_btnScrape.press(g_btnScrape.contains(g_touchX, g_touchY));
	} else {
		g_btnRozel.press(false);
		g_btnBatterDose.press(false);
		g_btnBatterOnOff.press(false);
		g_btnPlateMotor.press(false);
		g_btnHeating.press(false);
		g_btnExit.press(false);
		g_btnFilling1.press(false);
		g_btnFilling2.press(false);
		g_btnScrape.press(false);
	}

	if( g_btnRozel.justReleased() ) {
		g_rozelDown = !g_rozelDown;
		if( g_rozelDown ) {
			g_btnRozel.drawButton(true);
		} else {
			g_btnRozel.drawButton(false);
		}
	}

	if( g_btnBatterOnOff.justPressed() )
		g_batterValve = true;
	else if( g_btnBatterOnOff.justReleased() )
		g_batterValve = false;

	if( g_spinBatterAmount.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_batterAmount = g_spinBatterAmount.getValue();
		EEPROM.put(EEPROM_IDX_BATTER_AMOUNT, g_batterAmount);
	}

	if( g_btnBatterDose.justReleased() && m_batterStartTime == 0 ) {
		// start timer
		m_batterStartTime = millis();
		g_batterValve = true;
	}

	if( m_batterStartTime > 0 && millis() - m_batterStartTime >= g_spinBatterAmount.getValue() * 1000 ) {
		m_batterStartTime = 0;
		g_batterValve = false;
	}

	if( g_btnPlateMotor.justReleased() ) {
		g_plateMotor = !g_plateMotor;
		g_btnPlateMotor.drawButton(g_plateMotor);
	}

	if( g_spinPlateSpeed.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_plateMotorSpeed = g_spinPlateSpeed.getValue();
	}

	if( g_btnHeating.justReleased() ) {
		g_heatingEnabled = !g_heatingEnabled;
		g_btnHeating.drawButton(g_heatingEnabled);
	}

	if( g_spinTemp.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_setTemp = g_spinTemp.getValue();
	}

	if( g_spinBakingTime.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_bakingTime = g_spinBakingTime.getValue();
		EEPROM.put(EEPROM_IDX_BAKING_TIME, g_bakingTime);
	}

	if( g_spinPrice.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_price = g_spinPrice.getValue();
		EEPROM.put(EEPROM_IDX_PRICE, g_price);
	}

	if( g_spinFillingTime1.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_amountFilling1 = g_spinFillingTime1.getValue();
		EEPROM.put(EEPROM_IDX_FILLING1, g_amountFilling1);
	}

	if( g_btnFilling1.justPressed() && m_fillStartTime == 0 ) {
		// start timer for filling
		m_fillStartTime = millis();
		g_fillingValve1 = true;
	}

	if( g_spinFillingTime2.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_amountFilling2 = g_spinFillingTime2.getValue();
		EEPROM.put(EEPROM_IDX_FILLING2, g_amountFilling2);
	}

	if( g_btnFilling2.justPressed() && m_fillStartTime == 0 ) {
		// start timer for filling
		m_fillStartTime = millis();
		g_fillingValve2 = true;
	}

	if( m_fillStartTime > 0 ) {
		if( g_fillingValve1 && millis() - m_fillStartTime >= g_spinFillingTime1.getValue() * 1000 ) {
			m_fillStartTime = 0;
			g_fillingValve1 = false;
		} 
		if( g_fillingValve2 && millis() - m_fillStartTime >= g_spinFillingTime2.getValue() * 1000 ) {
			m_fillStartTime = 0;
			g_fillingValve2 = false;
		} 
	}


	if( g_btnScrape.justReleased() )
		g_scraperControl.startScrape();

	if( g_btnExit.justReleased() )
		switchState(&STATE_IDLE);
}

void StateMaintIdle::refreshDisplay() {
	refreshStatusBar();

//	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
//	g_display->setTextSize(3);

//	bool result = g_rozelController.updateStatus();
//	ControllerData& controllerData = g_rozelController.getStatus();
//
//	g_display->setCursor(70, 260);
//	g_display->printf("pos: %3.2f speed: %3.2f, %d   ", controllerData.position, controllerData.speed, controllerData.m_controllerStatus);

}
