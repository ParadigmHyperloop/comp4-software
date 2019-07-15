#include "CoreControlThread.h"
#include "States.h"

void coreControlLoopThread(TelemetryManager pod){

  TIMED_FUNC(timeObj);

    el::Helpers::setThreadName("Core Control Thread");
    while(pod.getPodStateValue() != psShutdown){
      TIMED_SCOPE(timeBlkObj, "Control Loop");
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




