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

	if( g_btnSkip.justReleased() )
		switchState(&STATE_MAINTENANCE_IDLE);
	else if( g_btnSkip.justPressed() )
		g_btnSkip.drawButton(true);
}

void StatePreheat::refreshDisplay() {
	refreshStatusBar();
}
