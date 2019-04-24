#include "FlightComputer/Pod.h"
#include "ProtoBuffer/Paradigm.pb.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>


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


void retrieveNodeUpdate(Pod* Pod, int32_t iNodeServerSocket)
{
		char cBuffer[30] = {0};
		bzero(&cBuffer, sizeof cBuffer);
		int32_t iRecievedPacketSize = recvfrom(iNodeServerSocket, cBuffer, 300, 0, nullptr, nullptr);
		if(iRecievedPacketSize != -1)
		{
			LOG(DEBUG)<<"Packet Recieved on socket:" << iRecievedPacketSize;
			parseBrakeNodeUpdate(Pod, cBuffer);
		}
		else
		{
			LOG(DEBUG)<<"No Packet Recieved on socket:" << iNodeServerSocket;
		}
}


const char* getPodUpdateMessage(Pod* Pod)
{
	// A temporary plain text message for easy debugging on the node sim side
	PodStates PodState = Pod->getPodState();
	switch(PodState)
	{
	case psBooting:
		return "Booting";
	case psStandby:
		return "StandBy";
	case psArming:
		return "Arming";
	case psArmed:
		return "Armed";
	case psPreFlight:
		return "PreFlight";
	case psAcceleration:
		return "Acceleration";
	case psCoasting:
		return "Coasting";
	case psBraking:
		return "Braking";
	case psDisarming:
		return "Disarming";
	case psRetrieval:
		return "Retrieval";
	default:
		return "Emergency";
	}
}

/**
 *Wait on socket, parse the recieved message into a protobuf and hand it off.
 */
 int32_t nodeNetworkThread(Pod Pod)
{
	 // create outbound socket

	 // Create array of nodes

	 // for each node
	 	 // give update
	 	 // get update


	 return 1;
}







/**
 *Wait on socket, parse the recieved message into a protobuf and hand it off.
 */
 int32_t podInternalNetworkThread(Pod Pod)
{
	// Network setup
	int32_t iNodeServerSocket = createNodeServerSocket(Pod.sPodNetworkValues->iNodeServerPortNumber);
	std::vector<std::string> cNodeIpAddrs = Pod.sPodNetworkValues->cNodeIpAddrs;

	if(iNodeServerSocket < 1)
	{
		LOG(INFO)<<"ERROR Making Node Server Socket";
		return iNodeServerSocket;
	}

	clientSocketConfig cscNodeClientSocket = initializeClientSocket(Pod);
	LOG(INFO)<<"Starting Node Server Thread";
	// While mode isnt shutdown
	while(1){
		// Serve all nodes with an update
		// Create Update Packet
		const char* cMesssage = getPodUpdateMessage(&Pod);
		//Serve Update to all Nodes

		for(std::size_t i=0; i<cNodeIpAddrs.size(); ++i)
		{
			sendDataUdp(&cscNodeClientSocket, cMesssage, strlen(cMesssage), cNodeIpAddrs[i]);
		}

		// Check all sockets for an update and parse
		retrieveNodeUpdate(&Pod, iNodeServerSocket);

	}
	close(cscNodeClientSocket.sckt);
	close(iNodeServerSocket);
	return 0;
}


