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
    int fd;
    fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
    if(fd == -1)						/* Error Checking */
        printf("\n Error! in Opening");
    else
        printf("\n Opened Successfully ");

    struct termios SerialPortSettings;
    tcgetattr(fd, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B19200);
    cfsetospeed(&SerialPortSettings,B19200);
    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |=  CS8;
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_cc[VMIN] = 0;
    SerialPortSettings.c_cc[VTIME] = 100;

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) { /* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
    }
    tcflush(fd, TCIFLUSH);
    char read_buffer[10] = {};
    int  bytes_read = 0;
    int total_bytes_read = 0;
    std::stringstream data;
    while(1)
    {
        write (fd, "1", 1);
        while(total_bytes_read < 9){
            bytes_read=read(fd,read_buffer, sizeof(read_buffer));
            if(bytes_read > 0){
                total_bytes_read += bytes_read;
                data << read_buffer;
                memset(&read_buffer,0, bytes_read);
            } else{
                std::cout<<"Expired"<< std::endl;
            }
        }
        std::cout<<data.str()<< std::endl;
        data.str(std::string());
        total_bytes_read = 0;
    }
    close(fd); /* Close the serial port */
}
