/*
 * StateIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATEDOSEBATTER_H_
#define STATES_STATEDOSEBATTER_H_

#include "AbstractState.h"
#include <cstdint>


class StateDoseBatter: public AbstractState {
public:
	StateDoseBatter();
	virtual ~StateDoseBatter();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "IDLE"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_batterStartTime;
};

extern StateDoseBatter STATE_DOSE_BATTER;


#endif /* STATES_STATEDOSEBATTER_H_ */
