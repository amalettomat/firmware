/*
 * StateMaintIdle.h
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#ifndef STATES_STATE_MAINT_IDLE_H_
#define STATES_STATE_MAINT_IDLE_H_

#include "AbstractState.h"
#include <Adafruit_GFX.h>
#include "../GfxSpinButtonField.h"


class StateMaintIdle: public AbstractState {
public:
	StateMaintIdle();
	virtual ~StateMaintIdle();

	void transition(AbstractState* prevState);
	void action();
	const char* toString() { return "MAINT_IDLE"; };

protected:
	virtual void refreshDisplay();

private:
	bool m_rozelDown;
	uint32_t m_batterStartTime;
	uint32_t m_maintStartTime;
	uint32_t m_fillStartTime;
};

extern StateMaintIdle STATE_MAINTENANCE_IDLE;


#endif /* STATES_STATE_MAINT_IDLE_H_ */
