

#ifndef TITLE_SCREEN_INCLUDED
#define TITLE_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"
#include "game/fruitManager.hpp"

class TitleScreen : public GameScreen {
    private:

    GraphicsElement* background;
    GraphicsElement* samuraiLeft;
    GraphicsElement* samuraiRight;
    GraphicsElement* startButton;
    GraphicsElement* highScoresButton;
    GraphicsElement* fruitLogo;
    FruitManager* fruitManager;

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern TitleScreen _titleScreen;

#endif //TITLE_SCREEN_INCLUDED