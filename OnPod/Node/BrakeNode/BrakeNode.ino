BrakeNodeState::TransitionToNextState() {
  if (state_==STATE::BOOT) {
    CheckBoot();
  }
  else if (state_==STATE::STANDBY) {
    CheckStandby();
  }
  else if (state_==STATE::Arming) {
    CheckArming();
  }
  else if (state_==STATE::Armed) {
    CheckArmed();
  }
  else if (state_==STATE::FLight) {
    CheckFlight();
  }
  else if (state_==STATE::Braking) {
    CheckBraking();
  }
  else if (state_==STATE::Venting) {
    CheckVenting();
  }
  else if (state_==STATE::Retrieval) {
    CheckRetrieval();
  }
  else if (state_==STATE::Error) {
    CheckError();
  }
}
