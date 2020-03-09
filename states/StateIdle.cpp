/*
 * StateIdle.cpp
 *
 *  Created on: 09.08.2019
 *      Author: zwax
 */

#include "StateIdle.h"
#include "../Gui.h"


StateIdle STATE_IDLE;


StateIdle::StateIdle() {
}

StateIdle::~StateIdle() {
}

void StateIdle::transition(AbstractState* prevState) {
	clearMainArea();
}

void StateIdle::action() {
}

void StateIdle::refreshDisplay() {
	refreshStatusBar();
}

