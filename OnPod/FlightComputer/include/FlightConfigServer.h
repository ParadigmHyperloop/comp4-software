#ifndef FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
#define FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H

#include "../src/Constants/Constants.h"

/*
 * FlightConfigServer:
 *      - TCP server that will receive Config File
 *      and (TODO: update "InitConfigObject" or send config settings)
 *      TODO:
 *          - Create Socket
 *          - Bind Socket to IP/PORT
 *          - Mark the socket for Listening in
 *          - Accept a call
 *          - Close the listening Socket
 *          - While Receiving - Display message, echo message
 *          - Close Socket
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
    flightConfig operator()(char* controlLaptopAddr);


};


#endif //FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
