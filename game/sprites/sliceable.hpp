

#ifndef SLICEABLE_INCLUDED
#define SLICEABLE_INCLUDED

#include "graphics/sprite.hpp"

#define SLICEABLE_MAX_X 128
#define SLICEABLE_MIN_X -12
#define SLICEABLE_MAX_Y 128
#define SLICEABLE_MIN_Y -4

class Sliceable : public Sprite {
    private:

    float bearing; //in radians
    float vel; //in pixels per second
    float removeCounter;

    int points;
    bool isCut;
    bool removable;

    public:

    Sliceable(float bearing, float vel, int points, int x0, int y0,
              Animation* cutAnimation, BitmapImage* image);

    inline int getPoints() { return this->points; }

    inline float getBearing() { return this->bearing; }

    inline float getVelocity() { return this->vel; }

    void cut();

    inline bool shouldRemove() { return this->removable; }

    void onGameTick(float dt);
};

#endif //SLICEABLE_INCLUDED