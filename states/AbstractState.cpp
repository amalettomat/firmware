/*
 * AbstractState.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "AbstractState.h"
#include "Arduino.h"
#include "../layout.h"
#include <Adafruit_GFX.h>
#include "../RunningAverage.h"


NoopState INITIAL_STATE;

AbstractState* AbstractState::currentState = &INITIAL_STATE;

extern Adafruit_GFX* g_display;
extern bool g_heating;
extern bool g_heatingEnabled;
extern RunningAverage<float> g_plateTempAverage;
extern RunningAverage<float> g_pressureAverage;
extern float g_credit;


AbstractState::AbstractState() {
}

AbstractState::~AbstractState() {
}

void AbstractState::switchState(AbstractState* newState) {
	newState->transition(currentState);
	currentState = newState;
	Serial.print("change state to ");
	Serial.println(newState->toString());
}

void AbstractState::handleState() {
	currentState->action();
}

void AbstractState::refreshStateDisplay() {
	currentState->refreshDisplay();
}

void AbstractState::refreshStatusBar() {
	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
	g_display->setTextSize(3);

	// show heating indicator
	uint16_t color = (g_heatingEnabled && g_heating) ? COLOR565(255,64,64) : COLOR565(64,255,64);
	g_display->fillCircle(15, 300, 10, color);

	// write temp
	g_display->setCursor(60, 288);
	g_display->printf("%3.0f \xF7" "C", g_plateTempAverage.getAverage()); // degree character in code page 437 (minus 1 offset for TFT lib correction)

	// write pressure
	float pressure = g_pressureAverage.getAverage();
	if( pressure < 0.0 )
		pressure = 0.0F;
	g_display->setCursor(220, 288);
	g_display->printf("%1.2f bar", pressure);

	// credits
	g_display->setCursor(380, 288);
	g_display->printf("%5.2f", g_credit);

	// write state
	g_display->setCursor(5, 260);
	g_display->setTextSize(2);
	g_display->print(AbstractState::getCurrentState()->toString());
}
