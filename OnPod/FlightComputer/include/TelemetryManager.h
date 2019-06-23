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
		int32_t setPodState(PodStates, const std::string&);
		PodStates getPodStateValue();

		// Flags Array
        void setConnectionFlag(int32_t, int32_t);
        void setNodeSensorFlag(int32_t status, int32_t index);
        void setBmsSensorFlag(int32_t, int32_t);
        void setInverterSensorFlag(int32_t, int32_t);

		// Controls Interface
		void setControlsInterfaceState(ControlsInterfaceStates);

		// Manual State Control
		void setAutomaticTransitions(bool);
		void setManualBrakeNodeState(BrakeNodeStates);
		void setManualLvdcNodeState(LvdcNodeStates);
		void setManualPodState(PodStates);

		// Inverter
		void setMaxIgbtTemperature(float);
		void setGateDriverTemperature(float);
		void setInverterControlBoardTemperature(float);
		void setMotorTemperature(float);
		void setMotorSpeed(float);
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
        void setLowPressure(float, int32_t);
        void setHighPressure(float);
        void setSolenoid(bool value, int32_t identifier);
        void setPressureVesselTemperature(float);

        //Shared Memory Space
        struct PodValues* telemetry;
        struct PodNetwork* sPodNetworkValues;

		// Permissions
		bool bWritePodState = 0;
		bool bWriteControlsInterfaceState = 0;
		bool bWriteManualStates = 0;
		bool bWriteBms = 0;
		bool bWriteInverter = 0;
};

#endif //FLIGHTCOMPUTER_TELEMETRYMANAGER_H