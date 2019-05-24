#include "CanManager.h"




void processFrame(const struct canfd_frame &frame, Pod &pod) {
    switch (frame.can_id) {
        case 0x6b2: {
            std::vector<int> indices = {0,1};
            auto hvBatteryPackVoltage = extractCanValue <float> (frame.data, indices, (float)10.0);
            indices = {2,3};
            auto hvBatteryPackCurrent = extractCanValue <float>(frame.data, indices, (float)10.0);
            indices = {4,5};
            auto hvBatteryPackMaxCellVoltage = extractCanValue <float>(frame.data, indices, (float)10000.0);
            indices = {6,7};
            auto hvBatteryPackMinimumCellVoltage = extractCanValue <float>(frame.data, indices, (float)10000.0);

            pod.setHvBatteryPackCurrent(hvBatteryPackCurrent);
            pod.setHvBatteryPackVoltage(hvBatteryPackVoltage);
            pod.setHvBatteryPackMaxCellVoltage(hvBatteryPackMaxCellVoltage);
            pod.setHvBatteryPackMinimumCellVoltage(hvBatteryPackMinimumCellVoltage);
        }
            break;
        default:
            //todo throw error instead
            LOG(INFO) << "Unknown CAN ID" << frame.can_id;
            break;
    }
}


int CanThread(Pod Pod){
    //Logging
    el::Helpers::setThreadName("CAN Thread");
    LOG(INFO) << "Starting CAN Thread";

    // Service variables
    int operationStatus;

    // CAN connection variables
    struct sockaddr_can canSockAddr = {};
    struct ifreq interfaceRequest = {};
    int canSock;

    // Open the CAN network interface
    canSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canSock == -1) {
        LOG(INFO) << "Error: Creating CAN Socket" << std::strerror(errno);
        return -1;
    }

    // Set a receive filter so we only receive select CAN IDs
    struct can_filter canFilter[1];
    canFilter[0].can_id = 0x6B2;
    canFilter[0].can_mask = CAN_SFF_MASK;

    operationStatus = ::setsockopt(canSock, SOL_CAN_RAW, CAN_RAW_FILTER, &canFilter, sizeof(canFilter));
    if (operationStatus == -1) {
        LOG(INFO) << "Error: Setting CAN Filter: " << std::strerror(errno);
        return -1;
    }



    strcpy(interfaceRequest.ifr_name, "can0"); // Set Interface name
    operationStatus = ioctl(canSock, SIOCGIFINDEX, &interfaceRequest);
    if (operationStatus == -1) {
        LOG(INFO) << "Error: Setting CAN Interface" << std::strerror(errno);
        return -1;
    }

    // Bind the socket to the network interface
    canSockAddr.can_family = AF_CAN;
    canSockAddr.can_ifindex = interfaceRequest.ifr_ifindex;
    operationStatus = bind(
            canSock,
            reinterpret_cast<struct sockaddr *>(&canSockAddr),
            sizeof(canSockAddr)
    );
    if (operationStatus == -1) {
        LOG(INFO) << "Error: Binding CAN Interface" << std::strerror(errno);
        return -1;
    }


    LOG(INFO) << "Starting CAN Main Loop";
    while (Pod.telemetry->podState->getStateValue() != psShutdown) {
        struct canfd_frame canFrame = {0}; //TODO remove this zero once we dont need it
        // Read in a CAN CanFrame
        ssize_t iReceivedPacketSize = read(canSock, &canFrame, CANFD_MTU);
        if (iReceivedPacketSize) {
            processFrame(canFrame, Pod);
        }
    }
    close(canSock);
}
