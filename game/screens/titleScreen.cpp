

#include "titleScreen.hpp"
#include "assets/assets.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"
#include <functional>
#include <string>
#include "game/inputs.hpp"
#include "core/screenManager.hpp"
#include "game/highscore.hpp"
#include "globals.hpp"

void TitleScreen::onCreation() {
    
    this->background = new GraphicsElement(&idleBackground_0, 0, 0);
    this->background->getContext()->Image.layer = 0;
    _globalGraphics->registerGraphicsElement(this->background);

    this->initialized = false;
    this->setMode(false);

    _gamePlayScreen.setSpawnInterval(5);
    
    this->fruitManager = new FruitManager();
    this->joyEvtHandle = _joystick->attach(std::bind(&TitleScreen::handleJoy, this, placeholders::_1, placeholders::_2));
}

void TitleScreen::onDestruction() {

    _joystick->detach(this->joyEvtHandle);

    if (mode) {
        //Is in highscore mode

    } else {
        _globalGraphics->removeGraphicsElement(this->samuraiLeft);
        _globalGraphics->removeGraphicsElement(this->samuraiRight);
        _globalGraphics->removeGraphicsElement(this->fruitLogo);

        delete(this->samuraiLeft);
        delete(this->samuraiRight);
        delete(this->fruitLogo);
        delete(this->startButton);
        delete(this->highScoresButton);
    }

    delete(this->fruitManager);
    _globalGraphics->removeGraphicsElement(this->background);
    delete(this->background);
}

void TitleScreen::onGameLoop(float dt) {

}

void TitleScreen::setMode(bool isHighScores) {
    if (isHighScores == this->mode && this->initialized) {
        return; //No change
    }
    this->mode = isHighScores;
    if (isHighScores) {
        _globalGraphics->removeGraphicsElement(this->samuraiLeft);
        _globalGraphics->removeGraphicsElement(this->samuraiRight);
        _globalGraphics->removeGraphicsElement(this->fruitLogo);

        delete(this->startButton);
        delete(this->highScoresButton);
        delete(this->samuraiLeft);
        delete(this->samuraiRight);
        delete(this->fruitLogo);

        //Creating high scores section
        this->highScoreTitle = new GraphicsElement(&highScoreText_0, 7, 6);
        this->highScoreTitle->getContext()->Image.layer = 110;
        _globalGraphics->registerGraphicsElement(this->highScoreTitle);

        this->backButton = new GUIButton(&backButton_0, 47, 27, std::bind(&TitleScreen::back, this, placeholders::_1, placeholders::_2), 3);
        this->backButton->setSelected(true);

        //Now rendering text
        auto top = HighScores::getTopFive();
        int y = 46;

        for (auto it = top.begin(); it != top.end(); ++it) {
            if (it->name == "") {
                break;
            }

            RenderedText* name = new RenderedText(it->name, (128 - it->name.length() * 6) / 2, y);
            this->text.push_back(name);
            std::string scoreStr = std::to_string(it->score);
            RenderedText* score = new RenderedText(scoreStr, (128 - scoreStr.length() * 6) / 2, y + 8);
            this->text.push_back(score);

            y += 16;
        }

    } else {
        if (this->initialized) {
            //Implies that it does have to remove the high score stuff
            _globalGraphics->removeGraphicsElement(this->highScoreTitle);
            delete(this->backButton);
            delete(this->highScoreTitle);

            for (auto it = this->text.begin(); it != this->text.end(); ++it) {
                delete(*it);
            }

            this->text.clear();
        } else {
            this->initialized = true;
        }

        this->samuraiLeft = new GraphicsElement(&samuraiLogo_0, 9, 55);
        this->samuraiRight = new GraphicsElement(&samuraiLogo_1, 66, 55);
        this->fruitLogo = new GraphicsElement(&fruitLogo_0, 12, 24);

        this->samuraiLeft->getContext()->Image.layer = 110;
        this->samuraiRight->getContext()->Image.layer = 110;
        this->fruitLogo->getContext()->Image.layer = 110;

        _globalGraphics->registerGraphicsElement(this->fruitLogo);
        _globalGraphics->registerGraphicsElement(this->samuraiLeft);
        _globalGraphics->registerGraphicsElement(this->samuraiRight);

        this->startButton = new GUIButton(&startButton_0, 28, 98, std::bind(&TitleScreen::play, this, placeholders::_1, placeholders::_2), 5);

        this->highScoresButton = new GUIButton(&highScoresButton_0, 68, 98, std::bind(&TitleScreen::showHighScores, this, placeholders::_1, placeholders::_2), 7);

        this->startButton->setSelected(true);
    }
}

bool TitleScreen::play(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*)evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        _screenManager.changeScreen(&_gamePlayScreen);
    }
    return false;
}

bool TitleScreen::back(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*)evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        this->setMode(false);
    }
    return false;
}

bool TitleScreen::showHighScores(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*)evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        this->setMode(true);
    }
    return false;
}

bool TitleScreen::handleJoy(void* evtDetails, float dt) {
    JoystickInput::JoystickEvent evt = *((JoystickInput::JoystickEvent*) evtDetails);

    if (this->mode) {
        //No joy input for high scores screen
        return false;
    }

    if (evt.direction == JoystickInput::JOYSTICK_RIGHT && !this->highScoresButton->isSelected()) {
        this->highScoresButton->setSelected(true);
        this->startButton->setSelected(false);
    } else if (evt.direction == JoystickInput::JOYSTICK_LEFT && !this->startButton->isSelected()) {
        this->startButton->setSelected(true);
        this->highScoresButton->setSelected(false);
    }

    return false;
}

TitleScreen _titleScreen;