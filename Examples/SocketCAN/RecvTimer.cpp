
/**
 * Modified  examples from https://github.com/JCube001/socketcan-demo
 *
 *
 */


#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/bcm.h>
#include <iostream>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"



void print_can_frame(const struct can_frame * const frame)
{
    const unsigned char *data = frame->data;
    const unsigned int dlc = frame->can_dlc;
    unsigned int i;

    printf("%03X  [%u] ", frame->can_id, dlc);
    for (i = 0; i < dlc; ++i)
    {
        printf(" %02X", data[i]);
    }
}

int
main(void)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    struct can_msg //This can be called anything
    {
        struct bcm_msg_head msg_head;
        struct can_frame frame[1];
    } msg;

    s = socket(PF_CAN, SOCK_DGRAM, CAN_BCM); // DGRAM defines that we're using the brodcast manager
    /* Set socket to non-blocking */
    unsigned int flags = fcntl(s, F_GETFL, 0);
    if (flags < 0)
    {
        return errno;
    }
    if (fcntl(s, F_SETFL, flags | O_NONBLOCK) < 0) // AND the flags, dont overwrite them (!)
    {
        return errno;
    }


    strcpy(ifr.ifr_name, "can0"); // Set Interface
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    connect(s, (struct sockaddr *)&addr, sizeof(addr));

    msg.msg_head.opcode  = RX_SETUP;
    msg.msg_head.can_id  = 0; // Bogus ID
    msg.msg_head.flags   = SETTIMER | STARTTIMER;   // We're starting the timer right away. Probably wont want start the
                                                    // time until we recv one in the real application
    msg.msg_head.nframes = 0;
    msg.msg_head.count   = 0;

    /* Set the time interval value to 1s */
    msg.msg_head.ival1.tv_sec = 1;
    msg.msg_head.ival1.tv_usec = 0;



    if (write(s, &msg, sizeof(msg)) < 0)
    {
        return errno;
    }

    while(1) {
        /* Read from the CAN interface */
        ssize_t nbytes = read(s, &msg, sizeof(msg));
        if (nbytes < 0) {
            if (errno != EAGAIN) {
                perror("read");
            }
            usleep(10);
        }  else {
            struct can_frame *const frame = msg.frame;
            unsigned char *const data = frame->data;
            const unsigned int dlc = frame->can_dlc;
            unsigned int i;
            if(msg.msg_head.opcode == RX_TIMEOUT ){
                std::cout << "Oh fuck ya bud";
            }
            /* Print the received CAN frame */
            printf("RX:  ");
            print_can_frame(frame);
            printf("\n");
        }
    }


    return 0;
}
#pragma clang diagnostic pop