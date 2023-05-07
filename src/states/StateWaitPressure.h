#ifndef STATES_STATEWAITPRESSURE_H_
#define STATES_STATEWAITPRESSURE_H_

#include "AbstractState.h"


class StateWaitPressure: public AbstractState {
public:
	StateWaitPressure();
	virtual ~StateWaitPressure();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "PREHEAT"; };

protected:
	virtual void refreshDisplay();

};

extern StateWaitPressure STATE_WAIT_PRESSURE;


#endif /* STATES_STATEWAITPRESSURE_H_ */
