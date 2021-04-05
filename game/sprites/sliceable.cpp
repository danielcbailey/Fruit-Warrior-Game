

#include "sliceable.hpp"

#define SLICEABLE_MAX_X 128
#define SLICEABLE_MIN_X -12
#define SLICEABLE_MAX_Y 128
#define SLICEABLE_MIN_Y -12

Sliceable::Sliceable(float bearing, float vel, int points, int x0, int y0,
              Animation* cutAnimation, BitmapImage* image) :
              bearing(bearing), vel(vel), points(points) {
    
    this->isCut = false;
    this->removable = false;
    this->setPos(x0, y0);
    this->setImage(image);
    this->getAnimationPlayer()->setAnimation(cutAnimation, 0.5f,
                                             AnimationPlayer::PLAYBACK_ONCE_SINGLE_WAY);
}

void Sliceable::onGameTick(float dt) {
    int x = this->getX() + cos(this->bearing) * this->vel * dt;
    int y = this->getY() + sin(this->bearing) * this->vel * dt;

    if (x > SLICEABLE_MAX_X ||
        x < SLICEABLE_MIN_X ||
        y > SLICEABLE_MAX_Y ||
        y < SLICEABLE_MIN_Y) {
        
        //can be removed because it is off screen
        this->removable = true;
    }

    this->setPos(x, y);
}