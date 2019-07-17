#ifndef FLIGHTCOMPUTER_ISTATEDISPLAY_HPP
#define FLIGHTCOMPUTER_ISTATEDISPLAY_HPP

class IStateDisplay
{
public:
  IStateDisplay() {};
  virtual ~IStateDisplay();

  virtual void DisplayState() = 0;
};
#endif //FLIGHTCOMPUTER_ISTATEDISPLAY_HPP
