

#include "gamePlayScreen.hpp"
#include "assets/assets.hpp"
#include "graphics/graphicsController.hpp"
#include "sprites/knife.hpp"
#include "core/screenManager.hpp"
#include "screens/gameWinScreen.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>
#include "inputs/buttonInput.hpp"
#include "game/inputs.hpp"
#include "globals.hpp"
#include "game/soundManager.hpp"

void GamePlayScreen::onCreation() {
    //Registering the background
    this->background = new GraphicsElement(&backgroundGame_0, 0, 0);
    _globalGraphics->registerGraphicsElement(this->background);
    _sliceablePlayMode = true;

    this->fruitManager = new FruitManager();
    
    this->compostManager = new CompostManager();

    //default fruit spawn interval
    this->spawnInterval = 3;
    this->score = 0;

    this->player = new Ninja();
    this->knife = new Knife();
    this->comboMultiplier = 1;
    this->paused = false;
    this->buttonEvtHandle = _button2->attach(std::bind(&GamePlayScreen::handleButton, this, placeholders::_1, placeholders::_2));
    this->scoreText = new RenderedText("Score: 0", 4, 1);

    _soundManager.clearMusic();
    _soundManager.addMusic("sounds/gameplay4.wav");
    _soundManager.addMusic("sounds/gameplay3.wav");
    _soundManager.addMusic("sounds/gameplay2.wav");
    _soundManager.addMusic("sounds/gameplay1.wav");
}

void GamePlayScreen::onDestruction() {
    _globalGraphics->removeGraphicsElement(this->background);
    _sliceablePlayMode = false;
    delete(this->background);
    delete(this->fruitManager);
    delete(this->compostManager);
    delete(this->player);
    delete(this->knife);
    delete(this->scoreText);
    _button2->detach(this->buttonEvtHandle);
}

void GamePlayScreen::onGameLoop(float dt) {
    
}

void GamePlayScreen::redrawStatusBar() {
    //Generating text
    std::string text = "Score: ";
    text += std::to_string(this->score);
    this->scoreText->setText(text);
}

bool GamePlayScreen::handleButton(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*) evtDetails);

    if (evt.eventType == ButtonInput::BUTTON_PRESS) {
        if (this->paused) {
            //Resuming
            printfdbg("Resuming.\n");
            this->resume();
        } else {
            //Pausing
            printfdbg("Pausing.\n");
            this->pause();
        }
    }

    return false;
}

float* GamePlayScreen::genGaussianKernel(int kw, int kh) {
    float sigmaSq = 2.0;
    float twoPi = 6.28319f;
    float E = 2.71828f;

    float* kernel = (float*) malloc(kw * kh * sizeof(float));

    float* kc = kernel;
    float sum = 0;

    for (int y = 0 - kh / 2; kh / 2 >= y; y++) {
        for (int x = 0 - kw / 2; kw / 2 >= x; x++) {
            //The gaussian function
            *kc = (1 / sqrt(twoPi * sigmaSq)) * pow(E, -((x * x) + (y * y)) / (2 * sigmaSq));
            sum += *(kc++);
        }
    }

    kc = kernel;

    //Normalizing values
    for (int y = 0 - kh / 2; kh / 2 >= y; y++) {
        for (int x = 0 - kw / 2; kw / 2 >= x; x++) {
            *(kc++) /= sum;
        }
    }
    

    return kernel;
}

uint16_t* GamePlayScreen::blurSection(int xs, int ys, int width, int height, float* kernel, int kw, int kh) {

    uint16_t* ret = (uint16_t*) malloc(width * height * sizeof(uint16_t));

    uint16_t* rc = ret;
    
    //Prefetches all pixel samples to reduce duplicate sampling
    int prefetchWidth = width + (kw / 2) * 2;
    int prefetchHeight = height + (kh / 2) * 2;
    uint16_t* samples = (uint16_t*) malloc(prefetchWidth * prefetchHeight * sizeof(uint16_t));

    uint16_t* sc = samples;
    for (int y = ys - kh / 2; height + ys + kh / 2 > y; y++) {
        for (int x = xs - kw / 2; width + xs + kw / 2 > x; x++) {
            *(sc++) = _globalGraphics->samplePixel(x, y, 10000); //10000 is pretty much infinity for these purposes
        }
    }

    //yeah.. the big O on this one isn't that hot...
    for (int y = ys; height + ys > y; y++) {
        for (int x = xs; width + xs > x; x++) {
            uint32_t red = 0;
            uint32_t green = 0;
            uint32_t blue = 0;
            float* kc = kernel;

            for (int yss = y - kh / 2; y + kh / 2 >= yss; yss++) {
                for (int xss = x - kw / 2; x + kw / 2 >= xss; xss++) {
                    //Obtaining the sample
                    int index = ((yss - (ys - (kh >> 1))) * prefetchWidth + (xss - (xs - (kw >> 1))));

                    uint16_t sample = samples[index];

                    //Breaking it up into its components
                    char redv = (sample & 0xF8) >> 3;
                    char greenv = ((sample & 0x7) << 3) | ((sample & 0xE000) >> 13);
                    char bluev = (sample & 0x1F00) >> 8;

                    //Applying the kernel
                    uint32_t kvi = ((*kc) * 1048576);
                    red += redv * kvi;
                    green += greenv * kvi;
                    blue += bluev * kvi;

                    kc++;
                }
            }

            red = red >> 20;
            green = green >> 20;
            blue = blue >> 20;

            //Now to store the result in the resulting image
            uint8_t colorLower = ((((uint8_t) green) & 0x7) << 5) | (((uint8_t) blue) & 0x1F);
            uint8_t colorUpper = ((((uint8_t) red) & 0x1F) << 3) | ((((uint8_t) green) & 0x38) >> 3);

            *(rc++) = ((uint16_t) colorLower << 8) | colorUpper;
        }
    }

    free(samples);

    return ret;
}

void GamePlayScreen::pause() {
    this->paused = true;
    //Blurring the screen
    //Breaks screen into 16 32x32 px blocks

    float* kernel = this->genGaussianKernel(9, 9);

    //Printing the kernel for testing
    float* kc = kernel;
    printfdbg("Kernel:\n");
    for (int y = 0; 9 > y; y++) {
        for (int x = 0; 9 > x; x++) {
            printfdbg(" %f", *(kc++));
        }
        printfdbg("\n");
    }

    for (int by = 0; 4 > by; by++) {
        for (int bx = 0; 4 > bx; bx++) {

            uint16_t* section = this->blurSection(bx * 32, by * 32, 32, 32, kernel, 9, 9);

            _globalGraphics->getLCD()->BLIT(bx * 32, by * 32, 32, 32, section, true);
        }
    }

    _globalGraphics->getLCD()->drawCircle(63, 63, 34, 0xFFFF);
    _globalGraphics->getLCD()->drawRectangleFilled(53, 47, 59, 79, 0xFFFF);
    _globalGraphics->getLCD()->drawRectangleFilled(68, 47, 74, 79, 0xFFFF);

    _screenManager.setPaused(true);
}

void GamePlayScreen::resume() {
    this->paused = false;

    _globalGraphics->refresh();
    this->redrawStatusBar();

    _screenManager.setPaused(false);
}

GamePlayScreen _gamePlayScreen;