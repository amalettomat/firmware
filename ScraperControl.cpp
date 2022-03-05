#include "ScraperControl.h"
#include "config.h"


ScraperControl::ScraperControl(int pinStep, int pinDir, int pinEnable, int pinEndStop) : m_motor(AccelStepper::DRIVER, pinStep, pinDir),
                                                                                         m_pinEnable(pinEnable),
																						 m_pinEndstop(pinEndStop) {
	pinMode(m_pinEnable, OUTPUT);
	pinMode(pinStep, OUTPUT);
	pinMode(pinDir, OUTPUT);
	pinMode(pinEndStop, INPUT_PULLUP);

	m_motor.setMaxSpeed(125.0 * SCRAPER_MICROSTEPS);
	// m_motor.setSpeed(125.0 * SCRAPER_MICROSTEPS);
	m_motor.setSpeed(50.0 * SCRAPER_MICROSTEPS);
	// m_motor.setAcceleration(100.0 * SCRAPER_MICROSTEPS);
	m_motor.setAcceleration(25.0 * SCRAPER_MICROSTEPS);

	digitalWrite(m_pinEnable, LOW); // sleep mode
}

ScraperControl::~ScraperControl() {
}

void ScraperControl::run() {
	m_motor.run();
}

void ScraperControl::moveBack() {
	m_motor.moveTo(400);
}
