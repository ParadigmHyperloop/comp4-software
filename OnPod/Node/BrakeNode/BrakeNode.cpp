#include "BrakeNodeStates.h"

BrakeNodeState::BrakeNodeState(State s) : state_(s) {
	if (flagStruct_.heartbeatValid == true){
		state_= State::STANDBY;
	}
}

void BrakeNodeState::CheckBoot() {

}

void BrakeNodeState::CheckStandby() {
	state_ = State::ARMING;

}

void BrakeNodeState::CheckArming() {
	state_ = State::ARMED;

}

void BrakeNodeState::CheckArmed() {
	state_ = State::FLIGHT;

}

void BrakeNodeState::CheckFlight() {
	state_ = State::BRAKING;

}

void BrakeNodeState::CheckBraking() {

}

void BrakeNodeState::CheckVenting() {
	state_ = State::ERROR;

}

void BrakeNodeState::CheckRetrieval() {
  
}

void BrakeNodeState::CheckError() {
  state_ = State::RETRIEVAL;
}

void BrakeNodeState::TransitionToNextState() {
    if (state_==State::BOOT) {
        CheckBoot();
    }
    else if (state_==State::STANDBY) {
        CheckStandby();
    }
    else if (state_==State::ARMING) {
        CheckArming();
    }
    else if (state_==State::ARMED) {
        CheckArmed();
    }
    else if (state_==State::FLIGHT) {
        CheckFlight();
    }
    else if (state_==State::BRAKING) {
        CheckBraking();
    }
    else if (state_==State::VENTING) {
        CheckVenting();
    }
    else if (state_==State::RETRIEVAL) {
        CheckRetrieval();
    }
    else if (state_==State::ERROR) {
        CheckError();
    }
}

State BrakeNodeState::GetState() {
  return state_;
}
