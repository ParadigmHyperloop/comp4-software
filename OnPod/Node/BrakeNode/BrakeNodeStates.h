#include <iostream>

enum State {
    BOOT,
    STANDBY,
    ARMING,
    ARMED,
    FLIGHT,
    BRAKING,
    VENTING,
    RETRIEVAL,
    ERROR
}

class BrakeNodeState {
    
    public:
    /*
     *Checks if the state can be transitioned to Standby
     * if yes sets state to Standby.
    */
    void CanTransitionToStandby();
    /*
     *Checks if the state can be transitioned to Arming
     * if yes sets state to Arming.
    */
    void CanTransitionToArming();
    /*
     *Checks if the state can be transitioned to Armed
     * if yes sets state to Armed.
    */
    void CanTransitionToArmed();
    /*
     *Checks if the state can be transitioned to Flight
     * if yes sets state to Flight.
    */
    void CanTransitionToFlgiht();
    /*
     *Checks if the state can be transitioned to Braking
     * if yes sets state to Braking.
    */
    void CanTransitionToBraking();
    /*
     *Checks if the state can be transitioned to Venting
     * if yes sets state to Venting.
    */
    void CanTransitionToVenting();
    /*
     *Checks if the state can be transitioned to Retrieval
     * if yes sets state to Retrieval.
    */
    void CanTransitionToRetrieval();
    /*
     *Checks if the state can be transitioned to Error
     * if yes sets state to Error.
    */
    void CanTransitionToError();
    /*
     *Checks what the current state is. Calls the method
     *corresponding to the current state.
    */
    void TransitionToNextState();

    private:
    State state_;
    
};

