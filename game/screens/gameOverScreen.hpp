

#ifndef GAME_OVER_SCREEN_INCLUDED
#define GAME_OVER_SCREEN_INCLUDED

#include "core/gameScreen.hpp"

class GameOverScreen : public GameScreen {
    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern GameOverScreen _gameOverScreen;

#endif //GAME_OVER_SCREEN_INCLUDED