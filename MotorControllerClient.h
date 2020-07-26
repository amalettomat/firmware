/*
 * MotorControllerClient.h
 *
 *  Created on: 03.07.2020
 *      Author: zwax
 */

#ifndef MOTORCONTROLLERCLIENT_H_
#define MOTORCONTROLLERCLIENT_H_

#include "MotorController.h"
#include <Wire.h>


class MotorControllerClient {
public:
	MotorControllerClient();
	MotorControllerClient( int twiAddress );
	virtual ~MotorControllerClient();
	static void init();

	int stop();
	int home(int16_t speed);
	int moveTo(int32_t pos);
	int setSpeed(int16_t speed);
	bool updateStatus();

	ControllerData& getStatus() { return m_controllerData; };

protected:
	static void onReceive(int count);

	int sendCommand(char twiCommand);
	int sendCommand16(char twiCommand, int16_t param);
	int sendCommand32(char twiCommand, int32_t param);

private:
	int m_twiAddress;
	ControllerData m_controllerData;
};

#endif /* MOTORCONTROLLERCLIENT_H_ */
