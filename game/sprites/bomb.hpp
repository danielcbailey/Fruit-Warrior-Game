

#ifndef BOMB_INCLUDED
#define BOMB_INCLUDED

#include "graphics/sprite.hpp"

class Bomb : public Sprite {
    private:

    float vel;
    float bearing;
    bool removable;

    public:

    Bomb(float bearing, float velocity, int x0, int y0);

    void onGameTick(float dt);

    inline bool shouldRemove() { return this->removable; }
};

#endif //BOMB_INCLUDED