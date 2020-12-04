/*
 * Gui.h
 *
 *  Created on: 20.09.2019
 *      Author: zwax
 */

#ifndef GUI_H_
#define GUI_H_

#include "layout.h"
#include "Adafruit_ILI9486_Teensy.h"
#include "GfxSpinButtonField.h"


extern Adafruit_ILI9486_Teensy* g_display;
extern int g_touchX;
extern int g_touchY;
extern bool g_touchPressed;


// state PREHEAT
extern Adafruit_GFX_Button g_btnSkip;

// state IDLE
extern Adafruit_GFX_Button g_btnMaintenance;

// state MAINTENANCE
extern Adafruit_GFX_Button g_btnRozel;
extern GfxSpinButtonField g_spinRozelPos;
extern Adafruit_GFX_Button g_btnBatterOnOff;
extern Adafruit_GFX_Button g_btnBatterDose;
extern GfxSpinButtonField g_spinBatterAmount;
extern Adafruit_GFX_Button g_btnPlateMotor;
extern GfxSpinButtonField g_spinPlateSpeed;
extern Adafruit_GFX_Button g_btnHeating;
extern GfxSpinButtonField g_spinTemp;

// functions

void clearMainArea();

#endif /* GUI_H_ */
