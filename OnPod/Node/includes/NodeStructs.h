#ifndef NODESTRUCTS
#define NODESTRUCTS

static const int8_t  END_OF_HEADER[] = "\r\n\r\n";

enum NodeType {
	RESERVED = 0,
	BRAKE 	 = 1,
	LVDC  	 = 2,
	REAR  	 = 3
};

enum NodeNum {
	PRIMARY    = 0,
	SECONDARY  = 1,
	TERTIARY   = 2
};



enum TxIntervalMS {
	SHORTEST_DELAY = 40,
	LONGEST_DELAY  = 100
};

struct UDPParams {
	uint8_t  BBB_IP	[4]	= {192, 168, 0, 1};
	uint16_t BBB_UDP_PORT = 8888;
	uint8_t  BRAKE_NODE_IP [4] = {192, 168, 2, 51};
	uint8_t  LVDC_NODE_IP  [4] = {192, 168, 2, 52};
	uint8_t  REAR_NODE_IP  [4] = {192, 168, 2, 53};
	uint16_t NODE_RX_UDP_PORT = 777;
};

struct NodeRxPkg {
	NodeType nodeType;
	uint32_t packetNum;
	uint32_t bbbState;
	uint16_t overrideCode;
	uint16_t overrideValue;
};

struct BrakePkg {
	uint32_t packetNum;
	uint16_t bbbState;
	uint16_t nodeState;
	uint32_t valueValidFlags;
	uint16_t adcValues[16];
	uint32_t solenoidValues[8];
	uint32_t dacValues[2];
	uint32_t gpioValues[4];
	uint32_t spare;
};

struct RearNodeTxPkg {
	uint32_t packetNum;
	uint16_t adcValues[16];
	uint32_t dacValues[2];
	uint32_t errCode;
};

#endif
