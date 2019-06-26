/*
 * serial_communication
 * ------------------
 * $Revision: 1.1 $
 * $Date: 2015/01/04 13:14:18 $
 * Author: K Raghav
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <iostream>
#include <vector>
#include <sstream>

void reading(int fd)
{
    char red[20];
    memset(red, 0, sizeof(red));
    int ret,n = 0;

    int num=0;
    while(1)
    {
        ret=read(fd,red, sizeof(red));
        if(ret > 0){
            std::string num(red);
            std::cout<<num << std::endl;
        }
    }
}

int open_port(void)
{
    int serial_port,ret;
    char red[88];
    serial_port = open("/dev/ttyUSB1", O_RDONLY | O_NOCTTY);
    printf("\nfunction name is  %s >>\n",__func__);
    printf("returned fd is :%d\n",serial_port );
    if (serial_port == -1)
    {
        perror("open_port: Unable to open /dev/ttyUSB0 - ");
    }
    // Create new termios struc, we call it 'tty' for convention
    struct termios tty = {};
    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; /*CLEAR Parity Bit PARENB*/
    tty.c_cflag |=  PARENB; /*SET   Parity Bit PARENB*/
    tty.c_cflag &= ~PARENB;   // No Parity
    tty.c_cflag &= ~CSTOPB; //Stop bits = 1
    tty.c_cflag &= ~CSIZE; /* Clears the Mask       */
    tty.c_cflag |=  CS8;   /* Set the data bits = 8 */
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);


    tty.c_cc[VMIN] = 100;
    tty.c_cc[VTIME] = 0;
// Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

// Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    return (serial_port);
}

/*
int main()
{
    int k = open_port();
    if(k < 0)
        return 0;
    reading(k);
    return 0;
}
*/

int main(void)
{
    int fd;/*File Descriptor*/

    printf("\n +----------------------------------+");
    printf("\n |        Serial Port Read          |");
    printf("\n +----------------------------------+");

    /*------------------------------- Opening the Serial Port -------------------------------*/

    /* Change /dev/ttyUSB0 to the one corresponding to your system */

    fd = open("/dev/ttyUSB1",O_RDWR | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
    /* O_RDWR   - Read/Write access to serial port       */
    /* O_NOCTTY - No terminal will control the process   */
    /* Open in blocking mode,read will wait              */



    if(fd == -1)						/* Error Checking */
        printf("\n  Error! in Opening ttyUSB0  ");
    else
        printf("\n  ttyUSB0 Opened Successfully ");


    /*---------- Setting the Attributes of the serial port using termios structure --------- */

    struct termios SerialPortSettings;	/* Create the structure                          */

    tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

    /* Setting the Baud rate */
    cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

    /* 8N1 Mode */
    SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

    SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 13; /* Read at least 10 characters */
    SerialPortSettings.c_cc[VTIME] = 1; /* Wait indefinetly   */

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
    else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

    /*------------------------------- Read data from serial port -----------------------------*/

    tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

    char read_buffer[30];   /* Buffer to store the data received              */
    memset(&read_buffer,0, sizeof(read_buffer));
    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int i = 0;

    while(1)
    {
        bytes_read=read(fd,read_buffer, sizeof(read_buffer));
        if(bytes_read > 0){
            std::string num(read_buffer);
            std::cout<<num << std::endl;
            //memset(&read_buffer,0, sizeof(read_buffer));
        }
    }
    close(fd); /* Close the serial port */
}
