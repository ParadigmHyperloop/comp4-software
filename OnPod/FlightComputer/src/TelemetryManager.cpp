#include "TelemetryManager.h"
#include "Common.h"
#include "comparingHelpers.h"
#include "SensorConfig.h"

TelemetryManager::TelemetryManager()=default;

TelemetryManager::TelemetryManager(PodValues *sPodValues, PodNetwork *sNetworkVals) {
    this->telemetry = sPodValues;
    this->sPodNetworkValues = sNetworkVals;
};


int32_t TelemetryManager::setPodState(PodStates newState, const std::string &reason) {
    if (this->bWritePodState){
        LOG(INFO) << reason;
        this->telemetry->podState = std::move(PodState::createState(newState));
        return 1;
    } else {
        return 0;
    }
};



void TelemetryManager::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    if (this->bWriteControlsInterfaceState) {
        this->telemetry->terminalState = eTerminalState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Controls Interface State";
    }
}

ControlsInterfaceStates TelemetryManager::getControlsInterfaceState() {
    return this->telemetry->eTerminalCommand;
}




MotorStates TelemetryManager::getMotorState() {
    return this->telemetry->motorState;
};

int32_t TelemetryManager::setMotorState(MotorStates eMotorState) {
    if (this->bWriteMotorState) {
        this->telemetry->motorState = eMotorState;
        return 1;
    } else {
        return 0;
    }
};


unsigned char *TelemetryManager::getFlagsArray() {
    return this->telemetry->flagsArray;
};

int32_t TelemetryManager::getFlagsArraySize() {
    return this->telemetry->iFlagsArraySize;
};



void TelemetryManager::setAutomaticTransitions(bool val) {
    if (this->bWriteManualStates) {
        this->telemetry->automaticTransitions = val;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Automatic Transition";
    }
}

void TelemetryManager::setManualBrakeNodeState(BrakeNodeStates eBrakeNodeState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualBrakeNodeState = eBrakeNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void TelemetryManager::setManualLvdcNodeState(LvdcNodeStates eLvdcNodeState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualLvdcNodeState = eLvdcNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void TelemetryManager::setManualPodState(PodStates ePodState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualPodState = ePodState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void TelemetryManager::setHvBatteryPackVoltage(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackVoltage = value;
}

float TelemetryManager::getHvBatteryPackVoltage(){
    return this->telemetry->hvBatteryPackVoltage;
}


void TelemetryManager::setHvBatteryPackCurrent(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackCurrent = value;
}

float TelemetryManager::getHvBatteryPackCurrent(){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    return this->telemetry->hvBatteryPackCurrent;
}

void TelemetryManager::setHvBatteryPackMinimumCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackMinimumCellVoltage = value;
}

float TelemetryManager::getHvBatteryPackMinimumCellVoltage(){
    return this->telemetry->hvBatteryPackMinimumCellVoltage;
}

void TelemetryManager::setHvBatteryPackMaxCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackMaxCellVoltage = value;
}

float TelemetryManager::getHvBatteryPackMaxCellVoltage(){
    return this->telemetry->hvBatteryPackMaxCellVoltage;
}


void TelemetryManager::setLowPressure(float value, int identifier){

    bool status = true;
    PodStates currentState = this->telemetry->podState->getStateValue();

    // Set Value in Memory
    switch(identifier){
        case LP1_INDEX:
            this->telemetry->lowPressure1 = value;
            break;
        case LP2_INDEX:
            this->telemetry->lowPressure2 = value;
            break;
        case LP3_INDEX:
            this->telemetry->lowPressure2 = value;
        default:
            break;
    }

    // Check if nominal for current state
    if(currentState == psStandby){
        status = inRange<float>(value, LOWPRESSURE_UNARMED_MIN, LOWPRESSURE_UNARMED_MAX);
    } else if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inThreshold<float>(value, this->telemetry->tubePressure, PNEUMATICS_THRESHOLD);
    }else if( currentState == psBraking ){
        status = inThreshold<float>(value, LOWPRESSURE_ENGAGED, PNEUMATICS_THRESHOLD);
    }

    // Set sensor flag
    this->setSensorFlag(status, identifier);
}

void TelemetryManager::setHighPressure(float value){
    bool status = false;
    PodStates currentState = this->telemetry->podState->getStateValue();

    this->telemetry->highPressure = value;

    // Check if nominal for current state

    if(currentState == psStandby){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX);
    } else if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inRange<float>(value, HIGHPRESSURE_ARMED_MIN, HIGHPRESSURE_ARMED_MAX);
    }else if( currentState == psBraking ){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX); //TODO confirm this
    }

    // Set sensor flag
    this->setSensorFlag(status, HP_INDEX);
}


void TelemetryManager::setPressureVesselTemperature(float){

}

void TelemetryManager::setSensorFlag(int32_t status, int32_t index){

    int32_t currentStatus = this->telemetry->flagsArray[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->flagsArray[index] = status;
        return;
    }
}