#ifndef STUCTS_H
#define STUCTS_H
#include <netinet/in.h>
#include "ProtoBuffer/ProtoStructs.pb.h"


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
  int iFlagsArraySize = 3;
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
  int iLowPressure1;
  int iLowPressure2;
  int iLowPressure3;
  int iLowPressure4;
  int iHighPressure;
  int iPressureVesselTemperature;

};

struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};

#endif


