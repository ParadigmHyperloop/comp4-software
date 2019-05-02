#include <cerrno>

#include "FlightComputer/Network.h"
#include "EasyLogger/easylogging++.h"
#include "ProtoBuffer/Paradigm.pb.h"
#include "FlightComputer/Heartbeat.h"

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
		LOG(INFO)<<pPodCommand.manualbrakenodestate();
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
		LOG(INFO)<<"Command/HeartBeat Recieved";
		return 1;
	}
	else
	{
		LOG(ERROR)<<"Error Parsing Command";
		return 0;
	}
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int32_t commanderThread(Pod Pod)
 {
	//Logging
	el::Helpers::setThreadName("Commander Thread");
	LOG(INFO)<<"Starting Commander Thread";

	//Sockets and buffers
	 int32_t iNewSockFd, iMessageSize;
	 int32_t iSockfd = createCommanderServerSocket(Pod.sPodNetworkValues->iCommanderPortNumber);
	 char buffer[256]={0};
	 if(iSockfd < 0 )
	 {
		 // Restart thread?
		 LOG(INFO)<<"ERROR initializing commader thread";
		 return 0;
	 }

	 //Watchdog
	 Heartbeat pulse = Heartbeat(Pod.sPodNetworkValues->iCommaderTimeoutMili);

	 //pod state != shutdown
	 while(1)
	 {

		 /* Accepted connection gets put iNewSockfd,
		 * thread will hang here until a connection is recieved.
		 */
		 iNewSockFd = accept(iSockfd, nullptr, nullptr);
		 if (iNewSockFd < 0)
		 {

			 LOG(INFO)<<"ERROR on accept";
		 }
		 fcntl(iNewSockFd, F_SETFL, fcntl(iNewSockFd, F_GETFL, 0) | O_NONBLOCK);
		 LOG(INFO)<< "Controls Interface Connected";
		 pulse.feed();
		 while(1){
			 iMessageSize = read(iNewSockFd,buffer,255);
			 if(iMessageSize < 0)
			 {
				 if(errno == 11) //Erno 11 means no message available on non blocking socket
				 {
					 if(pulse.expired())
					 {
						 LOG(INFO)<<"ERROR: Controls Interface Connection Timeout";
						 break;
					 }
				 }
				 else
				 {
					 LOG(INFO)<<"ERROR: Receveiving message : "<< errno;
				 }
			 }
			 if(iMessageSize == 0)
			 {
				 LOG(INFO)<<"Controls Interface Connection Closed";
				 break;
			 }
			 else if(iMessageSize > 0)
			 {
				 pulse.feed();
				 int32_t iParseMessage = unserializeProtoMessage(&Pod, buffer, iMessageSize);
				 memset(buffer, 0, sizeof buffer);
				 if(iParseMessage){
					 //Return 1 to confirm reception
					 iMessageSize = write(iNewSockFd,"1",1);
				 }
				 else
				 {
					 //Return 0 indicating bad message
					 iMessageSize = write(iNewSockFd,"0",1);
				 }
				 if (iMessageSize < 0){
					 if(errno == 104)
					 {
						 LOG(INFO)<<"Controls Interface Connection Closed";
					 }
					 else
					 {
						 LOG(INFO)<<"ERROR writing to socket: "<< errno;
					 }
					 break;
				 }
			 }
		 }
		 close(iNewSockFd);
	 }
     close(iSockfd);
 }
#pragma clang diagnostic pop



