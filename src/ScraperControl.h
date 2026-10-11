#ifndef SCRAPERCONTROL_H_
#define SCRAPERCONTROL_H_

#include <AccelStepper.h>
#include "Debounce.h"


class ScraperControl {
public:
	ScraperControl();
	virtual ~ScraperControl();

	enum State { IDLE, MOVING_BACK, PRE_SCRAPING, SCRAPING };

	void moveBack();
	void startScrape();
	bool isRunning();

	void run();

protected:
	void handleState();

private:
	AccelStepper m_motor;
	Debounce m_endstop;
	State m_state;
};

#endif /* SCRAPERCONTROL_H_ */
