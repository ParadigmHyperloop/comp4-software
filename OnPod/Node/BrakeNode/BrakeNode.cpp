#include "BrakeNodeStates.h"

BrakeNodeState::BrakeNodeState(State s) : state_(s) {
}

void BrakeNodeState::CheckBoot() {
	if (flagStruct_.setupFail == true){
    if (flagStruct_.nodeConnectFC == true) {
      state_=State::STANDBY;
    }
    else {
      state_ = State::BOOT;
    }
	}
	else {
		state_ = State::ERROR;
	}
}

void BrakeNodeState::CheckStandby() {
  if (flagStruct_.heartbeatValid == true and flagStruct_.sensorsValid == true) {
    if (flagStruct_.FCstate_ == flightComputerStates::FCARMING) {
      state_ = State::ARMING;
    }
    else {
      state_ = State::STANDBY;
    }
  }
  else {
    state_ = State::ERROR;
  }
}

void BrakeNodeState::CheckArming() {
  if (flagStruct_.heartbeatValid == true and flagStruct_.sensorsValid == true) {
    if (flagStruct_.FCstate_ == flightComputerStates::FCVENTING) {
      state_ = State::VENTING;
    }
    else {
      state_ = State::ARMED;
    }
  }
  else {
    state_ = State::ERROR;
  }
}

void BrakeNodeState::CheckArmed() {
  if (flagStruct_.heartbeatValid == true and flagStruct_.sensorsValid == true){
    if (flagStruct_.FCstate_ == flightComputerStates::FCVENTING) {
      state_ = State::VENTING;
    }
    else if (flagStruct_.FCstate_ == flightComputerStates::FCPREFLIGHT) {
      state_ = State::FLIGHT;
    }
    else {
      state_ = State::ARMED;
    }
  }
  else {
    state_ = State::ERROR;
  }
}

void BrakeNodeState::CheckFlight() {
  if (flagStruct_.heartbeatValid == true and flagStruct_.sensorsValid == true and flagStruct_.estop == false) {
    state_ = State::FLIGHT;
  }
  else {
    state_ = State::BRAKING;
  }
}

void BrakeNodeState::CheckBraking() {
  if (flagStruct_.FCstate_ == flightComputerStates::FCVENTING) {
    state_ = State::VENTING; 
  }
  else {
    state_ = State::BRAKING;
  }
}

void BrakeNodeState::CheckVenting() {
  if (flagStruct_.taxiCommand == false) {
    if (flagStruct_.sensorsValid == true){
      if (flagStruct_.moveToRetrieval == true) {
        state_ = State::RETRIEVAL;
      }
      else {
        state_ = State::VENTING;
      }
    }
    else {
      state_ = State::ERROR;
    }
  }
  else {
    state_ = State::FLIGHT;
  }
}

void BrakeNodeState::CheckRetrieval() {
  
}

void BrakeNodeState::CheckError() {
  if (flagStruct_.moveToRetrieval == true) {
    state_ = State::RETRIEVAL;
  }
  else {
    state_ = State::ERROR;
  }

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
