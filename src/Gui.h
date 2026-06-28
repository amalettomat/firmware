/*
 * Gui.h
 *
 *  Created on: 20.09.2019
 *      Author: zwax
 */

#ifndef GUI_H_
#define GUI_H_

#include "layout.h"
// #include "Adafruit_ILI9486_Teensy.h"
#include "ILI9486_SPI.h"
#include "Adafruit_GFX.h"
#include "GfxSpinButtonField.h"
#include "GfxImageButton.h"


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// extern Adafruit_ILI9486_Teensy* g_display;
extern ILI9486_SPI* g_display;
extern int g_touchX;
extern int g_touchY;
extern bool g_touchPressed;


// state PREHEAT
extern Adafruit_GFX_Button g_btnSkip;

// state IDLE
// extern Adafruit_GFX_Button g_btnMaintenance;
extern GfxImageButton g_btnSelectFill1;
extern GfxImageButton g_btnSelectFill2;

// state MAINTENANCE
extern Adafruit_GFX_Button g_btnRozel;
// extern GfxSpinButtonField g_spinRozelPos;
extern Adafruit_GFX_Button g_btnBatterOnOff;
extern Adafruit_GFX_Button g_btnBatterDose;
extern GfxSpinButtonField g_spinBatterAmount;
extern Adafruit_GFX_Button g_btnPlateMotor;
extern GfxSpinButtonField g_spinPlateSpeed;
extern Adafruit_GFX_Button g_btnHeating;
extern GfxSpinButtonField g_spinTemp;
extern GfxSpinButtonField g_spinPrice;
extern GfxSpinButtonField g_spinBakingTime;
extern Adafruit_GFX_Button g_btnFilling1;
extern GfxSpinButtonField g_spinFillingTime1;
extern Adafruit_GFX_Button g_btnFilling2;
extern GfxSpinButtonField g_spinFillingTime2;
extern Adafruit_GFX_Button g_btnScrape;

extern Adafruit_GFX_Button g_btnExit;

extern const GFXfont* DEFAULT_FONT;
extern const GFXfont* BIG_FONT;

// functions

void clearMainArea();

#endif /* GUI_H_ */
