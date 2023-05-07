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
#include "../Gui.h"


NoopState INITIAL_STATE;

AbstractState* AbstractState::currentState = &INITIAL_STATE;

extern bool g_heating;
extern bool g_heatingEnabled;
extern float g_plateTemp;
extern float g_pressure;
extern float g_credit;


// GFXcanvas16 AbstractState::canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
int AbstractState::prevHeating = -1;
int AbstractState::prevTemp = -1.0F;
float AbstractState::prevPressure = -1.0F;
float AbstractState::prevCredit = -1.0F;


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

//void AbstractState::refreshStatusBar() {
//	g_display->setTextColor(COL_STATUS_TEXT, COL_BACKGROUND);
//	g_display->setTextSize(1);
//
//	// show heating indicator
//	uint16_t color = (g_heatingEnabled && g_heating) ? COLOR565(255,64,64) : COLOR565(64,255,64);
//	g_display->fillCircle(15, 300, 10, color);
//
//	// write temp
//	// g_display->setCursor(60, 288);
//	g_display->setCursor(60, STATUS_TEXT_YPOS);
//	// g_display->printf("%3.0f \xF7" "C", g_plateTempAverage.getAverage()); // degree character in code page 437 (minus 1 offset for TFT lib correction)
//	g_display->printf("%3.0f °C", g_plateTempAverage.getAverage());
//
//	// write pressure
//	float pressure = g_pressureAverage.getAverage();
//	if( pressure < 0.0 )
//		pressure = 0.0F;
//	// g_display->setCursor(220, 288);
//	g_display->setCursor(220, STATUS_TEXT_YPOS);
//	g_display->printf("%1.2f bar", pressure);
//
//	// credits
//	// g_display->setCursor(380, 288);
//	g_display->setCursor(380, STATUS_TEXT_YPOS);
//	g_display->printf("%5.2f", g_credit);
//
//	// write state
////	g_display->setCursor(5, 310);
////	g_display->setTextSize(2);
////	g_display->setTextColor(0x6B6D, COL_BACKGROUND);
////	g_display->print(AbstractState::getCurrentState()->toString());
//}

void AbstractState::refreshStatusBar() {
	// show heating indicator
	int heatState = (g_heatingEnabled ? 2:0) + (g_heating ? 1:0);
	if( heatState != prevHeating ) {
		uint16_t color = COLOR565(64,64,64); // grey if heating is disabled
		if( g_heatingEnabled )
			color = g_heating ? COLOR565(255,64,64) : COLOR565(64,255,64);

		g_display->fillCircle(15, 305, 10, color);
		prevHeating = heatState;
	}

	// canvas.fillScreen(COL_BACKGROUND);
//	canvas.fillScreen(0x6B6D);
//
//	canvas.setTextColor(COL_STATUS_TEXT);
//	canvas.setFont(DEFAULT_FONT);

	// write temp
	int curTemp = g_plateTemp;
	if( curTemp != prevTemp  ) {
		GFXcanvas16 canvas(50, CANVAS_HEIGHT);
		// canvas.fillScreen(0x6B6D);
		canvas.setTextColor(COL_STATUS_TEXT);
		canvas.setFont(DEFAULT_FONT);
		canvas.setCursor(0, STATUS_TEXT_YPOS);
		canvas.printf("%3d °C", curTemp);
		g_display->drawRGBBitmap(60, 294, canvas.getBuffer(), 50, CANVAS_HEIGHT);
		prevTemp = curTemp;
	}

	// write pressure
	float pressure = ((int)(g_pressure*100.0F))/100.0F;
	if( pressure != prevPressure ) {
		GFXcanvas16 canvas(70, CANVAS_HEIGHT);
		// canvas.fillScreen(0x6B6D);
		canvas.setTextColor(COL_STATUS_TEXT);
		canvas.setFont(DEFAULT_FONT);
		canvas.setCursor(0, STATUS_TEXT_YPOS);
		canvas.printf("%1.2f bar", pressure);
		g_display->drawRGBBitmap(160, 294, canvas.getBuffer(), 70, CANVAS_HEIGHT);
		prevPressure = pressure;
	}

	// credits
	if( g_credit != prevCredit  ) {
		GFXcanvas16 canvas(50, CANVAS_HEIGHT);
		// canvas.fillScreen(0x6B6D);
		canvas.setTextColor(COL_STATUS_TEXT);
		canvas.setFont(DEFAULT_FONT);
		canvas.setCursor(0, STATUS_TEXT_YPOS);
		canvas.printf("%5.2f", g_credit);
		g_display->drawRGBBitmap(320, 294, canvas.getBuffer(), 50, CANVAS_HEIGHT);
		prevTemp = curTemp;
	}

	// write state
//	g_display->setCursor(5, 310);
//	g_display->setTextSize(2);
//	g_display->setTextColor(0x6B6D, COL_BACKGROUND);
//	g_display->print(AbstractState::getCurrentState()->toString());

//	g_display->drawRGBBitmap(60, 294, canvas.getBuffer(), CANVAS_WIDTH, CANVAS_HEIGHT);
}
