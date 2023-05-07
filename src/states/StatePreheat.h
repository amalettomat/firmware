/*
 * StatePreheat.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATEPREHEAT_H_
#define STATES_STATEPREHEAT_H_

#include "AbstractState.h"


class StatePreheat: public AbstractState {
public:
	StatePreheat();
	virtual ~StatePreheat();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "PREHEAT"; };

protected:
	virtual void refreshDisplay();

};

extern StatePreheat STATE_PREHEAT;


#endif /* STATES_STATEPREHEAT_H_ */
