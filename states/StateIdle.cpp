/*
 * StateIdle.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateIdle.h"
#include "StateMaintIdle.h"
#include "StateLowerRozel.h"
#include "../Gui.h"
#include "../BmpImage.h"


StateIdle STATE_IDLE;

extern const struct BmpImage icon_choc;
extern const struct BmpImage icon_jam;
extern bool g_showMaint;
extern bool g_plateMotor;
extern bool g_batterValve;


StateIdle::StateIdle() {
}

StateIdle::~StateIdle() {
}

void StateIdle::transition(AbstractState* prevState) {
	clearMainArea();

	g_batterValve = false;
	g_plateMotor = false;

	g_display->drawRGBBitmap_fast(60, 60, (const uint8_t*)(icon_choc.pixel_data), icon_choc.width, icon_choc.height);
	g_btnSelectFill1.initButtonUL(g_display, 60, 60, 160, 160, COL_BUTTON_SELECT, COL_BACKGROUND, 20);
	g_btnSelectFill1.drawButton(false);

	g_display->drawRGBBitmap_fast(260, 60, (const uint8_t*)(icon_jam.pixel_data), icon_jam.width, icon_jam.height);
	g_btnSelectFill2.initButtonUL(g_display, 260, 60, 160, 160, COL_BUTTON_SELECT, COL_BACKGROUND, 20);
	g_btnSelectFill2.drawButton(false);

	g_btnMaintenance.initButtonUL(g_display, 400, 235, 70, 30, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "MAINT", TEXTSIZE_BUTTON);
	if( g_showMaint ) {
		g_btnMaintenance.drawButton(false);
	}
}

void StateIdle::action() {
	static bool prevShowMaint = g_showMaint;
	if( g_showMaint != prevShowMaint ) {
		prevShowMaint = g_showMaint;
		g_btnMaintenance.drawButton(false);
	}

	if( g_touchPressed ) {
		if( g_showMaint )
			g_btnMaintenance.press(g_btnMaintenance.contains(g_touchX, g_touchY));

		g_btnSelectFill1.press(g_btnSelectFill1.contains(g_touchX, g_touchY));
		g_btnSelectFill2.press(g_btnSelectFill2.contains(g_touchX, g_touchY));
	} else {
		g_btnMaintenance.press(false);
		g_btnSelectFill1.press(false);
		g_btnSelectFill2.press(false);
	}

	if( g_btnMaintenance.justReleased() )
		switchState(&STATE_MAINTENANCE_IDLE);
	else if( g_btnSkip.justPressed() )
		g_btnMaintenance.drawButton(true);

	if( g_btnSelectFill1.justPressed() ) {
		g_btnSelectFill1.drawButton(true);
		// TODO store filling selection
		switchState(&STATE_LOWER_ROZEL);
	} else if( g_btnSelectFill1.justReleased() ) {
		g_btnSelectFill1.drawButton(false);
	}

	if( g_btnSelectFill2.justPressed() ) {
		g_btnSelectFill2.drawButton(true);
		// TODO store filling selection
		switchState(&STATE_LOWER_ROZEL);
	} else if( g_btnSelectFill2.justReleased() ) {
		g_btnSelectFill2.drawButton(false);
	}
}

void StateIdle::refreshDisplay() {
	refreshStatusBar();
}

