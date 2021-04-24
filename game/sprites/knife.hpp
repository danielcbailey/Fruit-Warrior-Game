
#ifndef KNIFE_INCLUDED
#define KNIFE_INCLUDED

#include "graphics/sprite.hpp"
#include "knifeTrajectoryMarker.hpp"

class Knife : public Sprite {
    private:

    KnifeTrajectoryMarker* markers[4];
    float trajectory;
    bool thrown;
    float speed;

    bool throwKnife(void* evtDetails, float dt);
    int evtListener;
    int joyEvtListener;

    bool aimKnife(void* evtDetails, float dt);

    public:

    Knife();

    ~Knife();

    void onGameTick(float dt);
};

#endif //KNIFE_INCLUDED