#include "FlightComputer/Network.h"
#include "EasyLogger/easylogging++.h"

// Get manual state change commands. Get Estop command

 int32_t createCommanderServerSocket(int32_t iPortNumber) {
	 int32_t iSockfd;
     struct sockaddr_in serv_addr;
     iSockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (iSockfd < 0)
     {
         LOG(INFO)<<"ERROR opening commander socket";
     }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(iPortNumber);
     if (bind(iSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
    	 LOG(INFO)<<"ERROR binding commander socket";
     }
     //Queue 3 requests before regecting
     listen(iSockfd,3);
     return iSockfd;
 }

int32_t unserializeProtoMessage(*char cBuffer, iMessageSize)
{
	fc::brakeNodeData pNodeUpdate;
	bool bProtoPacketParsed = pNodeUpdate.ParseFromArray(&cBuffer, iRecievedPacketSize);
	if(bProtoPacketParsed)
	{
		LOG(INFO)<<"Packet Recieved";
		parseBreakNodePacket( pNodeUpdate,*Pod);
	}
	else
	{
		LOG(ERROR)<<"Error Parsing Protobuf packet";
	}
	return 1;
}


void commanderThread(Pod Pod)
 {
	 socklen_t clilen;
	 int32_t iNewSockFd, iMessageSize;
	 int32_t iSockfd = createCommanderServerSocket(5009);
	 struct sockaddr_in cli_addr;
	 clilen = sizeof(cli_addr);
	 char buffer[256];

	 //pod state != shutdown
	 while(1)
	 {
		 //Accepted connection gets put on a new socket
		 iNewSockFd = accept(iSockfd, (struct sockaddr *) &cli_addr, &clilen);
		 if (iNewSockFd < 0)
		 {
			 LOG(INFO)<<"ERROR on accept";
		 }
		 //Zero the recieving buffer
		 bzero(buffer,256);
		 iMessageSize = read(iNewSockFd,buffer,255);
		 if (iMessageSize < 0){
			 LOG(INFO)<<"ERROR reading from socket";
		 }
		 LOG(INFO)<<"Here is the message: " << buffer;

		 int32_t = unserializeProtoMessage(&buffer, iMessageSize);

		 //Return 1 to confirm reception
		 iMessageSize = write(iNewSockFd,"1",1);
		 if (iMessageSize < 0){
			 LOG(INFO)<<"ERROR writing to socket";
		 }
	 }
     close(iNewSockFd);
     close(iSockfd);
 }



