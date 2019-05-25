#ifndef FLIGHTCOMPUTER_STATES_H
#define FLIGHTCOMPUTER_STATES_H

#include "TelemetryManager.h"

class TelemetryManager;

class PodState{

public:
    PodState();

    PodState(TelemetryManager*);

    PodStates getStateValue();

    virtual bool testTransitions();

    PodStates getNewState();

    bool isTransitioning();

    BrakeNodeStates getBrakeNodeState();

    LvdcNodeStates getLvdcNodeState();

    const std::string getTransitionReason();

    unsigned int getTimeInStateMilis();

    static std::unique_ptr<PodState> createState(PodStates);

    void setupTransition(PodStates, const std::string&);


protected:
    std::chrono::steady_clock::time_point _enterStateTime;
    bool _transitioning = false;
    std::string _transitionReason = "";
    PodStates _stateIdentifier = psBooting;
    PodStates _nextStateIdentifier = psBooting;
    BrakeNodeStates _brakeNodeState = bnsBooting;
    LvdcNodeStates  _lvdcNodeState = lvdcBooting;
    TelemetryManager* pod;
};


class Booting: public PodState{
public:
    Booting(){
        _stateIdentifier = psBooting;
        _brakeNodeState = bnsBooting;
        _lvdcNodeState = lvdcBooting;
    }
    bool testTransitions() override;
};

class Standby: public PodState{
    Standby(){
        _stateIdentifier = psStandby;
        _brakeNodeState = bnsStandby;
        _lvdcNodeState = lvdcStandby;
    }
    bool testTransitions() override;
};


#endif //FLIGHTCOMPUTER_STATES_H
