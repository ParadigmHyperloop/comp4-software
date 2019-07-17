#ifndef FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
#define FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP


#include "TelemetryManager.h"
#include "IStateDisplay.hpp"

class SevenSegStateDisplay : public IStateDisplay{

public:
  SevenSegStateDisplay(TelemetryManager*);
  ~SevenSegStateDisplay();

  void DisplayState();
  void WriteToGPIO(int16_t pin, bool value);
  void ClearDsiplay();
protected:
  PodStates getPodState();

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
  const TelemetryManager* _telemetryManager;
  const int8_t _seg1 = 69;
  const int8_t _seg2 = 44;
  const int8_t _seg3 = 45;
  const int8_t _seg4 = 47;
  const int8_t _segBottom = 26;
};


#endif //FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
