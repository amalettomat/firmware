/*
 * Gui.cpp
 *
 * All GUI widgets are defined here
 *
 *  Created on: 20.09.2019
 *      Author: zwax
 */

#include "Gui.h"


// state PREHEAT
Adafruit_GFX_Button g_btnSkip;

// state MAINTENANCE
Adafruit_GFX_Button g_btnRozel;
GfxSpinButtonField g_spinRozelPos;
Adafruit_GFX_Button g_btnBatterOnOff;
Adafruit_GFX_Button g_btnBatterDose;
GfxSpinButtonField g_spinBatterAmount;
Adafruit_GFX_Button g_btnPlateMotor;
GfxSpinButtonField g_spinPlateSpeed;
Adafruit_GFX_Button g_btnHeating;
GfxSpinButtonField g_spinTemp;


// functions
// -------------------------------------------------------------------------

void clearMainArea() {
	g_display->fillRect(0, 0, 480, STATUS_AREA_YPOS-1, COL_BACKGROUND);
}
