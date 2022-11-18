#include "StateInsertCoins.h"
#include "StateIdle.h"
#include "StateMaintIdle.h"
#include "../Gui.h"
#include "../LedControl.h"


extern float g_price;
extern float g_credit;
extern LedControl g_ledController;
// extern bool g_showMaint;
extern bool g_maintButton;


StateInsertCoins STATE_INSERT_COINS;


StateInsertCoins::StateInsertCoins() {
}

StateInsertCoins::~StateInsertCoins() {
}

void StateInsertCoins::transition(AbstractState* prevState) {
//	g_btnSkip.initButtonUL(g_display, 100, 100, 150, 50, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "SKIP", TEXTSIZE_BUTTON);
//	g_btnSkip.drawButton(false);

	clearMainArea();
	g_display->setTextColor(COL_STATUS_TEXT);
	g_display->setFont(BIG_FONT);
	g_display->setTextSize(1);
	g_display->setCursor(40, 80);
	g_display->print("Please insert Coins!");

	g_display->setFont(DEFAULT_FONT);
//	g_btnMaintenance.initButtonUL(g_display, 380, 235, 90, 30, COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT, "MAINT", TEXTSIZE_BUTTON);
//	if( g_showMaint ) {
//		g_btnMaintenance.drawButton(false);
//	}

	g_ledController.setPattern(LedControl::LEDS_WAVE);
}

void StateInsertCoins::action() {

	if( g_maintButton )
		switchState(&STATE_MAINTENANCE_IDLE);

	if( g_credit >= g_price )
		switchState(&STATE_IDLE);

//	static bool prevShowMaint = g_showMaint;
//	if( g_showMaint != prevShowMaint ) {
//		prevShowMaint = g_showMaint;
//		g_btnMaintenance.drawButton(false);
//	}

//	if( g_touchPressed ) {
//		if( g_showMaint )
//			g_btnMaintenance.press(g_btnMaintenance.contains(g_touchX, g_touchY));
//	} else {
//		g_btnMaintenance.press(false);
//	}
//
//	if( g_btnMaintenance.justReleased() )
//		switchState(&STATE_MAINTENANCE_IDLE);
//	else if( g_btnSkip.justPressed() )
//		g_btnMaintenance.drawButton(true);

//	if( !g_heating )
//		switchState(&STATE_MAINTENANCE_IDLE);

//	if( g_touchPressed ) {
//		g_btnSkip.press(g_btnSkip.contains(g_touchX, g_touchY));
//	} else {
//		g_btnSkip.press(false);
//	}
//
//	if( g_btnSkip.justReleased() )
//		switchState(&STATE_IDLE);
//	else if( g_btnSkip.justPressed() )
//		g_btnSkip.drawButton(true);

}

void StateInsertCoins::refreshDisplay() {
	refreshStatusBar();
}
