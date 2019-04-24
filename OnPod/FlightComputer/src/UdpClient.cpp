#include <FlightComputer/Network.h>
#include "EasyLogger/easylogging++.h"
#include <cerrno>

using namespace std;



clientSocketConfig createUdpClientSocket(int32_t iPort)
{
  LOG(INFO)<<"Creating Client Socket";
  int32_t iSocket = socket(PF_INET, SOCK_DGRAM, 0);
  if (iSocket < 0){
	  LOG(INFO)<<"Could not create Node Client socket :" + std::strerror(errno);
	  throw std::runtime_error();
  }
  return iSocket;
}



sockaddr_in createGenericNodeAddr()
{
  //int iNodePort = iPort; // Node Port
  struct sockaddr_in SocketAddrStruct;
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  SocketAddrStruct.sin_family = AF_INET;
  //SocketAddrStruct.sin_port = htons(iNodePort);

  return SocketAddrStruct;
}





/**createUdpClientSocket
 *
 * Create the socket that outbound UDP packets will be transmitted over. Create
 * a sockarrd_inn struct and populate it with common fields. Group into a
 * socketConfig struct
 *
 * Param:None
 * Returns : SocketConfig struct
 *
 * Throws: Runtime error if the socket cant be created.
 */
clientSocketConfig createUdpClientSocketConfig(int32_t iPort)
{
  LOG(INFO)<<"Creating Client Socket";
  int iNodePort = iPort; // Node Port
  int iSocket;
  struct sockaddr_in SocketAddrStruct;
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  iSocket = socket(PF_INET, SOCK_DGRAM, 0);
  if (iSocket < 0){
	  LOG(INFO)<<"Could not create Node Client socket :" + std::strerror(errno);
	  throw std::runtime_error();
  }
  SocketAddrStruct.sin_family = AF_INET;
  SocketAddrStruct.sin_port = htons(iNodePort);
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




