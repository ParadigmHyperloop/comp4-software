#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/structs.h"
#include "FlightComputer/ProtoStructs.pb.h"


#define NodeUDPSocketPort 5007

using namespace std;

/**initializeClientSocket
 *
 * Create the socket that outbound UDP packets will be transmitted over. Create
 * a sockarrd_inn struct and populate it with common fields. Group into a
 * socketConfig struct
 *
 * Param:None
 * Returns : SocketConfig struct
 */
clientSocketConfig initializeClientSocket() {
  printf("Creating Client Socket \n");
  // Create destination info struct
  sockaddr_in node_addr;
  bzero(&node_addr, sizeof node_addr);
  node_addr.sin_family = AF_INET;
  node_addr.sin_port = htons(NodeUDPSocketPort);  // Node Port in Network Byte Order
  int sckt = socket(AF_INET, SOCK_DGRAM, 0);  // Create the Socket
  if (sckt == -1)
  {
    // TODO throw error
  }
  else
  {
	  printf("Client Socket Created \n");
  }
  clientSocketConfig info;
  info.addr = &node_addr;
  info.sckt = sckt;

  return info;
}

/**killConfigSocket
 *
 *Close the socket of the socketConfig struct provided
 *
 */
void killConfigSocket(clientSocketConfig socketInfo) { close(socketInfo.sckt); return ;}


/**SendState
 *
 * Sends a UDP packet to all IP addresses given using the Socket in the config
 * given
 */
void sendDataUdp( clientSocketConfig socketInfo, string* data){
  printf("Sending UDP Packet... \n");
  struct hostent* host;
  const char *payLoad = data->c_str();
  for (int i = 0; i < socketInfo.nodeNum; i++)
  {
  host = gethostbyname(socketInfo.nodeIp[i]);
  socketInfo.addr->sin_addr = *((struct in_addr *)host->h_addr);

    sendto(socketInfo.sckt, payLoad, strlen(payLoad), 0,
           (struct sockaddr *)socketInfo.addr, sizeof(struct sockaddr));
  }
  printf("UDP Packets Sent. \n");
  return;
}


