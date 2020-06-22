/*
 * MotorControllerClient.h
 *
 *  Created on: 05.04.2020
 *      Author: zwax
 */

#ifndef MOTORCONTROLLERCLIENT_H_
#define MOTORCONTROLLERCLIENT_H_

#include "../MotorController/MotorController.h"
#include <sys/types.h>
#include <Wire.h>


class MotorControllerClient {
public:
	MotorControllerClient();
	virtual ~MotorControllerClient();

	void runAtSpeed(int16_t speed);
	void stop();
	void startHoming();
	void moveToPos(int32_t pos);
	void fetchControllerData();

protected:
	void sendCommand(uint8_t cmd);
	void sendCommandParam16(uint8_t cmd, int16_t param);
	void sendCommandParam32(uint8_t cmd, int32_t param);

public:
	ControllerData m_controllerData;
	size_t m_numOfBytes;
};

#endif /* MOTORCONTROLLERCLIENT_H_ */
