#include "StateScraping.h"
#include "StateIdle.h"
#include "../ScraperControl.h"
#include "../Gui.h"
#include "../config.h"


extern ScraperControl g_scraperControl;
extern bool g_plateMotor;
extern float g_fillingOverrunTime;


StateScraping STATE_SCRAPING;


StateScraping::StateScraping() : m_begin(0) {
}

StateScraping::~StateScraping() {
}

void StateScraping::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Scraping...");

	m_begin = millis() + g_fillingOverrunTime * 1000;
	m_started = false;
}

void StateScraping::action() {
	if( !m_started ) {
		// filling overrun phase
		if( millis() >= m_begin ) {
			g_plateMotor = false;
			m_started = true;
			g_scraperControl.startScrape();
		}
	} else {
		if( !g_scraperControl.isRunning() ) {
			switchState(&STATE_IDLE);
		}
	}
}

void StateScraping::refreshDisplay() {
	// refreshStatusBar();
}

