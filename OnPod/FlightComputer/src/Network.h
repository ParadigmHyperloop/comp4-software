#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include "ProtoBuffer/Paradigm.pb.h"
#include "structs.h"
#include "Pod.h"

int32_t podInternalNetworkThread(Pod);

int32_t createCommanderServerSocket(int32_t);

int32_t commanderThread(Pod);

sockaddr_in createGenericNodeAddr();

int32_t createUdpClientSocket();

#endif
