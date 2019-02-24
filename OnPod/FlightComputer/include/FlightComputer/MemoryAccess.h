#ifndef MEMORYACCESS_H
#define MEMORYACCESS_H

#include "FlightComputer/structs.h"

class MemoryAccess
{
	public:
	 PodValues sPodValues;
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

	 MemoryAccess(PodValues);

	 int setPodState(ePodStates);
	 ePodStates getPodState();

	 eBreakNodeStates getBrakeNodeState();
	 int setBrakeNodeState(eBreakNodeStates);

	 eTerminalCommands getTerminalCommand();
	 int setTerminalCommand(eTerminalCommands);

	 eMotorStates getMotorState();
	 int setMotorState(eMotorStates);

	 char* getFlagsArray();
	 int getFlagsArraySize();
};

#endif
