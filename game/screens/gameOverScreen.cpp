

#include "gameOverScreen.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"
#include "game/highscore.hpp"
#include "game/keyboard.hpp"

void GameOverScreen::onCreation() {
    uLCD* lcd = _globalGraphics->getLCD();
    lcd->cls();
    lcd->locate(5, 1);
    lcd->printf("GAME OVER");
    lcd->locate(4, 2);
    lcd->printf("Your Score:");
    lcd->locate(8, 3);
    lcd->printf("%d", _gamePlayScreen.getScore());

    //checking for high score
    int pos = HighScores::checkScoreIndex(_gamePlayScreen.getScore());

    if (pos >= 0) {
        lcd->locate(0, 5);
        lcd->printf("Your score rank:");
        lcd->locate(8, 6);
        lcd->printf("%d", _gamePlayScreen.getScore());
        lcd->locate(0, 7);
        lcd->printf("Type your name:");

        std::string name = Keyboard::keyboardInput("");
        if (name == "") {
            name = "Anonymous";
        }

        HighScores::insertScore(name, _gamePlayScreen.getScore());
    }
}

void GameOverScreen::onDestruction() {

}

void GameOverScreen::onGameLoop(float dt) {

}

GameOverScreen _gameOverScreen;