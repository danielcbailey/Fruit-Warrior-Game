

#ifndef GAME_PLAY_SCREEN_INCLUDED
#define GAME_PLAY_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"
#include "game/fruitManager.hpp"

class GamePlayScreen : public GameScreen {
    private:

    GraphicsElement* background;
    FruitManager* fruitManager;

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern GamePlayScreen _gamePlayScreen;

#endif //GAME_PLAY_SCREEN_INCLUDED