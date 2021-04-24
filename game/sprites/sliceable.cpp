

#include "sliceable.hpp"
#include "screens/gamePlayScreen.hpp"
#include "globals.hpp"
#include "game/compostManager.hpp"

bool _sliceablePlayMode = false;

Sliceable::Sliceable(float bearing, float vel, int points, int x0, int y0,
              Animation* cutAnimation, BitmapImage* image) :
              bearing(bearing), vel(vel), points(points) {
    
    this->isCut = false;
    this->removable = false;
    this->setLayer(10); //10 is the fruit layer
    this->setPos(x0, y0);
    this->setImage(image);
    this->getAnimationPlayer()->setAnimation(cutAnimation, 0.5f,
                                             AnimationPlayer::PLAYBACK_ONCE_SINGLE_WAY);
}

void Sliceable::onGameTick(float dt) {
    float x = this->getX() + cos(this->bearing) * this->vel * dt;
    float y = this->getY() + sin(this->bearing) * this->vel * dt;

    if (_sliceablePlayMode && !this->isCut && _gamePlayScreen.getCompostManager()->testCollision(this->getX() + 5, this->getY() + 5, 8)) {
        //Hit a compost pile
        this->removable = true;
    }

    if (this->isCut && this->getAnimationPlayer()->isDone()) {
        //slicing animation is finished, can remove
        this->removable = true;
    }

    if (x > SLICEABLE_MAX_X ||
        x < SLICEABLE_MIN_X ||
        y > SLICEABLE_MAX_Y ||
        y < SLICEABLE_MIN_Y) {
        
        //can be removed because it is off screen
        this->removable = true;
    }

    this->setPos(x, y);
}

void Sliceable::cut() {
    if (this->isCut) return;
    this->isCut = true;
    this->enableAnimation();

    if (_sliceablePlayMode) {
        _gamePlayScreen.setScore(_gamePlayScreen.getScore() + this->points * _gamePlayScreen.getComboMultiplier());
        _gamePlayScreen.setComboMultiplier(_gamePlayScreen.getComboMultiplier() + 1);
    }
}