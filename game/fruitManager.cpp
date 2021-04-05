

#include "fruitManager.hpp"
#include "graphics/gameTick.hpp"

FruitManager::FruitManager() {
    this->knifeX = 0;
    this->knifeY = 0;
    
    _tickManager.registerForTick(this);
}

FruitManager::~FruitManager() {
    _tickManager.removeRegistration(this);
}

void FruitManager::onGameTick(float dt) {
    //checking for fruit to remove or fruit to slice
    std::list<Sliceable*>::iterator it = this->fruits.begin();

    GraphicsElement::RegionOfInfluence knifeROI;
    knifeROI.x = this->knifeX;
    knifeROI.y = this->knifeY;
    knifeROI.x2 = this->knifeX + 3;
    knifeROI.y2 = this->knifeY + 3;

    while (it != this->fruits.end()) {
        Sliceable* fruit = *it;
        if (fruit->shouldRemove()) {
            //needs to remove the fruit
            ++it;

            this->fruits.remove(fruit);

            delete(fruit);

            continue;
        } else if (fruit->isInROI(knifeROI)) {
            //should slice the fruit
            fruit->cut();
        }

        ++it;
    }

    //now testing bombs
    std::list<Bomb*>::iterator bIt = this->bombs.begin();

    while (bIt != this->bombs.end()) {
        if ((*it)->isInROI(knifeROI)) {
            //well, game over I guess
            //TODO: switch screens
        }
    }
}