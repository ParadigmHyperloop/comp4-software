#include "TelemetryManager.h"
#include "Common.h"
#include "comparingHelpers.h"
#include "Constants/SensorConfig.h"

TelemetryManager::TelemetryManager()=default;

TelemetryManager::TelemetryManager(PodValues *sPodValues, PodNetwork *sNetworkVals) {
    this->telemetry = sPodValues;
    this->sPodNetworkValues = sNetworkVals;
};


int32_t TelemetryManager::setPodState(PodStates newState, const std::string &reason) {
    if (this->bWritePodState){
        LOG(INFO) << reason;
        this->telemetry->podState = std::move(PodState::createState(newState, this));
        return 1;
    } else {
        return 0;
    }
};

void TelemetryManager::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    if (this->bWriteControlsInterfaceState) {
        this->telemetry->controlsInterfaceState = eTerminalState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Controls Interface State";
    }
}

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

void TelemetryManager::setSolenoid(bool value, int32_t identifier){
    // Set Value in Memory
    switch(identifier){
        case SOL1_INDEX:
            this->telemetry->solenoid1 = value;
            break;
        case SOL2_INDEX:
            this->telemetry->solenoid2 = value;
            break;
        case SOL3_INDEX:
            this->telemetry->solenoid3 = value;
        case SOL4_INDEX:
            this->telemetry->solenoid4 = value;
        default:
            break;
    }
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
    if(currentState == psStandby){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX);
    } else if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inRange<float>(value, HIGHPRESSURE_ARMED_MIN, HIGHPRESSURE_ARMED_MAX);
    }else if( currentState == psBraking ){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX); //TODO confirm this
    }
    this->setSensorFlag(status, HP_INDEX);
}


void TelemetryManager::setPressureVesselTemperature(float value){
    bool status;
    this->telemetry->pressureVesselTemperature = value;
    status = inRange<float>(value, PRESSURE_TEMP_MIN, PRESSURE_TEMP_MAX);
    this->setSensorFlag(status, HP_TEMP_INDEX);
}


void TelemetryManager::setSensorFlag(int32_t status, int32_t index){
    int32_t currentStatus = this->telemetry->sensorFlags[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->sensorFlags[index] = status;
        //ForceControlLoopIteration? TODO
        return;
    }
}

void TelemetryManager::setConnectionFlag(int32_t status, int32_t index){
    int32_t currentStatus = this->telemetry->connectionFlags[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->connectionFlags[index] = status;
        //ForceControlLoopIteration? TODO
        return;
    }
}