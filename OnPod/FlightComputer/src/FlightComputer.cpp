
#include <thread>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FlightComputer/nodeSim.h"
#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/NodeServer.h"

int main(int argc, char **argv) {
  int sckt = createNodeServerSocket();
  clientSocketConfig* sckt2 = initializeClientSocket();

  std::thread server(nodeServerThread, sckt);

  std::thread sim(runNodeSimulator, sckt2);

  server.join();

  sim.join();

  return 0;
}

/*int sckt = createNodeServerSocket();


   //thread first(nodeServerThread, sckt);

  //  first.join();                // pauses until first finishes


         cout << "StartSim" << endl;

         // thread sim(runNodeSimulator);

         int serverSckt;

         serverSckt = createNodeServerSocket();


         nodeServerThread(serverSckt);

        // thread server(nodeServerThread, serverSckt);


        // sim.join();

         //server.join();

         //runNodeSimulator();
         cout << "FinishSim" <<endl;
*/
