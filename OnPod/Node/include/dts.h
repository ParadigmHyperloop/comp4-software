#ifndef DTS_H
#define DTS_H

enum DTSState {
    STATE_STANDBY,
    STATE_ACCELERATING,
    STATE_BRAKING,
    STATE_VENTING,
};

struct DTSData {
    bool bBrakeSolenoidState = 0;
    bool bVentSolenoidState = 0;
    float fRotorTemperature = 0;
};

class DTS {
public:
    DTSState state = STATE_STANDBY;
    DTSData data;
};

#endif
