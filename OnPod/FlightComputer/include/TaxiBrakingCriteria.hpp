#ifndef FLIGHTCOMPUTER_TAXIBRAKINGCRITERIA_HPP
#define FLIGHTCOMPUTER_TAXIBRAKINGCRITERIA_HPP

#include "TelemetryManager.h"
#include "easylogging++.h"
#include "ITaxiBrakingCriteria.hpp"

class TaxiBrakingCriteria: ITaxiBrakingCriteria {


public:
  enum class BrakingMethod {
    COASTING,
    BRAKES
  };

  /*
   * @params:
   *  - targetDistance - total distance to travel ( in meters)
   *  - targetVelocity - Target speed to coast at (in m/s)
   */
public:
  TaxiBrakingCriteria();
  TaxiBrakingCriteria(uint32_t targetDistance,
      uint32_t targetSpeed,
      TelemetryManager* telemetryManager,
      bool shouldBrakeAfter);
  ~TaxiBrakingCriteria();
  bool ShouldEndTaxi();
  PodStates NextStateToTransitionTo();
  void updateMotorProfile();

protected:
  int32_t convertTargetSpeedToRPM(uint32_t targetSpeed);

  // in Meters
  uint32_t getTargetDistanceToBrake(uint32_t targetVelocity);

private:
  TelemetryManager* _telemManager;
  BrakingMethod _brakingMethod;
  uint32_t _motorDistanceToBrake;

  uint32_t _targetDistance; // meters.
  int32_t _targetVelocityInRPM;  // RPM

};

#endif //FLIGHTCOMPUTER_TAXIBRAKINGCRITERIA_HPP