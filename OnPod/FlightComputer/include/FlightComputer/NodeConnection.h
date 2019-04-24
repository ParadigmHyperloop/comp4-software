#include "FlightComputer/Heartbeat.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>

class NodeConnection
{
	public:

		virtual ~NodeConnection();

		NodeConnection(std::string strIp, int32_t iNodeAddrPort,int32_t iNodeServerPort, int32_t timeoutMilis, Pod pod, int32_t iSocket):iServerPort(iNodeServerPort), iOutboundSocket(iSocket), pod(pod), iAddrPort(iNodeAddrPort), strNodeIp(strIp){
			iInboundSocket = -1;
			this->pulse = Heartbeat(timeoutMilis);
			this->sNodeAddr = sockaddr_in;
			memset(&this->sNodeAddr, '\0', sizeof(this->sNodeAddr));
			this->sNodeAddr = createGenericNodeAddr();
			this->sNodeAddr.sin_port = htons(iAddrPort);
			this->sNodeAddr.sin_addr.s_addr = inet_addr(strIp.c_str());
		}

		const char* getPodUpdateFormat(){
			// A temporary plain text message for easy debugging on the node sim side
			PodStates PodState = this->pod->getPodState();
			switch(PodState)
			{
			case psBooting:
				return "Booting";
			case psStandby:
				return "StandBy";
			case psArming:
				return "Arming";
			case psArmed:
				return "Armed";
			case psPreFlight:
				return "PreFlight";
			case psAcceleration:
				return "Acceleration";
			case psCoasting:
				return "Coasting";
			case psBraking:
				return "Braking";
			case psDisarming:
				return "Disarming";
			case psRetrieval:
				return "Retrieval";
			default:
				return "Emergency";
			}
		}

		bool giveUpdate(){
			const char* cPayload = this->getPodUpdateFormat();
			int32_t sent = sendto(this->iOutboundSocket, cPayload, strlen(cPayload), 0, (struct sockaddr*)&this->sNodeAddr, sizeof(this->sNodeAddr));
			if(sent == -1)
			{
				throw std::runtime_error();
			}
			return true;
		}

		bool getUpdate(){
			char cBuffer[30] = {0};
			bzero(&cBuffer, sizeof cBuffer);
			int32_t iRecievedPacketSize = recvfrom(this->iInboundSocket, cBuffer, 300, 0, nullptr, nullptr);
			if(iRecievedPacketSize != -1)
			{
				LOG(DEBUG)<<"Packet Recieved on socket:" << iRecievedPacketSize;
				try{
					this->parseUpdate(Pod, cBuffer);
				}
				catch(){}
				this->pulse.feed();
			}
			else
			{
				LOG(DEBUG)<<"No Packet Recieved on socket:" << iNodeServerSocket;
			}
			return true;
		}

		bool createServerSocket(){
			int32_t iSocket;
			struct sockaddr_in SocketAddrStruct;
			iSocket = socket(AF_INET, SOCK_DGRAM, 0);
		    if (iSocket < 0)
		    {
		    	LOG(INFO)<<"ERROR Creating Node Server Socket: " <<  + std::strerror(errno);
		    	throw std::runtime_error();
		    }
			int flags = fcntl(iSocket, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(iSocket, F_SETFL, flags);
			SocketAddrStruct.sin_family = AF_INET;
			SocketAddrStruct.sin_port = htons(iServerPort);
			SocketAddrStruct.sin_addr.s_addr = INADDR_ANY;
			int32_t iBind = bind(iSocket, (struct sockaddr*)&SocketAddrStruct, sizeof(SocketAddrStruct));
			if(iBind < 0)
			{
		    	LOG(INFO)<<"ERROR Binding Node Server Socket: " <<  + std::strerror(errno);
		    	throw std::runtime_error();
			}
			this->iInboundSocket = iSocket;
			return true;
		}

		virtual bool parseUpdate(){




		}

		Pod* pod;
		int32_t iOutboundSocket;
		int32_t iInboundSocket;
		int32_t iAddrPort;
		int32_t iServerPort;
		std::string strNodeIp;
		Heartbeat pulse;
		std::string strNodeName;
		struct sockaddr_in sNodeAddr;
};


void sendDataUdp(clientSocketConfig* cscSocketInfo, const void* vPayload,  int32_t iPayloadSize, std::string sAddress)
{
  cscSocketInfo->addr.sin_addr.s_addr = inet_addr(sAddress.c_str());
  sendto(cscSocketInfo->sckt, vPayload, iPayloadSize, 0,
         (struct sockaddr*)&cscSocketInfo->addr, sizeof(cscSocketInfo->addr));
}
