

#include "screenManager.hpp"
#include "graphics/gameTick.hpp"
#include "game/inputs.hpp"
#include "globals.hpp"
#include "game/soundManager.hpp"

ScreenManager::ScreenManager(GameScreen* initialScreen) {
    this->totalTime = 0;
    this->currentScreen = initialScreen;
    this->requestedScreen = nullptr;

    this->fps = 0;
    this->frameCount = 0;
    this->deltaFrames = 0;
    this->previousTime = 0;
    this->prevTotalTime = 0;
    this->paused = false;
}

void ScreenManager::gameStart() {
    //Lets get the party started
    this->timer.start();
    this->currentScreen->onCreation();
}

void ScreenManager::onMainLoop() {
    //detecting if the a screen change request has been made:
    if (this->requestedScreen != nullptr) {
        //A new screen has been requested
        //Assumes all screens are statically defined, so does not delete them.
        this->currentScreen->onDestruction();
        this->currentScreen = this->requestedScreen;
        this->requestedScreen = nullptr;
        this->currentScreen->onCreation();
    }

    //Calculating ellapsed time since the previous frame
    long long int dtu = this->timer.elapsed_time().count() - this->previousTime;
    float dt = dtu / 1000000.0f;
    this->previousTime += dtu;
    this->totalTime += dt;

    //Updating frame rate information
    this->frameCount++;
    this->deltaFrames++;

    if ((int) this->totalTime != (int)this->previousTime) {
        //Roughly one second has passed
        this->fps = this->deltaFrames / (this->totalTime - this->prevTotalTime);

        this->prevTotalTime = totalTime;
        this->deltaFrames = 0;
    }

    //Polling for inputs
    _inputArbiter->pollInputs(dt);

    //Calling screen update function
    //The idea of the screen update function is to act before tickable events are
    //processed. For example, processing an input before the tickable acts.
    //Most events should be handled by tickable, so the game loop is provided as
    //an option but is by no means mandatory to have it do something meaningful.
    this->currentScreen->incrementTime(dt);
    this->currentScreen->onGameLoop(dt);

    //Now dispatching tickable events and dispatching animation ticks
    if (!this->paused) {
        //Game ticks get suspended while paused.
        _tickManager.dispatchGameTick(dt);
    }

    //Checking for music updates
    _soundManager.checkMusicChange();
}