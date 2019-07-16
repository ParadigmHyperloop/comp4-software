#include <Heartbeat.h>
#include "CoreControlThread.h"
#include "States.h"

void coreControlLoopThread(TelemetryManager pod){

  TIMED_FUNC(timeObj);
  std::string threadLable("Core Control Thread");
    el::Helpers::setThreadName("Core Control Thread");
    Heartbeat loggerBeat = Heartbeat(700);
    while(pod.getPodStateValue() != psShutdown){

      TIMED_FUNC_IF(timeBlkObj, loggerBeat.expired());
      if (loggerBeat.expired())
        loggerBeat.feed();

      while(pod.telemetry->automaticTransitions && pod.getPodStateValue() != psShutdown){
        pod.telemetry->podState->testTransitions();
        if (pod.telemetry->podState->isTransitioning()) {
            const std::string reason = pod.telemetry->podState->getTransitionReason();
            PodStates newState = pod.telemetry->podState->getNewState();
            pod.setPodState(newState, reason);
            pod.sendUpdate(reason);
        }
      }
      while(!pod.telemetry->automaticTransitions && pod.getPodStateValue() != psShutdown){
        if(pod.getPodStateValue() != pod.telemetry->manualPodState){
            const std::string reason = "Manual State Change";
            PodStates newState = pod.telemetry->manualPodState;
            pod.setPodState(newState, reason);
            pod.sendUpdate(reason);
        }
      }
      const std::string reason = "Leaving Manual Control Mode. Pod --> Standby";
      PodStates newState = psStandby;
      pod.setPodState(newState, reason);
    }
}