/*
 * Game Tick Manager Class
 *
 * Recieves the game tick and distributes it to all sprites and to the graphics controller
 * to render animations
 */

#ifndef GAME_TICK_INCLUDED
#define GAME_TICK_INCLUDED

#include <list>

class Tickable;

class GameTickManager {

    std::list<Tickable*> registrations;

    public:

    void dispatchGameTick(float dt);

    inline void registerForTick(Tickable* obj) { this->registrations.push_back(obj); }

    inline void removeRegistration(Tickable* obj) { this->registrations.remove(obj); }
};

extern GameTickManager _tickManager;

#endif //GAME_TICK_INCLUDED