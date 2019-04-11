#include "FlightComputer/Heartbeat.h"

Heartbeat::Heartbeat(int iTimeOutMs)
{
	this->iTimeOutms = iTimeOutms;
}

void Heartbeat::feed()
{
	this->aLastStart = std::chrono::steady_clock::now();
}

bool Heartbeat::expired()
{
	auto current = std::chrono::steady_clock::now();
	if(std::chrono::duration_cast<std::chrono::milliseconds>(current - this->aLastStart).count() > this->iTimeOutms)
	{
		return 1;
	}
	return 0;
}

