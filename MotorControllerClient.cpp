/*
 * MotorControllerClient.cpp
 *
 *  Created on: 05.04.2020
 *      Author: zwax
 */

#include "MotorControllerClient.h"


MotorControllerClient::MotorControllerClient() {
}

MotorControllerClient::~MotorControllerClient() {
}

void MotorControllerClient::runAtSpeed(int16_t speed) {
	sendCommandParam16(TWI_CMD_RUN, speed);
}

void MotorControllerClient::startHoming() {
	sendCommand(TWI_CMD_HOME);
}

void MotorControllerClient::stop() {
	sendCommand(TWI_CMD_STOP);
}
void MotorControllerClient::moveToPos(int32_t pos) {
	sendCommandParam32(TWI_CMD_MOVE_TO, pos);
}

void MotorControllerClient::sendCommand(uint8_t cmd) {
	Wire.beginTransmission(MOTORCONTROLLER_TWI_ADDRESS);
	Wire.write(cmd);
	Wire.endTransmission();
}

void MotorControllerClient::sendCommandParam16(uint8_t cmd, int16_t param) {
	Wire.beginTransmission(MOTORCONTROLLER_TWI_ADDRESS);
	Wire.write(cmd);
	Wire.write( param & 0xFF );
	Wire.write( (param >> 8) & 0xFF );
	Wire.endTransmission();
}

void MotorControllerClient::sendCommandParam32(uint8_t cmd, int32_t param) {
	Wire.beginTransmission(MOTORCONTROLLER_TWI_ADDRESS);
	Wire.write(cmd);
	Wire.write( param & 0xFF );
	Wire.write( (param >> 8) & 0xFF );
	Wire.write( (param >> 16) & 0xFF );
	Wire.write( (param >> 24) & 0xFF );
	Wire.endTransmission();
}

void MotorControllerClient::fetchControllerData() {
	Wire.requestFrom(MOTORCONTROLLER_TWI_ADDRESS, sizeof(ControllerData));
	m_numOfBytes = Wire.readBytes((uint8_t*)&m_controllerData, (size_t)sizeof(ControllerData));
}
