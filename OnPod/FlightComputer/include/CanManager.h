#ifndef FLIGHTCOMPUTER_CANMANAGER_H
#define FLIGHTCOMPUTER_CANMANAGER_H


#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/bcm.h>
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

struct broadcastManagerConfig
{
    struct bcm_msg_head msg_head;
    struct can_frame frame[1];
};

void processFrame(const struct can_frame &frame, TelemetryManager &pod);

template <class T>
T extractCanValue(const __u8 data[], const std::vector<int> &byteIndices, T conversionFactor){
    unsigned int value = 0;
    T tConverted = 0;
    std::stringstream strStream;
    strStream << std::hex;
    for(auto const& index: byteIndices){
        int byte = data[index];
        if(byte <= 15){
            strStream << 0;
        }
        strStream << byte;
    }
    strStream >> value;
    tConverted = value/conversionFactor;
    return tConverted;
}


#endif //FLIGHTCOMPUTER_CANMANAGER_H