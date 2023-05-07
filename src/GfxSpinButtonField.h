/*
 * GfxSpinButtonField.h
 *
 *  Created on: 19.09.2019
 *      Author: zwax
 */

#ifndef GFXSPINBUTTONFIELD_H_
#define GFXSPINBUTTONFIELD_H_


class Adafruit_GFX;
class Adafruit_GFX_Button;


class GfxSpinButtonField {
public:
	GfxSpinButtonField();
	virtual ~GfxSpinButtonField();

	void init(Adafruit_GFX* gfx, int x1, int y1, float value, float step, float min, float max, const char* format);
	void draw();

	// return true if value changed
	bool handleTouch(bool touched, int touchX, int touchY);

	float getValue() { return m_value; };
	void setValue(float value) { m_value = value; };

protected:
	void refreshText();

private:
	Adafruit_GFX *m_gfx;
	int m_x1;
	int m_y1;
	int m_width;
	int m_height;
	int m_textX;
	int m_textY;
	float m_value;
	float m_step;
	float m_min;
	float m_max;
	char m_format[11];
	Adafruit_GFX_Button* m_btnInc;
	Adafruit_GFX_Button* m_btnDec;
};

#endif /* GFXSPINBUTTONFIELD_H_ */
