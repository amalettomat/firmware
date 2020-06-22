/*
 * StatePreheat.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StatePreheat.h"
#include "StateMaintIdle.h"
#include "StateIdle.h"
#include "../Gui.h"

// ### TEST
#include "../MotorControllerClient.h"
extern MotorControllerClient g_rozelController;

extern bool g_heating;

StatePreheat STATE_PREHEAT;


StatePreheat::StatePreheat() {
}

StatePreheat::~StatePreheat() {
}

void StatePreheat::transition(AbstractState* prevState) {
	g_heating = true;

	g_btnSkip.initButtonUL(g_display, 100, 100, 150, 50, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "SKIP", TEXTSIZE_BUTTON);
	g_btnSkip.drawButton(false);
}

void StatePreheat::action() {
	if( !g_heating )
		switchState(&STATE_MAINTENANCE_IDLE);

	if( g_touchPressed ) {
		g_btnSkip.press(g_btnSkip.contains(g_touchX, g_touchY));
	} else {
		g_btnSkip.press(false);
	}

	// ### TEST
//	if( g_btnSkip.justReleased() )
//		switchState(&STATE_MAINTENANCE_IDLE);
//	else if( g_btnSkip.justPressed() )
//		g_btnSkip.drawButton(true);
	if( g_btnSkip.justReleased() ) {
		g_btnSkip.drawButton(false);
		g_rozelController.moveToPos(0);
	} else if( g_btnSkip.justPressed() ){
		g_btnSkip.drawButton(true);
		g_rozelController.moveToPos(1000);
	}
}

void StatePreheat::refreshDisplay() {
	// ### TEST
	// refreshStatusBar();

	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(3);

	g_rozelController.fetchControllerData();
	g_display->setCursor(60, 288);
	g_display->printf(" %3.2f %d", g_rozelController.m_controllerData.position, g_rozelController.m_numOfBytes);

}
