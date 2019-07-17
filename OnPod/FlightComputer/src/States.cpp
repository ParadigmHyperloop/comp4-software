#include "States.h"
#include "easylogging++.h"
#include "Constants/SensorConfig.h"

#define MIN_BRAKING_TIME 20
#define BRAKING_DISTANCE 250

class CriticalErrorException : public std::runtime_error{
public:
    explicit CriticalErrorException(std::string error): std::runtime_error(error){
        this->error = std::move(error);
    };

    const char * what () const throw ()
    {
        return error.c_str();
    }

protected:
    std::string error;
};

PodState::PodState() = default;

PodState::PodState(TelemetryManager* pod){
    this->pod = pod;
    this->_enterStateTime = std::chrono::steady_clock::now();
}

void PodState::setFailure(const std::string &failure){
    if(this->_currentFailure != failure){
        this->_currentFailure = failure;
        pod->sendUpdate(this->_currentFailure);
    }
}

PodState::~PodState(){
    this->pod->telemetry->controlsInterfaceState = ciNone;
}

float PodState::timeInStateSeconds() {
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count()/1000.0;
}

float PodState::timeInFlightSeconds() {
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_flightStartTime).count()/1000.0;
}

bool PodState::isNodeSensorCritical(int sensorIndex) {
    std::vector<int> criticalSensors = {NODE_FLAGS::HP_INDEX, NODE_FLAGS::LP1_INDEX, NODE_FLAGS::LP2_INDEX, NODE_FLAGS::LP3_INDEX, NODE_FLAGS::LP4_INDEX,
                                        NODE_FLAGS::ENCLOSURE_TEMPERATURE_INDEX, NODE_FLAGS::ENCLOSURE_PRESSURE_INDEX};
    return std::find(criticalSensors.begin(), criticalSensors.end(), sensorIndex) != criticalSensors.end();
}

bool PodState::isConnectionFlagCritical(int sensorIndex) {
    std::vector<int> criticalSensors = {};
    return std::find(criticalSensors.begin(), criticalSensors.end(), sensorIndex) != criticalSensors.end();
}

bool PodState::isInverterSensorCritical(int sensorIndex) {
    std::vector<int> criticalSensors = {};
    return std::find(criticalSensors.begin(), criticalSensors.end(), sensorIndex) != criticalSensors.end();
}

int8_t PodState::checkFlags(std::vector<int8_t > &flags){
    for(std::size_t i=0; i<flags.size(); ++i){
        if(flags[i] == 2){
            continue;
        }
        if(flags[i] != 1){
            return i;
        }
    }
    return GENERAL_CONSTANTS::FLAGS_GOOD;
}

int32_t PodState::checkNodeStates(){
    if(!this->pod->telemetry->checkNodeStates){
        return GENERAL_CONSTANTS::FLAGS_GOOD;
    }
    if(this->pod->telemetry->commandedBrakeNodeState != this->pod->telemetry->receivedBrakeNodeState){
        return CONNECTION_FLAGS::BRAKE_NODE_HEARTBEAT_INDEX;
    }
    return GENERAL_CONSTANTS::FLAGS_GOOD;
}

void PodState::commonChecks() {
    int32_t status;
    status = this->checkFlags(this->pod->telemetry->nodeSensorFlags);
    if( status != GENERAL_CONSTANTS::FLAGS_GOOD ){
        if(isNodeSensorCritical(status)){
            std::string error = "Failed on Critical Node sensor : " + std::to_string(status);
            throw CriticalErrorException(error);
        }
        std::string error = "Failed on Node sensor : " + std::to_string(status);
        throw std::runtime_error(error);
    }
    status = this->checkFlags(this->pod->telemetry->connectionFlags);
    if(status != GENERAL_CONSTANTS::FLAGS_GOOD){
        if(isConnectionFlagCritical(status)){
            std::string error = "Failed on critical communication flag : " + std::to_string(status);
            throw CriticalErrorException(error);
        }
        std::string error = "Failed on communication flag : " + std::to_string(status);
        throw std::runtime_error(error);
    }
    status = this->checkNodeStates();
    if(status != GENERAL_CONSTANTS::FLAGS_GOOD){
        std::string error = "Failed on Node state agreement : " + std::to_string(status);
        throw CriticalErrorException(error);
    }
}

