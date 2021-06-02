/*
 * LedControl.h
 *
 *  Created on: 02.06.2021
 *      Author: zwax
 */

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

#include <cstdint>


class LedControl {
public:
	enum eLedPattern {
		LEDS_OFF,
		LEDS_ALL_RED,
		LEDS_ALL_GREEN,
		LEDS_WAVE,
		LEDS_BLINK_GREEN
	};

	LedControl();
	virtual ~LedControl();

	void init();
	void setPattern(eLedPattern pattern);
	void process();

protected:
	void setColor(uint32_t color);
	void wave(uint32_t color);
	void blink(uint32_t color);

private:
	eLedPattern m_pattern;
};

#endif /* LEDCONTROL_H_ */
