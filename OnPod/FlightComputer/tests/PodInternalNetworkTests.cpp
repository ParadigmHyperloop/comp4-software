#include "Client.h"
#include "catch.hpp"
#include "structs.h"

TEST_CASE(" Create the client socket, check its valid then destroy it ") {
	clientSocketConfig* socketInfo = initializeClientSocket();
  REQUIRE(socketInfo->sckt != -1);
}
