#include "PodValuesLogger.hpp"

PodValuesLogger::PodValuesLogger(PodValues* podValues)
: _podValues(podValues)
{

}

void PodValuesLogger::LogPodValueTitlesToLog() {

  std::string titleRow = "Value1, Value2, Value3";
  LOG(DEBUG) << titleRow;
}

PodValuesLogger::~PodValuesLogger() {
  _podValues = nullptr;
}



void PodValuesLogger::LogPodValues() {

  std::lock_guard<std::mutex> lck(_podValues->stateLock);
  //EG:
//  LOG(DEBUG) << _podValues->automaticTransitions << ", " << _podValues->commandedBrakeNodeState;
}