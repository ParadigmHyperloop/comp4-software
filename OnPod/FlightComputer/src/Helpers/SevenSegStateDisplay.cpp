#include "SevenSegStateDisplay.hpp"


SevenSegStateDisplay::SevenSegStateDisplay(bool onTarget) {

  this -> _onTarget= onTarget;
  if (_onTarget)
  {
    this-> _gpManager = GPIO::GPIOManager::getInstance();
    this->SetupGPIOPins();
  }
}

SevenSegStateDisplay::~SevenSegStateDisplay() {
  // Do not delete Telemetry Manager
  delete this->_gpManager;
}


void SevenSegStateDisplay::TestDisplay (int state)
{
  assert(state <= 0 && state >= 9);
  switch (state)
  {
    case 0:
      displayBooting();
      break;
    case 1:
      displayStandby();
      break;
    case 2:
      displayArming();
      break;
    case 3:
      displayArmed();
      break;
    case 4:
      displayPreFlight();
      break;
    case 5:
      displayAcceleration();
      break;
    case 6:
      displayCoasting();
      break;
    case 7:
      displayBraking();
      break;
    case 8:
      displayShutdown();
      break;
    case 9:
      displayNone();
      break;
    default:
      break;
  }
}

void SevenSegStateDisplay::SetupGPIOPins()
{
  _gpManager -> setDirection(this->_seg1, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg2, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg3, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_seg4, GPIO::OUTPUT);
  _gpManager -> setDirection(this->_segBottom, GPIO::OUTPUT);
}

void SevenSegStateDisplay::DisplayState(PodStates state) {
  if (_onTarget)
  {
    switch (state)
    {
      case psBooting:
        displayBooting();
        break;
      case psStandby:
        displayStandby();
        break;
      case psArming:
        displayArming();
        break;
      case psArmed:
        displayArmed();
        break;
      case psPreFlight:
        displayPreFlight();
        break;
      case psAcceleration:
        displayAcceleration();
        break;
      case psCoasting:
        displayCoasting();
        break;
      case psBraking:
        displayBraking();
        break;
      case psShutdown:
        displayShutdown();
        break;
      case psNone:
        displayNone();
        break;
      default:
        break;
    }
  }

  auto logMsg = "Current State is : " + std::to_string(state);
  LOG(INFO) << logMsg;
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