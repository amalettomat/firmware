/*
 * AbstractState.h
 *
 * Base class for states.
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_ABSTRACTSTATE_H_
#define STATES_ABSTRACTSTATE_H_

#include <stdint.h>


class GFXcanvas16;


#define CANVAS_WIDTH 365
#define CANVAS_HEIGHT 22


class AbstractState {
public:
	AbstractState();
	virtual ~AbstractState();

	static void switchState(AbstractState* newState);
	static void handleState();
	static AbstractState* getCurrentState() { return currentState; };

	virtual void transition(AbstractState* prevState) = 0;
	virtual void action() = 0;
	virtual const char* toString() = 0;

	static void refreshStateDisplay();

protected:
	virtual void refreshDisplay() = 0;
	void refreshStatusBar();

private:
	static AbstractState* currentState;
	static int prevHeating;
	static float prevPressure;
	static int prevTemp;
	static float prevCredit;
	static uint32_t prevCounter;
};

// NO-OP state
class NoopState : public AbstractState {
	void action() {};
	void transition(AbstractState* prevState) {};
	const char* toString() { return "NOOP"; };
	virtual void refreshDisplay() {};
};

extern NoopState INITIAL_STATE;


#endif /* STATES_ABSTRACTSTATE_H_ */
