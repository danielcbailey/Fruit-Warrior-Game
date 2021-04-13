

#include "gameTick.hpp"
#include "tickable.hpp"
#include "graphicsController.hpp"
#include "globals.hpp"

GameTickManager _tickManager;

void GameTickManager::dispatchGameTick(float dt) {
    for (std::list<Tickable*>::iterator it = this->registrations.begin(); it != this->registrations.end(); ++it) {
        (*it)->onGameTick(dt);
    }

    _globalGraphics->handleGraphicsTick(dt);
}