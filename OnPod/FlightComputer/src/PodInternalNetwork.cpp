#include "FlightComputer/PodInternalNetwork.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void initializeSocket() {}

/**
 * Sends a UDP packet to all nodes
 * Param: None
 * Returns: None
 *
 * TODO: Right now this opens and closes a socket on each function call and
 * initializes and unintializes the node IP's.
 * Im thinking we can keep the socket open for the course of the run. And just
 * check that its valid before each use. If its crashed or something
 * then have a function that trys to make a new one. Perhaps give sendState a
 * pointer to a socket thats created as part of the startup.
 */
void sendState(char *addresses[], int numberOfAddresses) {
  int port = 5005;  // Default port
  struct hostent *host;
  // char *address[3] = {"127.0.0.1", "127.0.0.1", "127.0.0.1"};

  // Create destination info struct
  struct sockaddr_in node_addr;
  node_addr.sin_family = AF_INET;
  node_addr.sin_port = htons(port);  // Node Port in Network Byte Orderr
  bzero(&(node_addr.sin_zero), 8);   // Zero the rest of the struct
  int sckt = socket(AF_INET, SOCK_DGRAM, 0);  // Create the Socket

  // Loop over all the IP addresses and send a packet to each one
  for (int i = 0; i < numberOfAddresses; i++) {
    node_addr.sin_port = htons(port);    // Node Port in Network Byte Order
    host = gethostbyname(addresses[i]);  // Puts address in a hostent struct
    node_addr.sin_addr = *((struct in_addr *)host->h_addr);
    sendto(sckt, "sup", strlen("sup"), 0, (struct sockaddr *)&node_addr,
           sizeof(struct sockaddr));
    port++;  // For testing
  }
  close(sckt);
  return;
}
