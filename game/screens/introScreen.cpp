

#include "introScreen.hpp"
#include "assets/assets.hpp"
#include <cstdint>
#include "core/screenManager.hpp"
#include "screens/titleScreen.hpp"
//#include "screens/gamePlayScreen.hpp"
#include "graphics/graphicsController.hpp"
#include "graphics/colors.hpp"
#include "game/soundManager.hpp"
#include "globals.hpp"

IntroScreen _introScreen;

void IntroScreen::onCreation() {
    this->left = nullptr;
    this->right = nullptr;
    _soundManager.addMusic("sounds/titleScreen.wav");
}

void IntroScreen::onDestruction() {
    if (left) {
        _globalGraphics->removeGraphicsElement(left);
    }
    if (right) {
        _globalGraphics->removeGraphicsElement(right);
    }
}

void IntroScreen::onGameLoop(float dt) {
    //Dynamically creating a color LUT
    ColorLUT* original = studioLogo_0.getColorLUT();

    float t = this->getTotalTime();

    if (t >= 6) {
        _screenManager.changeScreen(&_titleScreen);
        return;
    }

    //The transparency will be quadratic.
    float transparency = -t * (t - 6) / 9;

    uint16_t lutArr[] = {0, Colors::interpColor(0, original->lut[1], transparency), 
                         Colors::interpColor(0, original->lut[2], transparency), 
                         Colors::interpColor(0, original->lut[3], transparency)};

    ColorLUT lut = ColorLUT(lutArr);

    BitmapImage tempImg1(studioLogo_0.getRawBuffer(), 40, 35, &lut);
    BitmapImage tempImg2(studioLogo_1.getRawBuffer(), 40, 35, &lut);

    GraphicsElement* left = this->left;
    GraphicsElement* right = this->right;
    if (left == nullptr) {
        left = new GraphicsElement(&tempImg1, 24, 46);
        _globalGraphics->registerGraphicsElement(left);
        this->left = left;
    } else {
        left->getContext()->Image.image = &tempImg1;
        _globalGraphics->updateGraphicsElement(left);
    }
    if (right == nullptr) {
        right = new GraphicsElement(&tempImg2, 64, 46);
        _globalGraphics->registerGraphicsElement(right);
        this->right = right;
    } else {
        right->getContext()->Image.image = &tempImg2;
        _globalGraphics->updateGraphicsElement(right);
    }
}