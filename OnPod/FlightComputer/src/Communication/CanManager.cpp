#include "CanManager.h"
#include <algorithm>
#include <initializer_list>

void processFrame(const struct can_frame &frame, TelemetryManager &pod) {
    std::vector<int> indices = {0, 1};
    switch (frame.can_id) {
        case 0x6b3:{
            indices = {2, 3};
            auto hvBatteryPackVoltage = extractCanValue<float>(frame.data, indices, (float) 10.0);
            indices = {0, 1};
            auto hvBatteryPackCurrent = extractCanValue<float>(frame.data, indices, (float) 10.0);
            indices = {4};
            auto hvBatteryPackSoc = extractCanValue<int>(frame.data, indices, 2);
            pod.setHvBatteryPackStateOfCharge(hvBatteryPackSoc);
            pod.setHvBatteryPackCurrent(hvBatteryPackCurrent);
            pod.setHvBatteryPackVoltage(hvBatteryPackVoltage);
            pod.setConnectionFlag(1,BMS_HEARTBEAT_INDEX);
        }
        case 0x6b2: {
            indices = {2, 3};
            auto hvBatteryPackMaxCellTemperature = extractCanValue<float>(frame.data, indices, (float) 1.0);
            indices = {6, 7};
            auto hvBatteryPackMaxCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);
            indices = {4, 5};
            auto hvBatteryPackMinimumCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);
            pod.setHvBatteryPackMaxCellVoltage(hvBatteryPackMaxCellVoltage);
            pod.setHvBatteryPackMinimumCellVoltage(hvBatteryPackMinimumCellVoltage);
            pod.setHvBatteryPackMaxCellTemperature(hvBatteryPackMaxCellTemperature);

        }
        case 0x0A0: {
            indices = {1,0};
            auto igbtPhaseA = extractCanValue<int>(frame.data, indices, 10);
            indices = {3,2};
            auto igbtPhaseB = extractCanValue<int>(frame.data, indices, 10);
            indices = {5,4};
            auto igbtPhaseC = extractCanValue<int>(frame.data, indices, 10);
            auto maxIgbtTemperature = std::max({igbtPhaseA, igbtPhaseB, igbtPhaseC});
            indices = {7,6};
            auto gateDriverBoard = extractCanValue<int>(frame.data, indices, 10);
            pod.setMaxIgbtTemperature(maxIgbtTemperature);
            pod.setGateDriverTemperature(gateDriverBoard);
        }
        case 0x0A1: {
            indices = {1,0};
            auto controlBoard = extractCanValue<int>(frame.data, indices, 10);
            pod.setInverterControlBoardTemperature(controlBoard);
        }
        case 0x0A2: {
            indices = {5,4};
            auto motorTemp = extractCanValue<int>(frame.data, indices, 10);
            pod.setMotorTemperature(motorTemp);
        }
        case 0x0A5: {
            indices = {3,2};
            auto motorSpeed = extractCanValue<int>(frame.data, indices, 1);
            pod.setMotorSpeed(motorSpeed);
        }
        case 0x0A7: {
            indices = {1,0};
            auto dcBusVoltage = extractCanValue<int>(frame.data, indices, 10);
            pod.setInverterBusVoltage(dcBusVoltage);
            pod.setInverterHeartbeat(1);
        }
        default:
            return;
            //std::string error = "Unknown CAN ID : " + std::to_string(frame.can_id);
            //throw std::runtime_error(error);
    }
}


