#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "Paradigm.pb.h"

class PodState; // Fix for circular dependency

struct PodValues {
    // States
    std::unique_ptr<PodState> podState;
    ControlsInterfaceStates terminalState;
    MotorStates motorState;
    BrakeNodeStates brakeNodeState;
    LvdcNodeStates lvdcNodeState;

    //Manual States
    PodStates manualPodState;
    ControlsInterfaceStates manualTerminalState;
    MotorStates manualMotorState;
    BrakeNodeStates manualBrakeNodeState;
    LvdcNodeStates manualLvdcNodeState;
    bool automaticTransitions;

    //ConnectionsArray
    std::vector<bool> connectionsArray = {false, false, false, false, false}; // [brake , rear, lvdc, bms, inverter]

    // Navigation
    float distance = 0;
    float velocity = 0;
    // Rear Node
    float gpioValues;

    // HV-BMS
    float hvBatteryPackVoltage;
    float hvBatteryPackCurrent;
    float hvBatteryPackMinimumCellVoltage;
    float hvBatteryPackMaxCellVoltage;


    //FlagsV2
    unsigned char flagsArray[3] = {0};
    int32_t iFlagsArraySize = 3;
    // Atmosphere
    double tubePressure;
    // Terminal
    ControlsInterfaceStates eTerminalCommand;
    // Brake Node
    bool solenoid1;
    bool solenoid2;
    bool solenoid3;
    bool solenoid4;
    bool solenoid5;
    bool solenoid6;
    float lowPressure1;
    float lowPressure2;
    float lowPressure3;
    float lowPressure4;
    float highPressure;
    float pressureVesselTemperature;
    float railTemperature;
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


