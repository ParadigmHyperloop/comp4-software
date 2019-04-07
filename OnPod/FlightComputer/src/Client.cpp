#include <FlightComputer/Network.h>
#include "EasyLogger/easylogging++.h"

#define UDPPORT 5000

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
clientSocketConfig initializeClientSocket()
{
  LOG(INFO)<<"Creating Client Socket";
  int iPort = UDPPORT;
  int iSocket;
  struct sockaddr_in SocketAddrStruct;
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  iSocket = socket(PF_INET, SOCK_DGRAM, 0);
  SocketAddrStruct.sin_family = AF_INET;
  SocketAddrStruct.sin_port = htons(iPort);
  clientSocketConfig cscSocketInfo;
  cscSocketInfo.addr = SocketAddrStruct;
  cscSocketInfo.sckt = iSocket;
  return cscSocketInfo;
}

/**SendDataUdp
 *
 * TODO: attatched logging to the sendto return value for error logging and handeling
 *
 * Params:  cscSocketInfo: A client socket config structure that holds all the configurtions values for the socket
 * 			vPayload: A pointer to the data that is to be sent over the socket
 * 			iPayloadSize: The size of the data to be sent over the socket
 *
 * returns: None
 */
void sendDataUdp(clientSocketConfig* cscSocketInfo, const void* vPayload,  int32_t iPayloadSize, std::string sAddress)
{
  cscSocketInfo->addr.sin_addr.s_addr = inet_addr(sAddress.c_str());
  sendto(cscSocketInfo->sckt, vPayload, iPayloadSize, 0,
         (struct sockaddr*)&cscSocketInfo->addr, sizeof(cscSocketInfo->addr));
}




