

#include "joystickInput.hpp"

JoystickInput::JoystickInput(PinName xAxis, PinName yAxis, InputArbiter* arbiter, int flags) : xPin(xAxis), yPin(yAxis), InputBase(arbiter) {
    arbiter->AddInput(this);

    //Reading flags
    if (flags & JOYSTICK_INVERT_X) {
        this->invertX = true;
    } else {
        this->invertX = false;
    }

    if (flags & JOYSTICK_INVERT_Y) {
        this->invertY = true;
    } else {
        this->invertY = false;
    }

    //Getting current state to set as previous
    this->previousX = this->xPin.read() * 2.0f - 1.0f;
    this->previousY = this->yPin.read() * 2.0f - 1.0f;
}

void JoystickInput::poll(float dt) {
    //Forming event
    JoystickEvent* event = new(JoystickEvent);

    float x = this->xPin.read() * 2.0f - 1.0f;
    float y = this->yPin.read() * 2.0f - 1.0f;
    y = 0 - y;
    event->x = x;
    event->y = y;
    event->prevX = this->previousX;
    event->prevY = this->previousY;
    event->direction = JOYSTICK_NONE;

    if (event->x > 0.25f) {
        if (invertX) {
            event->direction = JOYSTICK_LEFT;
        } else {
            event->direction = JOYSTICK_RIGHT;
        }
    } else if (event->x < -0.25f) {
        if (invertX) {
            event->direction = JOYSTICK_RIGHT;
        } else {
            event->direction = JOYSTICK_LEFT;
        }
    }

    if (event->y > 0.25f) {
        if (invertY) {
            if (event->direction == JOYSTICK_LEFT) {
                event->direction = JOYSTICK_DOWN_LEFT;
            } else if (event->direction == JOYSTICK_RIGHT) {
                event->direction = JOYSTICK_DOWN_RIGHT;
            } else {
                event->direction = JOYSTICK_DOWN;
            }
        } else {
            if (event->direction == JOYSTICK_LEFT) {
                event->direction = JOYSTICK_UP_LEFT;
            } else if (event->direction == JOYSTICK_RIGHT) {
                event->direction = JOYSTICK_UP_RIGHT;
            } else {
                event->direction = JOYSTICK_UP;
            }
        }
    } else if (event->y < -0.25f) {
        if (invertY) {
            if (event->direction == JOYSTICK_LEFT) {
                event->direction = JOYSTICK_UP_LEFT;
            } else if (event->direction == JOYSTICK_RIGHT) {
                event->direction = JOYSTICK_UP_RIGHT;
            } else {
                event->direction = JOYSTICK_UP;
            }
        } else {
            if (event->direction == JOYSTICK_LEFT) {
                event->direction = JOYSTICK_DOWN_LEFT;
            } else if (event->direction == JOYSTICK_RIGHT) {
                event->direction = JOYSTICK_DOWN_RIGHT;
            } else {
                event->direction = JOYSTICK_DOWN;
            }
        }
    }

    if (this->invertX) {
        event->x = 0 - event->x;
    }

    if (!this->invertY) {
        event->y = 0 - event->y;
    }

    //Dispatching event
    bool prop = true;
    InputStackElement* curr = this->stackTop;
    while (prop && curr != nullptr) {
        prop = curr->cb(event, dt);
        curr = curr->nextElem;
    }

    this->previousX = x;
    this->previousY = y;
    delete(event);
}