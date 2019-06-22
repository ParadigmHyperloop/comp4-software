#include "States.h"
#include "easylogging++.h"

#define MIN_BRAKING_TIME 1

PodState::PodState() = default;

PodState::PodState(TelemetryManager* pod){
    this->pod = pod;
    this->_enterStateTime = std::chrono::steady_clock::now();
}

PodState::~PodState(){
    this->pod->telemetry->controlsInterfaceState = ciNone;
}

unsigned int PodState::timeInStateMilis() {
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count()/1000.0;
}

int32_t PodState::checkSensorFlags(){
    std::vector<int32_t> flags = this->pod->telemetry->sensorFlags;
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 0){
            return i;
        }
    }
    return FLAGS_GOOD;
}

int32_t PodState::checkCommunicationFlags(){
    std::vector<int32_t> flags = this->pod->telemetry->connectionFlags;
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 0){
            return i;
        }
    }
    return FLAGS_GOOD;
}

int32_t PodState::checkNodeStates(){
    if(this->_brakeNodeState != this->pod->telemetry->brakeNodeState){
        return BRAKE_NODE_INDEX;
    }
    if(this->_lvdcNodeState != this->pod->telemetry->lvdcNodeState){
        return LVDC_NODE_INDEX;
    }
    return FLAGS_GOOD;
}

void PodState::commonChecks() {
    int32_t status;
    status = this->checkSensorFlags();
    if( status != FLAGS_GOOD ){
        std::string error = "Failed on sensor : " + std::to_string(status) + " returning to standby.";
        throw std::runtime_error(error);
    }
    status = this->checkCommunicationFlags();
    if(status != FLAGS_GOOD){
        std::string error = "Failed on communication flag : " + std::to_string(status) + " returning to standby.";
        throw std::runtime_error(error);
    }
/*    status = this->checkNodeStates();
    if( status != FLAGS_GOOD && (this->timeInStateMilis() > 500)){
        std::string error = "Failed on node state agreement : " + std::to_string(status) + " returning to standby.";
        throw std::runtime_error(error);
    }*/
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

LvdcNodeStates PodState::getLvdcNodeState(){
    return _lvdcNodeState;
}

BrakeNodeStates PodState::getBrakeNodeState(){
    return _brakeNodeState;
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

/*
 *  ******************** BOOTING ***********************8
 */

Booting::Booting(TelemetryManager* pod): PodState(pod) {
    _stateIdentifier = psBooting;
    _brakeNodeState = bnsBooting;
    _lvdcNodeState = lvdcBooting;
}

Booting::~Booting() = default;

bool Booting::testTransitions(){
    this->setupTransition(psStandby, "Booting Complete");
    return true;
}

/*
 *  ******************** STANDBY ***********************8
 */
//TelemetryManager*

Standby::Standby(TelemetryManager * pod): PodState(pod) {
    _stateIdentifier = psStandby;
    _brakeNodeState = bnsStandby;
    _lvdcNodeState = lvdcStandby;
    this->pod->telemetry->controlsInterfaceState = ciNone; // Guard against auto transition
}

Standby::~Standby() = default; // todo set all sensors to a false value here

bool Standby::testTransitions() {
    try {
        this->commonChecks();
    }
    catch (const std::runtime_error &e ){
        //failing on e.what()
    }
    if(this->pod->telemetry->controlsInterfaceState == ciArm){
        if(this->pod->telemetry->maxFlightTime == 0){
            this->setupTransition(psStandby, (std::string)"Need flight profile to complete Arming sequence");
            return true;
        }
        this->setupTransition(psArming, (std::string)"Arm Command Received. Pod --> Arming");
        return true;
    }
    return false;
}

/*
 *  ******************** ARMING ***********************8
 */

Arming::Arming(TelemetryManager * pod ): PodState(pod) {
    _stateIdentifier = psArming;
    _brakeNodeState = bnsStandby;
    _lvdcNodeState = lvdcFlight;
}

Arming::~Arming() = default;

bool Arming::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop){
        this->setupTransition(psStandby, "Emergency Stop. Pod --> Standby");
        return true;
    }
    if(this->timeInStateMilis() < 3000 ){ //Allow nodes to update sensors or timeout if not
        return false;
    }
    if(this->timeInStateMilis() > 30000 ){
        this->setupTransition(psStandby, "Arming Timout. Pod --> Standby");
        return true;
    }
    try {
        this->commonChecks();
    }
    catch (const std::runtime_error &error ){
        this->setupTransition(psStandby, error.what());
        return true;
    }
    if(this->pod->telemetry->inverterHeartbeat){ // and this->checkInverterValues
        // all passed
    }
    //todo validate that we are receiving telemetry from the inverter
    if(true){
        this->setupTransition(psArmed, (std::string)"Arming Passed. Pod --> Armed");
        return true;
    }
    if(this->timeInStateMilis() > 30000 ){
        this->setupTransition(psStandby, "Arming Timout. Pod --> Standby");
        return true;
    }
    return false;
}

/*
 *  ******************** ARMED ***********************8
 */

Armed::Armed(TelemetryManager * pod) : PodState(pod) {
    _stateIdentifier = psArmed;
    _brakeNodeState = bnsStandby;
    _lvdcNodeState = lvdcFlight;
}

Armed::~Armed() {
    this->pod->telemetry->controlsInterfaceState = ciNone; // Redundant grantee
}

bool Armed::testTransitions() {
    if(this->pod->telemetry->controlsInterfaceState == ciEmergencyStop){
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


/*
 *  ******************** PREFLIGHT ***********************8
 */
PreFlight::PreFlight(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psPreFlight;
    _brakeNodeState = bnsFlight;
    _lvdcNodeState = lvdcFlight;
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



/*
 *  ******************** ACCELERATION ***********************8
 */


Acceleration::Acceleration(TelemetryManager * pod) : PodState(pod) {
    _stateIdentifier = psAcceleration;
    _brakeNodeState = bnsFlight;
    _lvdcNodeState = lvdcFlight;
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
        this->setupTransition(psBraking, error.what());
        return true;
    }
    // Navigation checks
    // todo inverter comms and bms
    if(this->timeInStateMilis() > this->pod->telemetry->maxFlightTime ){
        this->setupTransition(psBraking, (std::string)" Flight Timout of " + std::to_string(this->timeInStateMilis()) + " reached. Pod --> Braking");
        return true;
    }
    return false;
}


/*
 *  ******************** BRAKING ***********************8
 */

Braking::Braking(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psBraking;
    _brakeNodeState = bnsBraking;
    _lvdcNodeState = lvdcFlight;
}

Braking::~Braking() = default;

bool Braking::testTransitions() {
    if(!(this->timeInStateMilis() > MIN_BRAKING_TIME)){
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