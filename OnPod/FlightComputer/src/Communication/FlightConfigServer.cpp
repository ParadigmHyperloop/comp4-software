#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "EasyLogger/easylogging++.h"
#include <string>
#include <string.h>


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
    this->_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listener == -1)
        LOG(ERROR) << "Failed to create socket";
        // TODO: Throw Error for main thread or message to let it know it failed?

    // Bind socket to IP/PORT
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    // htons: Converts machine values between machine and network byte order.
    hint.sin_port = htons(this->_port);
    // Configure IP Address
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //127.0.0.1


  //  if (bind(this->_listener, hint.sin_addr, sizeof(int)) == -1)
    //if (bind(this->_listener,(struct  &hint.sin_addr), sizeof(int)) == -1)
   //     LOG(ERROR) << "Can't bind to IP/PORT";

}

