#include "FlightComputer/PodInternalNetwork.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NodeUDPSocketPort 5005

/**initializeClientSocket
 *
 * Create the socket that outbound UDP packets will be transmitted over. Create
 * a sockarrd_inn struct and populate it with common fields. Group into a
 * socketConfig struct
 *
 * Param:None
 * Returns : SocketConfig struct
 */
socketConfig initializeClientSocket() {

  // Create destination info struct
  sockaddr_in node_addr;
  node_addr.sin_family = AF_INET;
  node_addr.sin_port =
      htons(NodeUDPSocketPort);  // Node Port in Network Byte Order

  int sckt = socket(AF_INET, SOCK_DGRAM, 0);  // Create the Socket

  if (sckt == -1) {
    // TODO throw error
  }

  socketConfig info;
  info.addr = &node_addr;
  info.sckt = sckt;

  return info;
}

/**killConfigSocket
 *
 *Close the socket of the socketConfig struct provided
 *
 */
void killConfigSocket(socketConfig socketInfo) { close(socketInfo.sckt); }

/**SendState
 *
 * Sends a UDP packet to all IP addresses given using the Socket in the config
 * given
 */
void sendState( socketConfig socketInfo ) {
  struct hostent *host;

  // Loop over all the IP addresses and send a packet to each one
  for (int i = 0; i < socketInfo.nodeNum; i++) {

	  host = gethostbyname(socketInfo.nodeIp[i]);  // Puts address in a hostent struct

    socketInfo.addr->sin_addr = *((struct in_addr *)host->h_addr);

    sendto(socketInfo.sckt, "sup", strlen("sup"), 0,
           (struct sockaddr *)socketInfo.addr, sizeof(struct sockaddr));
  }
  return;
}
