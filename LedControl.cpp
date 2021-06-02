/*
 * LedControl.cpp
 *
 *  Created on: 02.06.2021
 *      Author: zwax
 */

#include "LedControl.h"
#include "Adafruit_DotStar.h"
#include "config.h"


Adafruit_DotStar ledStrip(NUM_PIXELS, PIN_LEDS_DATA, PIN_LEDS_CLOCK, DOTSTAR_BGR);


LedControl::LedControl() : m_pattern(LEDS_OFF) {
}

LedControl::~LedControl() {
}

void LedControl::init() {
	ledStrip.begin();
	process();
}

void LedControl::setColor(uint32_t color) {
	for( int index=0; index < NUM_PIXELS; index++ ) {
		ledStrip.setPixelColor(index, color);
	}
	ledStrip.show();
}

void LedControl::wave(uint32_t color) {
	static int pos = 0;

	for( int index=0; index < NUM_PIXELS; index++ )
		ledStrip.setPixelColor(index, 0);

	for( int index=pos; index < pos+3; index++ ) {
		ledStrip.setPixelColor(index % NUM_PIXELS, color);
	}

	pos++;
	if( pos >= NUM_PIXELS )
		pos = 0;

	ledStrip.show();
}

void LedControl::blink(uint32_t color) {
	static bool state = false;
	state = !state;
	setColor(state ? color : 0);
}

void LedControl::setPattern(eLedPattern pattern) {
	m_pattern = pattern;
}

void LedControl::process() {
	static eLedPattern prevPattern = m_pattern;
	bool changed = false;
	if( prevPattern != m_pattern ) {
		changed = true;
		prevPattern = m_pattern;
	}

	switch(m_pattern) {
	case LEDS_OFF:
		if( changed ) {
			setColor(0);
		}
		break;

	case LEDS_ALL_RED:
		if( changed ) {
			setColor(0x800000);
		}
		break;

	case LEDS_ALL_GREEN:
		if( changed ) {
			setColor(0x008000);
		}
		break;

	case LEDS_WAVE:
		wave(0x800000);
		break;

	case LEDS_BLINK_GREEN:
		blink(0x008000);
		break;
	}
}
