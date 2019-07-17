#include "SevenSegStateDisplay.hpp"

SevenSegStateDisplay::SevenSegStateDisplay(TelemetryManager* telemManager) {
  this->_telemetryManager = telemManager;

  if (1 + 1 == 2) // if on Beagle
  {
    this-> _gpManager = GPIO::GPIOManager::getInstance();
    this->SetupGPIOPins();
  }

}

SevenSegStateDisplay::~SevenSegStateDisplay() {
  // Do not delete Telemetry Manager
}


void SevenSegStateDisplay::SetupGPIOPins()
{
  _gpManager -> setDirection(this->_seg1, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg2, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg3, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg4, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_segBottom, GPIO::OUTPUT);
}

void SevenSegStateDisplay::DisplayState() {

  auto podState = this->getPodState();
  switch (podState)
  {
    case psBooting:
      displayBooting();
    case psStandby:
      displayStandby();
    case psArming:
      displayArming();
    case psArmed:
      displayArmed();
    case psPreFlight:
      displayPreFlight();
    case psAcceleration:
      displayAcceleration();
    case psCoasting:
      displayCoasting();
    case psBraking:
      displayBraking();
    case psShutdown:
      displayShutdown();
    case psNone:
      displayNone();
    default:
      break;
  }
}

void SevenSegStateDisplay::WriteToGPIO(int16_t pin, bool value) {

  auto signal = value ? GPIO::HIGH : GPIO::LOW;
  this->_gpManager->setValue(pin, signal);
}

void SevenSegStateDisplay::ClearDsiplay()
{
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
  WriteToGPIO(this->_segBottom, true);
}

PodStates SevenSegStateDisplay::getPodState() {
  std::lock_guard<std::mutex> lock(this->_telemetryManager->telemetry->stateLock);
  return this->_telemetryManager->telemetry->podState->getStateValue();
}

// 0
void SevenSegStateDisplay::displayBooting() {
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
}

// 1
void SevenSegStateDisplay::displayStandby(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, false);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
}

// 2
void SevenSegStateDisplay::displayArming(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, false);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
}

// 3
void SevenSegStateDisplay::displayArmed(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, false);
  WriteToGPIO(this->_seg2, false);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
}

// 4
void SevenSegStateDisplay::displayPreFlight(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, false);
  WriteToGPIO(this->_seg4, true);
}

// 5
void SevenSegStateDisplay::displayAcceleration(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, false);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, false);
  WriteToGPIO(this->_seg4, true);
}

// 6
void SevenSegStateDisplay::displayCoasting(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, false);
  WriteToGPIO(this->_seg3, false);
  WriteToGPIO(this->_seg4, true);
}

// 7
void SevenSegStateDisplay::displayBraking(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, false);
  WriteToGPIO(this->_seg2, false);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, true);
}

// 8
void SevenSegStateDisplay::displayShutdown(){
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, true);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, false);
}

// 9
void SevenSegStateDisplay::displayNone() {
  WriteToGPIO(this->_segBottom, false);
  WriteToGPIO(this->_seg1, false);
  WriteToGPIO(this->_seg2, true);
  WriteToGPIO(this->_seg3, true);
  WriteToGPIO(this->_seg4, false);
}