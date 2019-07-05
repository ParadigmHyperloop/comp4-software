#ifndef FLIGHTCOMPUTER_TELEMETRYMANAGER_H
#define FLIGHTCOMPUTER_TELEMETRYMANAGER_H

#include <mutex>
#include "Structs.h"
#include "States.h"
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

		// Flags Array
        void setConnectionFlag(int32_t, int32_t);
        void setNodeSensorFlag(int32_t status, int32_t index);
        void setBmsSensorFlag(int32_t, int32_t);
        void setInverterSensorFlag(int32_t, int32_t);

        //Navigation
        void addPodDistance(float);
        void setPodDistance(float);
        void getPodDistance(float);
        void setPodVelocity(float);

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

        // BMS
		void setHvBatteryPackVoltage(float fValue);
        void setHvBatteryPackCurrent(float fValue);
        void setHvBatteryPackMinimumCellVoltage(float value);
        void setHvBatteryPackMaxCellVoltage(float);
        void setHvBatteryPackStateOfCharge(int);
        void setHvBatteryPackMaxCellTemperature(float);

        // Brake Node
        void setRecievedBrakeNodeState(BrakeNodeStates);
        void setLowPressure(float, int32_t);
        void setHighPressure(float);
        void setSolenoid(bool value, int32_t identifier);
        void setPressureVesselTemperature(float);
        void setCoolantTemperature(float);

        //Shared Memory Space
        struct PodValues* telemetry;
        struct PodNetwork* sPodNetworkValues;
};

#endif //FLIGHTCOMPUTER_TELEMETRYMANAGER_H