#ifndef FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
#define FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP


#include "IStateDisplay.hpp"

class SevenSegStateDisplay : public IStateDisplay{

public:
  SevenSegStateDisplay();
  ~SevenSegStateDisplay();

  void DisplayState();
};


#endif //FLIGHTCOMPUTER_SEVENSEGSTATEDISPLAY_HPP
