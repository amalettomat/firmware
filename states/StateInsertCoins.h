#ifndef STATES_STATE_INSERT_COINS_H_
#define STATES_STATE_INSERT_COINS_H_

#include "AbstractState.h"


class StateInsertCoins: public AbstractState {
public:
	StateInsertCoins();
	virtual ~StateInsertCoins();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "PREHEAT"; };

protected:
	virtual void refreshDisplay();

};

extern StateInsertCoins STATE_INSERT_COINS;


#endif /* STATES_STATE_INSERT_COINS_H_ */
