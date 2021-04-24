
#include "sprites/knife.hpp"
#include "assets/assets.hpp"
#include "screens/gamePlayScreen.hpp"
#include "game/inputs.hpp"
#include "globals.hpp"
#include <functional>

Knife::Knife() {
    this->setLayer(99); //99 is the knife layer
    this->setPos(0, 0);
    this->setImage(&knife_0);
    this->thrown = false;
    this->evtListener = _button1->attach(std::bind(&Knife::throwKnife, this, placeholders::_1, placeholders::_2));
    this->speed = 0;
    this->trajectory = 0;
    this->joyEvtListener = -1;
    this->markers[0] = new KnifeTrajectoryMarker(0);
    this->markers[1] = new KnifeTrajectoryMarker(1);
    this->markers[2] = new KnifeTrajectoryMarker(2);
    this->markers[3] = new KnifeTrajectoryMarker(3);
}

Knife::~Knife() {
    _button1->detach(this->evtListener);
    if (this->joyEvtListener != -1) {
        _joystick->detach(this->joyEvtListener);
    }
    delete this->markers[0];
    delete this->markers[1];
    delete this->markers[2];
    delete this->markers[3];
}

void Knife::onGameTick(float dt) {
    if (this->thrown) {
        //advance to the right
        if (this->getX() > 127 || this->getX() < 0 || this->getY() < 0 || this->getY() > 127) {
            //return to the player
            this->thrown = false;
        }

        this->setPos(this->getX() + dt * this->speed * cosf(this->trajectory),
                     this->getY() + dt * this->speed * sinf(this->trajectory));
    } else {
        //follow player
        _gamePlayScreen.setComboMultiplier(1);
        Ninja* player = _gamePlayScreen.getPlayer();

        this->setPos(player->getX() + 8, player->getY() + 3);
    }

    _gamePlayScreen.getFruitManager()->reportKnifePosition(this->getX(), this->getY());
}

bool Knife::throwKnife(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*)evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        //Starting aiming process
        if (this->thrown) {
            return false;
        }
        this->joyEvtListener = _joystick->attach(std::bind(&Knife::aimKnife, this, placeholders::_1, placeholders::_2));
    } else if (evt.eventType == ButtonInput::BUTTON_RELEASE) {
        //Now releasing the knife
        if (this->joyEvtListener == -1) {
            return false;
        }

        for (int i = 0; 4 > i; i++) {
            this->markers[i]->hide();
        }
        _gamePlayScreen.setComboMultiplier(1);
        this->thrown = true;
        _joystick->detach(this->joyEvtListener);
        this->joyEvtListener = -1;
    }

    return false;
}

bool Knife::aimKnife(void* evtDetails, float dt) {
    JoystickInput::JoystickEvent evt = *((JoystickInput::JoystickEvent*)evtDetails);

    this->trajectory = atanf(evt.y / evt.x);

    if (evt.x < 0) {
        this->trajectory += 3.1415f;
    }

    this->speed = sqrtf((evt.x * evt.x) + (evt.y * evt.y)) * 64;

    if (this->speed < 20) {
        this->speed = 20;
    }

    //Updating marker positions
    //Each marker represents where the knife will be in a half second from each other
    for (int i = 0; 4 > i; i++) {
        this->markers[i]->setPos(cosf(this->trajectory) * this->speed * (i + 1) * 0.5f + this->getX(),
                                 sinf(this->trajectory) * this->speed * (i + 1) * 0.5f + this->getY());

        if (this->markers[i]->isHidden()) {
            this->markers[i]->show();
            printfdbg("test\n");
        }
    }

    return false;
}