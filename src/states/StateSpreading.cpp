/*
 * StateSpreading.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateSpreading.h"
#include "StateBaking.h"
#include "StateIdle.h"
#include "../Gui.h"
#include "../config.h"


extern bool g_rozelDown;
extern bool g_plateMotor;
extern float g_spreadTime;


StateSpreading STATE_SPREADING;


StateSpreading::StateSpreading() : m_begin(0) {
}

StateSpreading::~StateSpreading() {
}

void StateSpreading::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Spreading...");

	m_begin = millis();

	g_plateMotor = true;
}

void StateSpreading::action() {

	if( millis() - m_begin >= g_spreadTime * 1000 ) {
		g_rozelDown = false;
		g_plateMotor = false;
		switchState(&STATE_BAKING);
	}
}

void StateSpreading::refreshDisplay() {
	refreshStatusBar();
}

