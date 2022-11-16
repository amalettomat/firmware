/*
 * StateBaking.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateBaking.h"
#include "StateIdle.h"
#include "StateFilling.h"
#include "../Gui.h"
#include "../config.h"


StateBaking STATE_BAKING;


extern float g_bakingTime;


StateBaking::StateBaking() : m_begin(0), m_canvas(CANVAS_WIDTH, CANVAS_HEIGHT) {
}

StateBaking::~StateBaking() {
}

void StateBaking::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(200, 80);
	g_display->print("Baking...");

	m_begin = millis();
}

void StateBaking::action() {

	if( millis() - m_begin >= g_bakingTime * 1000 ) {
		switchState(&STATE_FILLING);
	}
}

void StateBaking::refreshDisplay() {
	// m_canvas.fillScreen(0x6B6D);
	m_canvas.fillScreen(COL_BACKGROUND);

	m_canvas.setTextColor(COL_STATUS_TEXT);
	m_canvas.setFont(BIG_FONT);

	// g_display->setCursor(210, 140);
	m_canvas.setCursor(0, CANVAS_HEIGHT-3);
	int countdown = g_bakingTime - (millis() - m_begin)/1000;
	m_canvas.printf("%2d", countdown);

	g_display->drawRGBBitmap(210, 140, m_canvas.getBuffer(), CANVAS_WIDTH, CANVAS_HEIGHT);

	refreshStatusBar();
}

