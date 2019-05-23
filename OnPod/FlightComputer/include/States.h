#ifndef FLIGHTCOMPUTER_STATES_H
#define FLIGHTCOMPUTER_STATES_H

#include "Common.h"

class State{

public:
    virtual void runTransitions(){};


    virtual PodStates getStateValue()=0;
    ~State() = default;
    State() = default;

    BrakeNodeStates getBrakeNodeState(){
        return _brakeNodeState;
    }

    LvdcNodeStates getLvdcNodeState(){
        return _lvdcNodeState;
    }

    unsigned int getTimeInStateMilis(){
        std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count();
    }

protected:
    std::chrono::steady_clock::time_point _enterStateTime;
    BrakeNodeStates _brakeNodeState = bnsBooting;
    LvdcNodeStates  _lvdcNodeState = lvdcBooting;
};


class Booting: public State{
    Booting(){
        _brakeNodeState = bnsBooting;
        _lvdcNodeState = lvdcBooting;
    }
    void runTransitions() override;
    PodStates getStateValue() override { return psBooting;}
};


#endif //FLIGHTCOMPUTER_STATES_H
