#include "StateFilling.h"
#include "StateScraping.h"
#include "StateMaintIdle.h"
#include "../Gui.h"
#include "../config.h"


extern bool g_plateMotor;
extern float g_amountFilling1;
extern float g_amountFilling2;
extern bool g_fillingValve1;
extern bool g_fillingValve2;
extern bool g_maintButton;
extern int g_selectedFilling;


StateFilling STATE_FILLING;


StateFilling::StateFilling() {
	m_startTime = 0;
}

StateFilling::~StateFilling() {
}

void StateFilling::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Filling...");

	m_startTime = millis();
	g_plateMotor = true;

	if( g_amountFilling1 > 0.0 && (g_selectedFilling & 1) == 1 )
		g_fillingValve1 = true;

	if( g_amountFilling2 > 0.0 && (g_selectedFilling & 2) == 2 )
		g_fillingValve2 = true;
}

void StateFilling::action() {
	uint32_t elapsed = millis() - m_startTime;

	if( g_maintButton ) {
		g_plateMotor = false;
		g_fillingValve1 = false;
		g_fillingValve2 = false;
		switchState(&STATE_MAINTENANCE_IDLE);
		return;
	}

	if( !g_fillingValve1 && !g_fillingValve2 ) {
		// both valves closed, start scraping (keep motor on!)
		switchState(&STATE_SCRAPING);
		return;
	} else {
		if( g_fillingValve1 ) {
			if( elapsed >= g_amountFilling1 * 1000 ) {
				g_fillingValve1 = false;
				// ??? m_startTime = millis();
			}
		}
		if( g_fillingValve2 ) {
			if( elapsed >= g_amountFilling2 * 1000 ) {
				g_fillingValve2 = false;
				// ??? m_startTime = millis();
			}
		}
	}
}

void StateFilling::refreshDisplay() {
	refreshStatusBar();
}

