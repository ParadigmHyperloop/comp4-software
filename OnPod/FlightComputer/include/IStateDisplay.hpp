#ifndef FLIGHTCOMPUTER_ISTATEDISPLAY_HPP
#define FLIGHTCOMPUTER_ISTATEDISPLAY_HPP

#include "easylogging++.h"

class IStateDisplay
{

protected:
  virtual ~IStateDisplay() = default;
public:
  virtual void DisplayState() = 0;
};
#endif //FLIGHTCOMPUTER_ISTATEDISPLAY_HPP
