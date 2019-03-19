#ifndef POD_H
#define POD_H

#include "FlightComputer/structs.h"

class Pod
{
	public:
		PodValues* sPodValues;
		Pod(PodValues*);
		// Pod States
		int32_t setPodState(PodStates, const char*);
		PodStates getPodState();
		// Brake Node State
		BrakeNodeStates getBrakeNodeState();
		int32_t setBrakeNodeState(BrakeNodeStates);
		// Terminal Commnand
		ControlsInterfaceStates getTerminalCommand();
		int32_t setTerminalCommand(ControlsInterfaceStates);
		// Motor State
		MotorStates getMotorState();
		int32_t setMotorState(MotorStates);
		// Flags Array
		unsigned char* getFlagsArray();
		int32_t getFlagsArraySize();
		int32_t getNodeServerPortNumber();

		// Permissions
		bool bWritePodState = 0;
		bool bWriteTerminalCommand = 0;
		bool bWriteMotorState = 0;
		bool bWriteBreakNodeState = 0;
		bool bWritePosition = 0;
		bool bWriteDistance = 0;
		bool bWriteVelocity = 0;
		bool bWriteFlagsArray = 0;
		bool bWriteTubePressure = 0;
		bool bWriteBrakeNode = 0;

};

#endif
