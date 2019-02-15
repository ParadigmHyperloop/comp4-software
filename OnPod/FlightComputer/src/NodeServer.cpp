#include "FlightComputer/PodInternalNetwork.h"

using namespace fc;

void parseBreakNodePacket(fc::brakeNodeData pPayload, PodValues* pvPodValues){
	//Grab State
	switch (pPayload.state()){
		case brakeNodeData::bnsBooting:
		{
			pvPodValues->BreakNodeState = bnsBooting;
			break;
		}
		case brakeNodeData::bnsStandby:
		{
			pvPodValues->BreakNodeState = bnsStandby;
			break;
		}
		case brakeNodeData::bnsArming:
		{
			pvPodValues->BreakNodeState = bnsArming;
			break;
		}
		case brakeNodeData::bnsArmed:
		{
			pvPodValues->BreakNodeState = bnsArmed;
			break;
		}
		case brakeNodeData::bnsFlight:
		{
			pvPodValues->BreakNodeState = bnsFlight;
			break;
		}
		case brakeNodeData::bnsBraking:
		{
			pvPodValues->BreakNodeState = bnsBraking;
			break;
		}
		case brakeNodeData::bnsVenting:
		{
			pvPodValues->BreakNodeState = bnsVenting;
			break;
		}
		case brakeNodeData::bnsRetrieval:
		{
			pvPodValues->BreakNodeState = bnsRetrieval;
			break;
		}
		case brakeNodeData::bnsError:
		{
			pvPodValues->BreakNodeState = bnsError;
			break;
		}
		default:
		{
			pvPodValues->BreakNodeState = bnsError;
		}
		//Offload Data
		pvPodValues->bSolenoid1 = pPayload.sol1();
		pvPodValues->bSolenoid2 = pPayload.sol2();
		pvPodValues->bSolenoid3 = pPayload.sol3();
		pvPodValues->bSolenoid4 = pPayload.sol4();
		pvPodValues->bSolenoid5 = pPayload.sol5();
		pvPodValues->bSolenoid6 = pPayload.sol6();
		pvPodValues->iLowPressure1 = pPayload.lp1();
		pvPodValues->iLowPressure2 = pPayload.lp2();
		pvPodValues->iLowPressure3 = pPayload.lp3();
		pvPodValues->iLowPressure4 = pPayload.lp4();
		pvPodValues->iHighPressure = pPayload.hp();
		pvPodValues->iPressureVesselTemperature = pPayload.temp();


	}
}





int createNodeServerSocket() {
  int iPort = 5008;
  int iSocket;
  struct sockaddr_in SocketAddrStruct;
  iSocket = socket(AF_INET, SOCK_DGRAM, 0);
 // memset(&SocketAddrStruct, '\0', sizeof(SocketAddrStruct));  //Replacement, I read online this isnt even needed??
  SocketAddrStruct.sin_family = AF_INET;
  SocketAddrStruct.sin_port = htons(iPort);
  SocketAddrStruct.sin_addr.s_addr = inet_addr("127.0.0.1");
  bind(iSocket, (struct sockaddr*)&SocketAddrStruct, sizeof(SocketAddrStruct));
  return iSocket;
}

int nodeServerThread(int iSocket, PodValues* pvPodValues ) {
  char cBuffer[100] = {0};
  while (1)
  {
    bzero(&cBuffer, sizeof cBuffer);
    printf("Waiting to recieve on socket: %i \n", iSocket);
    int iRecievedPacketSize = recvfrom(iSocket, cBuffer, 300, 0, nullptr, nullptr);
    fc::brakeNodeData pNodeUpdate;
    bool bProtoPacketParsed = pNodeUpdate.ParseFromArray(&cBuffer, iRecievedPacketSize);
    if(bProtoPacketParsed)
    {
    	printf("Contents are: \n%s \n", pNodeUpdate.DebugString().c_str());
    	parseBreakNodePacket( pNodeUpdate,pvPodValues);
    }
    else
    {
    	printf("Error Parsing Protobuf packet");
    }
  }
  close(iSocket);
  return 0;
}


