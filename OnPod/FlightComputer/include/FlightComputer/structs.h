enum PodStates { standby, arming, armed, acceleration, coasting, braking, disarm, retrieval, emergency, booting };

enum BreakNodeStates { bnBooting, bnStandby, bnArming, bnArmed, bnFlight, bnBraking, venting, bnRetrieval, bnError};

enum TerminalStates { connected, dropped, terminalEmergency};

enum TerminalCommands { terminalArm, terminalFlight, terminalStop, terminalNone};

enum MotorStates { idle, drive};

struct Nominalflags {
	  char highVoltageFlag;
	  char lowVoltageFlag;
	  char nodeWatchdog;
};


struct PodValues {
  // States
  PodStates podS = booting;
  TerminalStates terminalS;
  MotorStates motorS;
  BreakNodeStates breakNodeS;

  // Navigation
  float distance;
  float velocity;

  // Rear Node
  float gpioVals;

  // Flags
  Nominalflags* flags;

  //FlagsV2
  char flagsArray[3] = {0};
  int flagSize = 3;

  // Atmosphere
  double tubePressure;

  // Terminal
  TerminalCommands terminalCommand = terminalNone;

};


