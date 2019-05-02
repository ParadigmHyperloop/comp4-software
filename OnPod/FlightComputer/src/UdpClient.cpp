#include <FlightComputer/Network.h>
#include "EasyLogger/easylogging++.h"
#include <cerrno>

using namespace std;



int32_t createUdpClientSocket()
{
  LOG(INFO)<<"Creating Client Socket";
  int32_t iSocket = socket(PF_INET, SOCK_DGRAM, 0);
  if (iSocket < 0){
    std::string sError = std::string("Could not create Node Client socket: ") + std::strerror(errno);
    throw std::runtime_error(sError);
  }
  return iSocket;
}



sockaddr_in createGenericNodeAddr()
{
  struct sockaddr_in SocketAddrStruct;
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  SocketAddrStruct.sin_family = AF_INET;
  return SocketAddrStruct;
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




