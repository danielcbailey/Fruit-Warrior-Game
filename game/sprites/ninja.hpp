

#ifndef NINJA_INCLUDED
#define NINJA_INCLUDED

#include "graphics/sprite.hpp"

class Ninja : public Sprite {
    private:

    int evtListener;

    bool onJoyInput(void* evtDetails, float dt);

    public:

    Ninja();

    ~Ninja();

    inline void onGameTick(float dt) {}
};

#endif //NINJA_INCLUDED