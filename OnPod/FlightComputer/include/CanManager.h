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

#include "NetworkHelpers.h"
#include "Common.h"
#include "TelemetryManager.h"

int CanThread(TelemetryManager);

void processFrame(const struct canfd_frame &frame, TelemetryManager &pod);

template <class T>
T extractCanValue(const __u8 data[], const std::vector<int> &byteIndices, T conversionFactor){
    unsigned int value;
    T tConverted;
    std::stringstream strStream;
    strStream << std::hex;
    for(auto const& index: byteIndices){
        strStream << data[index];
    }
    strStream >> value;
    tConverted = value/conversionFactor;
    return tConverted;
}

int CanThread(TelemetryManager Pod);


#endif //FLIGHTCOMPUTER_CANMANAGER_H