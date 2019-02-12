#include "FlightComputer/ProtoStructs.pb.h"
#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/nodeSim.h"


#include <iostream>       // std::cout, std::endl
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace fc;

void runNodeSimulator(){

	clientSocketConfig  sckt = initializeClientSocket();
	std::string payload = "";

	for( int i = 0 ; i < 10 ; i++){
		brakeNodeData nodeUpdate;
		nodeUpdate.set_id(1);
		nodeUpdate.set_state(brakeNodeData::bnVenting);
		// Pressure Transducers
		nodeUpdate.set_hp(rand());
		nodeUpdate.set_lp1(rand());
		nodeUpdate.set_lp2(rand());
		nodeUpdate.set_lp3(rand());
		nodeUpdate.set_lp4(rand());
		// Solenoids
		nodeUpdate.set_sol1(rand());
		nodeUpdate.set_sol2(rand());
		nodeUpdate.set_sol3(rand());
		nodeUpdate.set_sol4(rand());
		nodeUpdate.set_sol5(rand());
		nodeUpdate.set_sol6(rand());


		std::string* nodeUpdateSerialized = &payload;

		nodeUpdate.SerializeToString(nodeUpdateSerialized);

		sendDataUdp(sckt, nodeUpdateSerialized);

		std::this_thread::sleep_for (std::chrono::seconds(1));

	}

	killConfigSocket(sckt);

	return;

}
