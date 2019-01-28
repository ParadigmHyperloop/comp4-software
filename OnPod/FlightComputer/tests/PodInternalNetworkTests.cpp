#include "PodInternalNetwork.h"
#include "catch.hpp"

TEST_CASE(" Create the client socket, check its valid then destroy it ") {
  socketConfig socketInfo = initializeClientSocket();
  REQUIRE(socketInfo.sckt != -1);
}
