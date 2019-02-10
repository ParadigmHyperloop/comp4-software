#include <Arduino.h>
#include "BrakeNodeStates.h"


void setup() {
  Serial.begin(9600);
  Serial.write("Brake Node Serial started, starting Ethernet next\r\n");
}

BrakeNodeState s(State::BOOT);

void loop() {
  s.TransitionToNextState();
  delay(2000);
  Serial.println((String)"The Current State is " + s.GetState());
  
}
