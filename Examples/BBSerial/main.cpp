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
    int  bytes_read, val_1, val_2 = 0;
    int total_bytes_read = 0;
    std::stringstream dataStream;
    std::string data;
    while(1)
    {
        write (fd, "1", 1);
        while(total_bytes_read < 9){
            bytes_read=read(fd,read_buffer, sizeof(read_buffer));
            if(bytes_read > 0){
                total_bytes_read += bytes_read;
                dataStream << read_buffer;
                memset(&read_buffer,0, bytes_read);
            } else{
                std::cout<<"Expired"<< std::endl;
            }
        }
        std::getline(dataStream, data, ',');
        val_1 = std::stoi(data);
        dataStream >> val_2;

        dataStream.str(std::string());
        total_bytes_read = 0;

        std::cout<<"Val1 : "<< val_1<< " Val2 : " << val_2<< std::endl;
    }
    close(fd); 
}
