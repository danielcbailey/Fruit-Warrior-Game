#include "gameWinScreen.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"

void GameWinScreen::onCreation() {
    uLCD* lcd = _globalGraphics->getLCD();
    lcd->cls();
    lcd->locate(5, 1);
    lcd->printf("YOU WON!");
}

void GameWinScreen::onDestruction() {

}

void GameWinScreen::onGameLoop(float dt) {

}

GameWinScreen _gameWinScreen;