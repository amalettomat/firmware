#include "StateScraping.h"
#include "StateIdle.h"
#include "../ScraperControl.h"
#include "../Gui.h"
#include "../config.h"


extern ScraperControl g_scraperControl;

static int timeout = 30; // sec


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

	m_begin = millis();
	g_scraperControl.startScrape();
}

void StateScraping::action() {

	if( !g_scraperControl.isRunning() ) { // || millis() - m_begin >=  timeout * 1000 ) {
		switchState(&STATE_IDLE);
	}
}

void StateScraping::refreshDisplay() {
	// refreshStatusBar();
}

