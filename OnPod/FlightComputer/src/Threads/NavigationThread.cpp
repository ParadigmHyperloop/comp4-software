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




int8_t getSerialPort(){
    int8_t serialPort = open("/dev/ttyO2",O_RDWR | O_NOCTTY);
    if (serialPort == -1){
        serialPort = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
    }
    //tcflush( serialPort, TCIFLUSH );
    struct termios SerialPortSettings = {};
    tcgetattr(serialPort, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,B115200);
    cfsetospeed(&SerialPortSettings,B115200);
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
    SerialPortSettings.c_cc[VTIME] = 1;
    int status = tcsetattr(serialPort,TCSANOW,&SerialPortSettings);

    if(status < 0){
        LOG(INFO)<< "Failed to set Serial Settings " <<  + std::strerror(errno);
    }

    return serialPort;
}


void updatePosition(int velocity, int irStripCount, TelemetryManager &pod){
    pod.telemetry->totalStripCount += irStripCount;
    float stripDistance = pod.telemetry->totalStripCount * GENERAL_CONSTANTS::STRIP_DISTANCE;
    pod.setPodDistance( stripDistance );
    pod.setPodVelocity(velocity/100);
}


void readNavigationNode(int serialPort, TelemetryManager &pod){
    std::stringstream dataStream;
    dataStream.str(std::string());
    char read_buffer[GENERAL_CONSTANTS::NAV_SERIAL_MESSAGE_SIZE + 1] = {0};
    int  bytes_read, stripCount, velocity, tubePressure = 0;
    int total_bytes_read = 0;
    int status = 0;
    status = write(serialPort, "1", 1);
    if(status < 0){
        std::string sError = std::string("Error writing to Navnode Serial Port");
        throw std::runtime_error(sError);
    }
    while(total_bytes_read < GENERAL_CONSTANTS::NAV_SERIAL_MESSAGE_SIZE){
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
    std::string data;
    std::getline(dataStream, data, ',');
    try {
        stripCount = std::stoi(data);
        std::getline(dataStream, data, ',');
        velocity = std::stoi(data);
    }
    catch (std::exception &e){
        std::string sError = std::string("Error Parsing Nav Node Data");
        throw std::runtime_error(sError);
    }
    dataStream >> tubePressure;
    std::stringstream().swap(dataStream);

    if(stripCount > 0){  //strip count    velocity   pressure
        pod.countIrTape();
    }
    pod.telemetry->tubePressure = tubePressure;
    pod.telemetry->stripVelocity = velocity;

    LOG(INFO)<<"string : "<< stripCount << " velocity : "<< velocity << "  pressure : " << tubePressure;
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
    Heartbeat navNodeUpdateFreq = Heartbeat(10);
    Heartbeat navNodeHeartbeat = Heartbeat(45);
    bool success;
    while(Pod.getPodStateValue() != psShutdown)
    {
        if(navNodeUpdateFreq.expired()){
            success = true;
            try {
                readNavigationNode(serialPort,Pod);
            }
            catch (std::runtime_error &error){
                success = false;
            }
            if(success){
                navNodeUpdateFreq.feed();
                navNodeHeartbeat.feed();
                Pod.telemetry->navNodeState = navConnected;
            }
            else{
                if(navNodeHeartbeat.expired()){
                    Pod.setConnectionFlag(0, CONNECTION_FLAGS::NAVIGATION_HEARTBEAT_INDEX);
                    Pod.telemetry->navNodeState = navNone;
                }
            }
        }
    }
    close(serialPort);
}