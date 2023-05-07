/*
 * StateBaking.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATESBAKING_H_
#define STATES_STATESBAKING_H_

#include "AbstractState.h"
#include <cstdint>
#include <Adafruit_GFX.h>


class GFXcanvas16;

#define CANVAS_WIDTH 55
#define CANVAS_HEIGHT 37


class StateBaking: public AbstractState {
public:
	StateBaking();
	virtual ~StateBaking();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "BAKING"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_begin;
	GFXcanvas16 m_canvas;
};

extern StateBaking STATE_BAKING;


#endif /* STATES_STATESBAKING_H_ */
