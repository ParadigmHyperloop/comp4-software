#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "Paradigm.pb.h"

class PodState;

struct PodValues {
    // States
    std::unique_ptr<PodState> podState;
    ControlsInterfaceStates terminalState;
    MotorStates motorState;
    BrakeNodeStates brakeNodeState;
    LvdcNodeStates lvdcNodeState;

    //Manual States
    PodStates manualPodState;
    MotorStates manualMotorState;
    BrakeNodeStates manualBrakeNodeState;
    LvdcNodeStates manualLvdcNodeState;
    bool automaticTransitions;

    //ConnectionsArray
    std::vector<int32_t> connectionFlags = {false, false, false, false, false};
    std::vector<int32_t> sensorFlags = {};

    // HV-BMS
    float hvBatteryPackVoltage;
    float hvBatteryPackCurrent;
    float hvBatteryPackMinimumCellVoltage;
    float hvBatteryPackMaxCellVoltage;

    //Inverter
    int32_t maxIgbtTemperature;
    int32_t gateDriverTemperature;
    int32_t inverterControlBoardTemperature;
    int32_t motorTemperature;
    int32_t inverterBusVoltage;

    // Atmosphere
    double tubePressure;

    // Terminal
    ControlsInterfaceStates eTerminalCommand;

    // Brake Node
    bool solenoid1;
    bool solenoid2;
    bool solenoid3;
    bool solenoid4;
    float lowPressure1;
    float lowPressure2;
    float lowPressure3;
    float lowPressure4;
    float highPressure;
    float pressureVesselTemperature;
    std::vector<bool> manualSolenoidConfiguration = {};

    // DTS
    float rotorTemperature;
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


