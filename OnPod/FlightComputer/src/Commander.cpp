#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "EasyLogger/easylogging++.h"
#include "FlightComputer/Commander.h"

// Get manual state change commands. Get Estop command

 int32_t createCommanderServerSocket(int32_t iPortNumber) {
	 int32_t iSockfd, iNewSockFd;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int32_t iMessageSize;
     iSockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (iSockfd < 0)
     {
         LOG(INFO)<<"ERROR opening commander socket";
     }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(iPortNumber);
     if (bind(iSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
    	 LOG(INFO)<<"ERROR binding commander socket";
     }

     listen(iSockfd,5);
     clilen = sizeof(cli_addr);
     iNewSockFd = accept(iSockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (iNewSockFd < 0)
     {
    	 LOG(INFO)<<"ERROR on accept";
     }
     bzero(buffer,256);
     iMessageSize = read(iNewSockFd,buffer,255);
     if (iMessageSize < 0){
    	 LOG(INFO)<<"ERROR reading from socket";
     }
     LOG(INFO)<<"Here is the message: " << buffer;
     iMessageSize = write(iNewSockFd,"I got your message",18);
     if (iMessageSize < 0){
    	 LOG(INFO)<<"ERROR writing to socket";
     }
     close(iNewSockFd);
     close(iSockfd);
     return 0; 
	 
}
