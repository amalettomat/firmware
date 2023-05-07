/*
 * BmpFile.cpp
 *
 *  Created on: 07.02.2019
 *      Author: zwax
 */

#include "BmpFile.h"


BmpFile::BmpFile() {
}

BmpFile::~BmpFile() {
	m_imgFile.close();
}

/**
 * Open image file and read headers
 */
bool BmpFile::open(const char* path) {
	if( !SD.exists(path) ) {
		return false;
	}

	m_imgFile = SD.open(path, FILE_READ);
	if( !m_imgFile ) {
		return false;
	}

	m_imgFile.read(&m_fileHeader, sizeof(m_fileHeader));
	m_imgFile.read(&m_imageHeader, sizeof(m_imageHeader));

	if( m_imageHeader.bits_per_pixel != 16 ) {
		Serial.println("Wrong BMP format! Require 16 bits/pixel");
		m_imgFile.close();
		return false;
	}

	return true;
}

uint16_t* BmpFile::readLine() {
	if( !m_imgFile.available() ) {
		Serial.println("BMP file not open!");
		return NULL;
	}

	m_imgFile.read(m_imgData, NUM_LINES_IN_BUFFER * IMG_WIDTH * sizeof(uint16_t));
	return m_imgData;
}
