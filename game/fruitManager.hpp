

#include <list>
#include "sprites/sliceable.hpp"
#include "sprites/bomb.hpp"

#ifndef FRUIT_MANAGER_INCLUDED
#define FRUIT_MANAGER_INCLUDED

class FruitManager : public Tickable {
    private:

    std::list<Sliceable*> fruits;
    std::list<Bomb*> bombs;

    int knifeX;
    int knifeY;
    bool justGennedFruit;

    public:

    FruitManager();

    ~FruitManager();

    void onGameTick(float dt);

    inline void reportKnifePosition(int x, int y) {
        this->knifeX = x;
        this->knifeY = y;
    }
};

#endif //FRUIT_MANAGER_INCLUDED