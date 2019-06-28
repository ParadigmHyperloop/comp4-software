#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "Paradigm.pb.h"

#define BRAKE_NODE_HEARTBEAT_INDEX 0
#define LVDC_NODE_HEARTBEAT_INDEX 1
#define BMS_HEARTBEAT_INDEX 2
#define INTERFACE_HEARTBEAT_INDEX 3
#define ENCLOSURE_HEARTBEAT_INDEX 4

class PodState;

struct PodValues {
    // States
    std::mutex stateLock;
    std::unique_ptr<PodState> podState;
    ControlsInterfaceStates controlsInterfaceState = ciNone;
    BrakeNodeStates receivedBrakeNodeState = bnsNone;
    BrakeNodeStates commandedBrakeNodeState = bnsNone;
    LvdcNodeStates lvdcNodeState = lvdcNone;

    // Flight Profile
    int32_t motorTorque = 0;
    int32_t flightDistance = 0;
    int32_t maxFlightTime = 0;

    // Updates
    std::mutex updatesLock;
    std::vector<std::string> updates;

    //Manual States
    PodStates manualPodState = psNone;
    BrakeNodeStates manualBrakeNodeState = bnsNone;
    LvdcNodeStates manualLvdcNodeState = lvdcNone;
    bool automaticTransitions = true;

    //ConnectionsArray
    std::vector<int32_t> connectionFlags;  // brakeNode, LVDCNode, BMS, Interface
    std::vector<int32_t> nodeSensorFlags;
    std::vector<int32_t> inverterSensorFlags;
    std::vector<int32_t> bmsSensorFlags;

    // HV-BMS
    float hvBatteryPackVoltage = 0;
    float hvBatteryPackCurrent = 0;
    float hvBatteryPackMinimumCellVoltage = 0;
    float hvBatteryPackMaxCellVoltage = 0;
    int hvBatteryPackStateOfCharge = 0;
    float hvBatteryPackMaxCellTemperature = 0;

    //Inverter
    int32_t maxIgbtTemperature = 0;
    int32_t gateDriverTemperature = 0;
    int32_t inverterControlBoardTemperature = 0;
    int32_t motorTemperature = 0;
    int32_t motorSpeed = 0;
    int32_t inverterBusVoltage = 0;
    int32_t commandedTorque = 0;
    int32_t inverterHeartbeat = 0;

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
    int32_t brakeNodeServerPortNumber;
    int32_t brakeNodePort; //Port # used by nodes to receive UDP updates
    int32_t brakeNodeUpdateFreq; // How often we send state to Brake Node;

    int32_t nodeTimeoutMili; // Max time to get update from a node
    int32_t nodeClientSocket; //Port used to send data to nodes

    int32_t commanderPortNumber; //Port # used by TCP Commander socket
    int32_t commaderTimeoutMili; //Timeout

    int32_t pdsUpdateFreq; // How often we send telem to PDS
    int32_t pdsTelemeteryPort; // Port # to send telemetry
    std::string strPdsIpAddr;
};

struct clientSocketConfig {
    sockaddr_in addr;
    int sckt;
};

#endif


