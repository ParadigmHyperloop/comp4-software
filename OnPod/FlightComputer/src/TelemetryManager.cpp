#include <Paradigm.pb.h>
#include "TelemetryManager.h"
#include "Common.h"
#include "comparingHelpers.h"
#include "Constants/SensorConfig.h"

#define REAR_WHEEL_CIRCUMFRENCE 0.933619f

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

void TelemetryManager::setPodState(PodStates newState, const std::string &reason) {
    std::lock_guard<std::mutex> lock(this->telemetry->stateLock);
    this->telemetry->podState = std::move(PodState::createState(newState, this));
};

void TelemetryManager::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    this->telemetry->controlsInterfaceState = eTerminalState;
}

void TelemetryManager::setAutomaticTransitions(bool val) {
    this->telemetry->automaticTransitions = val;
}

void TelemetryManager::setManualBrakeNodeState(BrakeNodeStates eBrakeNodeState) {
    this->telemetry->manualBrakeNodeState = eBrakeNodeState;
}

void TelemetryManager::setManualLvdcNodeState(LvdcNodeStates eLvdcNodeState) {
    this->telemetry->manualLvdcNodeState = eLvdcNodeState;
}

void TelemetryManager::setManualPodState(PodStates ePodState) {
        this->telemetry->manualPodState = ePodState;
}



//          HV BMS

void TelemetryManager::setHvBatteryPackVoltage(float value) {
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
    this->setBmsSensorFlag(status, HV_PACK_VOLTAGE_INDEX);
}

void TelemetryManager::setHvBatteryPackCurrent(float value) {
    this->telemetry->hvBatteryPackCurrent = value;
}

void TelemetryManager::setHvBatteryPackMinimumCellVoltage(float value){
    this->telemetry->hvBatteryPackMinimumCellVoltage = value;
}

void TelemetryManager::setHvBatteryPackMaxCellVoltage(float value){
    this->telemetry->hvBatteryPackMaxCellVoltage = value;
}

void TelemetryManager::setHvBatteryPackMaxCellTemperature(float value) {
    this->telemetry->hvBatteryPackMaxCellTemperature = value;

    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, HV_MAX_CELL_TEMP_INDEX);
}

void TelemetryManager::setHvBatteryPackStateOfCharge(int value) {
    this->telemetry->hvBatteryPackStateOfCharge = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();
    // Check if nominal for current state
    if(currentState == psArming || currentState == psArmed){
        // status = greaterThan //todo
    }
    else if(currentState == psAcceleration){
        // status = greaterThan
    }
    this->setBmsSensorFlag(status,  HV_SOC_INDEX);
}

//          LV BMS
void TelemetryManager::setLv1BatteryPackStateOfCharge(int value) {
    this->telemetry->lv1BatteryPackStateOfCharge = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();

    // Check if nominal for current state
    if(currentState == psArming || currentState == psArmed ||
    currentState == psPreFlight || currentState == psAcceleration){
        // status = greaterThan //todo
    }

    this->setBmsSensorFlag(status,  LV1_SOC_INDEX);
}

void TelemetryManager::setLv1BatteryPackTemperature(float value) {
    this->telemetry->lv1BatteryPackCellTemperature = value;

    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, LV1_CELL_TEMP_INDEX);
}

void TelemetryManager::setLv1BatteryPackVoltage(float value) {
    this->telemetry->lv1BatteryPackVoltage = value;
    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, LV1_PACK_VOLTAGE_INDEX);
}

void TelemetryManager::setLv2BatteryPackStateOfCharge(int value) {
    this->telemetry->lv2BatteryPackStateOfCharge = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();

    // Check if nominal for current state
    if(currentState == psArming || currentState == psArmed ||
       currentState == psPreFlight || currentState == psAcceleration){
        // status = greaterThan //todo
    }

    this->setBmsSensorFlag(status,  LV2_SOC_INDEX);
}

void TelemetryManager::setLv2BatteryPackTemperature(float value) {
    this->telemetry->lv2BatteryPackCellTemperature = value;

    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, LV2_CELL_TEMP_INDEX);
}

