

#include "buttonInput.hpp"

ButtonInput::ButtonInput(PinName pin, InputArbiter* arbiter) : pin(pin) {
    arbiter->AddInput(this);

    //Must make previous state the same as state
    this->previousState = this->pin.read();
}

void ButtonInput::poll() {
    //Forming event
    int state = this->pin.read();
    ButtonEvent* event = new(ButtonEvent);
    event->previousState = this->previousState;
    event->state = state;

    if (this->previousState == state) {
        event->eventType = BUTTON_SAME;
    } else if (this->previousState == 0) {
        //state must be 1, so it is a press
        event->eventType = BUTTON_PRESS;
    } else {
        //state must be 0 and previous must be 1, so it is a release
        event->eventType = BUTTON_RELEASE;
    }

    //Dispatching event
    bool prop = true;
    InputStackElement* curr = this->stackTop;
    while (prop && curr != nullptr) {
        prop = curr->cb(event);
        curr = curr->nextElem;
    }

    delete(event);
    this->previousState = state;
}