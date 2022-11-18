/*
 * StateDoseBatter.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateDoseBatter.h"
#include "StateSpreading.h"
#include "StateMaintIdle.h"
#include "../Gui.h"
#include "../config.h"
#include "../MotorControllerClient.h"


extern bool g_batterValve;
extern float g_batterAmount;
extern bool g_plateMotor;
extern bool g_maintButton;
extern MotorControllerClient g_rozelController;

StateDoseBatter STATE_DOSE_BATTER;


StateDoseBatter::StateDoseBatter() {
	m_batterStartTime = 0;
}

StateDoseBatter::~StateDoseBatter() {
}

void StateDoseBatter::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Dosing...");

	m_batterStartTime = millis();
	g_batterValve = true;
	g_plateMotor = true;
}

void StateDoseBatter::action() {
	if( g_maintButton ) {
		g_rozelController.home(ROZEL_SPEED_UP);
		g_rozelController.home(ROZEL_SPEED_UP);
		g_batterValve = false;
		g_plateMotor = false;
		switchState(&STATE_MAINTENANCE_IDLE);
	}

	if( millis() - m_batterStartTime >= g_batterAmount * 1000 ) {
		g_batterValve = false;
		switchState(&STATE_SPREADING);
	}
}

void StateDoseBatter::refreshDisplay() {
	refreshStatusBar();
}

