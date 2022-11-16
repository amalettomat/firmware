#ifndef STATES_STATEFILLING_H_
#define STATES_STATEFILLING_H_

#include "AbstractState.h"
#include <cstdint>


class StateFilling: public AbstractState {
public:
	StateFilling();
	virtual ~StateFilling();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "FILLING"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_startTime;
	int m_spotCount;
};

extern StateFilling STATE_FILLING;


#endif /* STATES_STATEFILLING_H_ */
