

#ifndef TITLE_SCREEN_INCLUDED
#define TITLE_SCREEN_INCLUDED

#include "core/gameScreen.hpp"

class TitleScreen : public GameScreen {
    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);
};

extern TitleScreen _titleScreen;

#endif //TITLE_SCREEN_INCLUDED