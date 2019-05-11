#include "NetworkHelpers.h"
#include "catch.hpp"
#include "Structs.h"
#include "FlightComputer/Pod.h"
#include <string>



TEST_CASE(" Create the client socket, check its valid then destroy it ") {
		// Create Shared Memory
		PodNetwork sPodNetworkValues;
		PodValues sPodValues;

		std::string cNodeIpAddrs[] =  {"192.168.7.1","127.0.0.1"};
		sPodNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs
		sPodNetworkValues.iCommanderPortNumber = 5005; //Port # for TCP Commander
		sPodNetworkValues.iNodePort = 5000; // Port # that Nodes are listening on
		sPodNetworkValues.iNodeServerPortNumber = 5010; // Port # to recieve UDP from Nodes



		// Core Control Loop Thread
		Pod pCoreControlLoop = Pod(&sPodValues, &sPodNetworkValues);
		clientSocketConfig socketInfo = initializeClientSocket(pCoreControlLoop);

		REQUIRE(socketInfo.sckt != -1);
}
