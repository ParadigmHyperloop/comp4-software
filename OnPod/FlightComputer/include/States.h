#ifndef FLIGHTCOMPUTER_STATES_H
#define FLIGHTCOMPUTER_STATES_H

#include "TelemetryManager.h"

#define FLAGS_GOOD -1

class TelemetryManager;

class PodState{

public:
    PodState();
    virtual ~PodState();

    PodState(TelemetryManager*);

    PodStates getStateValue();

    virtual bool testTransitions();

    PodStates getNewState();

    bool isTransitioning();

    BrakeNodeStates getBrakeNodeState();

    LvdcNodeStates getLvdcNodeState();

    const std::string getTransitionReason();

    unsigned int timeInStateSeconds();

    static std::unique_ptr<PodState> createState(PodStates, TelemetryManager* );

    void setupTransition(PodStates, const std::string&);

    int32_t checkSensorFlags();

    int32_t checkCommunicationFlags();

    int32_t checkNodeStates();

    void commonChecks();

    void armedChecks();


protected:
    std::chrono::steady_clock::time_point _enterStateTime;
    bool _transitioning = false;
    std::string _transitionReason = "";
    PodStates _stateIdentifier = psBooting;
    PodStates _nextStateIdentifier = psBooting;
    TelemetryManager* pod;
};

class Booting: public PodState {
public:
    Booting(TelemetryManager*);
    ~Booting();
    bool testTransitions() override;
};

class Standby: public PodState {
public:
    Standby(TelemetryManager*);
    ~Standby();
    bool testTransitions() override;
};

class Arming : public PodState {
public:
    Arming(TelemetryManager*);
    ~Arming();
    bool testTransitions() override;
};

class Armed : public PodState {
public:
    Armed(TelemetryManager*);
    ~Armed();
    bool testTransitions() override;
};

class PreFlight : public PodState {
public:
    PreFlight(TelemetryManager*);
    ~PreFlight() override ;
    bool testTransitions() override;
};

class Acceleration : public PodState {
public:
    Acceleration(TelemetryManager*);
    ~Acceleration();
    bool testTransitions() override;
};

class Braking : public PodState {
public:
    Braking(TelemetryManager*);
    ~Braking();
    bool testTransitions() override;
};

#endif //FLIGHTCOMPUTER_STATES_H
