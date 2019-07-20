#ifndef FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
#define FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP

#include <Paradigm.pb.h>
#include "GPIOManager.hpp"
#include "GPIOConst.hpp"

#include "IStateDisplay.hpp"

class SevenSegStateDisplay {

public:
  SevenSegStateDisplay(bool);
  ~SevenSegStateDisplay();

  void DisplayState(PodStates state);
  void TestDisplay(int);
  void SetupGPIOPins();
  void WriteToGPIO(int16_t pin, bool value);
  void ClearDsiplay();

protected:
  void displayBooting();
  void displayStandby();
  void displayArming();
  void displayArmed();
  void displayPreFlight();
  void displayAcceleration();
  void displayCoasting();
  void displayBraking();
  void displayShutdown();
  void displayNone();

private:

  GPIO::GPIOManager* _gpManager = nullptr;
  bool _onTarget = false;
  const int8_t _seg1 = 69;
  const int8_t _seg2 = 44;
  const int8_t _seg3 = 45;
  const int8_t _seg4 = 47;
  const int8_t _segBottom = 26;
};


#endif //FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
