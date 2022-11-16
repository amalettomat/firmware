#include "StatePreheat.h"
#include "StateInsertCoins.h"
#include "../Gui.h"
#include "../LedControl.h"


extern bool g_heating;
extern LedControl g_ledController;


StatePreheat STATE_PREHEAT;


StatePreheat::StatePreheat() {
}

StatePreheat::~StatePreheat() {
}

void StatePreheat::transition(AbstractState* prevState) {
	g_heating = true;

	g_btnSkip.initButtonUL(g_display, 165, 100, 150, 50, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "SKIP", TEXTSIZE_BUTTON);
	g_btnSkip.drawButton(false);

	g_ledController.setPattern(LedControl::LEDS_WAVE);
}

void StatePreheat::action() {
	if( !g_heating )
		switchState(&STATE_INSERT_COINS);

	if( g_touchPressed ) {
		g_btnSkip.press(g_btnSkip.contains(g_touchX, g_touchY));
	} else {
		g_btnSkip.press(false);
	}

	if( g_btnSkip.justReleased() )
		switchState(&STATE_INSERT_COINS);
	else if( g_btnSkip.justPressed() )
		g_btnSkip.drawButton(true);

//	if( g_btnSkip.justReleased() ) {
//		g_btnSkip.drawButton(false);
//		g_rozelController.moveToPos(0);
//	} else if( g_btnSkip.justPressed() ){
//		g_btnSkip.drawButton(true);
//		g_rozelController.moveToPos(1000);
//	}
}

void StatePreheat::refreshDisplay() {
	refreshStatusBar();
}
