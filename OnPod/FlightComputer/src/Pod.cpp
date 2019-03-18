#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"
#include "ProtoBuffer/NodeTelem.pb.h"



 Pod::Pod(PodValues* sPodValues)
 {
	 this->sPodValues = sPodValues;
	 return;
 };


int32_t Pod::setPodState(PodStates state)
 {
	 if(this->bWritePodState)
	 {
		 this->sPodValues->ePodState = state;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 PodStates Pod::getPodState()
 {
	 return this->sPodValues->ePodState;
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


int32_t Pod::setTerminalCommand(TerminalCommands command){
	 if(this->bWriteTerminalCommand)
	 {
		 this->sPodValues->eTerminalCommand= command;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

 TerminalCommands Pod::getTerminalCommand()
 {
	 return this->sPodValues->eTerminalCommand;
 };


 MotorStates Pod::getMotorState()
 {
	 return this->sPodValues->eMotorState;
 };

int32_t Pod::setMotorState(MotorStates eMotorState)
 {
	 if(this->bWriteMotorState)
	 {
		 this->sPodValues->eMotorState= eMotorState;
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

