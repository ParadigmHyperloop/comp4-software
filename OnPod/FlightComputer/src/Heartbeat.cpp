#include "FlightComputer/Heartbeat.h"

Heartbeat::Heartbeat(int timeoutMilis)
{
	this->_timeOutMilis = timeoutMilis;
}

void Heartbeat::feed()
{
	this->_lastStartTime = std::chrono::steady_clock::now();
}

bool Heartbeat::expired()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_lastStartTime).count() > this->_timeOutMilis;
}

