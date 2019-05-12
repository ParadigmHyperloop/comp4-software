#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "EasyLogger/easylogging++.h"
#include <string>
#include <string.h>

#include "Paradigm.pb.h"
#include "FlightConfigServer.h"

FlightConfigServer* FlightConfigServer::_configServer = NULL;

FlightConfigServer::FlightConfigServer(int listeningPort)
: _port(listeningPort)
{
    LOG(INFO)<<"Creating FlightConfigServer Instance";

}

FlightConfigServer* FlightConfigServer::getServer(int32_t port)
{
    if (_configServer == NULL)
        _configServer = new FlightConfigServer(port);
    return _configServer;
}

flightConfig FlightConfigServer::operator()(char* controlLaptopAddr)
{
    LOG(INFO) << "Creating socket";
    this->_listenerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listenerSocketID == -1)
    {
        LOG(ERROR) << "Failed to create socket" << errno;
        throw;
    }

        // TODO: Throw Error for main thread or message to let it know it failed?

    // Bind socket to IP/PORT
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    // htons: Converts machine values between machine and network byte order.
    hint.sin_port = htons(this->_port);
    // Configure IP Address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //127.0.0.1

    if (bind(this->_listenerSocketID, (struct sockaddr *) &hint, sizeof(hint)) == -1)
    {
        LOG(ERROR) << "Cant Bind to IP/PORT" << errno;
        throw;
    }

    // make socket listen
    if (listen(this->_listenerSocketID, SOMAXCONN) == -1)
    {
        LOG(ERROR) << "Listen socket cannot listen " << errno;
        throw;
    }

    // Accept connections:
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    // Buffers to put host and clientPort in
    char clientPort[NI_MAXSERV];

    int clientSocket = accept(this->_listenerSocketID, (struct sockaddr *) &client, &clientSize);
    if (clientSocket == -1 )
    {
        LOG(ERROR) << "Problem Occurred with Client Socket" << errno;
        throw;
    }

    // If Read successful, then
    close(this->_listenerSocketID);
    memset(controlLaptopAddr, 0, NI_MAXHOST);
    memset(clientPort, 0, NI_MAXSERV);

    int32_t iResult = getnameinfo((sockaddr*) &client, clientSize, controlLaptopAddr, NI_MAXHOST, clientPort, NI_MAXSERV, 0);

    if (iResult)
    {
        LOG(INFO) << "Host connected using port" << clientPort;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, controlLaptopAddr, NI_MAXHOST);
        LOG(INFO) << controlLaptopAddr << "connected on port" << ntohs(client.sin_port);
    }


    // While receiving config
    char buf [4096];
    flightConfig config;

    while (true)
    {
        std::memset(buf, 0, 4096);
        int32_t iBytesReceived = recv(clientSocket, buf, 4096, MSG_DONTWAIT);
        if (iBytesReceived == -1) {
            LOG(INFO) << "Done Receiving Config";
            break;
        }
        else if (iBytesReceived == 0)
        {
            LOG(INFO)<<"The client disconnected";
            break;
        }
        std::string receivedConfig =  buf;
        config.ParseFromString(receivedConfig);
        // TODO: Send back response
    }
    char *SUCCESS_RECEIVE_CONFIG_RESPONSE = "OK";
    send(clientSocket,
            SUCCESS_RECEIVE_CONFIG_RESPONSE,
            strlen(SUCCESS_RECEIVE_CONFIG_RESPONSE),
            0);
    return config;
}

