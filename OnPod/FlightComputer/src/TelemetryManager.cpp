#include <Paradigm.pb.h>
#include "TelemetryManager.h"
#include "Common.h"
#include "comparingHelpers.h"
#include "Constants/SensorConfig.h"

TelemetryManager::TelemetryManager()=default;

TelemetryManager::TelemetryManager(PodValues *sPodValues, PodNetwork *sNetworkVals) {
    this->telemetry = sPodValues;
    this->sPodNetworkValues = sNetworkVals;
}


//          Global Getters


void TelemetryManager::sendUpdate(std::string update) {
    LOG(INFO)<<update;
    std::lock_guard<std::mutex> lock(this->telemetry->stateLock);
    this->telemetry->updates.push_back(update);
}

PodStates TelemetryManager::getPodStateValue() {
    std::lock_guard<std::mutex> lock(this->telemetry->stateLock);
    return this->telemetry->podState->getStateValue();
}


//          State setting

int32_t TelemetryManager::setPodState(PodStates newState, const std::string &reason) {
    if (this->bWritePodState){
        std::lock_guard<std::mutex> lock(this->telemetry->stateLock);
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



//          BMS

void TelemetryManager::setHvBatteryPackVoltage(float value) {
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackVoltage = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();

    //todo ranges

    // Check if nominal for current state
    if(currentState == psArming || currentState == psArmed){
        // status = inRange()
    }
    else if(currentState == psAcceleration){
        // status = greaterThan
    }
    // Set sensor flag
    this->setBmsSensorFlag(status, PACK_VOLTAGE_INDEX);
}

void TelemetryManager::setHvBatteryPackCurrent(float value) {
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackCurrent = value;
}

void TelemetryManager::setHvBatteryPackMinimumCellVoltage(float value){
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackMinimumCellVoltage = value;
}

void TelemetryManager::setHvBatteryPackMaxCellVoltage(float value){
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackMaxCellVoltage = value;
}

void TelemetryManager::setHvBatteryPackMaxCellTemperature(float value) {
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackMaxCellTemperature = value;

    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, MAX_CELL_TEMP_INDEX);
}

void TelemetryManager::setHvBatteryPackStateOfCharge(int value) {
    if (!this->bWriteBms) {
        LOG(INFO)<< "Lacking BMS Write Permissions";
    }
    this->telemetry->hvBatteryPackStateOfCharge = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();
    // Check if nominal for current state
    if(currentState == psArming || currentState == psArmed){
        // status = greaterThan
    }
    else if(currentState == psAcceleration){
        // status = greaterThan
    }
    this->setBmsSensorFlag(status,  SOC_INDEX);
}


//          Brake Node

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
    PodStates currentState = this->getPodStateValue();

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
    this->setNodeSensorFlag(status, identifier);
}

void TelemetryManager::setHighPressure(float value){
    bool status = false;
    PodStates currentState = this->getPodStateValue();

    this->telemetry->highPressure = value;
    if(currentState == psStandby){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX);
    } else if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inRange<float>(value, HIGHPRESSURE_ARMED_MIN, HIGHPRESSURE_ARMED_MAX);
    }else if( currentState == psBraking ){
        status = inRange<float>(value, VACUUM, HIGHPRESSURE_ARMED_MAX); //TODO confirm this
    }
    this->setNodeSensorFlag(status, HP_INDEX);
}

void TelemetryManager::setPressureVesselTemperature(float value){
    bool status;
    this->telemetry->pressureVesselTemperature = value;
    status = inRange<float>(value, PRESSURE_TEMP_MIN, PRESSURE_TEMP_MAX);
    this->setNodeSensorFlag(status, HP_TEMP_INDEX);
}

//          Enclosure
void TelemetryManager::setEnclosurePressure(float value) {
    bool status;
    this->telemetry->enclosurePressure = value;
    status = inRange<float>(value, ATMOSPHERE, ATMOSPHERE_THRESHOLD);
    this->setNodeSensorFlag(status, ENCLOSURE_PRESSURE_INDEX);
}

void TelemetryManager::setEnclosureTemperature(float value) {
    bool status;
    this->telemetry->enclosureTemperature = value;
    status = inRange<float>(value, ENCLOSURE_TEMP, ENCLOSURE_TEMP_THRESHOLD);
    this->setNodeSensorFlag(status, ENCLOSURE_TEMPERATURE_INDEX);
}

void TelemetryManager::setCoolantLinePressure(float value) {
    bool status;
    PodStates currentState = this->getPodStateValue();
    if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inThreshold<float>(value, COOLING_ENGAGED, COOLING_THRESHOLD);
    }else{
        status = inRange<float>(value, 10, COOLING_ENGAGED + COOLING_THRESHOLD);
    }
    // Set sensor flag
    this->setNodeSensorFlag(status, COOLING_PRESSURE_INDEX);
}


//          Inverter

void TelemetryManager::setMaxIgbtTemperature(float value) {
    if (!this->bWriteInverter) {
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->maxIgbtTemperature = value;
}

void TelemetryManager::setGateDriverTemperature(float value) {
    if (!this->bWriteInverter) {
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->gateDriverTemperature = value;
}

void TelemetryManager::setInverterControlBoardTemperature(float value) {
    if (!this->bWriteInverter) {
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->inverterControlBoardTemperature = value;
}

void TelemetryManager::setMotorTemperature(float value) {
    if (!this->bWriteInverter){
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->motorTemperature = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();

    if(currentState == psArming || currentState == psArmed){
        // status = less than todo
    }else if( currentState == psAcceleration ){
        // status = less than todo
    }
    // Set sensor flag
    this->setInverterSensorFlag(status, MOTOR_TEMPERATURE_INDEX);
}

void TelemetryManager::setMotorSpeed(float value) {
    if (!this->bWriteInverter) {
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->motorSpeed = value;
}

void TelemetryManager::setInverterBusVoltage(int value) {
    if (!this->bWriteInverter) {
        LOG(INFO)<< "Lacking Inverter Write Permissions";
    }
    this->telemetry->inverterBusVoltage = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();
    if(currentState == psArming || currentState == psArmed){
        // status = less than todo
    }else if( currentState == psAcceleration ){
        // status = less than todo
    }
    // Set sensor flag
    this->setInverterSensorFlag(status, BUS_VOLTAGE_INDEX);
}


//          Flag Setting

void TelemetryManager::setNodeSensorFlag(int32_t status, int32_t index){
    int32_t currentStatus = this->telemetry->nodeSensorFlags[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->nodeSensorFlags[index] = status;
        return;
    }
}

void TelemetryManager::setBmsSensorFlag(int32_t status, int32_t index) {
    int32_t currentStatus = this->telemetry->bmsSensorFlags[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->bmsSensorFlags[index] = status;
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
        return;
    }
}

void TelemetryManager::setInverterSensorFlag(int32_t status, int32_t index) {
    int32_t currentStatus = this->telemetry->inverterSensorFlags[index];
    if(currentStatus == 2){ // Manual override high
        return;
    }
    if(currentStatus != status){
        this->telemetry->inverterSensorFlags[index] = status;
        return;
    }
}

void TelemetryManager::setInverterHeartbeat(int32_t value) {
    if(this->telemetry->inverterHeartbeat != 2){
        this->telemetry->inverterHeartbeat = value;
    }
    return;
}

