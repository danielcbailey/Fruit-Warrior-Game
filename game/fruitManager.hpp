

#include <list>
#include "sprites/sliceable.hpp"
#include "sprites/bomb.hpp"

class FruitManager : public Tickable {
    private:

    std::list<Sliceable*> fruits;
    std::list<Bomb*> bombs;

    int knifeX;
    int knifeY;

    public:

    FruitManager();

    ~FruitManager();

    void onGameTick(float dt);

    inline void reportKnifePosition(int x, int y) {
        this->knifeX = x;
        this->knifeY = y;
    }
};