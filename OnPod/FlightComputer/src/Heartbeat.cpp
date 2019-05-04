#include "Heartbeat.h"
#include "EasyLogger/easylogging++.h"

Heartbeat::Heartbeat(int iTimeOut)
{
	this->iTimeOutms = iTimeOut;
}

void Heartbeat::feed()
{
	this->aLastStart = std::chrono::steady_clock::now();
}

bool Heartbeat::expired()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(current - this->aLastStart).count() > this->iTimeOutms)
	{
		return 1;
	}
	return 0;
}

