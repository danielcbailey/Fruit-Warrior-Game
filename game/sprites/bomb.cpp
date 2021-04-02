

#include "bomb.hpp"
#include "assets/assets.hpp"

Bomb::Bomb() {
    this->getAnimationPlayer()->setAnimation(&bomb_animation, 1, AnimationPlayer::PLAYBACK_REPEAT_BOTH_WAYS);
    this->setPos(50, 50);
    this->setLayer(7);
    this->enableAnimation();
}

void Bomb::onGameTick(float dt) {
    totalTime += dt / 4;

    this->setPos(50 * cos(totalTime) + 50, 50 * sin(totalTime) + 50);
}

