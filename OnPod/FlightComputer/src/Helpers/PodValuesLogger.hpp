#ifndef FLIGHTCOMPUTER_PODVALUESLOGGER_HPP
#define FLIGHTCOMPUTER_PODVALUESLOGGER_HPP


#include "Structs.h"
#include "IPodValuesLogger.hpp"
#include "easylogging++.h"


/*
 * Pod Logger to log Telemetry
 *
 */
class PodValuesLogger : public IPodValuesLogger {

private:
  PodValues* _podValues;
  bool _firstLog;

  /*
   * Gets title page of values to that are logged in CSV formatted string.
   */
  void LogPodValueTitlesToLog();

public:
  PodValuesLogger(PodValues*);
  ~PodValuesLogger();

  /*
   * Logs General Values you want ot log.
   */
  void LogPodValues();

  // Can Add Custom Logs Here and implement

};


#endif //FLIGHTCOMPUTER_PODVALUESLOGGER_HPP
