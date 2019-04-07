#pragma once


enum flightComputerStates{
    FCBOOT,
    FCSTANDBY,
    FCARMING,
    FCARMED,
    FCPREFLIGHT,
    FCACCELERATION,
    FCCOASTING,
    FCBRAKING,
    FCVENTING,
    FCDISARM

};

struct flagStruct {
    bool setupFail = true;
	  bool nodeConnectFC = true;
	  bool heartbeatValid = true;
    bool watchDogTimeout = true;
	  bool estop = true;
	  bool sensorsValid = true;
	  bool taxiCommand = false;
	  bool moveToRetrieval = true;
    bool ventAttemptsDone = true;
    bool brakePressureAmbient = true;
	  flightComputerStates FCstate_ = FCVENTING;
};


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
};



class BrakeNodeState {
    
    public:
	flagStruct flagStruct_;
    
    //Parameterized constructor
    BrakeNodeState (State s);
    /*
     *Checks if the state can be transitioned from Boot to the next state.
     * if yes sets state to the next state.
    */
    void CheckBoot();
    /*
     *Checks if the state can be transitioned from Standby to the next state.
     * if yes sets state to the next state.
    */
    void CheckStandby();
    /*
     *Checks if the state can be transitioned from Arming to the next state.
     * if yes sets state to the next state.
    */
    void CheckArming();
    /*
     *Checks if the state can be transitioned from Armed to the next state.
     * if yes sets state to the next state.
    */
    void CheckArmed();
    /*
     *Checks if the state can be transitioned from Flight to the next state.
     * if yes sets state to the next state.
    */
    void CheckFlight();
    /*
     *Checks if the state can be transitioned from Braking to the next state.
     * if yes sets state to the next state.
    */
    void CheckBraking();
    /*
     *Checks if the state can be transitioned from Venting to the next state.
     * if yes sets state to the next state.
    */
    void CheckVenting();
    /*
     *Checks if the state can be transitioned from Retrieval to the next state.
     * if yes sets state to the next state.
    */
    void CheckRetrieval();
    /*
     *Checks if the state can be transitioned from Error to the next state.
     * if yes sets state to the next state.
    */
    void CheckError();
    /*
     *Checks what the current state is. Calls the method
     *corresponding to the current state.
    */
    void TransitionToNextState();

    //void(* resetFunc) (void) = 0;
    
    //Returns value stored in state_
    State GetState();
    
    private:
    State state_;
};
