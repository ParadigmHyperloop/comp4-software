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
#include "Constants/Constants.h"
#include "Constants/SensorConfig.h"


#define BUFFER_SIZE 35
#define FRONT_WHEEL_RADIUS 0.09728
#define FRONT_WHEEL_CIRCUMFERENCE 0.611228
#define STRIP_DISTANCE 1.570796
#define REAR_WHEEL_RADIUS 0.29718


int8_t getSerialPort(){
    int8_t serialPort = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
    if (serialPort == -1){
        serialPort = open("/dev/ttyO4",O_RDWR | O_NOCTTY);
    }
    //tcflush( serialPort, TCIFLUSH );
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
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHONL);
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    SerialPortSettings.c_oflag &= ~ONLCR;
    SerialPortSettings.c_cc[VMIN] = 0;
    SerialPortSettings.c_cc[VTIME] = 30;
    int status = tcsetattr(serialPort,TCSANOW,&SerialPortSettings);

    if(status < 0){
        LOG(INFO)<< "Failed to set Serial Settings " <<  + std::strerror(errno);
    }

    return serialPort;
}

void readNavigationNode(int serialPort, TelemetryManager &pod){
    std::stringstream dataStream;
    char read_buffer[BUFFER_SIZE] = {};
    int  bytes_read, tachRpm, irRpm, tachSpokeCount, irStripCount = 0;
    int total_bytes_read, status = 0;
    status = write(serialPort, "1", 1);
    if(status < 0){
        std::string sError = std::string("Error writing to Navnode Serial Port");
        throw std::runtime_error(sError);
    }
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
    catch (std::exception &e){
        std::string sError = std::string("Error Parsing Nav Node Data");
        throw std::runtime_error(sError);
    }
    dataStream >> irStripCount;
    std::stringstream().swap(dataStream);

    //LOG(INFO)<<"TachRPM : "<<tachRpm << " IrRPM : "<< irRpm << " Spoke : "<< tachSpokeCount << " Strip : "<<irStripCount;

    LOG(INFO)<<"TachRPM : "<<tachRpm << "  Spoke Count : "<< tachSpokeCount << " IR Strip : " << irStripCount;

    pod.telemetry->irRpm = irRpm;
    pod.telemetry->tachRpm = tachRpm;
    pod.telemetry->tachDistance = (tachSpokeCount/8.0)*FRONT_WHEEL_CIRCUMFERENCE;
    pod.telemetry->irDistance = irStripCount*FRONT_WHEEL_CIRCUMFERENCE;
    pod.setConnectionFlag(1, NAVIGATION_HEARTBEAT_INDEX);
}

int32_t NavigationThread(TelemetryManager Pod) {
    el::Helpers::setThreadName("Navigation Thread");
    const int serialPort = getSerialPort();
    if(serialPort == -1) {
        std::string error = std::string("Error opening serial port : ") + std::strerror(errno);
        Pod.sendUpdate(error);
        return -1;
    }
    LOG(INFO)<<"Starting Nav thread with FD " << serialPort;
    Heartbeat navNodeUpdateFreq = Heartbeat(1000);
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
                Pod.setConnectionFlag(0, NAVIGATION_HEARTBEAT_INDEX);
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