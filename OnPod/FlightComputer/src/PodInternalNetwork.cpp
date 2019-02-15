#include "FlightComputer/PodInternalNetwork.h"

#define UDPPORT 5008

using namespace std;

/**killConfigSocket
 *
 *Close the socket of the socketConfig struct provided
 *
 */
void killConfigSocket(clientSocketConfig* cscSocketInfo)
{
  close(cscSocketInfo->sckt);
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
clientSocketConfig* initializeClientSocket()
{
  printf("Creating Client Socket \n");
  int iPort = UDPPORT;
  int iSocket;
  struct sockaddr_in SocketAddrStruct;
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  iSocket = socket(PF_INET, SOCK_DGRAM, 0);
  SocketAddrStruct.sin_family = AF_INET;
  SocketAddrStruct.sin_port = htons(iPort);
  clientSocketConfig* cscSocketInfo = new clientSocketConfig;
  cscSocketInfo->addr = SocketAddrStruct;
  cscSocketInfo->sckt = iSocket;
  return cscSocketInfo;
}

void sendDataUdp(clientSocketConfig* cscSocketInfo, void* vPayload, int iPayloadSize)
{
  cscSocketInfo->addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sendto(cscSocketInfo->sckt, vPayload, iPayloadSize, 0,
         (struct sockaddr*)&cscSocketInfo->addr, sizeof(cscSocketInfo->addr));
  printf("Data Sent \n");
}
