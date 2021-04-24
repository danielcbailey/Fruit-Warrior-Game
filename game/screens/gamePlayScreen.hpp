

#ifndef GAME_PLAY_SCREEN_INCLUDED
#define GAME_PLAY_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"
#include "game/fruitManager.hpp"
#include "sprites/ninja.hpp"
#include "game/compostManager.hpp"
#include <cstdint>
#include "graphics/renderedText.hpp"

class Knife;

#define COMPOST_MIN_Y 70

class GamePlayScreen : public GameScreen {
    private:

    RenderedText* scoreText;
    GraphicsElement* background;
    FruitManager* fruitManager;
    CompostManager* compostManager;
    int spawnInterval;
    int score;
    Ninja* player;
    Knife* knife;
    bool paused;
    int buttonEvtHandle;
    int comboMultiplier;

    bool handleButton(void* evtDetails, float dt);

    float* genGaussianKernel(int kw, int kh);

    uint16_t* blurSection(int xs, int ys, int width, int height, float* kernel, int kw, int kh);

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);

    //Specific functions
    int getSpawnInterval() { return this->spawnInterval; }

    void redrawStatusBar();

    void pause();

    void resume();

    inline bool isPaused() { return this->paused; }

    inline int getComboMultiplier() { return this->comboMultiplier; }

    inline void setComboMultiplier(int value) { this->comboMultiplier = value; }

    inline void setSpawnInterval(int value) { this->spawnInterval = value; }

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