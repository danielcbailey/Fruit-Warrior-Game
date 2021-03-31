
#ifndef BUTTON_INPUT_INCLUDED
#define BUTTON_INPUT_INCLUDED

#include "inputBase.hpp"
#include "inputArbiter.hpp"
#include "mbed.h"

class ButtonInput : InputBase {
    private:

    DigitalIn pin;

    void poll();

    int previousState;

    public:

    /*
     * BUTTON_PRESS is issued if previousState was 0 and state is now 1
     * BUTTON_RELEASE is issued if previousState was 1 and state is now 0
     * BUTTON_SAME is issued if previousState == state
     */
    enum ButtonEventType {BUTTON_PRESS, BUTTON_RELEASE, BUTTON_SAME};

    typedef struct {
        ButtonEventType eventType;
        int state; //1 for pressed, 0 for released
        int previousState;
    } ButtonEvent;

    ButtonInput(PinName pin, InputArbiter* arbiter);
};

#endif //BUTTON_INPUT_INCLUDED