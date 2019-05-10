#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "ProtoBuffer/Paradigm.pb.h"




//enum PodStates { psStandby, psArming, psArmed, psAcceleration, psCoasting, psBraking, psDisarm, psRetrieval, psEmergency, psBooting };

//enum eBreakNodeStates { bnsBooting, bnsStandby, bnsArming, bnsArmed, bnsFlight, bnsBraking, bnsVenting, bnsRetrieval, bnsError};

//enum TerminalStates { tsConnected, tsDropped, tsTerminalEmergency};

//enum TerminalCommands { tcTerminalArm, tcTerminalFlight, tcTerminalStop, tcTerminalNone};

//enum MotorStates { msIdle, msDrive};

struct PodValues {
    // States
    PodStates ePodState = psBooting;
    ControlsInterfaceStates eTerminalState;
    MotorStates eMotorState;
    BrakeNodeStates eBrakeNodeState;
    LvdcNodeStates eLvdcNodeState;

    //Manual States
    PodStates eManualPodState;
    ControlsInterfaceStates eManualTerminalState;
    MotorStates eManualMotorState;
    BrakeNodeStates eManualBrakeNodeState;
    LvdcNodeStates eManualLvdcNodeState;
    bool bAutomaticTransitions;

    //ConnectionsArray
    std::vector<bool> cConnectionsArray = {false, false, false, false, false}; // [brake , rear, lvdc, bms, inverter]

    // Navigation
    float fDistance = 0;
    float fVelocity = 0;
    // Rear Node
    float fGpioValues;

    //FlagsV2
    unsigned char cFlagsArray[3] = {0};
    int32_t iFlagsArraySize = 3;
    // Atmosphere
    double dTubePressure;
    // Terminal
    ControlsInterfaceStates eTerminalCommand;
    // Brake Node
    bool bSolenoid1;
    bool bSolenoid2;
    bool bSolenoid3;
    bool bSolenoid4;
    bool bSolenoid5;
    bool bSolenoid6;
    float iLowPressure1;
    int32_t iLowPressure2;
    int32_t iLowPressure3;
    int32_t iLowPressure4;
    float iHighPressure;
    float iPressureVesselTemperature;
    float iRailTemperature;
};

struct PodNetwork {

    std::vector<int32_t> iActiveNodes = {0, 0, 0};

    std::vector<std::string> cNodeIpAddrs; //IP addrs of all nodes order: Brake, Rear, LVDC, Enclosure
    int32_t iBrakeNodeServerPortNumber;
    int32_t iBrakeNodePort; //Port # used by nodes to receive UDP updates
    int32_t iNodeTimeoutMili;

    int32_t iNodeClientSocket; //Port used to send data to nodes

    int32_t iCommanderPortNumber; //Port # used by TCP Commander socket
    int32_t iCommaderTimeoutMili; //Timeout

    int32_t iPdsTelemeteryPort; // Port # to send telemetry
    std::string strPdsIpAddr;

};

struct clientSocketConfig {
    sockaddr_in addr;
    int sckt;
};

#endif


