/*
 * StateMaintIdle.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateMaintIdle.h"
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


StateMaintIdle STATE_MAINTENANCE_IDLE;


StateMaintIdle::StateMaintIdle() : m_rozelDown(false), m_batterStartTime(0) {
}

StateMaintIdle::~StateMaintIdle() {
}

#define COL_POS_LEFT 10
#define COL_POS_MID 160
#define COL_POS_RIGHT 310

void StateMaintIdle::transition(AbstractState* prevState) {
	clearMainArea();

	// rozel button
	m_rozelDown = false;
	// g_rozelStepper.move(-200*ROZEL_MICROSTEPS);

	g_btnRozel.initButtonUL(g_display,
			                COL_POS_MID, 5,                            // x-pos, y-pos
							MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
							COL_BUTTON_OUTLINE,	COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
							"ROZEL", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnRozel.drawButton(false);

	// rozel pos
	g_spinRozelPos.init(g_display,
			            COL_POS_RIGHT, 5, // x-pos, y-pos
			            16, 1, 0, 25,     // value, step, min, max
			            "%2.0f");         // format
	g_spinRozelPos.draw();

	// plate motor on/off
	g_btnPlateMotor.initButtonUL(g_display,
			                     COL_POS_MID, 65,                           // x-pos, y-pos
			                     MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT, // width, height
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, // colors
								 "PLATE", TEXTSIZE_BUTTON);                 // Label, testsize
	g_btnPlateMotor.drawButton(g_plateMotor);

	// plate speed
	g_spinPlateSpeed.init(g_display,
			              COL_POS_RIGHT, 65, // x-pos, y-pos
						  255, 5, 150, 255,  // value, step, min, max
						  "%3.0f");          // format
	g_spinPlateSpeed.draw();

	// heating
	g_btnHeating.initButtonUL(g_display,        // x-pos, y-pos
			                  COL_POS_MID, 125,
							  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
							  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
							  "HEATING", TEXTSIZE_BUTTON);
	g_btnHeating.drawButton(g_heatingEnabled);

	// plate temperature
	g_spinTemp.init(g_display,
			        COL_POS_RIGHT, 125, // x-pos, y-pos
					158, 2, 120, 200,   // value, step, min, max
					"%3.0f");
	g_spinTemp.draw();

	// batter button
	g_batterValve = false;
	g_btnBatterOnOff.initButtonUL(g_display,
			                      COL_POS_LEFT, 185,
								  MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								  COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								  "BATTER", TEXTSIZE_BUTTON);
	g_btnBatterOnOff.drawButton(false);

	// batter dose button
	g_btnBatterDose.initButtonUL(g_display,
			                     COL_POS_MID, 185,
								 MAINT_BUTTON_WIDTH, BUTTON_DEFAULT_HEIGHT,
								 COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
								 "DOSE", TEXTSIZE_BUTTON);
	g_btnBatterDose.drawButton(false);

	// batter amount
	g_spinBatterAmount.init(g_display,
			                COL_POS_RIGHT, 185,
							1.5, 0.1, 1.0, 3.0, // value, step, min, max
							"%3.1f");
	g_spinBatterAmount.draw();


}

void StateMaintIdle::action() {
	if( g_spinRozelPos.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		if( m_rozelDown ) {
			// if rozel pos changed and rozel is down, set new position
			// g_rozelStepper.moveTo(ROZEL_ENDPOS - 5 * g_spinRozelPos.getValue());
		}
	}

	if( g_touchPressed ) {
		g_btnRozel.press(g_btnRozel.contains(g_touchX, g_touchY));
		g_btnBatterDose.press(g_btnBatterDose.contains(g_touchX, g_touchY));
		g_btnBatterOnOff.press(g_btnBatterOnOff.contains(g_touchX, g_touchY));
		g_btnPlateMotor.press(g_btnPlateMotor.contains(g_touchX, g_touchY));
		g_btnHeating.press(g_btnHeating.contains(g_touchX, g_touchY));
	} else {
		g_btnRozel.press(false);
		g_btnBatterDose.press(false);
		g_btnBatterOnOff.press(false);
		g_btnPlateMotor.press(false);
		g_btnHeating.press(false);
	}

	if( g_btnRozel.justPressed() ) {
		m_rozelDown = !m_rozelDown;
		if( m_rozelDown ) {
			// g_rozelStepper.moveTo(ROZEL_ENDPOS - 5 * g_spinRozelPos.getValue());
			g_rozelController.moveToPos(ROZEL_ENDPOS);
			g_btnRozel.drawButton(true);
		} else {
			// g_rozelStepper.move(-200*ROZEL_MICROSTEPS); // move until hitting home switch
			g_rozelController.startHoming();
			g_btnRozel.drawButton(false);
		}
	}

	if( g_btnBatterOnOff.justPressed() )
		g_batterValve = true;
	else if( g_btnBatterOnOff.justReleased() )
		g_batterValve = false;

	g_spinBatterAmount.handleTouch(g_touchPressed, g_touchX, g_touchY);

	if( g_btnBatterDose.justPressed() && m_batterStartTime == 0 ) {
		// start timer
		m_batterStartTime = millis();
		g_batterValve = true;
	}

	if( m_batterStartTime > 0 && millis() - m_batterStartTime >= g_spinBatterAmount.getValue() * 1000 ) {
		m_batterStartTime = 0;
		g_batterValve = false;
	}

	if( g_btnPlateMotor.justPressed() ) {
		g_plateMotor = !g_plateMotor;
		g_btnPlateMotor.drawButton(g_plateMotor);
	}

	if( g_spinPlateSpeed.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_plateMotorSpeed = g_spinPlateSpeed.getValue();
	}

	if( g_btnHeating.justPressed() ) {
		g_heatingEnabled = !g_heatingEnabled;
		g_btnHeating.drawButton(g_heatingEnabled);
	}

	if( g_spinTemp.handleTouch(g_touchPressed, g_touchX, g_touchY) ) {
		g_setTemp = g_spinTemp.getValue();
	}

}

void StateMaintIdle::refreshDisplay() {
	refreshStatusBar();
}