void TelemetryManager::setLv2BatteryPackVoltage(float value) {
    this->telemetry->lv2BatteryPackVoltage = value;
    bool status = true;
    // status = lessThan maxx cell temp TODO
    this->setBmsSensorFlag(status, LV2_PACK_VOLTAGE_INDEX);
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

void TelemetryManager::setLowPressure4(float value, PodStates currentState) {
    bool status = true;
    if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inRange<float>(value, LOWPRESSURE_ENGAGED_MIN, LOWPRESSURE_ENGAGED_MAX);
    }
    this->setNodeSensorFlag(status, LP4_INDEX);
}

void TelemetryManager::setLowPressure(float value, int identifier){

    bool status = false;
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
            this->telemetry->lowPressure3 = value;
        case LP4_INDEX:
            this->telemetry->lowPressure4 = value;
        default:
            break;
    }
    if(identifier == LP4_INDEX){
        setLowPressure4(value, currentState);
        return;
    }
    // Check if nominal for current state
    if(currentState == psStandby){
        status = inRange<float>(value, LOWPRESSURE_UNARMED_MIN, LOWPRESSURE_UNARMED_MAX);
    }

    else if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inThreshold<float>(value, this->telemetry->tubePressure, PNEUMATICS_THRESHOLD);
    }

    else if( currentState == psBraking ){
        status = inRange<float>(value, LOWPRESSURE_ENGAGED_MIN, LOWPRESSURE_ENGAGED_MAX);
    }

    // Set sensor flag
    this->setNodeSensorFlag(status, identifier);
}

void TelemetryManager::setHighPressure(float value){
    bool status = false;
    PodStates currentState = this->getPodStateValue();

    this->telemetry->highPressure = value;
    LOG(INFO)<<"high pressure : " << value;

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
    bool status = false;
    this->telemetry->pressureVesselTemperature = value;
    status = inRange<float>(value, PRESSURE_TEMP_MIN, PRESSURE_TEMP_MAX);
    this->setNodeSensorFlag(status, HP_TEMP_INDEX);
}

void TelemetryManager::setCoolantTemperature(float value) {
    bool status = true;
    this->telemetry->coolingTemperature = value;

    PodStates currentState = this->getPodStateValue();
    if(currentState == psStandby || currentState == psArming || currentState == psArmed){
        status = inRange<float>(value, COOLING_TEMP_MIN, COOLING_TEMP_MAX);
    } //todo temp during run

    this->setNodeSensorFlag(status, COOLING_TEMPERATURE_INDEX);
}

void TelemetryManager::setRecievedBrakeNodeState(BrakeNodeStates value) {
    this->telemetry->receivedBrakeNodeState = value;
}

//          Enclosure
void TelemetryManager::setEnclosurePressure(float value) {
    bool status = false;
    this->telemetry->enclosurePressure = value;
    status = inRange<float>(value, ATMOSPHERE, ATMOSPHERE_THRESHOLD);
    this->setNodeSensorFlag(status, ENCLOSURE_PRESSURE_INDEX);
}

void TelemetryManager::setEnclosureTemperature(float value) {
    bool status;
    this->telemetry->enclosureTemperature = value;
    status = inRange<float>(value, ENCLOSURE_TEMP_MIN, ENCLOSURE_TEMP_MAX);
    this->setNodeSensorFlag(status, ENCLOSURE_TEMPERATURE_INDEX);
}

void TelemetryManager::setCoolantLinePressure(float value) {

    this->telemetry->coolingLinePressure = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();
    if(currentState == psArming || currentState == psArmed || currentState == psAcceleration || currentState == psCoasting){
        status = inRange<float>(value, COOLING_ENGAGED_MIN, COOLING_ENGAGED_MAX);
    }
    // Set sensor flag
    this->setNodeSensorFlag(status, COOLING_PRESSURE_INDEX);
}


//          Inverter

void TelemetryManager::setMaxIgbtTemperature(float value) {
    this->telemetry->maxIgbtTemperature = value;
    bool status = inRange<float>(value, COMPONENT_TEMP_MIN, COMPONENT_TEMP_MAX);
    this->setInverterSensorFlag(status, MAX_IGBT_INDEX);

}

void TelemetryManager::setGateDriverTemperature(float value) {
    this->telemetry->gateDriverTemperature = value;
    bool status = inRange<float>(value, COMPONENT_TEMP_MIN, COMPONENT_TEMP_MAX);
    this->setInverterSensorFlag(status, GATE_DRIVER_TEMP_INDEX);
}

