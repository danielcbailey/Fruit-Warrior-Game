/*
 * Game Screen Class
 *
 * The game screen class is an interface in nature to be a superclass of anything that
 * requires a game loop, for example a title screen or the game itself.
 *
 * All implementing classes should not override the default constructors/deconstructors.
 * Instead, they all objects and structures should be created/instantiated in the
 * appropriate mirror functions
 *
 * All implementing classes should not assume the screen to be in any known condition.
 */

#ifndef GAME_SCREEN_INCLUDED
#define GAME_SCREEN_INCLUDED

 class GameScreen {

    private:

    //TotalTime is the amount of time since the screen was constructed
    float totalTime;

    public:

    //Called by the game screen manager
    inline void incrementTime(float dt) { this->totalTime += dt; }

    inline float getTotalTime() { return this->totalTime; }

    //To be implemented by the subclass and called by the game screen manager

    virtual void onCreation() = 0;

    virtual void onDestruction() = 0;

    virtual void onGameLoop(float dt) = 0;
 };

 #endif //GAME_SCREEN_INCLUDED