/*
 * StateIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATELOWERROZEL_H_
#define STATES_STATELOWERROZEL_H_

#include "AbstractState.h"


class StateLowerRozel: public AbstractState {
public:
	StateLowerRozel();
	virtual ~StateLowerRozel();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "IDLE"; };

protected:
	virtual void refreshDisplay();

};

extern StateLowerRozel STATE_LOWER_ROZEL;


#endif /* STATES_STATELOWERROZEL_H_ */
