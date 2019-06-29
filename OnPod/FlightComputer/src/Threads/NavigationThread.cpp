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
#include "Heartbeat.h"


#define BUFFER_SIZE 23
#define FRONT_WHEEL_RADIUS 0.09728
#define FRONT_WHEEL_CIRCUMFERENCE 0.611228
#define STRIP_DISTANCE 1.570796
#define REAR_WHEEL_RADIUS 0.29718


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

void readNavigationNode(int serialPort, TelemetryManager &pod){
    std::stringstream dataStream;
    char read_buffer[BUFFER_SIZE] = {};
    int  bytes_read, tachRpm, irRpm, tachSpokeCount, irStripCount = 0;
    int total_bytes_read = 0;
    write (serialPort, "1", 1);
    while(total_bytes_read < BUFFER_SIZE){
        bytes_read=read(serialPort,read_buffer, sizeof(read_buffer));
        if(bytes_read > 0){
            total_bytes_read += bytes_read;
            dataStream << read_buffer;
            memset(&read_buffer,0, bytes_read);
        } else{
            std::string sError = std::string("Navnode timeout");
            throw std::runtime_error(sError);
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
        std::string sError = std::string("Parsing Nav Node Data: ") + std::strerror(errno);
        throw std::runtime_error(sError);
    }
    dataStream >> irStripCount;
    std::stringstream().swap(dataStream);
    pod.telemetry->irRpm = irRpm;
    pod.telemetry->tachRpm = tachRpm;
    pod.telemetry->tachDistance = (tachSpokeCount/8)*FRONT_WHEEL_CIRCUMFERENCE;
    pod.telemetry->irDistance = irStripCount*STRIP_DISTANCE;
    return;
}

int32_t NavigationThread(TelemetryManager Pod) {
    el::Helpers::setThreadName("Navigation Thread");
    const int serialPort = getSerialPort();
    if(serialPort == -1) {
        std::string error = std::string("Error opening serial port : ") + std::strerror(errno);
        Pod.sendUpdate(error);
        return -1;
    }

    Heartbeat navNodeUpdateFreq = Heartbeat(10);
    float tachVelocity, motorVelocity, podVelocity;

    while(Pod.getPodStateValue() != psShutdown)
    {
        if(navNodeUpdateFreq.expired()){
            navNodeUpdateFreq.feed();
            try {
                readNavigationNode(serialPort,Pod);
            }
            catch (std::runtime_error &error){
                Pod.sendUpdate(error.what());
                //todo what now
            }
        }
        tachVelocity =  FRONT_WHEEL_RADIUS*Pod.telemetry->tachRpm*0.10472;
        motorVelocity = REAR_WHEEL_RADIUS*Pod.telemetry->motorSpeed*0.10472;
        podVelocity = (motorVelocity > tachVelocity) ? motorVelocity : tachVelocity;
        Pod.telemetry->podVelocity = podVelocity; //todo setter
        Pod.telemetry->podPosition = ( Pod.telemetry->tachDistance > Pod.telemetry->irDistance ) ? Pod.telemetry->tachDistance : Pod.telemetry->irDistance;
    }
    close(serialPort);
}