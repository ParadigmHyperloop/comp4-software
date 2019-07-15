#include "CanManager.h"
#include "CanBusThread.h"
#include "Heartbeat.h"

#define INVERTER_BROADCAST_ID 0
#define INVERTER_FRAME_ID 0x0C0
#define INVERTER_HEARTBEAT_FRAME_ID 0x0A7
#define BMS_HEARTBEAT_FRAME_ID 0x6b3

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
    struct can_filter canFilter[10];
    canFilter[0].can_id = 0x6B2;
    canFilter[0].can_mask = CAN_SFF_MASK;
    canFilter[1].can_id = 0x0A0;
    canFilter[1].can_mask = CAN_SFF_MASK;
    canFilter[2].can_id = 0x0A1;
    canFilter[2].can_mask = CAN_SFF_MASK;
    canFilter[3].can_id = 0x0A2;
    canFilter[3].can_mask = CAN_SFF_MASK;
    canFilter[4].can_id = 0x0A7;
    canFilter[4].can_mask = CAN_SFF_MASK;
    canFilter[5].can_id = 0x0A5;
    canFilter[5].can_mask = CAN_SFF_MASK;
    canFilter[6].can_id = 0x6b3;
    canFilter[6].can_mask = CAN_SFF_MASK;
    canFilter[7].can_id = 0x0A6;
    canFilter[7].can_mask = CAN_SFF_MASK;
    canFilter[8].can_id = 0x6B5;
    canFilter[8].can_mask = CAN_SFF_MASK;
    canFilter[9].can_id = 0x6B6;
    canFilter[9].can_mask = CAN_SFF_MASK;


    operationStatus = ::setsockopt(canSock, SOL_CAN_RAW, CAN_RAW_FILTER, &canFilter, sizeof(canFilter));
    if (operationStatus == -1) {
        std::string error = std::string("Error: Setting CAN Filter: ") + std::strerror(errno);
        throw std::runtime_error(error);
    }
    strcpy(interfaceRequest.ifr_name, "can1"); // Set Interface name
    operationStatus = ioctl(canSock, SIOCGIFINDEX, &interfaceRequest);
    if (operationStatus == -1) {
        strcpy(interfaceRequest.ifr_name, "can0"); // Set Interface name
        operationStatus = ioctl(canSock, SIOCGIFINDEX, &interfaceRequest);
        if(operationStatus == -1){
            std::string error = std::string("Error: Setting CAN Interface :") + std::strerror(errno);
            throw std::runtime_error(error);
        }
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
    int flags = fcntl(bcmSocket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(bcmSocket, F_SETFL, flags);

    strcpy(interfaceRequest.ifr_name, "can1"); // Set Interface name
    operationStatus = ioctl(bcmSocket, SIOCGIFINDEX, &interfaceRequest);
    if(operationStatus < 0){
        strcpy(interfaceRequest.ifr_name, "can0"); // Set Interface name
        operationStatus = ioctl(bcmSocket, SIOCGIFINDEX, &interfaceRequest);
        if(operationStatus < 1){
            std::string error = std::string("Error: Connecting Broadcast Manager CAN Socket :") + std::strerror(errno);
            throw std::runtime_error(error);
        }
    }

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

void startCanHeartbeat(int bcmSocket, canid_t address, int32_t timeSeconds){
    struct broadcastManagerConfig bcmMessage = {};
    bcmMessage.msg_head.opcode  = RX_SETUP;
    bcmMessage.msg_head.can_id  = address;
    bcmMessage.msg_head.flags   = SETTIMER | STARTTIMER;   // We're starting the timer right away. Probably wont want start the
    bcmMessage.msg_head.nframes = 0;
    bcmMessage.msg_head.count   = 0;
    /* Set the time interval value to 1s */
    bcmMessage.msg_head.ival1.tv_sec = timeSeconds;
    bcmMessage.msg_head.ival1.tv_usec = 0;
    if (write(bcmSocket, &bcmMessage, sizeof(bcmMessage)) < 0)
    {
        std::string error = std::string("Error: Starting CAN Heartbeat :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
}

void startInverterBroadcast(int bcmSocket){
    struct broadcastManagerConfig bcmMessage = {};
    bcmMessage.msg_head.opcode  = TX_SETUP;
    bcmMessage.msg_head.can_id  = INVERTER_BROADCAST_ID;
    bcmMessage.msg_head.flags   = SETTIMER | STARTTIMER;
    bcmMessage.msg_head.nframes = 1;
    bcmMessage.msg_head.count   = 0;

    bcmMessage.msg_head.ival2.tv_sec = 0;
    bcmMessage.msg_head.ival2.tv_usec = 10000;

    bcmMessage.frame[0].can_id = INVERTER_FRAME_ID;
    bcmMessage.frame[0].can_dlc = 8;
    for(int i = 0; i < CAN_MAX_DLEN ; i++){
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
    torque *=10;
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
        msg.frame[0].data[INVERTER_RUN_B] = 1;
        msg.frame[0].data[DIRECTION_B] = 1;
    }
    msg.frame[0].data[TORQUE_HIGH_B] = highByte;
    msg.frame[0].data[TORQUE_LOW_B] = lowByte;
    if (write(bcmSocket, &msg, sizeof(msg)) < 0)
    {
        std::string error = std::string("Error: Setting inverter torque :") + std::strerror(errno);
        throw std::runtime_error(error);
    }
}

void readRawSocket(int socket, TelemetryManager& pod){
    struct can_frame receivedCanFrame = {0};
    ssize_t receivedPacketSize;
    receivedPacketSize = read(socket, &receivedCanFrame, CAN_MTU);
    if (receivedPacketSize > 0 ) {
        processFrame(receivedCanFrame, pod);
        return;
    }
    else if(receivedPacketSize < 0){
        if(errno != EAGAIN){
            std::string error = std::string("Error: Reading CAN socket raw :") + std::strerror(errno);
            throw std::runtime_error(error);
        }
    }
}

void readBcmSocket(int socket, TelemetryManager& pod) {
    struct broadcastManagerConfig bcmUpdate = {};
    ssize_t receivedPacketSize;
    receivedPacketSize = read(socket, &bcmUpdate, sizeof(bcmUpdate));
    if (receivedPacketSize > 0 ) {
        if(bcmUpdate.msg_head.opcode == RX_TIMEOUT ){
            switch(bcmUpdate.msg_head.can_id){
                case INVERTER_HEARTBEAT_FRAME_ID:
                    pod.setInverterHeartbeat(0);
                    pod.sendUpdate("Inverter Heartbeat Expired");
                    break;
                case BMS_HEARTBEAT_FRAME_ID:
                    pod.setConnectionFlag(0,2);
                    pod.sendUpdate("BMS Heartbeat Expired");
                    break;
            }
        }
    }
    else if(receivedPacketSize < 0){
        if(errno != EAGAIN ) {
            std::string error = std::string("Error: Reading CAN socket BCM :") + std::strerror(errno);
            throw std::runtime_error(error);
        }
    }
}

int canNetworkThread(TelemetryManager Pod){
  TIMED_FUNC(timerObj);
    //Logging
    el::Helpers::setThreadName("CAN Thread");
    LOG(INFO) << "Starting CAN Thread on ";
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
       //  startInverterBroadcast(canSockBcm);
    }
    catch (const std::runtime_error &error){
        LOG(INFO) << error.what();
        return -1;
    }
    try{
        startCanHeartbeat(canSockBcm, INVERTER_HEARTBEAT_FRAME_ID, 2);
        startCanHeartbeat(canSockBcm, BMS_HEARTBEAT_FRAME_ID, 1);
    }
    catch (const std::runtime_error &error){
        LOG(INFO) << error.what();
        return -1;
    }

    std::string threadLabel = "NetworkThread";
    int32_t currentTorque, newTorque = 0;
    while ( Pod.getPodStateValue() != psShutdown) {
      TIMED_SCOPE(timeBlkObj, threadLabel);
      try {
          readRawSocket( canSockRaw, Pod);
          readBcmSocket( canSockBcm, Pod );
      }
      catch (const std::runtime_error &error){
          LOG(INFO) << error.what();
      }
      catch (const std::exception &error){
          LOG(INFO) << error.what();
          return -1;
      }

      newTorque = Pod.telemetry->commandedTorque;
      if(currentTorque != newTorque) {
          currentTorque = newTorque;
          setInverterTorque(currentTorque, canSockBcm);
      }
    }
    close(canSockRaw);
}
