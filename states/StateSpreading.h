/*
 * StateIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATESPREADING_H_
#define STATES_STATESPREADING_H_

#include "AbstractState.h"
#include <cstdint>


class StateSpreading: public AbstractState {
public:
	StateSpreading();
	virtual ~StateSpreading();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "IDLE"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_begin;
};

extern StateSpreading STATE_SPREADING;


#endif /* STATES_STATESPREADING_H_ */
