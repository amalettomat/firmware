#include "ScraperControl.h"
#include "config.h"


ScraperControl::ScraperControl() : m_motor(AccelStepper::DRIVER, PIN_STEPPER_SCRAPER_STEP, PIN_STEPPER_SCRAPER_DIR), m_state(IDLE) {
	m_motor.setEnablePin(PIN_STEPPER_SCRAPER_ENABLE);
	m_motor.setPinsInverted(false, false, true);

	pinMode(PIN_STEPPER_SCRAPER_ENDSTOP, INPUT_PULLUP);
	pinMode(PIN_ROLLER_MOTOR, OUTPUT);

	m_motor.disableOutputs();
	m_motor.setMaxSpeed(SCRAPER_SPEED * SCRAPER_MICROSTEPS);
	// m_motor.setSpeed(125.0 * SCRAPER_MICROSTEPS);
	// m_motor.setSpeed(50.0 * SCRAPER_MICROSTEPS);
	// m_motor.setAcceleration(100.0 * SCRAPER_MICROSTEPS);
	m_motor.setAcceleration(2000.0 * SCRAPER_MICROSTEPS);

	analogWrite(PIN_ROLLER_MOTOR, 0);
}

ScraperControl::~ScraperControl() {
}

bool ScraperControl::isRunning() {
	return m_state == IDLE && m_motor.isRunning();
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

	case SCRAPING:
		if( m_motor.speed() > 0 ) {
			if( !digitalRead(PIN_STEPPER_SCRAPER_ENDSTOP) ) {
				// hit end stop switch
				m_motor.setCurrentPosition(0);
				analogWrite(PIN_ROLLER_MOTOR, ROLLER_SPEED);
				// change direction
				m_motor.moveTo(-SCRAPER_DIST * SCRAPER_MICROSTEPS);
			}
		}

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
	m_motor.moveTo(SCRAPER_DIST * SCRAPER_MICROSTEPS);
	m_state = MOVING_BACK;
}

void ScraperControl::startScrape() {
	analogWrite(PIN_ROLLER_MOTOR, 0);

	// first move back until end stop
	m_motor.enableOutputs();
	m_motor.setSpeed(200.0 * SCRAPER_MICROSTEPS);
	m_motor.setCurrentPosition(0);
	m_motor.moveTo(SCRAPER_DIST * SCRAPER_MICROSTEPS);

	m_state = SCRAPING;
}

