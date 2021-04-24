#ifndef INTRO_SCREEN_INCLUDED
#define INTRO_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"

class IntroScreen : public GameScreen {
    private:

    GraphicsElement* left;
    GraphicsElement* right;

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern IntroScreen _introScreen;

#endif //INTRO_SCREEN_INCLUDED