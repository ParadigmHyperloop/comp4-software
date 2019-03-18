#ifndef MEMORYACCESS_H
#define MEMORYACCESS_H

#include "FlightComputer/structs.h"

class Pod
{
	public:
		PodValues* sPodValues;
		Pod(PodValues*);
		// Pod States
		int32_t setPodState(PodStates);
		PodStates getPodState();
		// Brake Node State
		fc::brakeNodeData::breakNodeState getBrakeNodeState();
		int32_t setBrakeNodeState(fc::brakeNodeData::breakNodeState);
		// Terminal Commnand
		TerminalCommands getTerminalCommand();
		int32_t setTerminalCommand(TerminalCommands);
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
