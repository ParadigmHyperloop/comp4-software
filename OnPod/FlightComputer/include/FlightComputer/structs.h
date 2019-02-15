#ifndef STUCTS_H
#define STUCTS_H

enum ePodStates { psStandby, psArming, psArmed, psAcceleration, psCoasting, psBraking, psDisarm, psRetrieval, psEmergency, psBooting };

enum eBreakNodeStates { bnsBooting, bnsStandby, bnsArming, bnsArmed, bnsFlight, bnsBraking, bnsVenting, bnsRetrieval, bnsError};

enum eTerminalStates { tsConnected, tsDropped, tsTerminalEmergency};

enum eTerminalCommands { tcTerminalArm, tcTerminalFlight, tcTerminalStop, tcTerminalNone};

enum eMotorStates { msIdle, msDrive};

struct PodValues
{
  // States
  ePodStates PodState = psBooting;
  eTerminalStates TerminalState;
  eMotorStates MotorState;
  eBreakNodeStates BreakNodeState;
  // Navigation
  float fDistance;
  float fVelocity;
  // Rear Node
  float fGpioValues;
  //FlagsV2
  char cFlagsArray[3] = {0};
  int iFlagsArraySize = 3;
  // Atmosphere
  double dTubePressure;
  // Terminal
  eTerminalCommands TerminalCommand = tcTerminalNone;
};

struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};

#endif


