#ifndef SCRAPERCONTROL_H_
#define SCRAPERCONTROL_H_

#include <AccelStepper.h>


class ScraperControl {
public:
	ScraperControl(int pinStep, int pinDir, int pinEnable, int pinEndstop);
	virtual ~ScraperControl();

	void moveBack();

	void run();

private:
	AccelStepper m_motor;
	int m_pinEnable;
	int m_pinEndstop;
};

#endif /* SCRAPERCONTROL_H_ */
