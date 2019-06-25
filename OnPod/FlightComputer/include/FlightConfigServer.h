#ifndef FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
#define FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H

#include "../src/Constants/Constants.h"

/*
 * FlightConfigServer:
 *      - TCP server that will receive Config File
 */
class FlightConfigServer {

private:
    int _listenerSocketID;
    int _port;
    static FlightConfigServer* _configServer;


    FlightConfigServer operator=(const FlightConfigServer&);
    explicit FlightConfigServer(int listeningPort);

public:
    static FlightConfigServer* getServer(int port = NetworkConstants::iCONFIG_SERVER_PORT);
    void closePorts();
    FlightConfig runServer();


};


#endif //FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
