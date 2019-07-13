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
            if(hvBatteryPackSoc > 1 && hvBatteryPackSoc < 110){
                pod.setHvBatteryPackStateOfCharge(hvBatteryPackSoc);
            }
            if(hvBatteryPackCurrent > -1  && hvBatteryPackCurrent < 1500 ){
                pod.setHvBatteryPackCurrent(hvBatteryPackCurrent);
            }
            if(hvBatteryPackVoltage < 1500){
                pod.setHvBatteryPackVoltage(hvBatteryPackVoltage);
            }
            break;
        }
        case 0x6b2: {
            indices = {2, 3};
            auto hvBatteryPackMaxCellTemperature = extractCanValue<float>(frame.data, indices, (float) 1.0);
            indices = {6, 7};
            auto hvBatteryPackMaxCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);
            indices = {4, 5};
            auto hvBatteryPackMinimumCellVoltage = extractCanValue<float>(frame.data, indices, (float) 10000.0);

            if(hvBatteryPackMinimumCellVoltage > 1 && hvBatteryPackMinimumCellVoltage < 100){
                pod.setHvBatteryPackMinimumCellVoltage(hvBatteryPackMinimumCellVoltage);
            }

            if(hvBatteryPackMaxCellVoltage > 1 && hvBatteryPackMaxCellVoltage < 200){
                pod.setHvBatteryPackMaxCellVoltage(hvBatteryPackMaxCellVoltage);
            }

            if(hvBatteryPackMaxCellTemperature > 1 && hvBatteryPackMaxCellTemperature < 300){
                pod.setHvBatteryPackMaxCellTemperature(hvBatteryPackMaxCellTemperature);
            }
            break;

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
            if(gateDriverBoard > 1 && gateDriverBoard < 200){
                pod.setGateDriverTemperature(gateDriverBoard);
            }
            if(gateDriverBoard > 1 && gateDriverBoard < 200){
                pod.setMaxIgbtTemperature(maxIgbtTemperature);
            }
            break;
        }
        case 0x0A1: {
            indices = {1,0};
            auto controlBoard = extractCanValue<int>(frame.data, indices, 10);
            if(controlBoard > 1 && controlBoard < 200){
                pod.setInverterControlBoardTemperature(controlBoard);
            }
            break;
        }
        case 0x0A2: {
            indices = {5,4};
            auto motorTemp = extractCanValue<int>(frame.data, indices, 10);
            if(motorTemp < 1 || motorTemp > 180){
                return;
            }
            pod.setMotorTemperature(motorTemp);
            break;
        }
        case 0x0A5: {
            indices = {3,2};
            auto motorSpeed = extractCanValue<int>(frame.data, indices, 1);
            if(motorSpeed > 12000 || motorSpeed == 1){
                return;
            }
            pod.setMotorSpeed(motorSpeed);
            pod.setConnectionFlag(1,BMS_HEARTBEAT_INDEX);
            break;
        }
        case 0x0A7: {
            indices = {1,0};
            auto dcBusVoltage = extractCanValue<int>(frame.data, indices, 10);
            pod.setInverterBusVoltage(dcBusVoltage);
            pod.setInverterHeartbeat(1);
            break;
        }
        case 0x6B4: { // lp1
            //todo
            indices = {0, 1};
            auto lv1BatteryPackVoltage = extractCanValue<float>(frame.data, indices, (float) 10.0);
            indices = {2};
            auto lv1BatteryPackSoc = extractCanValue<int>(frame.data, indices, 2);
            indices = {3, 4};
            auto lv1BatteryPackTemperature = extractCanValue<float>(frame.data, indices, (float) 1.0);


            if(lv1BatteryPackSoc > 1 && lv1BatteryPackSoc < 110){
                pod.setLv1BatteryPackStateOfCharge(lv1BatteryPackSoc);
            }
            if(lv1BatteryPackVoltage < 50){
                pod.setLv1BatteryPackVoltage(lv1BatteryPackVoltage);
            }
            if(lv1BatteryPackTemperature > 1 && lv1BatteryPackTemperature < 300){
                pod.setLv1BatteryPackTemperature(lv1BatteryPackTemperature);
            }

        }

        case 0x6B5: {  //hp
            //todo
            indices = {0, 1};
            auto lv2BatteryPackVoltage = extractCanValue<float>(frame.data, indices, (float) 10.0);

            indices = {2};
            auto lv2BatteryPackSoc = extractCanValue<int>(frame.data, indices, 2);

            indices = {3, 4};
            auto lv2BatteryPackTemperature = extractCanValue<float>(frame.data, indices, (float) 1.0);

            if(lv2BatteryPackSoc > 1 && lv2BatteryPackSoc < 110){
                pod.setLv2BatteryPackStateOfCharge(lv2BatteryPackSoc);
            }
            if(lv2BatteryPackVoltage < 50){
                pod.setLv2BatteryPackVoltage(lv2BatteryPackVoltage);
            }
            if(lv2BatteryPackTemperature > 1 && lv2BatteryPackTemperature < 300){
                pod.setLv2BatteryPackTemperature(lv2BatteryPackTemperature);
            }

        }

        default:
            return;
            //std::string error = "Unknown CAN ID : " + std::to_string(frame.can_id);
            //throw std::runtime_error(error);
    }
}


