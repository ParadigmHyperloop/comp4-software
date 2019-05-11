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
        this->sPodValues->ePodState = state;
        return 1;
    } else {
        return 0;
    }
};

PodStates Pod::getPodState() {
    return this->sPodValues->ePodState;
}



void Pod::setControlsInterfaceState(ControlsInterfaceStates eTerminalState) {
    if (this->bWriteControlsInterfaceState) {
        this->sPodValues->eTerminalState = eTerminalState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Controls Interface State";
    }
}

ControlsInterfaceStates Pod::getControlsInterfaceState() {
    return this->sPodValues->eTerminalCommand;
}



int32_t Pod::setBrakeNodeState(BrakeNodeStates eState) {
    if (this->bWriteBreakNodeState) {
        this->sPodValues->eBrakeNodeState = eState;
        return 1;
    } else {
        return 0;
    }
};

BrakeNodeStates Pod::getBrakeNodeState() {
    return this->sPodValues->eBrakeNodeState;
};



MotorStates Pod::getMotorState() {
    return this->sPodValues->eMotorState;
};

int32_t Pod::setMotorState(MotorStates eMotorState) {
    if (this->bWriteMotorState) {
        this->sPodValues->eMotorState = eMotorState;
        return 1;
    } else {
        return 0;
    }
};


unsigned char *Pod::getFlagsArray() {
    return this->sPodValues->cFlagsArray;
};

int32_t Pod::getFlagsArraySize() {
    return this->sPodValues->iFlagsArraySize;
};



void Pod::setAutomaticTransitions(bool val) {
    if (this->bWriteManualStates) {
        this->sPodValues->bAutomaticTransitions = val;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Automatic Transition";
    }
}

void Pod::setManualBrakeNodeState(BrakeNodeStates eBrakeNodeState) {
    if (this->bWriteManualStates) {
        this->sPodValues->eManualBrakeNodeState = eBrakeNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualLvdcNodeState(LvdcNodeStates eLvdcNodeState) {
    if (this->bWriteManualStates) {
        this->sPodValues->eManualLvdcNodeState = eLvdcNodeState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}

void Pod::setManualPodState(PodStates ePodState) {
    if (this->bWriteManualStates) {
        this->sPodValues->eManualPodState = ePodState;
    } else {
        LOG(INFO) << "ERROR: Permission Denied for writing Manual State";
    }
}



void Pod::setHvBatteryPackVoltage(float fValue) {
    if (!this->bWriteHighVoltage) {
        //todo throw error with message
    }

}






/*
    float iHvBatteryPackVoltage;
	float iHvBatteryPackCurrent;
	float iHvBatteryPackMinimumCellVoltage;
	float iHvBatteryPackMaxCellVoltage;*/


