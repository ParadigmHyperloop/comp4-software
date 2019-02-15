enum PodStates { psStandby, psArming, psArmed, psAcceleration, psCoasting, psBraking, psDisarm, psRetrieval, psEmergency, psBooting };

enum BreakNodeStates { bnsBooting, bnsStandby, bnsArming, bnsArmed, bnsFlight, bnsBraking, bnsVenting, bnsRetrieval, bnsError};

enum TerminalStates { tsConnected, tsDropped, tsTerminalEmergency};

enum TerminalCommands { tcTerminalArm, tcTerminalFlight, tcTerminalStop, tcTerminalNone};

enum MotorStates { msIdle, msDrive};

struct PodValues
{
  // States
  PodStates PodState = psBooting;
  TerminalStates TerminalState;
  MotorStates MotorState;
  BreakNodeStates BreakNodeState;
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
  TerminalCommands TerminalCommand = tcTerminalNone;
};