void PodState::armedChecks(){
    if(!this->pod->telemetry->inverterHeartbeat){
        std::string error = "Inverter Heartbeat Expired.";
        throw CriticalErrorException(error);
    }

    int status = checkFlags(pod->telemetry->inverterSensorFlags);
    if(status != GENERAL_CONSTANTS::FLAGS_GOOD){
        std::string error = "Failed on inverter sensor flag : " + std::to_string(status);
        throw std::runtime_error(error);
    }

    status = checkFlags(pod->telemetry->inverterFaults);
    if(status != GENERAL_CONSTANTS::FLAGS_GOOD){
        std::string error = "Failed on inverter fault, see pod messages";
        throw std::runtime_error(error);
    }

}

bool PodState::brakingCriteriaMet() {
    float position = pod->getPodDistance();
    float remainingTrack = pod->telemetry->flightDistance - position - pod->telemetry->brakeDistance;
    if(remainingTrack <= 0){
        pod->sendUpdate("Braking at : " + std::to_string(pod->telemetry->podPosition));
        return true;
    }
    
    if(pod->telemetry->totalStripCount >= pod->telemetry->maxStripCount){
        pod->sendUpdate("Braking at maximum strip count");
        return true;
    }
    return false;
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
        case psCoasting:
            return std::unique_ptr<PodState>(new Coasting(telemetry));
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
    this->pod->telemetry->commandedLvdcNodeState = lvdcStandby;
    this->pod->telemetry->controlsInterfaceState = ciNone; // Guard against auto transition
    this->_currentFailure = "";
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
    if(this->pod->getControlsInterfaceState() == ciStandby){
        pod->telemetry->motorDistance = 0;
        pod->telemetry->podPosition = 0;
        pod->telemetry->totalStripCount = 0;
    }
    try {
        this->commonChecks();
    }
    catch (const std::runtime_error &e ){
        setFailure(e.what());
        if(this->pod->getControlsInterfaceState() == ciArm){
            this->pod->telemetry->controlsInterfaceState = ciNone;
        }
        return false;
    }
    if(this->pod->getControlsInterfaceState() == ciArm){
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
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
}

Arming::~Arming() = default;

bool Arming::testTransitions() {
    if(this->pod->getControlsInterfaceState() == ciEmergencyStop || this->pod->getControlsInterfaceState() == ciStandby ){
        this->setupTransition(psStandby, "Emergency Stop. Pod --> Standby");
        return true;
    }
    if(this->timeInStateSeconds() < 3 ){ //Allow nodes to update sensors or timeout if not
        return false;
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
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
}

Armed::~Armed() {
    this->pod->telemetry->controlsInterfaceState = ciNone; // Redundant grantee
}

bool Armed::testTransitions() {
    if(this->pod->getControlsInterfaceState() == ciEmergencyStop || this->pod->getControlsInterfaceState() == ciStandby ){
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

    if(this->pod->getControlsInterfaceState() == ciFlight){
        this->setupTransition(psPreFlight, (std::string)"Flight Command Received. Pod --> Pre-flight");
        return true;
    }
    return false;
}



 // *  ******************** PREFLIGHT ***********************

PreFlight::PreFlight(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psPreFlight;
    this->pod->telemetry->commandedBrakeNodeState = bnsFlight;
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
}

PreFlight::~PreFlight() = default;

bool PreFlight::testTransitions() {
    if(this->pod->getControlsInterfaceState() == ciEmergencyStop){
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
    this->setupTransition(psAcceleration, (std::string)"Pre-flight Passed. Pod --> Acceleration");
    return true;
}




// *  ******************** ACCELERATION ***********************8



Acceleration::Acceleration(TelemetryManager * pod) : PodState(pod) {
    _stateIdentifier = psAcceleration;
    this->_flightStartTime = std::chrono::steady_clock::now();
    this->pod->telemetry->commandedBrakeNodeState = bnsFlight;
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
    this->pod->telemetry->commandedTorque = this->pod->telemetry->motorTorque;
}

Acceleration::~Acceleration() {
    this->pod->telemetry->motorTorque = 0;
    this->pod->telemetry->maxFlightTime = 0;
    this->pod->telemetry->flightDistance = 0;
    this->pod->telemetry->commandedTorque = 0;
    this->pod->telemetry->brakeDistance = 0;
    this->pod->telemetry->maxVelocity = 0;
    this->pod->telemetry->startTorque = 0;
    this->pod->telemetry->accelerationTime = 0;
    this->pod->telemetry->expectedTubePressure = 0;
}

bool Acceleration::testTransitions() {
    if(this->pod->getControlsInterfaceState() == ciEmergencyStop){
        this->setupTransition(psBraking, "Emergency Stop. Pod --> Braking");
        return true;
    }
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (CriticalErrorException &error){
        std::string reason = "Pod --> Braking";
        this->setupTransition(psBraking, error.what() + reason);
        return true;
    }
    catch (const std::runtime_error &error ){
        std::string reason = "Pod --> Coasting";
        this->setupTransition(psCoasting, error.what() + reason);
        return true;
    }

    if(brakingCriteriaMet()){
        this->setupTransition(psBraking,"Braking Distance Reached. Pod --> Braking");
        return true;
    }

    if(this->timeInFlightSeconds() > this->pod->telemetry->maxFlightTime ){
        this->setupTransition(psBraking, (std::string)" Flight Timout of " + std::to_string(this->timeInStateSeconds()) + " reached. Pod --> Braking");
        return true;
    }
    return false;
}

// *  ******************** COASTING ***********************
Coasting::Coasting(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psCoasting;
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
    this->pod->telemetry->commandedTorque = 0;
}

bool Coasting::testTransitions() {
    if(this->pod->getControlsInterfaceState() == ciEmergencyStop){
        this->setupTransition(psBraking, "Emergency Stop. Pod --> Braking");
        return true;
    }
    try {
        this->commonChecks();
        this->armedChecks();
    }
    catch (CriticalErrorException &error){
        std::string reason = "Pod --> Braking";
        this->setupTransition(psBraking, error.what() + reason);
        return true;
    }
    catch (const std::runtime_error &error ){
        // Ignore Non critical fault :  Already Coasting
    }

    if(brakingCriteriaMet()){
        this->setupTransition(psBraking,"Braking Distance Reached. Pod --> Braking");
        return true;
    }

    if(this->timeInFlightSeconds() > this->pod->telemetry->maxFlightTime ){
        this->setupTransition(psBraking, (std::string)" Flight Timout of " + std::to_string(this->timeInStateSeconds()) + " reached. Pod --> Braking");
        return true;
    }
    return false;
}

Coasting::~Coasting() = default;

 // *  ******************** BRAKING ***********************

Braking::Braking(TelemetryManager* pod) : PodState(pod) {
    _stateIdentifier = psBraking;
    this->pod->telemetry->commandedBrakeNodeState = bnsBraking;
    this->pod->telemetry->commandedLvdcNodeState = lvdcFlight;
}

Braking::~Braking() = default;

bool Braking::testTransitions() {
    if(this->timeInStateSeconds() < MIN_BRAKING_TIME){
        return false;
    }
    else if(this->pod->getControlsInterfaceState() == ciStandby){
        this->setupTransition(psStandby, (std::string)"Disarm Command Received. Pod --> Standby");
        return true;
    }
    else if(this->pod->getControlsInterfaceState() == ciArm){
        this->setupTransition(psArming, (std::string)"Arm Command Received. Pod --> Arming");
        return true;
    }
    return false;
}

