

#include "gamePlayScreen.hpp"
#include "assets/assets.hpp"
#include "graphics/graphicsController.hpp"
#include "sprites/knife.hpp"
#include "core/screenManager.hpp"
#include "screens/gameWinScreen.hpp"

void GamePlayScreen::onCreation() {
    //Registering the background
    this->background = new GraphicsElement(&backgroundGame_0, 0, 0);
    _globalGraphics->registerGraphicsElement(this->background);

    this->fruitManager = new FruitManager();
    this->compostManager = new CompostManager();

    //default fruit spawn interval
    this->spawnInterval = 5;

    this->player = new Ninja();
    this->knife = new Knife();
}

void GamePlayScreen::onDestruction() {
    _globalGraphics->removeGraphicsElement(this->background);
    delete(this->background);
    delete(this->fruitManager);
    delete(this->compostManager);
    delete(this->player);
    delete(this->knife);
}

void GamePlayScreen::onGameLoop(float dt) {
    if (this->score >= 10) {
        //the game has been won
        _screenManager.changeScreen(&_gameWinScreen);
    }
}

void GamePlayScreen::redrawStatusBar() {
    //Currently only draws the points
    uLCD* lcd = _globalGraphics->getLCD();
    lcd->drawRectangleFilled(0, 0, 127, 7, 0x0);
    lcd->locate(0, 0);
    lcd->printf("Score: %d", this->score);
}

GamePlayScreen _gamePlayScreen;