#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <fcntl.h>


#include "FlightComputer/Structs.h"
#include "FlightComputer/Pod.h"

int32_t podInternalNetworkThread(Pod);

int32_t createCommanderServerSocket(int32_t);

int32_t commanderThread(Pod);

sockaddr_in createGenericNodeAddr();

int32_t createUdpClientSocket();

#endif
