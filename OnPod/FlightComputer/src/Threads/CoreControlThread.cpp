#include "CoreControlThread.h"
#include "States.h"

void coreControlLoop(Pod pod){
    // State will be initialized to booting from the struct factory

    while(pod.telemetry->podState->getStateValue() != psShutdown){
        while(pod.telemetry->automaticTransitions && pod.telemetry->podState->getStateValue() != psShutdown) {

            pod.telemetry->podState->testTransitions();
            if (pod.telemetry->podState->isTransitioning()) {
                //TODO lock pod state

                const std::string reason = pod.telemetry->podState->getTransitionReason();

                PodStates newState = pod.telemetry->podState->getNewState();

                pod.setPodState(newState, reason);


                //TODO Update All nodes
            }
        }


        while(!pod.telemetry->automaticTransitions && pod.telemetry->podState->getStateValue() != psShutdown){
            if(pod.telemetry->podState->getStateValue() != pod.telemetry->manualPodState){
                //TODO lock pod state
                const std::string reason = "Manual State Change";
                PodStates newState = pod.telemetry->manualPodState;
                pod.setPodState(newState, reason);
                //TODO Update All nodes
            }
        }

    }



}




