/**
 * Modified  examples from https://github.com/JCube001/socketcan-demo
 *
 *
 */

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




int cyclicMessages(void)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    struct can_msg
    {
        struct bcm_msg_head msg_head;
        struct can_frame frame[1];
    } msg;

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
    msg.msg_head.ival2.tv_sec = 1;
    msg.msg_head.ival2.tv_usec = 200000; // If you done set this makes sure its 0

    /* Create the example messages */
    for (int i = 0; i < 1; ++i)
    {
        struct can_frame * const frame = msg.frame + i;
        frame->can_id = 0x0C0 + i;
        frame->can_dlc = 3;
        memset(frame->data, i, 3); //ew
    }

    /* Register the cyclic messages. Note that all of the messages in the msg
     * struct will be sent with the same periodicity because they share the
     * same bcm_msg_head setup data.
     */
    if (write(s, &msg, sizeof(msg)) < 0)
    {
        //error
    }

    // The cyclic messages only live the length of the program.
    while(1){}

    return 0;
}