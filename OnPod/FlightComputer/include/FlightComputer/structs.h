struct podValues {
  // States
  unsigned char podS = 0;
  unsigned char terminalS = 0;
  unsigned char motorS = 0;
  unsigned char rearNodeS;
  // Navigation
  float distance = 0;
  float velocity = 0;
  // Rear Node
  float gpioVals;
};
