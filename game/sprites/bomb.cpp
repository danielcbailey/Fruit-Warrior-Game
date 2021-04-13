

#include "bomb.hpp"
#include "assets/assets.hpp"
#include "sliceable.hpp"
#include "globals.hpp"

Bomb::Bomb(float bearing, float velocity, int x0, int y0) : bearing(bearing), vel(velocity) {
    this->removable = false;
    this->setLayer(11); //11 is the bomb layer
    this->setPos(x0, y0);
    this->getAnimationPlayer()->setAnimation(&bomb_animation, 1, AnimationPlayer::PLAYBACK_REPEAT_BOTH_WAYS);
    this->enableAnimation();
}

void Bomb::onGameTick(float dt) {
    float x = this->getX() + cos(this->bearing) * this->vel * dt;
    float y = this->getY() + sin(this->bearing) * this->vel * dt;

    if (x > SLICEABLE_MAX_X ||
        x < SLICEABLE_MIN_X ||
        y > SLICEABLE_MAX_Y ||
        y < SLICEABLE_MIN_Y) {
        
        //can be removed because it is off screen
        this->removable = true;
    }

    this->setPos(x, y);
}

