#include "NavigationThread.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <iostream>
#include <vector>
#include <sstream>
#define BUFFER_SIZE 23

int8_t getSerialPort(){
    int8_t serialPort = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
    struct termios SerialPortSettings = {};
    tcgetattr(serialPort, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B9600);
    cfsetospeed(&SerialPortSettings,B9600);
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
    SerialPortSettings.c_cc[VTIME] = 30;
    tcsetattr(serialPort,TCSANOW,&SerialPortSettings);
    tcflush(serialPort, TCIFLUSH);
    return serialPort;
}

int32_t NavigationThread(TelemetryManager Pod) {
    el::Helpers::setThreadName("Navigation Thread");
    int serialPort = getSerialPort();
    if(serialPort == -1) {
        std::string error = std::string("Error opening serial port : ") + std::strerror(errno);
        Pod.sendUpdate(error);
        return -1;
    }
    char read_buffer[BUFFER_SIZE] = {};
    int  bytes_read, tachRpm, irRpm, tachSpokeCount, irStripCount = 0;
    int total_bytes_read = 0;
    std::stringstream dataStream;
    while(Pod.getPodStateValue() != psShutdown)
    {
        write (serialPort, "1", 1);
        while(total_bytes_read < BUFFER_SIZE){
            bytes_read=read(serialPort,read_buffer, sizeof(read_buffer));
            if(bytes_read > 0){
                total_bytes_read += bytes_read;
                dataStream << read_buffer;
                memset(&read_buffer,0, bytes_read);
            } else{
                LOG(INFO)<<"Expired"; //todo update coms
            }
        }
        total_bytes_read = 0;

        std::string data;
        std::getline(dataStream, data, ',');
        try {
            tachRpm = std::stoi(data);

            std::getline(dataStream, data, ',');
            irRpm = std::stoi(data);

            std::getline(dataStream, data, ',');
            tachSpokeCount = std::stoi(data);
        }
        catch (std::invalid_argument &e){
            LOG(INFO)<<"Invalid Arg";
        }
        dataStream >> irStripCount;
        std::stringstream().swap(dataStream);

        Pod.telemetry->irRpm = irRpm;
        Pod.telemetry->tachRpm = tachRpm;
        Pod.telemetry->tachDistance = (tachSpokeCount/8)*0.611228;
        Pod.telemetry->irDistance = irStripCount*1.570796;

        LOG(INFO)<<"Tach RPM< : "<< tachRpm<< " IR RPM : " << irRpm  << " Spoke Count : " << tachSpokeCount  <<  " Strip Count : " << irStripCount;
        sleep(1);
    }
    close(serialPort);
}