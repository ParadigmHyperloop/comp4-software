
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "FlightComputer/PodInternalNetwork.h"

struct pod {
  // States
  unsigned char podS = 0;
  unsigned char terminalS = 0;
  unsigned char motorS = 0;
  unsigned char rearNodeS;
  // Navigation
  float distance = 0;
  float velocity = 0;
  // Rear Node
  float gpioVals;
};

int main(int argc, char *argv[]) {
  /*  char *nodeIp[3] = {"127.0.0.1", "127.0.0.1", "127.0.0.1"};
    sendState(nodeIp, 3);*/

  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //  if (sockfd < 0) error("ERROR opening socket");
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)

    listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);

  printf("Here is the message: %s\n", buffer);

  close(newsockfd);
  close(sockfd);
  return 0;

  return 0;
}
