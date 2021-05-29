/*
 * GfxImageButton.h
 *
 *  Created on: 29.05.2021
 *      Author: zwax
 */

#ifndef GFXIMAGEBUTTON_H_
#define GFXIMAGEBUTTON_H_

#include <Adafruit_GFX.h>


class GfxImageButton: public Adafruit_GFX_Button {
public:
	GfxImageButton();
	virtual ~GfxImageButton();

	void initButtonUL(Adafruit_GFX *gfx, int16_t x1,
	                  int16_t y1, uint16_t w, uint16_t h,
	                  uint16_t outline, uint16_t bgcolor, uint16_t radius);
	void drawButton(bool inverted);

protected:
	Adafruit_GFX *_gfx;
	int16_t _x1, _y1; // Coordinates of top-left corner
	uint16_t _w, _h;
	uint16_t _outlinecolor;
	uint16_t _bgcolor;
	uint16_t _radius;
};

#endif /* GFXIMAGEBUTTON_H_ */
