#include "inputs.hpp"
#include "globals.hpp"

JoystickInput* _joystick;
InputArbiter* _inputArbiter;
ButtonInput* _button1;
ButtonInput* _button2;

bool testFunc(void* eventDetails) {
    JoystickInput::JoystickEvent evt = *((JoystickInput::JoystickEvent*) eventDetails);

    printfdbg("%f %f\n", evt.x, evt.y);
    return false;
}

void registerInputs() {
    _inputArbiter = new InputArbiter();
    _joystick = new JoystickInput(p19, p20, _inputArbiter, JOYSTICK_INVERT_X);
    _button1 = new ButtonInput(p21, _inputArbiter);
    _button2 = new ButtonInput(p22, _inputArbiter);
    //_joystick->attach(testFunc, nullptr);
}