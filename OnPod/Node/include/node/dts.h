#ifndef DTS_H
#define DTS_H

#include "Paradigm.pb.h"

struct DTSData {
    bool brakeSolenoidState = 1;
    bool ventSolenoidState = 2;
    float rotorTemperature = 3;
    float pneumaticTemperature = 4;
    float tankPressure = 5;
    float brakePressure = 6;
};

class DTS {
public:
    BrakeNodeStates state = BrakeNodeStates_bnsFlight;
    DTSData data;
};

#endif
