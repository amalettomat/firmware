/*
 * layout.h
 *
 * Defines for screen layout, e.g. colors.
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <Adafruit_GFX.h>

// RGB565 color convert
#define COLOR565( R, G, B ) (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3))


#define COL_BACKGROUND     0x0000
#define COL_LINES          0xFFFF
#define COL_STATUS_TEXT    0xFFFF
#define STATUS_AREA_YPOS   280

#define COL_BUTTON_INFILL  0x6B6D
#define COL_BUTTON_OUTLINE 0xFFFF
#define COL_BUTTON_TEXT    0xFFFF
#define TEXTSIZE_BUTTON    2
#define BUTTON_DEFAULT_HEIGHT 50

#define SPINBTN_DEFAULT_WIDTH  150
#define SPINBTN_DEFAULT_HEIGHT 50
#define SPINBTN_PADDING        5
#define SPINBTN_COL_TEXT       0xFFFF

#define MAINT_BUTTON_WIDTH   75


#endif /* LAYOUT_H_ */
