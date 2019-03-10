#include "BrakeNodeStates.h"

BrakeNodeState::BrakeNodeState(State s) : state_(s) {
}

void BrakeNodeState::CheckBoot() {
  if (0-Sol-1 == open and 0-Sol-2 == open and 1-Sol-1 == open and 2-Sol-1 == open and 3-Sol-1 == open and EnclosureTemp == nominal) {
    state_ = State::STANDBY;
  }
}

void BrakeNodeState::CheckStandby() {
	state_ = State::ARMING;
//  if (flightCompTriggered == true) {
//    state_ = State::ARMING;
//  }
//
//  if (communicationFail == true) {
//    state_ = State::ERROR;
//  }
}

void BrakeNodeState::CheckArming() {
	state_ = State::ARMED;
//  if (nodeValuesNomial == true) {
//    state_ = State::ARMED;
//  }
//
// if (flightCompCommunicationFail == true) {
//    state_ = State::VENTING;
// }
}

void BrakeNodeState::CheckArmed() {
	state_ = State::FLIGHT;
//  if (flightCompTriggered == true) {
//    state_ = State::FLIGHT;
//  }
//
//  if (flightCompCommunicationFail == true) {
//    state_ = State::VENTING;
//  }
}

void BrakeNodeState::CheckFlight() {
	state_ = State::BRAKING;
//  if (lossPinConnection or abnormalPressureValues or watchdogTimeout == true) {
//    state_ = State::BRAKING;
//  }
}

void BrakeNodeState::CheckBraking() {
//  if (flightCompTriggered == true) {
//    state_ = State::VENTING;
//  }
//
//  if (lossPinConnection and abnormalPressureValues and watchdogTimeout == false) {
//    state_ = State::FLIGHT;
//  }
}

void BrakeNodeState::CheckVenting() {
	state_ = State::ERROR;
//  if (ventingSuccessfull == true) {
//    state_ = State::RETRIEVAL;
//  }
//
//  if (ventingSuccessfull == false) {
//   state_ = State::ERROR;
//  }
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
