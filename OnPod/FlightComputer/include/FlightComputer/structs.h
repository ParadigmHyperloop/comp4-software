#ifndef STUCTS_H
#define STUCTS_H
#include <netinet/in.h>
#include <cstdint>
#include "ProtoBuffer/Paradigm.pb.h"



//enum PodStates { psStandby, psArming, psArmed, psAcceleration, psCoasting, psBraking, psDisarm, psRetrieval, psEmergency, psBooting };

//enum eBreakNodeStates { bnsBooting, bnsStandby, bnsArming, bnsArmed, bnsFlight, bnsBraking, bnsVenting, bnsRetrieval, bnsError};

//enum TerminalStates { tsConnected, tsDropped, tsTerminalEmergency};

//enum TerminalCommands { tcTerminalArm, tcTerminalFlight, tcTerminalStop, tcTerminalNone};

//enum MotorStates { msIdle, msDrive};

struct PodValues
{
	// States
	PodStates ePodState = psBooting;
	ControlsInterfaceStates eTerminalState;
	MotorStates eMotorState;
	BrakeNodeStates eBrakeNodeState;
	LvdcNodeStates eLvdcNodeState;

	//Manual States
	PodStates eManualPodState;
	ControlsInterfaceStates eManualTerminalState;
	MotorStates eManualMotorState;
	BrakeNodeStates eManualBrakeNodeState;
	LvdcNodeStates eManualLvdcNodeState;
	bool bAutomaticTransitions;

	// Navigation
	float fDistance;
	float fVelocity;
	// Rear Node
	float fGpioValues;
	//FlagsV2
	unsigned char cFlagsArray[3] = {0};
	int32_t iFlagsArraySize = 3;
	// Atmosphere
	double dTubePressure;
	// Terminal
	ControlsInterfaceStates eTerminalCommand;
	// Brake Node
	bool bSolenoid1;
	bool bSolenoid2;
	bool bSolenoid3;
	bool bSolenoid4;
	bool bSolenoid5;
	bool bSolenoid6;
	int32_t iLowPressure1;
	int32_t iLowPressure2;
	int32_t iLowPressure3;
	int32_t iLowPressure4;
	int32_t iHighPressure;
	int32_t iPressureVesselTemperature;

	//Network
	int32_t iNodeServerPortNumber;

};

struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};

#endif


