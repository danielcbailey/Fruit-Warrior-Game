

#include "titleScreen.hpp"
#include "assets/assets.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"

void TitleScreen::onCreation() {
    this->background = new GraphicsElement(&idleBackground_0, 0, 0);
    this->samuraiLeft = new GraphicsElement(&samuraiLogo_0, 9, 55);
    this->samuraiRight = new GraphicsElement(&samuraiLogo_1, 66, 55);
    this->fruitLogo = new GraphicsElement(&fruitLogo_0, 12, 24);
    this->startButton = new GraphicsElement(&startButton_0, 28, 98);
    this->highScoresButton = new GraphicsElement(&highScoresButton_0, 68, 98);

    this->background->getContext()->Image.layer = 0;
    this->samuraiLeft->getContext()->Image.layer = 110;
    this->samuraiRight->getContext()->Image.layer = 110;
    this->fruitLogo->getContext()->Image.layer = 110;
    this->startButton->getContext()->Image.layer = 110;
    this->highScoresButton->getContext()->Image.layer = 110;

    _globalGraphics->registerGraphicsElement(this->background);
    _globalGraphics->registerGraphicsElement(this->fruitLogo);
    _globalGraphics->registerGraphicsElement(this->samuraiLeft);
    _globalGraphics->registerGraphicsElement(this->samuraiRight);
    _globalGraphics->registerGraphicsElement(this->startButton);
    _globalGraphics->registerGraphicsElement(this->highScoresButton);

    _gamePlayScreen.setSpawnInterval(5);
    
    this->fruitManager = new FruitManager();
}

void TitleScreen::onDestruction() {
    _globalGraphics->removeGraphicsElement(this->background);
    _globalGraphics->removeGraphicsElement(this->samuraiLeft);
    _globalGraphics->removeGraphicsElement(this->samuraiRight);
    _globalGraphics->removeGraphicsElement(this->fruitLogo);
    _globalGraphics->removeGraphicsElement(this->startButton);
    _globalGraphics->removeGraphicsElement(this->highScoresButton);

    delete(this->fruitManager);
}

void TitleScreen::onGameLoop(float dt) {

}

TitleScreen _titleScreen;