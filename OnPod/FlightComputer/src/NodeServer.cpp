#include "FlightComputer/PodInternalNetwork.h"

int createNodeServerSocket() {
  int iPort = 5008;
  int iSocket;
  struct sockaddr_in SocketAddrStruct;
  iSocket = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));
  SocketAddrStruct.sin_family = AF_INET;
  SocketAddrStruct.sin_port = htons(iPort);
  SocketAddrStruct.sin_addr.s_addr = inet_addr("127.0.0.1");
  bind(iSocket, (struct sockaddr*)&SocketAddrStruct, sizeof(SocketAddrStruct));
  return iSocket;
}

int nodeServerThread(int sckt) {
  char cBuffer[100] = {0};
  while (1)
  {
    bzero(&cBuffer, sizeof cBuffer);
    printf("Waiting to recieve on socket: %i \n", sckt);
    int iRecievedPacketSize = recvfrom(sckt, cBuffer, 300, 0, nullptr, nullptr);
    fc::brakeNodeData pNodeUpdate;
    bool bProtoPacketParsed = pNodeUpdate.ParseFromArray(&cBuffer, iRecievedPacketSize);
    if(bProtoPacketParsed)
    {
    	printf("Contents are: \n%s \n", pNodeUpdate.DebugString().c_str());
    }
    else
    {
    	printf("Error Parsing Protobuf packet");
    }
  }
  close(sckt);
  return 0;
}
