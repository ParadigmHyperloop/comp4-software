#include "States.h"
#include "easylogging++.h"
#include "Constants/SensorConfig.h"

#define MIN_BRAKING_TIME 1
#define BRAKING_DISTANCE 250

PodState::PodState() = default;

PodState::PodState(TelemetryManager* pod){
    this->pod = pod;
    this->_enterStateTime = std::chrono::steady_clock::now();
}

void PodState::setFailure(const std::string &failure){
    if(this->currentFailure != failure){
        this->currentFailure = failure;
        pod->sendUpdate(this->currentFailure);
    }
}

PodState::~PodState(){
    this->pod->telemetry->controlsInterfaceState = ciNone;
}

unsigned int PodState::timeInStateSeconds() {
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count()/1000.0;
}

int8_t PodState::checkFlags(std::vector<int8_t > &flags){
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 0){
            return i;
        }
    }
    return FLAGS_GOOD;
}

int32_t PodState::checkNodeStates(){
    if(!this->pod->telemetry->checkNodeStates){
        return FLAGS_GOOD;
    }
    if(this->pod->telemetry->commandedBrakeNodeState != this->pod->telemetry->receivedBrakeNodeState){
        return BRAKE_NODE_HEARTBEAT_INDEX;
    }
    return FLAGS_GOOD;
}

void PodState::commonChecks() {
    int32_t status;
    status = this->checkFlags(this->pod->telemetry->nodeSensorFlags);
    if( status != FLAGS_GOOD ){
        std::string error = "Failed on Node sensor : " + std::to_string(status);
        throw std::runtime_error(error);
    }
    status = this->checkFlags(this->pod->telemetry->connectionFlags);
    if(status != FLAGS_GOOD){
        std::string error = "Failed on communication flag : " + std::to_string(status);
        throw std::runtime_error(error);
    }
    status = this->checkNodeStates();
    if( status != FLAGS_GOOD && (this->timeInStateSeconds() > 3)){
        std::string error = "Failed on node state agreement : " + std::to_string(status);
        throw std::runtime_error(error);
    }
}

void PodState::armedChecks(){
    if(!this->pod->telemetry->inverterHeartbeat){
        std::string error = "Inverter Heartbeat Expired.";
        throw std::runtime_error(error);
    }
    //todo check array for inverter values
}

void PodState::setupTransition(PodStates nextState, const std::string& reason){
    this->_transitioning = true;
    this->_transitionReason = reason;
    this->_nextStateIdentifier = nextState;
}

bool PodState::testTransitions(){
    return true;
}

PodStates PodState::getStateValue(){
    return this->_stateIdentifier;
}

PodStates PodState::getNewState(){
    return this->_nextStateIdentifier;
}

bool PodState::isTransitioning(){
    return this->_transitioning;
}

const std::string PodState::getTransitionReason(){
    return _transitionReason;
}

std::unique_ptr<PodState> PodState::createState(PodStates newState, TelemetryManager* telemetry) {
    switch (newState) {
        case psBooting:
            return std::unique_ptr<PodState>(new Booting(telemetry));
        case psStandby:
            return std::unique_ptr<PodState>(new Standby(telemetry));
        case psArming:
            return std::unique_ptr<PodState>(new Arming(telemetry));
        case psArmed:
            return std::unique_ptr<PodState>(new Armed(telemetry));
        case psPreFlight:
            return std::unique_ptr<PodState>(new PreFlight(telemetry));
        case psAcceleration:
            return std::unique_ptr<PodState>(new Acceleration(telemetry));
        case psBraking:
            return std::unique_ptr<PodState>(new Braking(telemetry));
        default:
            return std::unique_ptr<PodState>(new Braking(telemetry));
    }
}


 // *  ******************** BOOTING ***********************8


Booting::Booting(TelemetryManager* pod): PodState(pod) {
    _stateIdentifier = psBooting;
    this->pod->telemetry->commandedBrakeNodeState = bnsBooting;
    //_lvdcNodeState = lvdcBooting;
}

Booting::~Booting() = default;

bool Booting::testTransitions(){
    this->setupTransition(psStandby, "Booting Complete");
    return true;
}


// *  ******************** STANDBY ***********************8

Standby::Standby(TelemetryManager * pod): PodState(pod) {
    _stateIdentifier = psStandby;
    this->pod->telemetry->commandedBrakeNodeState = bnsStandby;
    //_lvdcNodeState = lvdcStandby;
    this->pod->telemetry->controlsInterfaceState = ciNone; // Guard against auto transition
    this->currentFailure = "";
}

Standby::~Standby(){
    for(auto &flag : this->pod->telemetry->nodeSensorFlags){
        if(flag != 2){
            flag = 0;
        }
    }
    for(auto &flag : this->pod->telemetry->bmsSensorFlags){
        if(flag != 2){
            flag = 0;
        }
    }
}

bool Standby::testTransitions() {
    try {
        this->commonChecks();
    }
    catch (const std::runtime_error &e ){
        setFailure(e.what());
        if(this->pod->telemetry->controlsInterfaceState == ciArm){
            this->pod->telemetry->controlsInterfaceState = ciNone;
        }
        return false;
    }
    if(this->pod->telemetry->controlsInterfaceState == ciArm){
        this->pod->telemetry->controlsInterfaceState = ciNone;
        if(this->pod->telemetry->maxFlightTime == 0){
            std::string failure = "Need flight profile to complete Arming sequence";
            setFailure(failure);
            return false;
        }
        this->setupTransition(psArming, (std::string)"Arm Command Received. Pod --> Arming");
        return true;
    }
    std::string failure = "Waiting for Arm Command";
    setFailure(failure);
    return false;
}


