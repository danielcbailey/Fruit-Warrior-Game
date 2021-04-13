

#include "fruitManager.hpp"
#include "graphics/gameTick.hpp"
#include "core/screenManager.hpp"
#include "screens/gameOverScreen.hpp"
#include "screens/gamePlayScreen.hpp"
#include "assets/assets.hpp"
#include "globals.hpp"

FruitManager::FruitManager() {
    this->knifeX = -100; //just somewhere far away before it is told where it is
    this->knifeY = -100;
    this->justGennedFruit = false;
    
    _tickManager.registerForTick(this);
}

FruitManager::~FruitManager() {
    _tickManager.removeRegistration(this);

    //Deleting all created fruit/bombs
    for (std::list<Sliceable*>::iterator it = this->fruits.begin(); it != this->fruits.end(); ++it) {
        delete(*it);
    }

    for (std::list<Bomb*>::iterator it = this->bombs.begin(); it != this->bombs.end(); ++it) {
        delete(*it);
    }
}

void FruitManager::onGameTick(float dt) {
    //checking for fruit to remove or fruit to slice
    std::list<Sliceable*>::iterator it = this->fruits.begin();

    GraphicsElement::RegionOfInfluence knifeROI;
    knifeROI.x = this->knifeX;
    knifeROI.y = this->knifeY;
    knifeROI.x2 = this->knifeX + 3;
    knifeROI.y2 = this->knifeY + 3;

    int counter = 0;

    while (it != this->fruits.end()) {
        counter++;
        Sliceable* fruit = *it;
        if (fruit->shouldRemove()) {
            //needs to remove the fruit
            ++it;

            this->fruits.remove(fruit);
            printfdbg("Removed fruit\n");

            delete(fruit);

            if (counter > this->fruits.size()) {
                //The removed fruit was the last, so the loop must be
                //exited manually
                break;
            }

            continue;
        } else if (fruit->isInROI(knifeROI)) {
            //should slice the fruit
            fruit->cut();
        }

        ++it;
    }

    //now testing bombs
    std::list<Bomb*>::iterator bIt = this->bombs.begin();
    counter = 0;

    while (bIt != this->bombs.end()) {
        counter++;
        if ((*bIt)->shouldRemove()) {
            //needs to remove the bomb
            Bomb* oldBomb = *bIt;
            ++bIt;

            printfdbg("Removed bomb\n");
            this->bombs.remove(oldBomb);

            delete(oldBomb);

            if (counter > this->bombs.size()) {
                //The removed bomb was the last, so the loop must be
                //exited manually
                break;
            }

            continue;
        }
        if ((*bIt)->isInROI(knifeROI)) {
            //well, game over I guess
            _screenManager.changeScreen(&_gameOverScreen);
        }

        ++bIt;
    }

    //now evaluate if new fruit should be generated
    if (((int)_screenManager.getTotalTime() % _gamePlayScreen.getSpawnInterval()) == 0) {
        if (this->justGennedFruit) return;
        //generate a new fruit/bomb
        this->justGennedFruit = true;
        printfdbg("Genned fruit.\n");
        
        //seeding the RNG
        srand((unsigned int)(_screenManager.getTotalTime() * 1000000));

        //picking a starting edge
        //0 is left, 1 is right, 2 and 3 are top
        int edge = rand() % 4;
        int x, y, minX, maxX, minY, maxY;
        float bearing;
        float velocity = 8; //constant for now
        minX = 4 * velocity;
        maxX = 128 - (4 * velocity);
        minY = 4 * velocity + 8;
        maxY = 80;

        switch (edge) {
        case 0:
            x = -10;
            //y is [-10, COMPOST_MIN_Y)
            y = rand() % (COMPOST_MIN_Y + 10) - 10;
            break;
        case 1:
            x = 127;
            //y is [-10, COMPOST_MIN_Y)
            y = rand()  % (COMPOST_MIN_Y + 10) - 10;
            break;
        case 2: //2 is the same as 3
        case 3:
            y = -2;
            //y is [-2, 128)
            x = rand() % (138) - 10;
            break;
        }

        //bearing must be such that the fruit can make it at least 4 seconds
        //whilst remaining in space that the ninja can reach.
        //As such, within this time, it cannot:
        // - Go offscreen
        // - Go into the compost region, specifically not below COMPOST_MIN_Y
        //
        //To accomplish this, a valid region is calculated and the pixel is randomly
        //selected from that valid region and used to calculate the angle.
        //This region must be at least velocity * 4 pixels away from the starting point.
        //For simplicity, the region must be rectangular.

        //Just using a constant region for now.
        int tx, ty;
        tx = (rand() % (maxX - minX)) + minX;
        ty = (rand() % (maxY - minY)) + minY;

        //Finding the angle between random target point and the generation start
        bearing = atan((float)(ty - y) / (tx - x));

        if ((tx - x) < 0) {
            //The bearing returned by tan is 180 degrees from where it should be
            bearing += 3.1415f;
        }

        //Now randomizing the type of fruit and adding the fruit to its
        //appropriate list.
        Sliceable* newFruit = nullptr;

        Bomb* newBomb = nullptr;
        int type = rand() % 10;
        switch (type) {
        case 0:
        case 1:
            //apple
            newFruit = new Sliceable(bearing, velocity, 1, x, y, &apple_cut_animation, &apple_0);
            break;
        case 2:
        case 3:
            //orange
            newFruit = new Sliceable(bearing, velocity, 1, x, y, &orange_cut_animation, &orange_0);
            break;
        case 4:
        case 5:
            //banana
            newFruit = new Sliceable(bearing, velocity, 1, x, y, &banana_cut_animation, &banana_0);
            break;
        case 6:
        case 7:
            //kiwi
            newFruit = new Sliceable(bearing, velocity, 1, x, y, &kiwi_cut_animation, &kiwi_0);
            break;
        case 8:
            //lemon
            newFruit = new Sliceable(bearing, velocity, 1, x, y, &lemon_cut_animation, &lemon_0);
            break;
        case 9:
            //bomb
            newBomb = new Bomb(bearing, velocity, x, y);
            break;
        }

        if (newBomb != nullptr) {
            this->bombs.push_back(newBomb);
        } else if (newFruit != nullptr) {
            this->fruits.push_back(newFruit);
        }
    } else {
        this->justGennedFruit = false;
    }
}