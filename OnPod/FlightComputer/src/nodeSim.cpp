#include "FlightComputer/nodeSim.h"
#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/ProtoStructs.pb.h"

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace fc;

void runNodeSimulator(clientSocketConfig* sckt) {
  std::string payload = "";

  for (int i = 0; i <2; i++) {
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

    size_t size = nodeUpdate.ByteSizeLong();

    printf("Prepared Packet Size: %i \n",(int)size);
    void *buffer = malloc(size);
    nodeUpdate.SerializeToArray(buffer, size);

    sendDataUdp(sckt, buffer);

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  killConfigSocket(sckt);

  return;
}
