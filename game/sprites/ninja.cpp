#include "ninja.hpp"
#include "assets/assets.hpp"
#include "inputs/joystickInput.hpp"
#include "game/inputs.hpp"
#include <functional>
#include "game/compostManager.hpp"
#include "screens/gamePlayScreen.hpp"

Ninja::Ninja() {
    this->setLayer(4); //4 is the ninja's layer
    this->setPos(0, 32);
    this->setImage(&ninja_0);
    this->evtListener = _joystick->attach(std::bind(&Ninja::onJoyInput, this, placeholders::_1, placeholders::_2));
}

Ninja::~Ninja() {
    _joystick->detach(this->evtListener);
}

bool Ninja::onJoyInput(void* evtDetails, float dt) {
    JoystickInput::JoystickEvent evt = *((JoystickInput::JoystickEvent*)evtDetails);

    if (abs(evt.y) < 0.25) return false;

    float newY = this->getY() + dt * 32 * evt.y;

    int compY = 127 - _gamePlayScreen.getCompostManager()->getTallestPileHeight() - 14;

    if (newY < 8) {
        newY = 8;
    } else if (newY > compY) {
        newY = compY;
    }

    this->setPos(this->getX(), newY);

    return false;
}