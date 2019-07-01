/**
 * Modified  examples from https://github.com/JCube001/socketcan-demo
 *
 *
 */

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/bcm.h>
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
#include<stdio.h>
#include <chrono>


#define TORQUE_LOW_B 0
#define TORQUE_HIGH_B 1
#define SPEED_LOW_B 2
#define SPEED_HIGH_B 3
#define DIRECTION_B 4
#define FORWARD 1
#define REVERSE 0
#define INVERTER_RUN_B 5
#define ON 1
#define OFF 0
#define INVERTER_BROADCAST_ID 0
#define INVERTER_FRAME_ID 0x0C0
#define INVERTER_HEARTBEAT_FRAME_ID 0x0A7
#define BMS_HEARTBEAT_FRAME_ID 0x6b3
#include <fcntl.h>



#define START_DELAY_SECONDS 1
#define DRIVE_TIME_SECONDS 5
#define ACCEL_TIME_INCREMENTS_MS 100

#define START_TORQUE 100
#define ACCELTIME 4
#define FINAL_TORQUE 400


struct can_msg_bcm
{
    struct bcm_msg_head msg_head;
    struct can_frame frame[1];
};

struct broadcastManagerConfig
{
    struct bcm_msg_head msg_head;
    struct can_frame frame[1];
};


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
        if(operationStatus < 0){
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

void setInverterTorque(int torque, int bcmSocket){
    std::cout<<torque<<std::endl;
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

class Heartbeat{
public:
    Heartbeat();
    explicit Heartbeat(int32_t timeoutMilis);
    void feed();
    bool expired();

private:
    std::chrono::steady_clock::time_point _lastStartTime;
    int _timeOutMilis;
};

Heartbeat::Heartbeat() {
    this->_timeOutMilis = 0;
    this->_lastStartTime = std::chrono::steady_clock::now();
}

Heartbeat::Heartbeat(int32_t timeoutMilis)
{
    this->_timeOutMilis = timeoutMilis;
}

void Heartbeat::feed()
{
    this->_lastStartTime = std::chrono::steady_clock::now();
}

bool Heartbeat::expired()
{
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_lastStartTime).count() > this->_timeOutMilis;
}




int main()
{
    int broadcastSocket = getCanSocketBrodcastManager();
    startInverterBroadcast(broadcastSocket);

    sleep(START_DELAY_SECONDS);

    int torque = START_TORQUE;
    setInverterTorque(START_TORQUE, broadcastSocket);
    int numberOfIncrements = (ACCELTIME*1000)/ACCEL_TIME_INCREMENTS_MS;
    int torqueIncrement = (FINAL_TORQUE-START_TORQUE)/numberOfIncrements;

    Heartbeat timer = Heartbeat(100);
    timer.feed();

    int increment = 0;
    while(increment < numberOfIncrements){
        if(timer.expired()){
            timer.feed();

            torque += torqueIncrement;
            setInverterTorque(torque, broadcastSocket);

            increment++;
        }
    }
    setInverterTorque(FINAL_TORQUE, broadcastSocket);

    sleep(DRIVE_TIME_SECONDS);

    setInverterTorque(0, broadcastSocket);

    sleep(5);



    return 0;
}