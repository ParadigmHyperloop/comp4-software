#ifndef FLIGHTCOMPUTER_TELEMETRYMANAGER_H
#define FLIGHTCOMPUTER_TELEMETRYMANAGER_H

#include "Structs.h"
#include "States.h"

class TelemetryManager
{
	public:
		TelemetryManager();

		TelemetryManager(PodValues*, PodNetwork*);

		// TelemetryManager States
		int32_t setPodState(PodStates, const std::string&);


		// Motor State
		MotorStates getMotorState();
		int32_t setMotorState(MotorStates);

		// Flags Array
		unsigned char* getFlagsArray();
		int32_t getFlagsArraySize();
        void setSensorFlag(int32_t, int32_t);


		// Controls Interface
		void setControlsInterfaceState(ControlsInterfaceStates);
		ControlsInterfaceStates getControlsInterfaceState();

		// Manual State Control
		void setAutomaticTransitions(bool);
		void setManualBrakeNodeState(BrakeNodeStates);
		void setManualLvdcNodeState(LvdcNodeStates);
		void setManualPodState(PodStates);

		// High Voltage
		void setHvBatteryPackVoltage(float fValue);
        float getHvBatteryPackVoltage();
        void setHvBatteryPackCurrent(float fValue);
        float getHvBatteryPackCurrent();
        void setHvBatteryPackMinimumCellVoltage(float value);
        float getHvBatteryPackMinimumCellVoltage();
        void setHvBatteryPackMaxCellVoltage(float);
        float getHvBatteryPackMaxCellVoltage();

        //Shared Memory Space
        struct PodValues* telemetry;
        struct PodNetwork* sPodNetworkValues;

        // Brake Node
        void setLowPressure(float, int32_t);
        void setHighPressure(float);
        void setSolenoid(float value, int32_t identifier);
        void setPressureVesselTemperature(float);


		// Permissions
		bool bWritePodState = 0;
		bool bWriteControlsInterfaceState = 0;
		bool bWriteMotorState = 0;
		bool bWriteBreakNodeState = 0;
		bool bWriteManualStates = 0;
		bool bWriteHighVoltage = 0;
};

#endif //FLIGHTCOMPUTER_TELEMETRYMANAGER_H