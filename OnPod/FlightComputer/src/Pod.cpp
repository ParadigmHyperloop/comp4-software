#include "Common.h"


Pod::Pod(PodValues *sPodValues, PodNetwork *sNetworkVals) {
    this->telemetry = sPodValues;
    this->sPodNetworkValues = sNetworkVals;
    return;
};


int32_t Pod::setPodState(PodStates newState, const std::string &reason) {
    if (this->bWritePodState){
        LOG(INFO) << reason;
        this->telemetry->podState = std::move(PodState::createState(newState));
        return 1;
    } else {
        return 0;
    }
};



void Pod::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    if (this->bWriteControlsInterfaceState) {
        this->telemetry->terminalState = eTerminalState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Controls Interface State";
    }
}

ControlsInterfaceStates Pod::getControlsInterfaceState() {
    return this->telemetry->eTerminalCommand;
}




MotorStates Pod::getMotorState() {
    return this->telemetry->motorState;
};

int32_t Pod::setMotorState(MotorStates eMotorState) {
    if (this->bWriteMotorState) {
        this->telemetry->motorState = eMotorState;
        return 1;
    } else {
        return 0;
    }
};


unsigned char *Pod::getFlagsArray() {
    return this->telemetry->flagsArray;
};

int32_t Pod::getFlagsArraySize() {
    return this->telemetry->iFlagsArraySize;
};



void Pod::setAutomaticTransitions(bool val) {
    if (this->bWriteManualStates) {
        this->telemetry->automaticTransitions = val;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Automatic Transition";
    }
}

void Pod::setManualBrakeNodeState(BrakeNodeStates eBrakeNodeState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualBrakeNodeState = eBrakeNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualLvdcNodeState(LvdcNodeStates eLvdcNodeState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualLvdcNodeState = eLvdcNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualPodState(PodStates ePodState) {
    if (this->bWriteManualStates) {
        this->telemetry->manualPodState = ePodState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setHvBatteryPackVoltage(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackVoltage = value;
}

float Pod::getHvBatteryPackVoltage(){
    return this->telemetry->hvBatteryPackVoltage;
}


void Pod::setHvBatteryPackCurrent(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackCurrent = value;
}

float Pod::getHvBatteryPackCurrent(){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    return this->telemetry->hvBatteryPackCurrent;
}

void Pod::setHvBatteryPackMinimumCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackMinimumCellVoltage = value;
}

float Pod::getHvBatteryPackMinimumCellVoltage(){
    return this->telemetry->hvBatteryPackMinimumCellVoltage;
}

void Pod::setHvBatteryPackMaxCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->telemetry->hvBatteryPackMaxCellVoltage = value;
}

float Pod::getHvBatteryPackMaxCellVoltage(){
    return this->telemetry->hvBatteryPackMaxCellVoltage;
}


