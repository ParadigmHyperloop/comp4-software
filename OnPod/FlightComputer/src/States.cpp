#include "States.h"

PodState::PodState() = default;

PodState::PodState(TelemetryManager* pod){
    this->pod = pod;
}

int32_t PodState::checkSensorFlags(){
    std::vector<int32_t> flags = this->pod->telemetry->sensorFlags;
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 0){
            return i;
        }
    }
    return FLAGS_GOOD;
}

int32_t PodState::checkCommunicationFlags(){
    std::vector<int32_t> flags = this->pod->telemetry->connectionFlags;
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 0){
            return i;
        }
    }
    return FLAGS_GOOD;
}

int32_t PodState::checkNodeStates(){
    if(this->_brakeNodeState != this->pod->telemetry->brakeNodeState){
        return BRAKE_NODE_INDEX;
    }
    if(this->_lvdcNodeState != this->pod->telemetry->lvdcNodeState){
        return LVDC_NODE_INDEX;
    }
    return FLAGS_GOOD;
}

void PodState::setupTransition(PodStates nextState, const std::string& reason){
    this->_transitioning = true;
    this->_transitionReason = reason;
    this->_nextStateIdentifier = nextState;
}

bool PodState::testTransitions(){
    return true;
}

PodStates PodState::getStateValue(){
    return this->_stateIdentifier;
}

PodStates PodState::getNewState(){
    return this->_nextStateIdentifier;
}

bool PodState::isTransitioning(){
    return this->_transitioning;
}

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

/*
 *  ******************** STANDBY ***********************8
 */
bool Standby::testTransitions() {
    int32_t status;

    status = this->checkSensorFlags();
    if( status != FLAGS_GOOD ){
        // Failing on sensor : status
        return false;
    }
    status = this->checkCommunicationFlags();
    if(status != FLAGS_GOOD){
        // Failing on heartbeat : status
        return false;
    }
    status = this->checkNodeStates();
    if(status != FLAGS_GOOD){
        // Waiting on node status
        return false;
    }
    if(this->pod->telemetry->terminalState == ciArm){
        this->setupTransition(psArming, (std::string)"Arm Command Received");
        return true;
    }
    return false;
}
