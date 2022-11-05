#include "ScraperControl.h"
#include "config.h"


ScraperControl::ScraperControl() : m_motor(AccelStepper::DRIVER, PIN_STEPPER_SCRAPER_STEP, PIN_STEPPER_SCRAPER_DIR), m_state(IDLE) {
	m_motor.setEnablePin(PIN_STEPPER_SCRAPER_ENABLE);
	m_motor.setPinsInverted(false, false, true);

	pinMode(PIN_STEPPER_SCRAPER_ENDSTOP, INPUT_PULLUP);
	pinMode(PIN_ROLLER_MOTOR, OUTPUT);

	m_motor.disableOutputs();
	m_motor.setMaxSpeed(SCRAPER_SPEED);
	m_motor.setAcceleration(SCRAPER_ACCELARATION);

	analogWrite(PIN_ROLLER_MOTOR, 0);
}

ScraperControl::~ScraperControl() {
}

bool ScraperControl::isRunning() {
	return m_state != IDLE;
}

void ScraperControl::handleState() {
	switch( m_state ) {
	case MOVING_BACK:
		if( !digitalRead(PIN_STEPPER_SCRAPER_ENDSTOP) ) {
			// hit end stop switch
			m_motor.stop();
			m_motor.setCurrentPosition(0);
			m_state = IDLE;
		}
		break;

	case PRE_SCRAPING:
		if( !digitalRead(PIN_STEPPER_SCRAPER_ENDSTOP) ) {
			// hit end stop switch
			m_motor.stop();
			m_motor.setCurrentPosition(0);
			m_motor.moveTo(-SCRAPER_DIST);
			m_state = SCRAPING;
		}
		break;

	case SCRAPING:
		if( !m_motor.isRunning() ) {
			moveBack();
		}
		break;

	default:;
	}
}

void ScraperControl::run() {
	handleState();

	m_motor.run();
}

void ScraperControl::moveBack() {
	analogWrite(PIN_ROLLER_MOTOR, 0);

	m_motor.enableOutputs();
	m_motor.setCurrentPosition(0);
	m_motor.moveTo(SCRAPER_DIST);
	m_state = MOVING_BACK;
}

void ScraperControl::startScrape() {
	analogWrite(PIN_ROLLER_MOTOR, ROLLER_SPEED);

	// first move back until end stop
	m_motor.enableOutputs();
	m_motor.setCurrentPosition(0);
	m_motor.moveTo(SCRAPER_DIST);
	m_state = PRE_SCRAPING;
}

