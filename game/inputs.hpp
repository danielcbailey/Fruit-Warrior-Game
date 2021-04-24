

#ifndef INPUTS_INCLUDED
#define INPUTS_INCLUDED

#include "inputs/joystickInput.hpp"
#include "inputs/buttonInput.hpp"

void registerInputs();

extern JoystickInput* _joystick;
extern InputArbiter* _inputArbiter;
extern ButtonInput* _button1;
extern ButtonInput* _button2;

#endif //INPUTS_INCLUDED