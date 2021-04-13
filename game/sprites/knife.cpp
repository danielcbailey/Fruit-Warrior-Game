
#include "sprites/knife.hpp"
#include "assets/assets.hpp"
#include "screens/gamePlayScreen.hpp"
#include "game/inputs.hpp"
#include "globals.hpp"
#include <functional>

Knife::Knife() {
    this->setLayer(11); //11 is the knife layer
    this->setPos(0, 0);
    this->setImage(&knife_0);
    this->thrown = false;
    this->evtListener = _button1->attach(std::bind(&Knife::throwKnife, this, placeholders::_1, placeholders::_2));
}

Knife::~Knife() {
    _button1->detach(this->evtListener);
}

void Knife::onGameTick(float dt) {
    if (this->thrown) {
        //advance to the right
        if (this->getX() > 127) {
            //return to the player
            this->thrown = false;
        }

        this->setPos(this->getX() + dt * 50, this->getY());
    } else {
        //follow player
        Ninja* player = _gamePlayScreen.getPlayer();

        this->setPos(player->getX() + 8, player->getY() + 3);
    }

    _gamePlayScreen.getFruitManager()->reportKnifePosition(this->getX(), this->getY());
}

bool Knife::throwKnife(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*)evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        printfdbg("away it goes!\n");
        this->thrown = true;
    }

    return false;
}