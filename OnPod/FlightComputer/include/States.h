#ifndef FLIGHTCOMPUTER_STATES_H
#define FLIGHTCOMPUTER_STATES_H

#include "Common.h"

class PodState{

public:
    PodState() = default;

    PodStates getStateValue(){
        return this->_stateIdentifier;
    };

    bool testTransitions(){
        return false;
    };

    PodStates getNewState(){
        return this->_nextStateIdentifier;
    };

    bool isTransitioning(){
        return this->_transitioning;
    };

    BrakeNodeStates getBrakeNodeState(){
        return _brakeNodeState;
    }

    LvdcNodeStates getLvdcNodeState(){
        return _lvdcNodeState;
    }

    const std::string getTransitionReason(){
        return _transitionReason;
    }
    unsigned int getTimeInStateMilis();

    static std::unique_ptr<PodState> createState(PodStates);


protected:
    std::chrono::steady_clock::time_point _enterStateTime;
    bool _transitioning = false;
    std::string _transitionReason = "";
    PodStates _stateIdentifier = psBooting;
    PodStates _nextStateIdentifier = psBooting;
    BrakeNodeStates _brakeNodeState = bnsBooting;
    LvdcNodeStates  _lvdcNodeState = lvdcBooting;

};



class Booting: public PodState{
public:
    Booting(){
        _brakeNodeState = bnsBooting;
        _lvdcNodeState = lvdcBooting;
    }

    PodStates getStateValue() { return psBooting;};
};


#endif //FLIGHTCOMPUTER_STATES_H
