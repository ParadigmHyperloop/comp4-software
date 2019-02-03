#include "BrakeNodeStates.h"

BrakeNodeState::BrakeNodeState(State s) : state_s(s) {
}

void BrakeNodeState::CheckBoot() {
  
  BrakeNodeState s(State::STANDBY);
  s.TransitionToNextState;
}

void BrakeNodeState::CheckStandby() {
  
 BrakeNodeState s(State::ARMING);
 s.TransistionToNextState;

 BrakeNodeState s(State::ERROR);
 s.TransisitionToNextState;
}

void BrakeNodeState::CheckArming() {
 
 BrakeNodeState s(State::Armed);
 s.TransitionToNextState;
 
 BrakeNodeState s(State::Venting);
 s.TransitionToNextState; 
}

void BrakeNodeState::CheckArmed() {

  BrakeNodeState s(State::FLIGHT);
  s.TransitionToNextState;

  BrakeNodeState s(State::VENTING);
  s.TransitionToNextState;
}

void BrakeNodeState::CheckFlight() {

  BrakeNodeState s(State::BRAKING);
  s.TransitionToNextState;
}

void BrakeNodeState::CheckBraking() {

  BrakeNodeState s(State::VENTING);
  s.TransitionToNextState;

  BrakeNodeState s(State::FLIGHT);
  s.TransitionToNextState;
}

void BrakeNodeState::CheckVenting() {

  BrakeNodeState s(State::RETRIEVAL);
  s.TransitionToNextState;

  BrakeNodeState s(State::ERROR);
  s.TransitionToNextState;
}

void BrakeNodeState::CheckRetrieval() {

  
}

void BrakeNodeState::CheckError() {

  BrakeNodeState s(State::RETRIEVAL);
  s.TransitionToNextState;
}

void BrakeNodeState::TransitionToNextState() {
    if (state_==State::BOOT) {
        CheckBoot();
    }
    else if (state_==State::STANDBY) {
        CheckStandby();
    }
    else if (state_==Staet::ARMING) {
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
