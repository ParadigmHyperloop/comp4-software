#ifndef NODESTRUCTS
#define NODESTRUCTS

enum NodeType {
	RESERVED = 0,
	BRAKE 	 = 1,
	LVDC  	 = 2,
	REAR  	 = 3
};

struct UDPParams {
	uint8_t  BBB_IP	[4]	= {192, 168, 0, 2};
	uint16_t BBB_PORT = 5555;
	uint8_t  BRAKE_NODE_IP [4] = {192, 168, 0, 3};
	uint8_t  LVDC_NODE_IP  [4] = {192, 168, 0, 4};
	uint8_t  REAR_NODE_IP  [4] = {192, 168, 0, 5};
	uint16_t NODE_RX_PORT = 5555;
};

#endif
