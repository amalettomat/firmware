/*
 * StateBaking.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateBaking.h"
#include "StateIdle.h"
#include "StateScraping.h"
#include "../Gui.h"
#include "../config.h"


StateBaking STATE_BAKING;

extern float g_bakingTime;


StateBaking::StateBaking() : m_begin(0) {
}

StateBaking::~StateBaking() {
}

void StateBaking::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(160, 80);
	g_display->print("Baking...");

	m_begin = millis();
}

void StateBaking::action() {

	if( millis() - m_begin >= g_bakingTime * 1000 ) {
		switchState(&STATE_SCRAPING);
	}
}

void StateBaking::refreshDisplay() {
	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(5);
	g_display->setCursor(210, 140);
	int countdown = g_bakingTime - (millis() - m_begin)/1000;
	g_display->printf("%2d", countdown);

	refreshStatusBar();
}

