#ifndef STATES_STATESCRAPING_H_
#define STATES_STATESCRAPING_H_

#include "AbstractState.h"
#include <cstdint>


class StateScraping: public AbstractState {
public:
	StateScraping();
	virtual ~StateScraping();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "SCRAPING"; };

protected:
	virtual void refreshDisplay();

private:
	uint32_t m_begin;
};

extern StateScraping STATE_SCRAPING;


#endif /* STATES_STATESCRAPING_H_ */
