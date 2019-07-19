#ifndef FLIGHTCOMPUTER_ITAXIBRAKINGCRITERIA_HPP
#define FLIGHTCOMPUTER_ITAXIBRAKINGCRITERIA_HPP

#include "States.h"

class ITaxiBrakingCriteria
{
  ITaxiBrakingCriteria();
  virtual ~ITaxiBrakingCriteria() = default;
  virtual bool ShouldEndTaxi() = 0;
  virtual PodStates NextStateToTransitionTo() = 0;
};

#endif //FLIGHTCOMPUTER_ITAXIBRAKINGCRITERIA_HPP
