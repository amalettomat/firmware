/*
 * StateIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATEIDLE_H_
#define STATES_STATEIDLE_H_

#include "AbstractState.h"
#include <cstdint>


class StateIdle: public AbstractState {
public:
	StateIdle();
	virtual ~StateIdle();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "IDLE"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_startTime;
};

extern StateIdle STATE_IDLE;


#endif /* STATES_STATEIDLE_H_ */
