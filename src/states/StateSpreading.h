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
	const char* toString() { return "SPREADING"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_begin;
};

extern StateSpreading STATE_SPREADING;


#endif /* STATES_STATESPREADING_H_ */
