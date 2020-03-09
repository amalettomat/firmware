/*
 * GfxSpinButtonField.cpp
 *
 *  Created on: 19.09.2019
 *      Author: zwax
 */

#include "GfxSpinButtonField.h"
#include "layout.h"


GfxSpinButtonField::GfxSpinButtonField() : m_gfx(NULL), m_x1(0), m_y1(0),
                                           m_width(SPINBTN_DEFAULT_WIDTH), m_height(SPINBTN_DEFAULT_HEIGHT),
										   m_textX(0), m_textY(0),
                                           m_value(0.0), m_step(1.0), m_min(-100000.0), m_max(10000.0), m_format("%5.1f") {
	m_btnInc = new Adafruit_GFX_Button();
	m_btnDec = new Adafruit_GFX_Button();
}

GfxSpinButtonField::~GfxSpinButtonField() {
	delete m_btnInc;
	delete m_btnDec;
}

void GfxSpinButtonField::init(Adafruit_GFX* gfx, int x1, int y1, float value, float step, float min, float max, const char* format) {
	m_gfx = gfx;
	m_x1 = x1;
	m_y1 = y1;
	m_value = value;
	m_step = step;
	m_min = min;
	m_max = max;
	strncpy(m_format, format, 10);

	int btnSize = m_height - 2 * SPINBTN_PADDING;
	m_btnDec->initButtonUL(gfx, x1+SPINBTN_PADDING, y1+SPINBTN_PADDING, btnSize, btnSize,
			               COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
						   "-", 3);
	m_btnInc->initButtonUL(gfx, x1+m_width-btnSize-SPINBTN_PADDING, y1+SPINBTN_PADDING, btnSize, btnSize,
			               COL_BUTTON_OUTLINE, COL_BUTTON_INFILL, COL_BUTTON_TEXT,
						   "+", 3);

    char output[20];
    snprintf(output, 20, m_format, min);
    int16_t x, y;
    uint16_t txtWidth, txtHeight;
    m_gfx->setTextSize(2, 2);
    m_gfx->getTextBounds(output, 0, 0, &x, &y, &txtWidth, &txtHeight);
	m_textX = x1 + m_width/2 - txtWidth/2;
	m_textY = y1 + m_height/2 - txtHeight/2;
}

void GfxSpinButtonField::draw() {
	m_btnInc->drawButton(false);
	m_btnDec->drawButton(false);
    m_gfx->drawRoundRect(m_x1, m_y1, m_width, m_height, 3, COL_BUTTON_OUTLINE);
    refreshText();
}

void GfxSpinButtonField::refreshText() {
    char output[20];
    snprintf(output, 20, m_format, m_value);
    m_gfx->setCursor(m_textX, m_textY);
    m_gfx->setTextColor(SPINBTN_COL_TEXT, COL_BACKGROUND);
    m_gfx->setTextSize(2, 2);
    m_gfx->print(output);
}

bool GfxSpinButtonField::handleTouch(bool touched, int touchX, int touchY) {
	if( !touched ) {
		m_btnInc->press(false);
		m_btnDec->press(false);
	} else {
		m_btnInc->press(m_btnInc->contains(touchX, touchY));
		m_btnDec->press(m_btnDec->contains(touchX, touchY));
	}

	if( m_btnInc->justPressed() ) {
		m_value += m_step;
		if( m_value > m_max )
			m_value = m_max;
		refreshText();
		return true;
	}

	if (m_btnDec->justPressed() ) {
		m_value -= m_step;
		if( m_value < m_min )
			m_value = m_min;
		refreshText();
		return true;
	}

	return false;
}
