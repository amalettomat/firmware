/*
 * MotorControllerClient.cpp
 *
 *  Created on: 03.07.2020
 *      Author: zwax
 */

#include "MotorControllerClient.h"
#include <Wire.h>
#include <Arduino.h>


MotorControllerClient::MotorControllerClient() {
	m_twiAddress = MOTORCONTROLLER_TWI_ADDRESS;
}

MotorControllerClient::MotorControllerClient(int twiAddress) {
	m_twiAddress = twiAddress;
}

MotorControllerClient::~MotorControllerClient() {
}


int MotorControllerClient::stop() {
	return sendCommand(TWI_CMD_STOP);
}

int MotorControllerClient::home(int16_t speed) {
	m_controllerData.setFlag(STATUS_HOMING);

	int result = sendCommand16(TWI_CMD_SETSPEED, speed);
	if( result != 0 ) {
		return result;
	}
	delay(50);
	return sendCommand16(TWI_CMD_HOME, speed);
}

int MotorControllerClient::moveTo(int32_t pos) {
	return sendCommand32(TWI_CMD_MOVE_TO, pos);
}

int MotorControllerClient::setSpeed(int16_t speed) {
	return sendCommand16(TWI_CMD_SETSPEED, speed);
}

bool MotorControllerClient::updateStatus() {
	Wire.requestFrom(m_twiAddress, sizeof(m_controllerData));

	int count = Wire.readBytes((char*)&m_controllerData, sizeof(m_controllerData));
	return count == sizeof(m_controllerData);
}

void MotorControllerClient::init() {
	Wire.onReceive(onReceive);
}

void MotorControllerClient::onReceive(int count) {
	Serial.print("Received: ");

	for( int i=0; i < count; i++ ) {
		byte b = Wire.read();
		Serial.print(b, HEX);
		Serial.print(" ");
	}
	Serial.println();
}

int MotorControllerClient::sendCommand(char twiCommand) {
	Wire.beginTransmission(m_twiAddress);
	Wire.print(twiCommand);
	return Wire.endTransmission();
}

int MotorControllerClient::sendCommand16(char twiCommand, int16_t param) {
	Wire.beginTransmission(m_twiAddress);
	Wire.print(twiCommand);
	char* buffer = (char*)&param;
	Wire.print(buffer[0]);
	Wire.print(buffer[1]);
	return Wire.endTransmission();
}

int MotorControllerClient::sendCommand32(char twiCommand, int32_t param) {
	Wire.beginTransmission(m_twiAddress);
	Wire.print(twiCommand);
	char* buffer = (char*)&param;
	Wire.print(buffer[0]);
	Wire.print(buffer[1]);
	Wire.print(buffer[2]);
	Wire.print(buffer[3]);
	return Wire.endTransmission();
}


