#ifndef FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
#define FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H

#include "../Constants/Constants.h"

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
    int _listener;
    int _port;
    static FlightConfigServer* _configServer;


    FlightConfigServer operator=(const FlightConfigServer&);
    explicit FlightConfigServer(int listeningPort);

public:
    static FlightConfigServer* getServer(int port = NetworkConstants::CONFIG_SERVER_PORT);
    int operator()();


};


#endif //FLIGHTCOMPUTER_FLIGHTCONFIGSERVER_H