void TelemetryManager::setInverterControlBoardTemperature(float value) {
    this->telemetry->inverterControlBoardTemperature = value;
    bool status = inRange<float>(value, COMPONENT_TEMP_MIN, COMPONENT_TEMP_MAX);
    this->setInverterSensorFlag(status, CONTROL_BOARD_TEMP_INDEX);
}

void TelemetryManager::setMotorTemperature(float value) {
    this->telemetry->motorTemperature = value;

    bool status = true;
    PodStates currentState = this->getPodStateValue();

    if(currentState == psArming || currentState == psArmed){
        status = inRange<float>(value, MOTOR_UNARMED_TEMP_MIN, MOTOR_UNARMED_TEMP_MAX);
    }else if( currentState == psAcceleration ){
        status = inRange<float>(value, MOTOR_ARMED_TEMP_MIN, MOTOR_ARMED_TEMP_MAX);
    }
    this->setInverterSensorFlag(status, MOTOR_TEMPERATURE_INDEX);
}

void TelemetryManager::setMotorSpeed(int32_t value) {
    auto lastTime = this->telemetry->lastMotorReadTime;
    auto thisTime = std::chrono::high_resolution_clock::now();
    this->telemetry->lastMotorReadTime = thisTime;
    int32_t lastRPM = this->telemetry->motorSpeed;
    this->telemetry->motorSpeed = value;
    float average = (value+lastRPM)/2.0;
    average /= (1000.0*60.0);
    float milliseconds = (std::chrono::duration_cast<std::chrono::microseconds>(thisTime - lastTime).count())/1000.0;
    float distance = average*milliseconds*REAR_WHEEL_CIRCUMFRENCE;

    addPodDistance(distance);
}

void TelemetryManager::setInverterBusVoltage(int value) {
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


// Position

void TelemetryManager::addPodDistance(float distance) {
    std::lock_guard<std::mutex> lock(this->telemetry->positionLock);
    telemetry->podPosition+= distance;
}

void TelemetryManager::setPodDistance(float distance) {
    std::lock_guard<std::mutex> lock(this->telemetry->positionLock);
    telemetry->podPosition = distance;
}

void TelemetryManager::setPodVelocity(float velocity) {
    telemetry->podVelocity = velocity;
}



//          Flag Setting

void TelemetryManager::resetValues(int32_t index){
    switch(index){
        case ENCLOSURE_HEARTBEAT_INDEX:{
            telemetry->enclosureTemperature = 0;
            telemetry->enclosurePressure = 0;
            telemetry->coolingLinePressure = 0;
            break;
        }
        case LVDC_NODE_HEARTBEAT_INDEX:{
            //todo
        }
        case BMS_HEARTBEAT_INDEX:{
            telemetry->hvBatteryPackMaxCellTemperature = 0;
            telemetry->hvBatteryPackCurrent = 0;
            telemetry->hvBatteryPackVoltage = 0;
            telemetry->hvBatteryPackMinimumCellVoltage = 0;
            telemetry->hvBatteryPackStateOfCharge = 0;
            telemetry->hvBatteryPackMaxCellVoltage = 0;
            break;
        }
        case BRAKE_NODE_HEARTBEAT_INDEX:{
            telemetry->receivedBrakeNodeState = bnsNone;
            telemetry->lowPressure1 = 0 ;
            telemetry->lowPressure2 = 0 ;
            telemetry->lowPressure3 = 0 ;
            telemetry->lowPressure4 = 0 ;
            telemetry->highPressure = 0;
            telemetry->pressureVesselTemperature = 0;
            telemetry->coolingLinePressure = 0;
            telemetry->coolingTemperature = 0;
            break;
        }
        default:{
            return;
        }
    }
    sendUpdate("Lost Connection Index : " + std::to_string(index));
}

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
        if(currentStatus == 1 && status == 0 ){
            resetValues(index);
        }
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
        if(this->telemetry->inverterHeartbeat == 1 && value == 0){
            telemetry->maxIgbtTemperature = 0;
            telemetry->gateDriverTemperature = 0;
            telemetry->inverterControlBoardTemperature = 0;
            telemetry->motorTemperature = 0;
            telemetry->inverterBusVoltage = 0;
        }
        this->telemetry->inverterHeartbeat = value;
    }
}

