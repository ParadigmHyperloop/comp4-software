#ifndef POD_H
#define POD_H

#include "FlightComputer/Structs.h"

class Pod
{
	public:

		PodValues* sPodValues;
		PodNetwork* sPodNetworkValues;

		Pod(){};

		Pod(PodValues*, PodNetwork*);
		// Pod States
		int32_t setPodState(PodStates, const char*);
		PodStates getPodState();

		// Brake Node State
		BrakeNodeStates getBrakeNodeState();
		int32_t setBrakeNodeState(BrakeNodeStates);

		// Motor State
		MotorStates getMotorState();
		int32_t setMotorState(MotorStates);

		// Flags Array
		unsigned char* getFlagsArray();
		int32_t getFlagsArraySize();

		int32_t getNodeServerPortNumber();

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


		// Permissions
		bool bWritePodState = 0;
		bool bWriteControlsInterfaceState = 0;
		bool bWriteMotorState = 0;
		bool bWriteBreakNodeState = 0;
		bool bWriteManualStates = 0;
		bool bWriteHighVoltage = 0;





};

#endif
