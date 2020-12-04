/*
 * StateIdle.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateIdle.h"
#include "StateMaintIdle.h"
#include "../Gui.h"
#include "../BmpImage.h"


StateIdle STATE_IDLE;

extern const struct BmpImage icon_choc;
extern const struct BmpImage icon_jam;


StateIdle::StateIdle() {
}

StateIdle::~StateIdle() {
}

void StateIdle::transition(AbstractState* prevState) {
	clearMainArea();

	g_display->drawRGBBitmap_fast(60, 60, (const uint8_t*)(icon_choc.pixel_data), icon_choc.width, icon_choc.height);
	g_display->drawRGBBitmap_fast(260, 60, (const uint8_t*)(icon_jam.pixel_data), icon_jam.width, icon_jam.height);

	g_btnMaintenance.initButtonUL(g_display, 400, 235, 70, 30, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "MAINT", TEXTSIZE_BUTTON);
	g_btnMaintenance.drawButton(false);
}

void StateIdle::action() {
	if( g_touchPressed ) {
		g_btnMaintenance.press(g_btnMaintenance.contains(g_touchX, g_touchY));
	} else {
		g_btnMaintenance.press(false);
	}

	if( g_btnMaintenance.justReleased() )
		switchState(&STATE_MAINTENANCE_IDLE);
	else if( g_btnSkip.justPressed() )
		g_btnMaintenance.drawButton(true);

}

void StateIdle::refreshDisplay() {
	refreshStatusBar();
}

