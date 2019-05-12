#include "FlightComputer/Common.h"

#ifndef FLIGHTCOMPUTER_HEATBEAT_H
#define FLIGHTCOMPUTER_HEATBEAT_H



class Heartbeat{
public:
	explicit Heartbeat(int32_t timeoutMilis);
	void feed();
	bool expired();

private:
	std::chrono::steady_clock::time_point _lastStartTime;
	int _timeOutMilis;
};


#endif //FLIGHTCOMPUTER_HEATBEAT_H