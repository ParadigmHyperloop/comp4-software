#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "Paradigm.pb.h"

class PodState;

struct PodValues {
    // States
    std::unique_ptr<PodState> podState;
    ControlsInterfaceStates controlsInterfaceState = ciNone;
    BrakeNodeStates brakeNodeState = bnsNone;
    LvdcNodeStates lvdcNodeState = lvdcNone;

    // Flight Profile
    int32_t motorTorque = 0;
    int32_t flightDistance = 0;
    int32_t maxFlightTime = 0;

    //Manual States
    PodStates manualPodState = psNone;
    BrakeNodeStates manualBrakeNodeState = bnsNone;
    LvdcNodeStates manualLvdcNodeState = lvdcNone;
    bool automaticTransitions = true;

    //ConnectionsArray
    std::vector<int32_t> connectionFlags;
    std::vector<int32_t> sensorFlags;

    // HV-BMS
    float hvBatteryPackVoltage = 0;
    float hvBatteryPackCurrent = 0;
    float hvBatteryPackMinimumCellVoltage = 0;
    float hvBatteryPackMaxCellVoltage = 0;

    //Inverter
    int32_t maxIgbtTemperature = 0;
    int32_t gateDriverTemperature = 0;
    int32_t inverterControlBoardTemperature = 0;
    int32_t motorTemperature = 0;
    int32_t inverterBusVoltage = 0;

    // Atmosphere
    double tubePressure = 0;

    // Brake Node
    bool solenoid1;
    bool solenoid2;
    bool solenoid3;
    bool solenoid4;
    float lowPressure1 = 0;
    float lowPressure2 = 0;
    float lowPressure3 = 0;
    float lowPressure4 = 0;
    float highPressure = 0;
    float pressureVesselTemperature = 0;
    std::vector<bool> manualSolenoidConfiguration = {};

    // DTS
    float rotorTemperature = 0;
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