// *  ******************** ARMING ***********************8


Arming::Arming(TelemetryManager * pod ): PodState(pod) {
    _stateIdentifier = psArming;
    this->pod->telemetry->commandedBrakeNodeState = bnsStandby;
    //_lvdcNodeState = lvdcFlight;
}

Arming::~Arming() = default;

bool Arming::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop || this->pod->telemetry->controlsInterfaceState == ciStandby ){
        this->setupTransition(psStandby, "Emergency Stop. Pod --> Standby");
        return true;
    }
    if(this->timeInStateSeconds() < 3 ){ //Allow nodes to update sensors or timeout if not
        return false;
    }
    if(this->timeInStateSeconds() > 20 ){
        this->setupTransition(psStandby, "Arming Timout. Pod --> Standby");
        return true;
    }
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (const std::runtime_error &error ){
        this->setupTransition(psStandby, error.what());
        return true;
    }

    this->setupTransition(psArmed, (std::string)"Arming Passed. Pod --> Armed");
    return true;
}


 //*  ******************** ARMED ***********************8


Armed::Armed(TelemetryManager * pod) : PodState(pod) {
    _stateIdentifier = psArmed;
    this->pod->telemetry->commandedBrakeNodeState = bnsStandby;
    //_lvdcNodeState = lvdcFlight;
}

Armed::~Armed() {
    this->pod->telemetry->controlsInterfaceState = ciNone; // Redundant grantee
}

bool Armed::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop || this->pod->telemetry->controlsInterfaceState == ciStandby ){
        this->setupTransition(psStandby, "Emergency Stop. Pod --> Standby");
        return true;
    }
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (const std::runtime_error &error ){
        this->setupTransition(psStandby, error.what());
        return true;
    }

    // todo inverter comms
    if(this->pod->telemetry->controlsInterfaceState == ciFlight){
        this->pod->telemetry->controlsInterfaceState = ciNone; // Use up command
        this->setupTransition(psPreFlight, (std::string)"Flight Command Received. Pod --> Pre-flight");
        return true;
    }
    return false;
}



 // *  ******************** PREFLIGHT ***********************

PreFlight::PreFlight(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psPreFlight;
    this->pod->telemetry->commandedBrakeNodeState = bnsFlight;
    //_lvdcNodeState = lvdcFlight;
}

PreFlight::~PreFlight() = default;

bool PreFlight::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop){
        this->setupTransition(psBraking, "Emergency Stop. Pod --> Braking");
        return true;
    }
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (const std::runtime_error &error ){
        this->setupTransition(psStandby, error.what());
    }

    // todo inverter comms
    this->setupTransition(psAcceleration, (std::string)"Pre-flight Passed. Pod --> Acceleration");
    return true;

    return false;
}




// *  ******************** ACCELERATION ***********************8



Acceleration::Acceleration(TelemetryManager * pod) : PodState(pod) {
    _stateIdentifier = psAcceleration;
    this->pod->telemetry->commandedBrakeNodeState = bnsFlight;
    //_lvdcNodeState = lvdcFlight;
    this->pod->telemetry->commandedTorque = this->pod->telemetry->motorTorque;
    LOG(INFO)<<this->pod->telemetry->commandedTorque;
}

Acceleration::~Acceleration() {
    this->pod->telemetry->motorTorque = 0;
    this->pod->telemetry->maxFlightTime = 0;
    this->pod->telemetry->flightDistance = 0;
    this->pod->telemetry->commandedTorque = 0;
}

bool Acceleration::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop){
        this->setupTransition(psBraking, "Emergency Stop. Pod --> Braking");
        return true;
    }
    // todo critical vs non critical changes
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (const std::runtime_error &error ){
        std::string reason = "Pod --> Braking";
        this->setupTransition(psBraking, error.what() + reason);
        return true;
    }

    // Navigation checks todo
    std::unique_lock<std::mutex> lock(pod->telemetry->positionLock);
    float remainingTrack = pod->telemetry->flightDistance - (pod->telemetry->podPosition);// - BRAKING_DISTANCE;
    lock.unlock();

    if(remainingTrack <= 0){
        this->setupTransition(psBraking,"Braking Distance Reached. Pod --> Braking");
        return true;
    }
    if(this->timeInStateSeconds() > this->pod->telemetry->maxFlightTime ){
        this->setupTransition(psBraking, (std::string)" Flight Timout of " + std::to_string(this->timeInStateSeconds()) + " reached. Pod --> Braking");
        return true;
    }
    return false;
}



 // *  ******************** BRAKING ***********************


Braking::Braking(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psBraking;
    this->pod->telemetry->commandedBrakeNodeState = bnsBraking;
    //_lvdcNodeState = lvdcFlight;
}

Braking::~Braking() = default;

bool Braking::testTransitions() {
    if(!(this->timeInStateSeconds() > MIN_BRAKING_TIME)){
        return false;
    }
    else if(this->pod->telemetry->controlsInterfaceState == ciStandby){
        this->pod->telemetry->controlsInterfaceState = ciNone; // Use up command
        this->setupTransition(psStandby, (std::string)"Disarm Command Received. Pod --> Standby");
        return true;
    }
    else if(this->pod->telemetry->controlsInterfaceState == ciArm){
        this->pod->telemetry->controlsInterfaceState = ciNone; // Use up command
        this->setupTransition(psArming, (std::string)"Arm Command Received. Pod --> Arming");
        return true;
    }
    return false;
}