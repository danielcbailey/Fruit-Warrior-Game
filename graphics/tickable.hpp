/*
 * Tickable class
 * 
 * Defines a common function to be called for all classes that are to register with the
 * tick management system.
 *
 * (c) Daniel Cooper 2021
 */

#ifndef TICKABLE_INCLUDED
#define TICKABLE_INCLUDED

class Tickable {
    public:

    virtual void onGameTick(float dt) = 0;
};

#endif //TICKABLE_INCLUDED