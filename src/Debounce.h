#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include "Arduino.h"

class Debounce {
public:
	explicit Debounce(uint32_t delay = 50, bool initialValue = false) : m_delay(delay), m_time(0), m_value(initialValue) {}

	bool update(bool raw) {
		if (raw != m_value) {
			if (m_time == 0)
				m_time = millis();
			else if (millis() - m_time >= m_delay)
				m_value = raw;
		} else {
			m_time = 0;
		}
		return m_value;
	}

	bool value() const { return m_value; }

private:
	uint32_t m_delay;
	uint32_t m_time;
	bool m_value;
};

#endif /* DEBOUNCE_H_ */
