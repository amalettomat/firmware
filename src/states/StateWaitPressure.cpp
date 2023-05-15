#include "StateWaitPressure.h"
#include "StateLowerRozel.h"
#include "StateMaintIdle.h"
#include "../Gui.h"
#include "../LedControl.h"
#include "../config.h"


extern bool g_maintButton;
extern float g_pressure;
extern bool g_compressor;

StateWaitPressure STATE_WAIT_PRESSURE;


StateWaitPressure::StateWaitPressure() {
}

StateWaitPressure::~StateWaitPressure() {
}

void StateWaitPressure::transition(AbstractState* prevState) {
	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setCursor(120, 80);
	g_display->print("Filling pressure tank ...");
}

void StateWaitPressure::action() {
	if( g_maintButton ) {
		switchState(&STATE_MAINTENANCE_IDLE);
	}

	if( g_pressure < MIN_PRESSURE_FOR_START )
		g_compressor = true;

	if( !g_compressor )
		switchState(&STATE_LOWER_ROZEL);
}

void StateWaitPressure::refreshDisplay() {
	refreshStatusBar();
}
