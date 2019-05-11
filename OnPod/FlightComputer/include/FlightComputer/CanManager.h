#ifndef FLIGHTCOMPUTER_CANMANAGER_H
#define FLIGHTCOMPUTER_CANMANAGER_H


#include <linux/can.h>
#include <linux/can/raw.h>
#include <endian.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <iomanip>
#include <cerrno>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "FlightComputer/NetworkHelpers.h"
#include "FlightComputer/Common.h"
#include "EasyLogger/easylogging++.h"

int CanThread(Pod);

unsigned int convertBytesToInt(int, int);


template <class T>
T extractCanValue(__uint8_t data[], int byteIndexs[], int numberOfBytes , T conversionFactor){
    unsigned int value;
    T tConverted;
    std::stringstream strStream;
    strStream << std::hex;
    for(int i = 0 ; i< numberOfBytes ; i++){
        strStream << data[byteIndexs[i]];
    }
    strStream >> value;
    tConverted = value/conversionFactor;
    return tConverted;
}


#endif //FLIGHTCOMPUTER_CANMANAGER_H