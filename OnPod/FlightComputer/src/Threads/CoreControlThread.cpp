#include "CoreControlThread.h"
#include "States.h"

void coreControlLoopThread(TelemetryManager pod){
    el::Helpers::setThreadName("Core Control Thread");
    while(pod.getPodStateValue() != psShutdown){
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
                LOG(INFO)<<reason;
                //TODO Update All nodes?
            }
        }
    }
}




