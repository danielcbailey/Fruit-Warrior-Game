

#ifndef COMPOST_MANAGER_INCLUDED
#define COMPOST_MANAGER_INCLUDED

#include "compostPile.hpp"

class CompostManager {
    private:
    CompostPile* piles[11];

    public:
    CompostManager();

    ~CompostManager();

    //Tests for the collision and also adds to the compost if there was a collision
    bool testCollision(int x, int y, int amt);

    int getTallestPileHeight();
};

#endif //COMPOST_MANAGER_INCLUDED