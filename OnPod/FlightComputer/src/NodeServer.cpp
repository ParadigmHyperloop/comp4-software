#include <Client.h>
#include "FlightComputer/Pod.h"
#include "ProtoBuffer/NodeTelem.pb.h"
#include "EasyLogger/easylogging++.h"
#include <fcntl.h>
#include <iostream>
#include <string>

using namespace fc;
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
	int flags = fcntl(iSocket, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(iSocket, F_SETFL, flags);
	SocketAddrStruct.sin_family = AF_INET;
	SocketAddrStruct.sin_port = htons(iPortNumber);
	SocketAddrStruct.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(iSocket, (struct sockaddr*)&SocketAddrStruct, sizeof(SocketAddrStruct));
	return iSocket;
}


 void parseBrakeNodeUpdate(Pod* Pod, char cUpdate[])
 {
	 brakeNodeData::breakNodeState bnsBrakeNodeStates[9] = {brakeNodeData::bnsBooting,
			 	 	 	 								 brakeNodeData::bnsStandby,
														 brakeNodeData::bnsArming,
														 brakeNodeData::bnsArmed,
														 brakeNodeData::bnsFlight,
														 brakeNodeData::bnsBraking,
														 brakeNodeData::bnsVenting,
														 brakeNodeData::bnsRetrieval,
														 brakeNodeData::bnsError};
	 std::string sNodeState(cUpdate);
	 int32_t iStateNumber = std::stoi(sNodeState);
	 brakeNodeData::breakNodeState bnsNodeState = bnsBrakeNodeStates[iStateNumber];
	 Pod->setBrakeNodeState(bnsNodeState);
	 return;
 }


 void retrieveNodeUpdate(Pod* Pod, int32_t iNodeServerSocket)
 {
		char cBuffer[30] = {0};
		bzero(&cBuffer, sizeof cBuffer);
		LOG(DEBUG)<<"Waiting to recieve on socket: " << iNodeServerSocket;
		int32_t iRecievedPacketSize = recvfrom(iNodeServerSocket, cBuffer, 300, 0, nullptr, nullptr);
		if(iRecievedPacketSize != -1)
		{
			parseBrakeNodeUpdate(Pod, cBuffer);
			LOG(DEBUG)<<"Packet Recieved on socket:" << iRecievedPacketSize;
		}
		else
		{
			LOG(DEBUG)<<"No Packet Recieved on socket:" << iNodeServerSocket;
		}
 }

const char* getPodUpdateMessage(Pod* Pod)
{
	ePodStates PodState = Pod->getPodState();
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
	case psAcceleration:
		return "Acceleration";
	case psCoasting:
		return "Coasting";
	case psBraking:
		return "Braking";
	case psDisarm:
		return "Disarm";
	case psRetrieval:
		return "Retrieval";
	default:
		return "Emergency";
	}
}




/**
 *Wait on socket, parse the recieved message into a protobuf and hand it off.
 */
 int32_t podInternalNetworkThread(Pod* Pod)
{
	 Pod->sPodValues->iNodeServerPortNumber = 5005;

	// Store in Config
	std::string cNodeAddresses[] = {"127.0.0.1","127.0.0.1"};
	int32_t iNumberOfNodes = 2;
	// Network setup
	int32_t iNodeServerSocket = createNodeServerSocket(Pod->getNodeServerPortNumber());
	clientSocketConfig cscNodeClientSocket = initializeClientSocket();

	// While mode isnt shutdown
	while(1){
		// Serve all nodes with an update
		// Create Update Packet
		const char* cMesssage = getPodUpdateMessage(Pod);
		//Serve Update to all Nodes
		for (int i=0 ; i<iNumberOfNodes ; i++)
		{
			sendDataUdp(&cscNodeClientSocket, cMesssage, strlen(cMesssage), cNodeAddresses[i]);
		}
		// Check all sockets for an update and parse
		retrieveNodeUpdate(Pod, iNodeServerSocket);
	}
}


