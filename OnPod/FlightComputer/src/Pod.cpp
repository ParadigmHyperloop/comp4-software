#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"
#include "ProtoBuffer/Paradigm.pb.h"
#include "EasyLogger/easylogging++.h"



 Pod::Pod(PodValues* sPodValues)
 {
	 this->sPodValues = sPodValues;
	 return;
 };


int32_t Pod::setPodState(PodStates state,  const char* reason)
 {

	 if(this->bWritePodState)
	 {
		 LOG(INFO)<< reason;
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


int32_t Pod::setBrakeNodeState(BrakeNodeStates eState)
{
	 if(this->bWriteBreakNodeState)
	 {
		 this->sPodValues->eBreakNodeState = eState;
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 };

BrakeNodeStates Pod::getBrakeNodeState()
 {
	 return this->sPodValues->eBreakNodeState;
 };


int32_t Pod::setTerminalCommand(ControlsInterfaceStates command){
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

ControlsInterfaceStates Pod::getTerminalCommand()
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

