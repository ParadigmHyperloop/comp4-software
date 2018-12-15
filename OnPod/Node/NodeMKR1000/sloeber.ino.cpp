#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-12-15 16:18:02

#include "Arduino.h"
#include <Ethernet.h>
#include <Sodaq_wdt.h>
#include "Timer.h"

void setup();
void loop() ;
void connectToTCP();
int readTCP();
void sendToBBB(void *ignore);
void readADCs(void *ignore);
bool startBrakeSequence(bool fromBBB);
bool readInverter();
bool readBrakes();
void activateSolenoidH1();
void activateSolenoidH2();
void activateSolenoidBleed();


#include "Node.ino"

#endif
