

#include "screenManager.hpp"
#include "graphics/gameTick.hpp"
#include "game/inputs.hpp"
#include "globals.hpp"

ScreenManager::ScreenManager(GameScreen* initialScreen) {
    this->totalTime = 0;
    this->currentScreen = initialScreen;
    this->requestedScreen = nullptr;

    for (int i = 0; 5 > i; i++) {
        this->frameTimes[i] = 0;
    }

    this->fps = 0;
    this->currentFpsIndex = 0;
    this->previousTime = 0;
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
    this->frameTimes[this->currentFpsIndex] = dt;

    float fpsTotalTime = 0;
    for (int i = 0; 5 > i; i++) {
        fpsTotalTime += this->frameTimes[i];
    }

    this->fps = 5.0f / fpsTotalTime;

    this->currentFpsIndex = (this->currentFpsIndex + 1) % 5;

    //Polling for inputs
    _inputArbiter->pollInputs(dt);

    //Calling screen update function
    //The idea of the screen update function is to act before tickable events are
    //processed. For example, processing an input before the tickable acts.
    //Most events should be handled by tickable, so the game loop is provided as
    //an option but is by no means mandatory to have it do something meaningful.
    this->currentScreen->onGameLoop(dt);

    //Now dispatching tickable events and dispatching animation ticks
    _tickManager.dispatchGameTick(dt);
}