/*
 * Game Tick Manager Class
 *
 * Recieves the game tick and distributes it to all sprites and to the graphics controller
 * to render animations
 */

#ifndef GAME_TICK_INCLUDED
#define GAME_TICK_INCLUDED

#include <list>

class Sprite;

class GameTickManager {

    std::list<Sprite*> registrations;

    public:

    void dispatchGameTick(float dt);

    inline void registerForTick(Sprite* sprite) { this->registrations.push_back(sprite); }

    inline void removeRegistration(Sprite* sprite) { this->registrations.remove(sprite); }
};

extern GameTickManager _tickManager;

#endif //GAME_TICK_INCLUDED