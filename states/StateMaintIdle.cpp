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
#include "../MotorControllerClient.h"

extern MotorControllerClient g_rozelController;
extern bool g_batterValve;
extern bool g_plateMotor;
extern int g_plateMotorSpeed;
extern bool g_heatingEnabled;
extern float g_setTemp;
extern float g_batterAmount;
extern float g_bakingTime;
extern float g_spreadTime;


StateMaintIdle STATE_MAINTENANCE_IDLE;


StateMaintIdle::StateMaintIdle() : m_rozelDown(false), m_batterStartTime(0) {
}

StateMaintIdle::~StateMaintIdle() {
}

#define COL_POS_ROW1 2
#define COL_POS_ROW2 82
#define COL_POS_ROW3 250
#define COL_POS_ROW4 328


void StateMaintIdle::transition(AbstractState* prevState) {
	clearMainArea();

	// rozel button
	m_rozelDown = false;

	g_btnRozel.initButtonUL(g_display,
			                COL_POS_ROW1, 2,                            // x-pos, y-pos
							MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
							COL_BUTTON_OUTLINE,	COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
							"ROZEL", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnRozel.drawButton(false);

	// rozel pos
//	g_spinRozelPos.init(g_display,
//			            COL_POS_ROW2, 2, // x-pos, y-pos
//			            16, 1, 0, 25,     // value, step, min, max
//			            "%2.0f");         // format
//	g_spinRozelPos.draw();

	// plate motor on/off
	g_btnPlateMotor.initButtonUL(g_display,
			                     COL_POS_ROW1, 58,                           // x-pos, y-pos
			                     MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
								 "PLATE", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnPlateMotor.drawButton(g_plateMotor);

	// plate speed
	g_spinPlateSpeed.init(g_display,
			              COL_POS_ROW2, 58, // x-pos, y-pos
						  g_plateMotorSpeed, 5, 150, 255,  // value, step, min, max
						  "%3.0f");          // format
	g_spinPlateSpeed.draw();

	// heating
	g_btnHeating.initButtonUL(g_display,        // x-pos, y-pos
			                  COL_POS_ROW1, 114,
							  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
							  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
							  "HEAT", TEXTSIZE_BUTTON);
	g_btnHeating.drawButton(g_heatingEnabled);

	// plate temperature
	g_spinTemp.init(g_display,
			        COL_POS_ROW2, 114, // x-pos, y-pos
					g_setTemp, 2, 120, 200,   // value, step, min, max
					"%3.0f");
	g_spinTemp.draw();

	// batter dose button
	g_btnBatterDose.initButtonUL(g_display,
			                     COL_POS_ROW1, 170,
								 MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								 "DOSE", TEXTSIZE_BUTTON);
	g_btnBatterDose.drawButton(false);

	// batter amount
	g_spinBatterAmount.init(g_display,
			                COL_POS_ROW2, 170,
							g_batterAmount, 0.1, 1.0, 3.0, // value, step, min, max
							"%3.1f");
	g_spinBatterAmount.draw();

	// batter button
	g_batterValve = false;
	g_btnBatterOnOff.initButtonUL(g_display,
			                      COL_POS_ROW1, 226,
								  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								  "BATTER", TEXTSIZE_BUTTON);
	g_btnBatterOnOff.drawButton(false);

	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(2);
	g_display->setCursor(COL_POS_ROW3, 18);
	g_display->print("BAKE");
	g_display->setCursor(COL_POS_ROW3, 74);
	g_display->print("SPREAD");

	// baking time
	g_spinBakingTime.init(g_display,
			              COL_POS_ROW4, 2,
						  g_bakingTime, 1, 40.0, 60.0,
						  "%2.0f");
	g_spinBakingTime.draw();

	// spreading time
	g_spinSpreadTime.init(g_display,
			              COL_POS_ROW4, 58,
						  g_spreadTime, 0.1, 2.0, 5.0,
						  "%1.1f");
	g_spinSpreadTime.draw();

	// exit
	g_btnExit.initButtonUL(g_display,
			               400, 226,
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
	} else {
		g_btnRozel.press(false);
		g_btnBatterDose.press(false);
		g_btnBatterOnOff.press(false);
		g_btnPlateMotor.press(false);
		g_btnHeating.press(false);
		g_btnExit.press(false);
	}

	if( g_btnRozel.justReleased() ) {
		m_rozelDown = !m_rozelDown;
		if( m_rozelDown ) {
			// g_rozelController.setSpeed(1200);
			g_rozelController.setSpeed(ROZEL_SPEED_DOWN);
			// g_rozelController.moveTo(ROZEL_ENDPOS - 10 * g_spinRozelPos.getValue());
			g_rozelController.moveTo(ROZEL_ENDPOS);
			g_btnRozel.drawButton(true);
		} else {
			g_rozelController.home(ROZEL_SPEED_UP);
			g_btnRozel.drawButton(false);
		}
	}

	if( g_btnBatterOnOff.justPressed() )
		g_batterValve = true;
	else if( g_btnBatterOnOff.justReleased() )
		g_batterValve = false;

	g_spinBatterAmount.handleTouch(g_touchPressed, g_touchX, g_touchY);

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

	if( g_spinBakingTime.handleTouch(g_touchPressed, g_touchX, g_touchY) )
		g_bakingTime = g_spinBakingTime.getValue();

	if( g_spinSpreadTime.handleTouch(g_touchPressed, g_touchX, g_touchY) )
		g_spreadTime = g_spinSpreadTime.getValue();

	if( g_btnExit.justReleased() )
		switchState(&STATE_IDLE);
}

void StateMaintIdle::refreshDisplay() {
	refreshStatusBar();

	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(3);

//	bool result = g_rozelController.updateStatus();
//	ControllerData& controllerData = g_rozelController.getStatus();
//
//	g_display->setCursor(70, 260);
//	g_display->printf("pos: %3.2f speed: %3.2f, %d   ", controllerData.position, controllerData.speed, controllerData.m_controllerStatus);

}
