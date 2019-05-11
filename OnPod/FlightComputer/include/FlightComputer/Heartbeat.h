#include "FlightComputer/Common.h"

class Heartbeat{
public:
	Heartbeat(int iTimeOutMs);
	void feed();
	bool expired();

private:
	std::chrono::steady_clock::time_point aLastStart;
	int iTimeOutms;
};
