/*
 * GfxImageButton.cpp
 *
 *  Created on: 29.05.2021
 *      Author: zwax
 */

#include "GfxImageButton.h"
#include <cstring>


GfxImageButton::GfxImageButton() {
	_gfx = 0;
	_radius = 2;
	_outlinecolor = 0;
	_bgcolor = 0;
	_h = 0;
	_w = 0;
	_x1 = 0;
	_y1 = 0;
}

GfxImageButton::~GfxImageButton() {
}


void GfxImageButton::initButtonUL(Adafruit_ILI9486_Teensy *gfx, const BmpImage& image,
		                          int16_t x1, int16_t y1, uint16_t w, uint16_t h,
                                  uint16_t outline, uint16_t bgcolor, uint16_t radius) {
	int xPad = (w - image.width) / 2;
	if( xPad < 0 )
		xPad = 0;
	int yPad = (h - image.height) / 2;
	if( yPad < 0 )
		yPad = 0;
	gfx->drawRGBBitmap_fast(x1+xPad, y1+yPad, (const uint8_t*)(image.pixel_data), image.width, image.height);

	Adafruit_GFX_Button::initButtonUL(gfx, x1, y1, w, h, outline, bgcolor, 0, "", 1);
	_h = h;
	_w = w;
	_x1 = x1;
	_y1 = y1;
	_outlinecolor = outline;
	_bgcolor = bgcolor;
	_radius = radius;
	_gfx = gfx;
}

void GfxImageButton::drawButton(bool inverted) {
  if (inverted) {
	  _gfx->drawRoundRect(_x1, _y1, _w, _h, _radius, _outlinecolor);
	  _gfx->drawRoundRect(_x1+1, _y1+1, _w-2, _h-2, _radius-1, _outlinecolor);
	  _gfx->drawRoundRect(_x1+2, _y1+2, _w-4, _h-4, _radius-2, _outlinecolor);
  } else {
	  _gfx->drawRoundRect(_x1, _y1, _w, _h, _radius, _bgcolor);
	  _gfx->drawRoundRect(_x1+1, _y1+1, _w-2, _h-2, _radius-1, _bgcolor);
	  _gfx->drawRoundRect(_x1+2, _y1+2, _w-4, _h-4, _radius-2, _bgcolor);
  }
}
