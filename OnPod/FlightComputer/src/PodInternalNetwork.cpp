#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/ProtoStructs.pb.h"
#include "FlightComputer/structs.h"

#define NodeUDPSocketPort 5008

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
  node_addr.sin_port =
      htons(NodeUDPSocketPort);               // Node Port in Network Byte Order
  int sckt = socket(AF_INET, SOCK_DGRAM, 0);  // Create the Socket
  if (sckt == -1) {
    printf("Error creating client Socket \n");
  } else {
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
void killConfigSocket(clientSocketConfig socketInfo) {
  close(socketInfo.sckt);
  return;
}

/**SendState
 *
 * Sends a UDP packet to all IP addresses given using the Socket in the config
 * given
 */
void sendDataUdp(clientSocketConfig socketInfo, string *data) {
  struct hostent *host;
  const char *payLoad = data->c_str();

  const char *local = "127.0.0.1";
  host = gethostbyname(local);

  socketInfo.addr->sin_addr = *((struct in_addr *)host->h_addr);
  socketInfo.addr->sin_family = AF_INET;

  int a = sendto(socketInfo.sckt, "hello", strlen("hello"), 0,
                 (struct sockaddr *)socketInfo.addr, sizeof(struct sockaddr));

  printf("Sending UDP Packet : %m over socket %o \n", a, socketInfo.sckt);

  return;
}

void sendDataUdp2() {
  int port = NodeUDPSocketPort;
  int sockfd;
  struct sockaddr_in serverAddr;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  strcpy(buffer, "Hello Server\n");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&serverAddr,
         sizeof(serverAddr));
  printf("[+]Data Send: %s", buffer);
}
