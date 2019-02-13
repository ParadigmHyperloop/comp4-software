#include "FlightComputer/PodInternalNetwork.h"

#define NodeUDPSocketPort 5008

using namespace std;


/**killConfigSocket
 *
 *Close the socket of the socketConfig struct provided
 *
 */
void killConfigSocket(clientSocketConfig* socketInfo) {
  close(socketInfo->sckt);
  return;
}


/**initializeClientSocket
 *
 * Create the socket that outbound UDP packets will be transmitted over. Create
 * a sockarrd_inn struct and populate it with common fields. Group into a
 * socketConfig struct
 *
 * Param:None
 * Returns : SocketConfig struct
 */
clientSocketConfig* initializeClientSocket() {
	  printf("Creating Client Socket \n");

	  int port = NodeUDPSocketPort;
	  int sockfd;
	  struct sockaddr_in serverAddr;
	  memset(&serverAddr, '\0', sizeof(serverAddr));
	  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	  serverAddr.sin_family = AF_INET;
	  serverAddr.sin_port = htons(port);
	  clientSocketConfig* info = new clientSocketConfig;
	  info->addr = serverAddr;
	  info->sckt = sockfd;
	  return info;

}

void sendDataUdp(clientSocketConfig* socketInfo, void* payload){

	  socketInfo->addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	  sendto(socketInfo->sckt, payload, sizeof(payload), 0, (struct sockaddr *)&socketInfo->addr,
	           sizeof(socketInfo->addr));

	  printf("Data Sent \n");

}


