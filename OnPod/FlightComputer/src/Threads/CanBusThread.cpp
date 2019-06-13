#include "CanManager.h"
#include "CanBusThread.h"
#include "Heartbeat.h"

#define INVERTER_BROADCAST_ID 0
#define INVERTER_FRAME_ID 0x0C0

int32_t getCanSocketRaw(){
    // Service variables
    int32_t operationStatus;

    // CAN connection variables
    struct sockaddr_can canSockAddr = {};
    struct ifreq interfaceRequest = {};
    int32_t canSock;

    // Open the CAN network interface
    canSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canSock == -1) {
        std::string error = std::string("Error: Creating CAN Socket :") + std::strerror(errno);
        throw std::runtime_error(error);
    }

    int flags = fcntl(canSock, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(canSock, F_SETFL, flags);



    // Set a receive filter so we only receive select CAN IDs
    struct can_filter canFilter[3];
    canFilter[0].can_id = 0x6B2;
    canFilter[0].can_mask = CAN_SFF_MASK;

    canFilter[1].can_id = 0x0A0;
    canFilter[1].can_mask = CAN_SFF_MASK;

    canFilter[2].can_id = 0x0A7;
    canFilter[2].can_mask = CAN_SFF_MASK;

    operationStatus = ::setsockopt(canSock, SOL_CAN_RAW, CAN_RAW_FILTER, &canFilter, sizeof(canFilter));
    if (operationStatus == -1) {
        std::string error = std::string("Error: Setting CAN Filter: ") + std::strerror(errno);
        throw std::runtime_error(error);
    }
    strcpy(interfaceRequest.ifr_name, "can0"); // Set Interface name
    operationStatus = ioctl(canSock, SIOCGIFINDEX, &interfaceRequest);
    if (operationStatus == -1) {
        std::string error = std::string("Error: Setting CAN Interface :") + std::strerror(errno);
        throw std::runtime_error(error);
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
        std::string error = std::string("Error: Binding CAN Interface :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
    return canSock;
}

int32_t getCanSocketBrodcastManager() {
    int bcmSocket;
    int32_t operationStatus;
    struct sockaddr_can socketAddr = {};
    struct ifreq interfaceRequest = {};

    bcmSocket = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
    if(bcmSocket < 0){
        std::string error = std::string("Error: Creating Broadcast Manager CAN Socket :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
    strcpy(interfaceRequest.ifr_name, "can0");
    ioctl(bcmSocket, SIOCGIFINDEX, &interfaceRequest);
    socketAddr.can_family = AF_CAN;
    socketAddr.can_ifindex = interfaceRequest.ifr_ifindex;
    operationStatus = connect(bcmSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr));
    if(operationStatus == -1){
        std::string error = std::string("Error: Connecting Broadcast Manager CAN Socket :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
    return bcmSocket;
}

void startInverterBroadcast(int bcmSocket){
    struct broadcastManagerConfig bcmMessage = {};
    bcmMessage.msg_head.opcode  = TX_SETUP;
    bcmMessage.msg_head.can_id  = INVERTER_BROADCAST_ID;
    bcmMessage.msg_head.flags   = SETTIMER | STARTTIMER;
    bcmMessage.msg_head.nframes = 1;
    bcmMessage.msg_head.count   = 0;

    bcmMessage.msg_head.ival2.tv_sec = 0;
    bcmMessage.msg_head.ival2.tv_usec = 200000;

    bcmMessage.frame[0].can_id = INVERTER_FRAME_ID;
    bcmMessage.frame[0].can_dlc = 8;
    for(int i = 0; i < 8 ; i++){
        bcmMessage.frame[0].data[i] = 0;
    }
    if (write(bcmSocket, &bcmMessage, sizeof(bcmMessage)) < 0)
    {
        std::string error = std::string("Error: Writing inverter configuration message :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
}

void setInverterTorque(int torque, int bcmSocket){
    struct broadcastManagerConfig msg = {};
    int32_t highByte = torque/256;
    int32_t lowByte = torque%256;
    msg.msg_head.opcode  = TX_SETUP;
    msg.msg_head.can_id  = 0;
    msg.msg_head.flags   = SETTIMER | STARTTIMER;
    msg.msg_head.nframes = 1;
    msg.msg_head.count   = 0;

    msg.msg_head.ival2.tv_sec = 0;
    msg.msg_head.ival2.tv_usec = 100000; // If you dont set this makes sure its 0

    msg.frame[0].can_id = 0x0C0;
    msg.frame[0].can_dlc = 8;
    for(int i = 0; i < 8 ; i++){
        msg.frame[0].data[i] = 0;
    }
    if(torque != 0){
        msg.frame[0].data[5] = 1;
        msg.frame[0].data[4] = 1;
    }
    msg.frame[0].data[3] = highByte;
    msg.frame[0].data[2] = lowByte;
    if (write(bcmSocket, &msg, sizeof(msg)) < 0)
    {
        std::string error = std::string("Error: Setting inverter torque :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
}

int canNetworkThread(TelemetryManager Pod){
    //Logging
    el::Helpers::setThreadName("CAN Thread");
    LOG(INFO) << "Starting CAN Thread";
    int32_t canSockRaw = 0;
    int32_t canSockBcm = 0;
    try{
        canSockRaw = getCanSocketRaw();
    }
    catch (std::runtime_error &e){
        LOG(INFO) << e.what();
        return -1;
    }
    try{
        canSockBcm = getCanSocketBrodcastManager();
    }
    catch (std::runtime_error &e){
        LOG(INFO) << e.what();
        return -1;
    }
    try{
        startInverterBroadcast(canSockBcm);
    }
    catch (std::runtime_error &e){
        LOG(INFO) << e.what();
        return -1;
    }

    struct canfd_frame canFrame = {0};
    int32_t currentTorque, newTorque = 0;
    LOG(INFO) << "Starting CAN Main Loop";
    while ( Pod.telemetry->podState->getStateValue() != psShutdown) {
        // Read in a CAN CanFrame
        ssize_t iReceivedPacketSize = read(canSockRaw, &canFrame, CANFD_MTU);
        if (iReceivedPacketSize > 0 ) {
            processFrame(canFrame, Pod);
        }
        newTorque = Pod.telemetry->motorTorque;
        if(currentTorque != newTorque) {
            currentTorque = newTorque;
            setInverterTorque(currentTorque, canSockBcm);
        }
    }
    close(canSockRaw);
}
