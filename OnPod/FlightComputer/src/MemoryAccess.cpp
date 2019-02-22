#include "FlightComputer/structs.h"
#include "FlightComputer/MemoryAccess.h"



 MemoryAccess::MemoryAccess(PodValues sPodValues)
 {
	 this->sPodValues = sPodValues;
 };


 int MemoryAccess::setPodState(ePodStates state)
 {
	 if(this->bWritePodState)
	 {
		 this->sPodValues.PodState = state;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 ePodStates MemoryAccess::getPodState()
 {
	 return this->sPodValues.PodState;
 }


 int MemoryAccess::setBrakeNodeState(eBreakNodeStates state){
	 if(this->bWriteBreakNodeState)
	 {
		 this->sPodValues.BreakNodeState = state;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 eBreakNodeStates MemoryAccess::getBrakeNodeState()
 {
	 return this->sPodValues.BreakNodeState;
 };


 int MemoryAccess::setTerminalCommand(eTerminalCommands command){
	 if(this->bWriteTerminalCommand)
	 {
		 this->sPodValues.TerminalCommand= command;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 eTerminalCommands MemoryAccess::getTerminalCommand()
 {
	 return this->sPodValues.TerminalCommand;
 };


 eMotorStates MemoryAccess::getMotorState()
 {
	 return this->sPodValues.MotorState;
 };

 int MemoryAccess::setMotorState(eMotorStates eMotorState)
 {
	 if(this->bWriteMotorState)
	 {
		 this->sPodValues.MotorState= eMotorState;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };


 char* MemoryAccess::getFlagsArray()
 {
	 return this->sPodValues.cFlagsArray;
 };

 int MemoryAccess::getFlagsArraySize()
 {
	 return this->sPodValues.iFlagsArraySize;
 };



