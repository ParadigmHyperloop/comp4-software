#ifndef FLIGHTCOMPUTER_STATES_H
#define FLIGHTCOMPUTER_STATES_H

#include "TelemetryManager.h"
#include "Constants/Constants.h"

class TelemetryManager;

class PodState{

public:
    PodState();
    virtual ~PodState();

    PodState(TelemetryManager*);

    PodStates getStateValue();

    virtual bool testTransitions();

    bool isNodeSensorCritical(uint32_t);

    bool isConnectionFlagCritical(uint32_t);

    bool isInverterSensorCritical(uint32_t);

    bool brakingCriteriaMet();

    PodStates getNewState();

    bool isTransitioning();

    void setFailure(const std::string&);
    const std::string getTransitionReason();

    float timeInStateSeconds();

    float timeInFlightSeconds();

    static std::unique_ptr<PodState> createState(PodStates, TelemetryManager* );

    void setupTransition(PodStates, const std::string&);

    int8_t checkFlags(std::vector<int32_t> &flags);

    int32_t checkNodeStates();

    void commonChecks();

    void armedChecks();

protected:
    std::string _currentFailure;
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
    float getBrakingDistance();
    bool testTransitions() override;
};

class Coasting : public PodState{
public:
    Coasting(TelemetryManager*);
    ~Coasting();
    bool testTransitions() override;
};

class Braking : public PodState {
public:
    Braking(TelemetryManager*);
    ~Braking();
    bool testTransitions() override;
};

#endif //FLIGHTCOMPUTER_STATES_H
