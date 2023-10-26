/*
 * Gui.cpp
 *
 * All GUI widgets are defined here
 *
 *  Created on: 20.09.2019
 *      Author: zwax
 */

#include "Gui.h"
// #include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans24pt7b.h"


// state PREHEAT
Adafruit_GFX_Button g_btnSkip;

// state IDLE
// Adafruit_GFX_Button g_btnMaintenance;
GfxImageButton g_btnSelectFill1;
GfxImageButton g_btnSelectFill2;

// state MAINTENANCE
Adafruit_GFX_Button g_btnRozel;
// GfxSpinButtonField g_spinRozelPos;
Adafruit_GFX_Button g_btnBatterOnOff;
Adafruit_GFX_Button g_btnBatterDose;
GfxSpinButtonField g_spinBatterAmount;
Adafruit_GFX_Button g_btnPlateMotor;
GfxSpinButtonField g_spinPlateSpeed;
Adafruit_GFX_Button g_btnHeating;
GfxSpinButtonField g_spinTemp;
GfxSpinButtonField g_spinPrice;
GfxSpinButtonField g_spinBakingTime;
Adafruit_GFX_Button g_btnFilling1;
GfxSpinButtonField g_spinFillingTime1;
Adafruit_GFX_Button g_btnScrape;


Adafruit_GFX_Button g_btnExit;

// const GFXfont* DEFAULT_FONT = &FreeSans12pt7b;
const GFXfont* DEFAULT_FONT = &FreeSans9pt7b;
const GFXfont* BIG_FONT = &FreeSans24pt7b;


// functions
// -------------------------------------------------------------------------

void clearMainArea() {
	g_display->fillRect(0, 0, 480, STATUS_AREA_YPOS-1, COL_BACKGROUND);
}

