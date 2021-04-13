

#ifndef JOYSTICK_INPUT_INCLUDED
#define JOYSTICK_INPUT_INCLUDED

#include "inputBase.hpp"
#include "inputArbiter.hpp"
#include "mbed.h"

#define JOYSTICK_FLAGS_NONE 0
#define JOYSTICK_INVERT_X 1
#define JOYSTICK_INVERT_Y 2

class JoystickInput : public InputBase {
    private:

    AnalogIn xPin;
    AnalogIn yPin;

    void poll(float dt);

    float previousX;
    float previousY;

    bool invertX;
    bool invertY;

    public:

    /*
     * BUTTON_PRESS is issued if previousState was 0 and state is now 1
     * BUTTON_RELEASE is issued if previousState was 1 and state is now 0
     * BUTTON_SAME is issued if previousState == state
     */
    enum JoystickDirection {JOYSTICK_LEFT,
                            JOYSTICK_RIGHT,
                            JOYSTICK_UP, 
                            JOYSTICK_DOWN,
                            JOYSTICK_UP_LEFT,
                            JOYSTICK_UP_RIGHT,
                            JOYSTICK_DOWN_LEFT,
                            JOYSTICK_DOWN_RIGHT,
                            JOYSTICK_NONE
                            };

    typedef struct {
        JoystickDirection direction;
        float x; //-1 for full left, 1 for full right
        float y; //-1 for full down, 1 for full up
        float prevX;
        float prevY;
    } JoystickEvent;

    JoystickInput(PinName xAxis, PinName yAxis, InputArbiter* arbiter, int flags);
};

#endif //JOYSTICK_INPUT_INCLUDED