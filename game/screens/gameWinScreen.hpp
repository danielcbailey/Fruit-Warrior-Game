

#ifndef GAME_WIN_SCREEN_INCLUDED
#define GAME_WIN_SCREEN_INCLUDED

#include "core/gameScreen.hpp"

class GameWinScreen : public GameScreen {
    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern GameWinScreen _gameWinScreen;

#endif //GAME_WIN_SCREEN_INCLUDED