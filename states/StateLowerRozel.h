/*
 * StateIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATELOWERROZEL_H_
#define STATES_STATELOWERROZEL_H_

#include "AbstractState.h"
#include <stdint.h>


class StateLowerRozel: public AbstractState {
public:
	StateLowerRozel();
	virtual ~StateLowerRozel();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "LOWROZ"; };

protected:
	virtual void refreshDisplay();

private:
	// uint32_t m_startTime;
};

extern StateLowerRozel STATE_LOWER_ROZEL;


#endif /* STATES_STATELOWERROZEL_H_ */
