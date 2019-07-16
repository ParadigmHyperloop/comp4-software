#ifndef STUCTS_H
#define STUCTS_H

#include <netinet/in.h>
#include <cstdint>
#include "Paradigm.pb.h"
#include <chrono>

class PodState;

struct PodValues {
    // States
    std::mutex stateLock;
    std::unique_ptr<PodState> podState;
    ControlsInterfaceStates controlsInterfaceState = ciNone;
    BrakeNodeStates receivedBrakeNodeState = bnsNone;
    BrakeNodeStates commandedBrakeNodeState = bnsNone;
    LvdcNodeStates receivedLvdcNodeState = lvdcNone;
    LvdcNodeStates commandedLvdcNodeState = lvdcNone;

    // Flight Profile
    uint32_t motorTorque = 0;
    uint32_t flightDistance = 0;
    uint32_t maxFlightTime = 0;
    uint32_t startTorque = 0;
    uint32_t accelerationTime = 0;
    uint32_t expectedTubePressure = 0;
    uint32_t maxVelocity = 0;
    uint32_t brakeDistance = 0;
    uint32_t maxStripCount = 0;
    bool taxi  = false;

    //Navigation
    std::mutex positionLock;
    float motorDistance = 0;
    float podVelocity = 0;
    float podPosition = 0;
    int totalStripCount = 0;
    float stripVelocity = 0;

    // Updates
    std::mutex updatesLock;
    std::vector<std::string> updates;

    //Manual States
    bool checkNodeStates = true;
    PodStates manualPodState = psNone;
    BrakeNodeStates manualBrakeNodeState = bnsNone;
    LvdcNodeStates manualLvdcNodeState = lvdcNone;
    bool automaticTransitions = true;

    //ConnectionsArray
    std::vector<int8_t> connectionFlags;  // brakeNode, LVDCNode, BMS, Interface
    std::vector<int8_t> nodeSensorFlags;
    std::vector<int8_t> inverterSensorFlags;
    std::vector<int8_t> bmsSensorFlags;

    // HV-BMS
    float hvBatteryPackVoltage = 0;
    float hvBatteryPackCurrent = 0;
    float hvBatteryPackMinimumCellVoltage = 0;
    float hvBatteryPackMaxCellVoltage = 0;
    int hvBatteryPackStateOfCharge = 0;
    float hvBatteryPackMaxCellTemperature = 0;
    int hvFaultCode1 = 0;
    int hvFaultCode2 = 0;

    // LV-BMS
    int32_t lv1BatteryPackStateOfCharge = 0;
    float lv1BatteryPackVoltage = 0;
    float lv1BatteryPackCellTemperature = 0;

    int32_t lv2BatteryPackStateOfCharge = 0;
    float lv2BatteryPackVoltage = 0;
    float lv2BatteryPackCellTemperature = 0;

    //Inverter
    float maxIgbtTemperature = 0;
    float gateDriverTemperature = 0;
    float inverterControlBoardTemperature = 0;
    float motorTemperature = 0;
    int32_t motorSpeed = 0;
    float inverterBusVoltage = 0;
    float commandedTorque = 0;
    int32_t inverterHeartbeat = 0;
    std::chrono::high_resolution_clock::time_point lastMotorReadTime;

    // Atmosphere
    double tubePressure = 0;

    // Enclosure
    float enclosurePressure = 0;
    float enclosureTemperature = 0;
    float coolingLinePressure = 0;

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
    float coolingTemperature = 0;
    std::vector<bool> manualSolenoidConfiguration = {};

    // DTS
    float rotorTemperature = 0;
};

struct PodNetwork {
    std::vector<int32_t> iActiveNodes = {0, 0, 0};

    std::vector<std::string> cNodeIpAddrs; //IP addrs of all nodes order: Brake, Rear, LVDC, Enclosure

    int32_t nodePort; //Port # used by nodes to receive UDP updates
    int32_t nodeTimeoutMili; // Max time to get update from a node
    int32_t nodeClientSocket; //Port used to send data to nodes

    int32_t brakeNodeServerPortNumber;
    int32_t brakeNodeUpdateFreq; // How often we send state to Brake Node;

    int32_t enclosureNodeServerNumber;
    int32_t lvdcNodeServerNumber;

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


