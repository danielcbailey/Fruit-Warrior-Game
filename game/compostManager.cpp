

#include "compostManager.hpp"

CompostManager::CompostManager() {
    for (int i = 0; 11 > i; i++) {
        piles[i] = new CompostPile(i);
    }
}

CompostManager::~CompostManager() {
    for (int i = 0; 11 > i; i++) {
        delete(piles[i]);
    }
}

//Tests for the collision and also adds to the compost if there was a collision
bool CompostManager::testCollision(int x, int y, int amt) {
    int col = (x - 3) / 11;

    if (col > 10) return false;

    int height = piles[col]->getHeight();

    if (127 - height < y) {
        //collision
        piles[col]->addCompost(amt);
        return true;
    }

    return false;
}

int CompostManager::getTallestPileHeight() {
    int ret = 0;
    for (int i = 0; 11 > i; i++) {
        int temp = piles[i]->getHeight();

        if (temp > ret) ret = temp;
    }

    return ret;
}