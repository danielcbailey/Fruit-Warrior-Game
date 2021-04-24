/*
 * Game Screen Manager Class
 *
 * The screen manager is solely responsible for controlling the contents of the screen.
 * It controls the creation, destruction, and updating of screens. Any function in any
 * part of the codebase can change the screen without any immediate consequences.
 *
 * The screen manager will queue the change until the current frame is finished to remove
 * the possibility of invalid memory accesses from having the old screen removed from the
 * display.
 */

#ifndef SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED

#include "gameScreen.hpp"
#include "inputs/inputArbiter.hpp"
#include "mbed.h"

class ScreenManager {
    private:

    GameScreen* currentScreen;
    GameScreen* requestedScreen;

    Timer timer;

    long long int frameCount;
    int deltaFrames;
    float fps;
    float prevTotalTime;
    bool paused;

    float totalTime;
    long long int previousTime;

    public:

    //Cannot be nullptr
    ScreenManager(GameScreen* initialScreen);

    inline void changeScreen(GameScreen* newScreen) { this->requestedScreen = newScreen; }

    inline float getFPS() { return this->fps; }

    inline float getTotalTime() { return this->totalTime; }

    inline void setPaused(bool value) { this->paused = value; }

    //Called by the main function in the true loop
    //Updates the tick manager
    //Polls for inputs
    void onMainLoop();

    //Gets the initialScreen started, telling the manager that all other global
    //game objects are ready to begin handling game activities, most notably the graphics
    //manager.
    void gameStart();
};

extern ScreenManager _screenManager;

#endif //SCREEN_MANAGER_INCLUDED