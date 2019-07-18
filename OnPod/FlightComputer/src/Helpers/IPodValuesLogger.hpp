#ifndef FLIGHTCOMPUTER_IPODVALUESLOGGER_HPP
#define FLIGHTCOMPUTER_IPODVALUESLOGGER_HPP

class IPodValuesLogger
{
public:
  virtual ~IPodValuesLogger() = default;
  virtual void LogPodValues() = 0;
};
#endif //FLIGHTCOMPUTER_IPODVALUESLOGGER_HPP