

#include "gameOverScreen.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"

void GameOverScreen::onCreation() {
    uLCD* lcd = _globalGraphics->getLCD();
    lcd->cls();
    lcd->locate(5, 1);
    lcd->printf("GAME OVER");
    lcd->locate(4, 2);
    lcd->printf("Your Score:");
    lcd->locate(8, 5);
    lcd->printf("%d", _gamePlayScreen.getScore());
}

void GameOverScreen::onDestruction() {

}

void GameOverScreen::onGameLoop(float dt) {

}

GameOverScreen _gameOverScreen;