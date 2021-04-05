

#include "gamePlayScreen.hpp"
#include "assets/assets.hpp"
#include "graphics/graphicsController.hpp"

void GamePlayScreen::onCreation() {
    //Registering the background
    this->background = new GraphicsElement(&backgroundGame_0, 0, 0);
    _globalGraphics->registerGraphicsElement(this->background);

    this->fruitManager = new FruitManager();
}

void GamePlayScreen::onDestruction() {
    _globalGraphics->removeGraphicsElement(this->background);
    delete(this->background);
    delete(this->fruitManager);
}

void GamePlayScreen::onGameLoop(float dt) {

}

GamePlayScreen _gamePlayScreen;