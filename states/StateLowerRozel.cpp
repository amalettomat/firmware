/*
 * StateLowerRozel.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateLowerRozel.h"
#include "StateDoseBatter.h"
#include "../Gui.h"
#include "../config.h"
#include "../MotorControllerClient.h"


extern MotorControllerClient g_rozelController;
extern bool g_plateMotor;


StateLowerRozel STATE_LOWER_ROZEL;


StateLowerRozel::StateLowerRozel() {
}

StateLowerRozel::~StateLowerRozel() {
}

void StateLowerRozel::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Rozel down...");

	g_rozelController.setSpeed(ROZEL_SPEED_DOWN);
	g_rozelController.moveTo(ROZEL_ENDPOS);
	g_plateMotor = true;
}

void StateLowerRozel::action() {
//	if( g_touchPressed ) {
//		g_btnMaintenance.press(g_btnMaintenance.contains(g_touchX, g_touchY));
//		g_btnSelectFill1.press(g_btnSelectFill1.contains(g_touchX, g_touchY));
//		g_btnSelectFill2.press(g_btnSelectFill2.contains(g_touchX, g_touchY));
//	} else {
//		g_btnMaintenance.press(false);
//		g_btnSelectFill1.press(false);
//		g_btnSelectFill2.press(false);
//	}
//
//	if( g_btnMaintenance.justReleased() )
//		switchState(&STATE_MAINTENANCE_IDLE);
//	else if( g_btnSkip.justPressed() )
//		g_btnMaintenance.drawButton(true);
//
//	if( g_btnSelectFill1.justPressed() ) {
//		g_btnSelectFill1.drawButton(true);
//	} else if( g_btnSelectFill1.justReleased() ) {
//		g_btnSelectFill1.drawButton(false);
//	}
//
//	if( g_btnSelectFill2.justPressed() ) {
//		g_btnSelectFill2.drawButton(true);
//	} else if( g_btnSelectFill2.justReleased() ) {
//		g_btnSelectFill2.drawButton(false);
//	}

	static int count = 0;
	if( count-- <= 0 ) {
		count = 100;
		g_rozelController.updateStatus();
		if( !g_rozelController.getStatus().isMoving() ) {
			switchState(&STATE_DOSE_BATTER);
		}
	}
}

void StateLowerRozel::refreshDisplay() {
	refreshStatusBar();
}

