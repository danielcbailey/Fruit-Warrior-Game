

#ifndef BOMB_INCLUDED
#define BOMB_INCLUDED

#include "graphics/sprite.hpp"

class Bomb : Sprite {
    private:

    float totalTime;

    public:

    Bomb();

    void onGameTick(float dt);
};

#endif //BOMB_INCLUDED