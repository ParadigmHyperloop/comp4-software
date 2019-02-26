#ifndef STUCTS_H
#define STUCTS_H
#include <netinet/in.h>
#include <cstdint>
#include "ProtoBuffer/NodeTelem.pb.h"


enum ePodStates { psStandby, psArming, psArmed, psAcceleration, psCoasting, psBraking, psDisarm, psRetrieval, psEmergency, psBooting };

//enum eBreakNodeStates { bnsBooting, bnsStandby, bnsArming, bnsArmed, bnsFlight, bnsBraking, bnsVenting, bnsRetrieval, bnsError};

enum eTerminalStates { tsConnected, tsDropped, tsTerminalEmergency};

enum eTerminalCommands { tcTerminalArm, tcTerminalFlight, tcTerminalStop, tcTerminalNone};

enum eMotorStates { msIdle, msDrive};

struct PodValues
{
	// States
	ePodStates PodState = psBooting;
	eTerminalStates TerminalState;
	eMotorStates MotorState;
	fc::brakeNodeData::breakNodeState BreakNodeState;
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
	eTerminalCommands TerminalCommand = tcTerminalNone;
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

};

struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};

#endif


