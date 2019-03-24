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
	uint32_t adcValues[16];
	uint32_t solenoidValues[8];
	uint32_t dacValues[2];
	uint32_t gpioValues[4];
	uint32_t spare;
};

struct RearNodeTxPkg {
	uint32_t packetNum;
	uint32_t adcValues[16];
	uint32_t dacValues[2];
};

#endif
