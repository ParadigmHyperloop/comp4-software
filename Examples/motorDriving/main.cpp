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


struct can_msg_bcm
{
    struct bcm_msg_head msg_head;
    struct can_frame frame[1];
};


int main()
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_msg_bcm msg = {};

    s = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    connect(s, (struct sockaddr *)&addr, sizeof(addr));

    /* Set up cyclic messages. Note that both SETTIMER and STARTTIMER are
    * required as flags in order to continue transmitting.
    */
    msg.msg_head.opcode  = TX_SETUP;
    msg.msg_head.can_id  = 0;
    msg.msg_head.flags   = SETTIMER | STARTTIMER;
    msg.msg_head.nframes = 1;
    msg.msg_head.count   = 0;

    /* Set the time interval value to 1200 ms */
    msg.msg_head.ival2.tv_sec = 0;
    msg.msg_head.ival2.tv_usec = 100000; // If you dont set this makes sure its 0

    msg.frame[0].can_id = 0x0C0;
    msg.frame[0].can_dlc = 8;
    for(int i = 0; i < 8 ; i++){
        msg.frame[0].data[i] = 0;
    }
    std::cout<<"Start straight zeros"<<std::endl;
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        std::cout<<"Error"<<std::endl;
    }
    sleep(10);

    msg.frame[0].data[INVERTER_RUN_B] = ON;
    msg.frame[0].data[DIRECTION_B] = FORWARD;

    std::cout<<"Inverter ON"<<std::endl;
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        std::cout<<"Error on retry"<<std::endl;
    }

    sleep(10);

    msg.frame[0].data[INVERTER_RUN_B] = ON;

    msg.frame[0].data[SPEED_HIGH_B] = 4;
    msg.frame[0].data[SPEED_LOW_B] = 0;

    msg.frame[0].data[TORQUE_HIGH_B] = 9;
    msg.frame[0].data[TORQUE_LOW_B] = 96;

    msg.frame[0].data[DIRECTION_B] = FORWARD;

    std::cout<<"DRIVE BITCH DRIVE"<<std::endl;
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        std::cout<<"Error on retry"<<std::endl;
    }

    sleep(5);

    msg.frame[0].data[INVERTER_RUN_B] = ON;

    msg.frame[0].data[SPEED_HIGH_B] = 0;
    msg.frame[0].data[SPEED_LOW_B] = 0;

    msg.frame[0].data[TORQUE_HIGH_B] = 0;
    msg.frame[0].data[TORQUE_LOW_B] = 0;

    msg.frame[0].data[DIRECTION_B] = FORWARD;

    std::cout<<"Torque off inverter on"<<std::endl;
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        std::cout<<"Error on retry"<<std::endl;
    }
    sleep(30);

    msg.frame[0].data[INVERTER_RUN_B] = OFF;
    msg.frame[0].data[TORQUE_LOW_B] = 0;
    std::cout<<"Inverter Off"<<std::endl;
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        std::cout<<"Error on retry"<<std::endl;
    }
    sleep(30);
    return 0;
}