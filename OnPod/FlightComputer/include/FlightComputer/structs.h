static enum PodStates { STANDBY, ARMING, ARMED, ACCELERATION, COASTING, BRAKING, DISARM, RETRIEVAL, EMERGENCY, BOOTING };

static enum BreakNodeStates { BOOTING, STANDBY, ARMING, ARMED, FLIGHT, BRAKING, VENTING, RETRIEVAL, ERROR};

static enum TerminalStates { CONNECTED, DROPPED, EMERGENCY};

static enum MotorStates { IDLE, DRIVE};


struct podValues {
  // States
  PodStates podS = BOOTING;
  TerminalStates terminalS;
  MotorStates motorS;
  BreakNodeStates breakNodeS;
  // Navigation
  float distance;
  float velocity;
  // Rear Node
  float gpioVals;
};


