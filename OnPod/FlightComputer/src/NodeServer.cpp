#include "FlightComputer/Pod.h"
#include "ProtoBuffer/Paradigm.pb.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>
#include <FlightComputer/NodeConnection.h>


#include <iostream>


/**
 * createNodeServerSocket
 *
 * returns: The socket discriptor number that is associated with the created socket
 * params: None
 *
 * Create the UDP socket that Pod Internal Work Node telemetry will be recieved on.
 */
 int32_t createNodeServerSocket(int32_t iPortNumber) {
	int32_t iSocket;
	struct sockaddr_in SocketAddrStruct;
	iSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocket < 0)
    {
    	LOG(INFO)<<"ERROR Making Node Server Socket";
    	return iSocket;
    }
	int flags = fcntl(iSocket, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(iSocket, F_SETFL, flags);
	SocketAddrStruct.sin_family = AF_INET;
	SocketAddrStruct.sin_port = htons(iPortNumber);
	SocketAddrStruct.sin_addr.s_addr = INADDR_ANY;
	int32_t iBind = bind(iSocket, (struct sockaddr*)&SocketAddrStruct, sizeof(SocketAddrStruct));
	if(iBind < 0)
	{
    	LOG(INFO)<<"ERROR Binding Node Server Socket";
    	return iBind;
	}
	return iSocket;
}


void parseBrakeNodeUpdate(Pod* Pod, char cUpdate[])
{
	 BrakeNodeStates bnsBrakeNodeStates[9] = {bnsBooting,
			 	 	 	 					  bnsStandby,
											  bnsArming,
											  bnsArmed,
											  bnsFlight,
											  bnsBraking,
											  bnsVenting,
											  bnsRetrieval,
											  bnsError};
	 std::string sNodeState(cUpdate);
	 int32_t iStateNumber = std::stoi(sNodeState);
	 BrakeNodeStates eBrakeNodeState = bnsBrakeNodeStates[iStateNumber];
	 Pod->setBrakeNodeState(eBrakeNodeState);
	 return;
}



/**
 *Wait on socket, parse the received message into a protobuf and hand it off.
 */
 int32_t podInternalNetworkThread(Pod Pod)
{
	 int32_t iClientSocket = -1;
	 iClientSocket = createUdpClientSocket();
	 if(iClientSocket < 0 )
	 {
		 LOG(INFO)<< std::string("Error creating client socket for Node Server: ") + std::strerror(errno);
		 return -1;
	 }

	 BrakeNodeConnection BrakeNode = BrakeNodeConnection();


	 if(!BrakeNode.initiate()){
		 LOG(INFO)<< " Failed to initiate : "
	 }
	 // Create array of nodes

	 //while true

		 // for each node
			 // give update
			 // get update

	 //close sockets

	 return 1;
}


