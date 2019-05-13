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

flightConfig FlightConfigServer::operator()(char *controlLaptopAddr) {
    LOG(INFO) << "Creating Pod Initilizer socket";
    this->_listenerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listenerSocketID == -1) {
        //todo throw runtime error with string
        LOG(ERROR) << "Failed to pod initlizer create socket" << std::strerror(errno);
        throw;
    }

    // Bind socket to IP/PORT
    sockaddr_in serverSockAddr;
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(this->_port);
    inet_pton(AF_INET, "0.0.0.0", &serverSockAddr.sin_addr);

    if (bind(this->_listenerSocketID, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) == -1) {
        LOG(ERROR) << "Cant Bind to IP/PORT" << std::strerror(errno);
        throw;
    }

    // make socket listen
    if (listen(this->_listenerSocketID, SOMAXCONN) == -1) {
        LOG(ERROR) << "Listen socket cannot listen " << errno;
        throw;
    }

    // Accept connections:
    sockaddr_in clientSockAddr;
    socklen_t clientSize = sizeof(clientSockAddr);

    // Buffers to put host and clientPort in
    char clientPort[NI_MAXSERV] = {0};

    int clientSocket = accept(this->_listenerSocketID, (struct sockaddr *) &clientSockAddr, &clientSize);
    if (clientSocket == -1) {
        LOG(ERROR) << "Problem Occurred with Client Socket" << errno;
        throw;
    }
    close(this->_listenerSocketID);


    int32_t iResult = getnameinfo((sockaddr *) &clientSockAddr, clientSize, controlLaptopAddr, NI_MAXHOST, clientPort,
                                  NI_MAXSERV, 0);

    if (iResult) {
        LOG(INFO) << "Host connected using port" << clientPort;
    } else {
        inet_ntop(AF_INET, &clientSockAddr.sin_addr, controlLaptopAddr, NI_MAXHOST);
        LOG(INFO) << controlLaptopAddr << "Connected on port" << ntohs(clientSockAddr.sin_port);
    }


    char buf[4096] = {0};
    flightConfig config;
    while (true) {
        int32_t iBytesReceived = recv(clientSocket, buf, 4096, MSG_DONTWAIT);
        if (iBytesReceived > 0 ) {
            std::string receivedConfig = buf;
            if(config.ParseFromString(receivedConfig)){
                LOG(INFO) << "Parsed config";
            }
            else{
                //todo failed to parse proto throw error and close socket
            }
            break;
        } else if (iBytesReceived == 0){
            LOG(INFO) << "The client disconnected";
            break;
        }
    }
    const char *SUCCESS_RECEIVE_CONFIG_RESPONSE = "OK";
    send(clientSocket,
         SUCCESS_RECEIVE_CONFIG_RESPONSE,
         strlen(SUCCESS_RECEIVE_CONFIG_RESPONSE),
         0);
    return config;
}

