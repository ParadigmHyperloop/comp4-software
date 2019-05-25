#include "States.h"

PodState::PodState() = default;

PodState::PodState(TelemetryManager* pod){
    this->pod = pod;
}

void PodState::setupTransition(PodStates nextState, const std::string& reason){
    this->_transitioning = true;
    this->_transitionReason = reason;
    this->_nextStateIdentifier = nextState;
};

bool PodState::testTransitions(){
    return true;
};

PodStates PodState::getStateValue(){
    return this->_stateIdentifier;
};

PodStates PodState::getNewState(){
    return this->_nextStateIdentifier;
};

bool PodState::isTransitioning(){
    return this->_transitioning;
};

LvdcNodeStates PodState::getLvdcNodeState(){
    return _lvdcNodeState;
}

BrakeNodeStates PodState::getBrakeNodeState(){
    return _brakeNodeState;
}

const std::string PodState::getTransitionReason(){
    return _transitionReason;
}

std::unique_ptr<PodState> PodState::createState(PodStates newState) {
    switch (newState) {
        case psBooting:
            return std::unique_ptr<PodState>(new Booting());
        case psStandby:
            return std::unique_ptr<PodState>(new Booting());
    }
}

unsigned int PodState::getTimeInStateMilis(){
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count();
}

/*
 *  ******************** BOOTING ***********************8
 */


bool Booting::testTransitions(){
    this->setupTransition(psStandby, "Booting Complete");
    return true;
};


bool Standby::testTransitions() {

}
