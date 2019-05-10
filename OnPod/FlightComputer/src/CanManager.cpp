
#include "FlightComputer/CanManager.h"




void processFrame(const struct canfd_frame &frame, Pod pod) {
    switch (frame.can_id) {
        case 0x6b2: {
            float iHvBatteryPackVoltage = extractCanValue <float> (&frame.data, [0,1], 2, 10.0);
            float iHvBatteryPackCurrent = extractCanValue <float>(&frame.data, [2,3], 2, 10.0);
            float iHvBatteryPackMaxCellVoltage = extractCanValue <float>(&frame.data, [4,5], 2, 10000.0);
            float iHvBatteryPackMinimumCellVoltage = extractCanValue <float>(&frame.data, [6,7], 2, 10000.0);

                    //setters

        }
            break;
        default:
            LOG(INFO) << "Unknown CAN ID" << frame.can_id;
            break;
    }
}

unsigned int convertBytesToInt(int iByteMostSig, int iByteLeastSig) {
    unsigned int uiValue;
    std::stringstream strStream;
    strStream << std::hex << iByteMostSig << iByteLeastSig;
    strStream >> uiValue;
    return uiValue;
}


int CanThread(Pod Pod){
    //Logging
    el::Helpers::setThreadName("CAN Thread");
    LOG(INFO) << "Starting CAN Thread";

    // Service variables
    int iOpStatus;

    // CAN connection variables
    struct sockaddr_can CanSockAddrStruct = {};
    struct ifreq InterfaceRequest = {};
    int iCanSock;

    // Open the CAN network interface
    iCanSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (-1 == iCanSock) {
        LOG(INFO) << "Error: Creating CAN Socket" << std::strerror(errno);
        return -1;
    }

    // Set a receive filter so we only receive select CAN IDs
    {
        struct can_filter filter[1];
        filter[0].can_id = 0x6B2;
        filter[0].can_mask = CAN_SFF_MASK;

        iOpStatus = ::setsockopt(iCanSock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
        if (-1 == iOpStatus) {
            LOG(INFO) << "Error: Setting CAN Filter: " << std::strerror(errno);
            return -1;
        }
    }


    strcpy(InterfaceRequest.ifr_name, "can0"); // Set Interface name
    iOpStatus = ioctl(iCanSock, SIOCGIFINDEX, &InterfaceRequest);
    if (iOpStatus == -1) {
        LOG(INFO) << "Error: Setting CAN Interface" << std::strerror(errno);
        return -1;
    }

    // Bind the socket to the network interface
    CanSockAddrStruct.can_family = AF_CAN;
    CanSockAddrStruct.can_ifindex = InterfaceRequest.ifr_ifindex;
    iOpStatus = bind(
            iCanSock,
            reinterpret_cast<struct sockaddr *>(&CanSockAddrStruct),
            sizeof(CanSockAddrStruct)
    );
    if (iOpStatus == -1) {
        LOG(INFO) << "Error: Binding CAN Interface" << std::strerror(errno);
        return -1;
    }


    LOG(INFO) << "Starting CAN Main Loop";
    while (Pod.sPodValues->ePodState != psShutdown) {
        struct canfd_frame CanFrame = {0}; //TODO remove this zero once we dont need it
        // Read in a CAN CanFrame
        ssize_t iReceivedPacketSize = read(iCanSock, &CanFrame, CANFD_MTU);
        if (iReceivedPacketSize) {
            processFrame(CanFrame, Pod);
        }
    }
    close(iCanSock);
}
