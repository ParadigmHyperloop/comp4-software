#ifndef FLIGHTCOMPUTER_TELEMETRYMANAGER_H
#define FLIGHTCOMPUTER_TELEMETRYMANAGER_H

#include <mutex>
#include "Structs.h"
#include "States.h"
#include "Constants/SensorConfig.h"
#include "Constants/Constants.h"
#include <mutex>

class TelemetryManager
{
	public:
		TelemetryManager();

		TelemetryManager(PodValues*, PodNetwork*);
        // Send Updates
        void sendUpdate(std::string);

		// TelemetryManager States
        void setPodState(PodStates newState, const std::string &reason);
		PodStates getPodStateValue();
		ControlsInterfaceStates getControlsInterfaceState();

		// Flags Array
        void setConnectionFlag(int32_t, int32_t);
        void setNodeSensorFlag(int32_t status, int32_t index);
        void setBmsSensorFlag(int32_t, int32_t);
        void setInverterSensorFlag(int32_t, int32_t);
        void resetValues(int32_t);

        //Navigation
        void addPodDistance(float);
        void setPodDistance(float);
        float getPodDistance();
        void setPodVelocity(float);
        void countIrTape();

		// Controls Interface
		void setControlsInterfaceState(ControlsInterfaceStates);

		// Manual State Control
		void setAutomaticTransitions(bool);
		void setManualBrakeNodeState(BrakeNodeStates);
		void setManualLvdcNodeState(LvdcNodeStates);
		void setManualPodState(PodStates);

		// Enclosure
		void setEnclosurePressure(float);
        void setEnclosureTemperature(float);
        void setCoolantLinePressure(float);

		// Inverter
		void setMaxIgbtTemperature(float);
		void setGateDriverTemperature(float);
		void setInverterControlBoardTemperature(float);
		void setMotorTemperature(float);
		void setMotorSpeed(int32_t value);
		void setInverterBusVoltage(int);
        void setInverterHeartbeat(int32_t);
        void setPostFaultLo(int32_t);
        void setPostFaultHi(int32_t);
        void setRunFaultLo(int32_t);
        void setRunFaultHi(int32_t);


        // HV BMS
		void setHvBatteryPackVoltage(float);
        void setHvBatteryPackCurrent(float);
        void setHvBatteryPackMinimumCellVoltage(float);
        void setHvBatteryPackMaxCellVoltage(float);
        void setHvBatteryPackStateOfCharge(int);
        void setHvBatteryPackMaxCellTemperature(float);
        void updateCellVoltage(int32_t ,float);

        // LV BMS
        void setLv1BatteryPackStateOfCharge(int);
        void setLv1BatteryPackVoltage(float);
        void setLv1BatteryPackTemperature(float);
        void setLv2BatteryPackStateOfCharge(int);
        void setLv2BatteryPackVoltage(float);
        void setLv2BatteryPackTemperature(float);


    // Brake Node
        void setReceivedBrakeNodeState(BrakeNodeStates value);
        void setLowPressure(float, int32_t);
        void setHighPressure(float);
        void setSolenoid(bool value, int32_t identifier);
        void setPressureVesselTemperature(float);
        void setCoolantTemperature(float);
        void setLowPressure4(float, PodStates);

        //Shared Memory Space
        struct PodValues* telemetry;
        struct PodNetwork* sPodNetworkValues;
};

#endif //FLIGHTCOMPUTER_TELEMETRYMANAGER_H