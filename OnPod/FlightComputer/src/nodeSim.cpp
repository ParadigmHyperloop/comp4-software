#include "FlightComputer/nodeSim.h"
#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/ProtoStructs.pb.h"

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace fc;

void runNodeSimulator(clientSocketConfig* cscSocket)
{
  std::string sPayload = "";
  for (int iIndex = 0; iIndex < 2; iIndex++)
  {
    brakeNodeData pNodeUpdate;
    pNodeUpdate.set_id(1);
    pNodeUpdate.set_state(brakeNodeData::bnVenting);
    // Pressure Transducers
    pNodeUpdate.set_hp(231);
    pNodeUpdate.set_lp1(666);
    pNodeUpdate.set_lp2(242);
    pNodeUpdate.set_lp3(65);
    pNodeUpdate.set_lp4(34);
    // Solenoids
    pNodeUpdate.set_sol1(1);
    pNodeUpdate.set_sol2(1);
    pNodeUpdate.set_sol3(0);
    pNodeUpdate.set_sol4(0);
    pNodeUpdate.set_sol5(0);
    pNodeUpdate.set_sol6(1);
    pNodeUpdate.set_temp(100);

    int iProtoPacketSize = pNodeUpdate.ByteSizeLong();
    pNodeUpdate.SerializeToString(&sPayload);
    void* vBuffer = malloc(iProtoPacketSize);
    pNodeUpdate.SerializeToArray(vBuffer, iProtoPacketSize);
    sendDataUdp(cscSocket, vBuffer, (int)iProtoPacketSize);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  killConfigSocket(cscSocket);
  return;
}
