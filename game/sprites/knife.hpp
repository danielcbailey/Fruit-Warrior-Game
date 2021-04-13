
#ifndef KNIFE_INCLUDED
#define KNIFE_INCLUDED

#include "graphics/sprite.hpp"

class Knife : public Sprite {
    private:

    bool thrown;

    bool throwKnife(void* evtDetails, float dt);
    int evtListener;

    public:

    Knife();

    ~Knife();

    void onGameTick(float dt);
};

#endif //KNIFE_INCLUDED