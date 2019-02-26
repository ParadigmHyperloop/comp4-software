#ifndef MEMORYACCESS_H
#define MEMORYACCESS_H

#include "FlightComputer/structs.h"

class MemoryAccess
{
	public:
	 PodValues* sPodValues;
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

	 MemoryAccess(PodValues*);

	  int32_t setPodState(ePodStates);
	 ePodStates getPodState();

	 fc::brakeNodeData::breakNodeState getBrakeNodeState();
	  int32_t setBrakeNodeState(fc::brakeNodeData::breakNodeState);

	 eTerminalCommands getTerminalCommand();
	  int32_t setTerminalCommand(eTerminalCommands);

	 eMotorStates getMotorState();
	  int32_t setMotorState(eMotorStates);

	 unsigned char* getFlagsArray();
	  int32_t getFlagsArraySize();
};

#endif
