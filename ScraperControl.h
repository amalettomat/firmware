#ifndef SCRAPERCONTROL_H_
#define SCRAPERCONTROL_H_

#include <AccelStepper.h>


class ScraperControl {
public:
	ScraperControl();
	virtual ~ScraperControl();

	enum State { IDLE, MOVING_BACK, SCRAPING };

	void moveBack();
	void startScrape();
	bool isRunning();

	void run();

protected:
	void handleState();

private:
	AccelStepper m_motor;
	State m_state;
};

#endif /* SCRAPERCONTROL_H_ */
