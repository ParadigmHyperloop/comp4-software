#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"
#include "ProtoBuffer/NodeTelem.pb.h"



 Pod::Pod(PodValues* sPodValues)
 {
	 this->sPodValues = sPodValues;
	 return;
 };


int32_t Pod::setPodState(ePodStates state)
 {
	 if(this->bWritePodState)
	 {
		 this->sPodValues->PodState = state;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 ePodStates Pod::getPodState()
 {
	 return this->sPodValues->PodState;
 }


int32_t Pod::setBrakeNodeState(fc::brakeNodeData::breakNodeState state)
{
	 if(this->bWriteBreakNodeState)
	 {
		 this->sPodValues->BreakNodeState = state;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 fc::brakeNodeData::breakNodeState Pod::getBrakeNodeState()
 {
	 return this->sPodValues->BreakNodeState;
 };


int32_t Pod::setTerminalCommand(eTerminalCommands command){
	 if(this->bWriteTerminalCommand)
	 {
		 this->sPodValues->TerminalCommand= command;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 eTerminalCommands Pod::getTerminalCommand()
 {
	 return this->sPodValues->TerminalCommand;
 };


 eMotorStates Pod::getMotorState()
 {
	 return this->sPodValues->MotorState;
 };

int32_t Pod::setMotorState(eMotorStates eMotorState)
 {
	 if(this->bWriteMotorState)
	 {
		 this->sPodValues->MotorState= eMotorState;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };


 unsigned char* Pod::getFlagsArray()
 {
	 return this->sPodValues->cFlagsArray;
 };

int32_t Pod::getFlagsArraySize()
 {
	 return this->sPodValues->iFlagsArraySize;
 };

int32_t Pod::getNodeServerPortNumber()
{
	return this->sPodValues->iNodeServerPortNumber;
}

