#include <NetworkHelpers.h>
#include "easylogging++.h"
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




