

#include "gameTick.hpp"
#include "sprite.hpp"
#include "graphicsController.hpp"

void GameTickManager::dispatchGameTick(float dt) {
    for (std::list<Sprite*>::iterator it = this->registrations.begin(); it != this->registrations.end(); ++it) {
        (*it)->onGameTick(dt);
    }

    _globalGraphics->handleGraphicsTick(dt);
}