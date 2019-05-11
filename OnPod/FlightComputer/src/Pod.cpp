#include "FlightComputer/Structs.h"
#include "FlightComputer/Pod.h"
#include "ProtoBuffer/Paradigm.pb.h"
#include "EasyLogger/easylogging++.h"


Pod::Pod(PodValues *sPodValues, PodNetwork *sNetworkVals) {
    this->sPodValues = sPodValues;
    this->sPodNetworkValues = sNetworkVals;
    return;
};



int32_t Pod::setPodState(PodStates state, const char *reason) {
    if (this->bWritePodState) {
        LOG(INFO) << reason;
        this->sPodValues->podState = state;
        return 1;
    } else {
        return 0;
    }
};

PodStates Pod::getPodState() {
    return this->sPodValues->podState;
}



void Pod::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    if (this->bWriteControlsInterfaceState) {
        this->sPodValues->terminalState = eTerminalState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Controls Interface State";
    }
}

ControlsInterfaceStates Pod::getControlsInterfaceState() {
    return this->sPodValues->eTerminalCommand;
}



int32_t Pod::setBrakeNodeState(BrakeNodeStates eState) {
    if (this->bWriteBreakNodeState) {
        this->sPodValues->brakeNodeState = eState;
        return 1;
    } else {
        return 0;
    }
};

BrakeNodeStates Pod::getBrakeNodeState() {
    return this->sPodValues->brakeNodeState;
};



MotorStates Pod::getMotorState() {
    return this->sPodValues->motorState;
};

int32_t Pod::setMotorState(MotorStates eMotorState) {
    if (this->bWriteMotorState) {
        this->sPodValues->motorState = eMotorState;
        return 1;
    } else {
        return 0;
    }
};


unsigned char *Pod::getFlagsArray() {
    return this->sPodValues->flagsArray;
};

int32_t Pod::getFlagsArraySize() {
    return this->sPodValues->iFlagsArraySize;
};



void Pod::setAutomaticTransitions(bool val) {
    if (this->bWriteManualStates) {
        this->sPodValues->automaticTransitions = val;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Automatic Transition";
    }
}

void Pod::setManualBrakeNodeState(BrakeNodeStates eBrakeNodeState) {
    if (this->bWriteManualStates) {
        this->sPodValues->manualBrakeNodeState = eBrakeNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualLvdcNodeState(LvdcNodeStates eLvdcNodeState) {
    if (this->bWriteManualStates) {
        this->sPodValues->manualLvdcNodeState = eLvdcNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualPodState(PodStates ePodState) {
    if (this->bWriteManualStates) {
        this->sPodValues->manualPodState = ePodState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setHvBatteryPackVoltage(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->sPodValues->hvBatteryPackVoltage = value;
}

float Pod::getHvBatteryPackVoltage(){
    return this->sPodValues->hvBatteryPackVoltage;
}


void Pod::setHvBatteryPackCurrent(float value) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->sPodValues->hvBatteryPackCurrent = value;
}

float Pod::getHvBatteryPackCurrent(){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    return this->sPodValues->hvBatteryPackCurrent;
}

void Pod::setHvBatteryPackMinimumCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->sPodValues->hvBatteryPackMinimumCellVoltage = value;
}

float Pod::getHvBatteryPackMinimumCellVoltage(){
    return this->sPodValues->hvBatteryPackMinimumCellVoltage;
}

void Pod::setHvBatteryPackMaxCellVoltage(float value){
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }
    this->sPodValues->hvBatteryPackMaxCellVoltage = value;
}

float Pod::getHvBatteryPackMaxCellVoltage(){
    return this->sPodValues->hvBatteryPackMaxCellVoltage;
}


