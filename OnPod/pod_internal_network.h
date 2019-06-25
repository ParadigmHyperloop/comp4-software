#ifndef POD_INTERNAL_NETWORK
#define POD_INTERNAL_NETWORK

// IP addresses
const uint8_t FC_IP[4] = {192, 168, 2, 13};
const uint8_t BRAKE_NODE_IP[4] = {192, 168, 2, 70};
const uint8_t LVDC_NODE_IP[4] = {192, 168, 0, 21};
const uint8_t ENCLOSURE_NODE_IP[4] = {192, 168, 0, 22};

// IP ports
const uint16_t FC_BRAKE_NODE_PORT = 59279;
const uint16_t FC_ENCLOSURE_NODE_PORT = 5551;
const uint16_t FC_LVDC_NODE_PORT = 5552;
const uint16_t BRAKE_NODE_PORT = 5555;
const uint16_t LVDC_NODE_PORT = 5555;
const uint16_t ENCLOSURE_NODE_PORT = 5555;

// send intervals
const uint16_t FC_TO_BRAKE_NODE_INTERVAL = 50;
const uint16_t FC_TO_LVDC_NODE_INTERVAL = 50;
const uint16_t BRAKE_NODE_TO_FC_INTERVAL = 100;
const uint16_t LVDC_NODE_TO_FC_INTERVAL = 50;
const uint16_t ENCLOSURE_NODE_TO_FC_INTERVAL = 50;

enum NodeType {
	RESERVED = 0,
	BRAKE = 1,
	LVDC = 2,
	ENCLOSURE = 3,
};

#endif
