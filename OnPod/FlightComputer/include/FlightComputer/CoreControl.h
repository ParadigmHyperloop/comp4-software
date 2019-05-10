#ifndef CORECONTROL_H
#define CORECONTROL_H

#include "FlightComputer/Structs.h"

void commonChecks(Pod podValues);

void bootingTransitions(Pod);

void standyTransitions(Pod);

void armingTransitions(Pod);

void armedTransitions(Pod);

void preflightTransitions(Pod);

void accelerationTransitions(Pod);

void coastingTranstions(Pod);

void brakingTransitons(Pod);

void disarmTransitions(Pod);

void retrievalTransitions(Pod);

void coreControlLoop(Pod);

#endif
