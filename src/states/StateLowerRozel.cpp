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


extern bool g_rozelDown;
extern bool g_rozelIsDown;
extern bool g_plateMotor;
extern bool g_compressor;
extern float g_pressure;


StateLowerRozel STATE_LOWER_ROZEL;


// #define ROZEL_CHECK_INTERVAL 500 // ms


StateLowerRozel::StateLowerRozel() /*: m_startTime(0)*/ {
}

StateLowerRozel::~StateLowerRozel() {
}

void StateLowerRozel::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Rozel down...");

	g_rozelDown = true;

	g_plateMotor = true;
	// m_startTime = millis() + ROZEL_CHECK_INTERVAL;
}

void StateLowerRozel::action() {

	if( g_rozelIsDown ) {
		switchState(&STATE_DOSE_BATTER);
	}
}

void StateLowerRozel::refreshDisplay() {
	refreshStatusBar();
}

