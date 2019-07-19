#include <math.h>
#include "TaxiBrakingCriteria.hpp"


//TaxiBrakingCriteria::TaxiBrakingCriteria(){}

TaxiBrakingCriteria::TaxiBrakingCriteria(
    uint32_t targetDistance,
    uint32_t targetVelocity,
//    TelemetryManager* telemetryManager,
    bool shouldBrakeAfter)
    :_targetDistance(targetDistance)//, _telemManager(telemetryManager)
{
  _targetVelocityInRPM = 0; //convertTargetSpeedToRPM(targetVelocity);
  _brakingMethod = shouldBrakeAfter ? BrakingMethod::BRAKES : BrakingMethod::COASTING;
  _motorDistanceToBrake = 0; //getTargetDistanceToBrake(targetDistance);
}

TaxiBrakingCriteria::~TaxiBrakingCriteria() {}


bool TaxiBrakingCriteria::ShouldEndTaxi() {
  this->updateMotorProfile();
  //return _motorDistanceToBrake >= _telemManager->telemetry->motorDistance;
}

void TaxiBrakingCriteria::updateMotorProfile()
{
  uint32_t offset = 5;
  /*
  if (_targetVelocityInRPM  < (this->_telemManager->telemetry->motorSpeed - offset))
  {
    break;// TODO Motor Speed to increase
  } else if (_targetVelocityInRPM >= (this->_telemManager->telemetry->motorSpeed + offset ))
  {
    break;// TODO Turn Motor off
  }
   */
}


PodStates TaxiBrakingCriteria::NextStateToTransitionTo() {
  switch(_brakingMethod)
  {
    case BrakingMethod::BRAKES:
      return PodStates::psBraking;
    case BrakingMethod ::COASTING:
      return PodStates::psCoasting;
  }
}


int32_t TaxiBrakingCriteria::convertTargetSpeedToRPM(uint32_t targetSpeed) {
  // Circumference / (motorSpeed * 60s)
  float rpmSpeed = (GENERAL_CONSTANTS::REAR_WHEEL_CIRCUMFRENCE ) / (targetSpeed * 60);
  // TODO: THIS IS TRUNCATED TO 0!!!!!!!
  return rpmSpeed;
}


uint32_t TaxiBrakingCriteria::getTargetDistanceToBrake(uint32_t targetVelocity) {
  uint32_t decelerationRate = this->_brakingMethod == BrakingMethod::BRAKES ? 1: 2;

  uint32_t distanceToBrake = (pow(targetVelocity, 2) / (2 * decelerationRate));
  uint32_t totalTargetDistance = _targetDistance - distanceToBrake;
  return totalTargetDistance;
}