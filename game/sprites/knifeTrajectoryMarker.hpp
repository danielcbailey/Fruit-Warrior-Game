

#ifndef KNIFE_TRAJECTORY_MARKER_INCLUDED
#define KNIFE_TRAJECTORY_MARKER_INCLUDED

#include "graphics/sprite.hpp"

class KnifeTrajectoryMarker: public Sprite {

    public:

    KnifeTrajectoryMarker(int index);

    void onGameTick(float dt);
};

#endif //KNIFE_TRAJECTORY_MARKER_INCLUDED