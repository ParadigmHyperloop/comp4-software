#include "FlightComputer/Network.h"
#include "EasyLogger/easylogging++.h"
#include "ProtoBuffer/Paradigm.pb.h"

// Get manual state change commands. Get Estop command

 int32_t createCommanderServerSocket(int32_t iPortNumber) {
	 int32_t iSockfd;
     struct sockaddr_in serv_addr;
     iSockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (iSockfd < 0)
     {
         LOG(INFO)<<"ERROR opening commander socket";
         return -1;
     }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(iPortNumber);
     if (bind(iSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
    	 LOG(INFO)<<"ERROR binding commander socket";
    	 return -1;
     }
     //Queue 3 requests before rejecting
     listen(iSockfd,3);
     return iSockfd;
 }

void parseProtoCommand(podCommand pPodCommand, Pod* Pod)
{
	if(pPodCommand.has_controlsinterfacestate())
	{
		Pod->setControlsInterfaceState(pPodCommand.controlsinterfacestate());
	}
	if(pPodCommand.has_automaticstatetransitions())
	{
		Pod->setAutomaticTransitions(pPodCommand.automaticstatetransitions());
	}
	if(pPodCommand.has_manualbrakenodestate())
	{
		Pod->setManualBrakeNodeState(pPodCommand.manualbrakenodestate());
	}
	if(pPodCommand.has_manuallvdcnodestate())
	{
		Pod->setManualLvdcNodeState(pPodCommand.manuallvdcnodestate());
	}
	if(pPodCommand.has_manualpodstate())
	{
		Pod->setManualPodState(pPodCommand.manualpodstate());
	}
	return;
}

int32_t unserializeProtoMessage(Pod* Pod,char cBuffer[], int32_t iMessageSize)
{
	podCommand pPodCommand;
	bool bProtoPacketParsed = pPodCommand.ParseFromArray(cBuffer, iMessageSize);
	if(bProtoPacketParsed)
	{
		parseProtoCommand(pPodCommand, Pod);
		LOG(INFO)<<"Command Recieved";
		return 1;
	}
	else
	{
		LOG(ERROR)<<"Error Parsing Command";
		return 0;
	}
}


int32_t commanderThread(Pod Pod)
 {
	 socklen_t clilen;
	 int32_t iNewSockFd, iMessageSize;
	 int32_t iSockfd = createCommanderServerSocket(Pod.sPodNetworkValues->iCommanderPortNumber);
	 if(iSockfd < 0 )
	 {
		 // Restart thread?
		 LOG(INFO)<<"ERROR initializing commader thread";
		 return 0;
	 }

	 struct sockaddr_in cli_addr;
	 clilen = sizeof(cli_addr);
	 char buffer[256]={0};

	 //pod state != shutdown
	 while(1)
	 {

		 //Accepted connection gets put on a new socket
		 iNewSockFd = accept(iSockfd, (struct sockaddr *) &cli_addr, &clilen);
		 if (iNewSockFd < 0)
		 {
			 LOG(INFO)<<"ERROR on accept";
		 }

		 int iOn = 1;
		 //int idle = 1;	/* Number of idle seconds before sending a KeepAlive probe. */
		 int interval = 3;	/* How often in seconds to resend an unacked KeepAlive probe. */
		 int count = 3;	/* How many times to resend a KA probe if previous probe was unacked. */

		 // Switch KeepAlive on or off for this side of the socket. */
		 if (setsockopt(iNewSockFd, SOL_SOCKET, SO_KEEPALIVE, &iOn, sizeof(int)) < 0)
		 {
			 LOG(INFO)<< "Error creating keep alive socket";
			 return -1;
		 }

		 //setsockopt(iNewSockFd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
		 setsockopt(iNewSockFd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
		 setsockopt(iNewSockFd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));

		 // For OSX you just need this config
		 setsockopt(iNewSockFd, IPPROTO_TCP, TCP_KEEPALIVE, &interval, sizeof(interval));


		 LOG(INFO)<< "Controls Interface Heart Beat Connected";
		 while(1){

			 iMessageSize = read(iNewSockFd,buffer,255);
			 if(iMessageSize < 0){
				 LOG(INFO)<<"ERROR reading from socket";
			 }
			 else if(iMessageSize == 0){
				 // No Message
			 }
			 else if(iMessageSize > 0)
			 {
				 int32_t iParseMessage = unserializeProtoMessage(&Pod, buffer, iMessageSize);
				 memset(buffer, 0, sizeof buffer);
				 if(iParseMessage){
					 //Return 1 to confirm reception
					 //iMessageSize = write(iNewSockFd,"1",1);
				 }
				 else
				 {
					 //Return 0 indicating bad message
					 //iMessageSize = write(iNewSockFd,"0",1);
				 }
				 if (iMessageSize < 0){
					 LOG(INFO)<<"ERROR writing to socket";
					 break;
				 }
			 }
		 }
		 close(iNewSockFd);
	 }

     close(iSockfd);
 }



