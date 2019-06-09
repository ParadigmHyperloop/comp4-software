#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "easylogging++.h"
#include <string>
#include <string.h>

#include "Paradigm.pb.h"
#include "FlightConfigServer.h"

FlightConfigServer *FlightConfigServer::_configServer = NULL;

FlightConfigServer::FlightConfigServer(int listeningPort)
        : _port(listeningPort) {
    LOG(INFO) << "Creating FlightConfigServer Instance";
}

FlightConfigServer *FlightConfigServer::getServer(int32_t port) {
    if (_configServer == NULL)
        _configServer = new FlightConfigServer(port);
    return _configServer;
}

void FlightConfigServer::closePorts(){
    close(this->_listenerSocketID);
}

FlightConfig FlightConfigServer::runServer() {
    LOG(INFO) << "Creating TelemetryManager Initilizer socket";
    char controlLaptopAddr[NI_MAXHOST] = {0};
    this->_listenerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(this->_listenerSocketID, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (this->_listenerSocketID == -1) {
        //todo throw runtime error with string
        std::string error = std::string("Failed to pod initializer create socket") + std::strerror(errno);
        throw std::runtime_error(error);
    }

    // Bind socket to IP/PORT
    sockaddr_in serverSockAddr{};
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(this->_port);
    inet_pton(AF_INET, "0.0.0.0", &serverSockAddr.sin_addr);

    if (bind(this->_listenerSocketID, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) == -1) {
        std::string strError = std::string("Error: Binding Config Server Port:  ") + std::strerror(errno);
        throw std::runtime_error(strError);
    }

    // make socket listen
    if (listen(this->_listenerSocketID, SOMAXCONN) == -1) {
        std::string strError = std::string("Error: Config server on listen():  ") + std::strerror(errno);
        throw std::runtime_error(strError);
    }

    sockaddr_in clientSockAddr;
    socklen_t clientSize = sizeof(clientSockAddr);

    // Buffers to put host and clientPort in
    char clientPort[NI_MAXSERV] = {0};
    int clientSocket = accept(this->_listenerSocketID, (struct sockaddr *) &clientSockAddr, &clientSize);
    if (clientSocket == -1) {
        std::string strError = std::string("Problem Occurred with Client Socket") + std::strerror(errno);
        throw std::runtime_error(strError);
    }

    close(this->_listenerSocketID);
    int32_t iResult = getnameinfo((sockaddr *) &clientSockAddr, clientSize, controlLaptopAddr, NI_MAXHOST, clientPort,
                                  NI_MAXSERV, 0);

    if (iResult) {
        LOG(INFO) << "Host connected using port" << clientPort;
    } else {
        inet_ntop(AF_INET, &clientSockAddr.sin_addr, controlLaptopAddr, NI_MAXHOST);
        LOG(INFO) << controlLaptopAddr << " Connected on port " << ntohs(clientSockAddr.sin_port);
    }


    char buf[4096] = {0};
    FlightConfig config;
    while (true) {
        int32_t iBytesReceived = recv(clientSocket, buf, 4096, MSG_DONTWAIT);
        if (iBytesReceived > 0 ) {
            std::string receivedConfig = buf;
            if(config.ParseFromString(receivedConfig)){
                LOG(INFO) << "Flight Configuration Successfully Received";
            }
            else{
                std::string strError = std::string("Failed to Parse Configuration Protobuf");
                throw std::runtime_error(strError);
            }
            break;
        } else if (iBytesReceived == 0){
            std::string strError = std::string("Flight Configuration client disconnected before sending");
            throw std::runtime_error(strError);
        }
    }
    const char *SUCCESS_RECEIVE_CONFIG_RESPONSE = "OK";
    send(clientSocket,
         SUCCESS_RECEIVE_CONFIG_RESPONSE,
         strlen(SUCCESS_RECEIVE_CONFIG_RESPONSE),
         0);
    std::string laptopAddress(controlLaptopAddr);
    config.set_controllaptopipaddr(laptopAddress);
    close(clientSocket);
    return config;
}

