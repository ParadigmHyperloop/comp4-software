#include "CanManager.h"
#include <algorithm>
#include <initializer_list>

void processFrame(const struct canfd_frame &frame, TelemetryManager &pod) {
    std::vector<int> indices = {0, 1};
    switch (frame.can_id) {
        case 0x6b2: {
            indices = {0, 1};
            auto hvBatteryPackVoltage = extractCanValue<float>(frame.data, indices, (float) 10.0);
            indices = {2, 3};
            auto hvBatteryPackCurrent = extractCanValue<float>(frame.data, indices, (float) 10.0);
            indices = {4, 5};
            auto hvBatteryPackMaxCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);
            indices = {6, 7};
            auto hvBatteryPackMinimumCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);
            pod.setHvBatteryPackCurrent(hvBatteryPackCurrent);
            pod.setHvBatteryPackVoltage(hvBatteryPackVoltage);
            pod.setHvBatteryPackMaxCellVoltage(hvBatteryPackMaxCellVoltage);
            pod.setHvBatteryPackMinimumCellVoltage(hvBatteryPackMinimumCellVoltage);
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

            pod.telemetry->maxIgbtTemperature = maxIgbtTemperature;
            pod.telemetry->gateDriverTemperature = gateDriverBoard;
        }
        case 0x0A1: {
            indices = {1,0};
            auto controlBoard = extractCanValue<int>(frame.data, indices, 10);
            pod.telemetry->inverterControlBoardTemperature = controlBoard;
        }
        case 0x0A2: {
            indices = {5,4};
            auto filteredMotorTemp = extractCanValue<int>(frame.data, indices, 10);
            pod.telemetry->motorTemperature = filteredMotorTemp;
        }
        case 0x0A7:{
            indices = {1,0};
            auto dcBusVoltage = extractCanValue<int>(frame.data, indices, 10);
            pod.telemetry->inverterBusVoltage = dcBusVoltage;
        }
            break;
        default:
            //todo throw error instead
            LOG(INFO) << "Unknown CAN ID" << frame.can_id;
            break;
    }
}


