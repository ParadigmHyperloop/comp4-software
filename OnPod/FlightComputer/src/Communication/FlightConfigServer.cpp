#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "EasyLogger/easylogging++.h"
#include <string>
#include <string.h>

#include "ProtoBuffer/Paradigm.pb.h"
#include "FlightConfigServer.h"

FlightConfigServer* FlightConfigServer::_configServer = NULL;

FlightConfigServer::FlightConfigServer(int listeningPort)
: _port(listeningPort)
{
    LOG(INFO)<<"Creating FlightConfigServer Instance";

}

FlightConfigServer* FlightConfigServer::getServer(int port)
{
    if (_configServer == NULL)
        _configServer = new FlightConfigServer(port);
    return _configServer;
}

int FlightConfigServer::operator()()
{
    LOG(INFO) << "Creating socket";
    this->_listenerSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listenerSocketID == -1)
        LOG(ERROR) << "Failed to create socket";
        // TODO: Throw Error for main thread or message to let it know it failed?

    // Bind socket to IP/PORT
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    // htons: Converts machine values between machine and network byte order.
    hint.sin_port = htons(this->_port);
    // Configure IP Address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //127.0.0.1

    if ( bind(this->_listenerSocketID, (struct sockaddr *) &hint, sizeof(hint)) == -1)
    {
        LOG(ERROR) << "Cant Bind to IP/PORT";
        //return;
    }

    // make socket listen
    if (listen(this->_listenerSocketID, SOMAXCONN) == -1)
    {
        LOG(ERROR) << "Can't Listen";
        //return;
    }

    // Accept connections:
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    // Buffers to put host and service in
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(this->_listenerSocketID, (struct sockaddr *) &client, &clientSize);

    if (clientSocket == -1 )
    {
        LOG(ERROR) << "Problem Occurred with Client Socket";
        return -4;
    }
    // If Read successful, then
    close(this->_listenerSocketID);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    // While receiving display message
    char buf [4096];
    while (true)
    {
        std::memset(buf, 0, 4096);
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
            break;
        else if (bytesReceived == 0)
        {
            LOG(INFO)<<"The client disconnected";
            break;
        }
        flightConfig config;
        config.ParseFromString(std::string(buf));

    }

}

