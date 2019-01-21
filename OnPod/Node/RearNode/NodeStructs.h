#ifndef NODESTRUCTS
#define NODESTRUCTS

char ENDOFHEADER[] = "\r\n\r\n";

enum NodeType {
	BRAKE = 1,
	LVDC = 2,
	REAR = 3
};

enum BBBTxIntervalMS {
	MIN = 100,
	MAX = 4
};

struct BrakePkg {
	unsigned short bbbState;
	unsigned short nodeState;
	unsigned valueValidFlags;
	int adcValues[16];
	int solenoidValues[8];
	int dacValues[2];
	int gpioValues[4];
	int spare;
};

struct RearPkg {
	unsigned bbbState;
	unsigned valueValidFlags;
	int adcValues[16];
	int solenoidValues[8];
	int dacValues[2];
	int gpioValues[4];
};

#endif
