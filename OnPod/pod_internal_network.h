#ifndef POD_INTERNAL_NETWORK
#define POD_INTERNAL_NETWORK

// IP addresses
const uint8_t FC_IP[4] = {192, 168, 0, 10};
const uint8_t BRAKE_NODE_IP[4] = {192, 168, 0, 20};
const uint8_t LVDC_NODE_IP[4] = {192, 168, 0, 21};
const uint8_t ENCLOSURE_ARDUINO_IP[4] = {192, 168, 0, 22};

// IP ports
const uint16_t FC_PORT = 5555;
const uint16_t BRAKE_NODE_PORT = 5555;
const uint16_t LVDC_NODE_PORT = 5555;
const uint16_t ENCLOSURE_ARDUINO_PORT = 5555;

// send intervals
const uint16_t FC_TO_BRAKE_NODE_INTERVAL = 50;
const uint16_t BRAKE_NODE_TO_FC_INTERVAL = 50;
const uint16_t LVDC_NODE_TO_FC_INTERVAL = 50;
const uint16_t ENCLOSURE_ARDUINO_TO_FC_INTERVAL = 50;

enum NodeType {
	RESERVED = 0,
	BRAKE = 1,
	LVDC = 2,
};

#endif
