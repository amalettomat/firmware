/*
 * RunningAverage.h
 *
 *  Created on: 06.02.2019
 *      Author: zwax
 */

#ifndef RUNNINGAVERAGE_H_
#define RUNNINGAVERAGE_H_


template <typename ValueType>
class RunningAverage {
public:
	RunningAverage(int size) : m_size(size), m_startIndex(0), m_sum(0), m_count(0) {
		m_buffer = new ValueType[size];
	};

	~RunningAverage() {
		delete[] m_buffer;
	}

	void addValue( ValueType value ) {
		if( m_count == m_size ) {
			// buffer full, overwrite entry
			ValueType oldValue = m_buffer[m_startIndex];
			m_sum -= oldValue;
		} else {
			m_count++;
		}
		m_buffer[m_startIndex] = value;
		m_startIndex++;
		if( m_startIndex == m_size )
			m_startIndex = 0;
		m_sum += value;
	};

	ValueType getAverage() {
		return m_sum / m_count;
	}

private:
	ValueType* m_buffer;
	int m_size;
	int m_startIndex;
	ValueType m_sum;
	int m_count;
};

#endif /* RUNNINGAVERAGE_H_ */
