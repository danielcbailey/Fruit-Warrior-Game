

#ifndef GAME_PLAY_SCREEN_INCLUDED
#define GAME_PLAY_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"
#include "game/fruitManager.hpp"
#include "sprites/ninja.hpp"
#include "game/compostManager.hpp"

class Knife;

#define COMPOST_MIN_Y 70

class GamePlayScreen : public GameScreen {
    private:

    GraphicsElement* background;
    FruitManager* fruitManager;
    CompostManager* compostManager;
    int spawnInterval;
    int score;
    Ninja* player;
    Knife* knife;

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);

    //Specific functions
    int getSpawnInterval() { return this->spawnInterval; }

    void redrawStatusBar();

    inline void setScore(int score) { 
        this->score = score;
        this->redrawStatusBar();
    }

    inline int getScore() { return this->score; }

    inline Ninja* getPlayer() { return this->player; }

    inline FruitManager* getFruitManager() { return this->fruitManager; }

    inline CompostManager* getCompostManager() { return this->compostManager; }
};

extern GamePlayScreen _gamePlayScreen;

#endif //GAME_PLAY_SCREEN_INCLUDED