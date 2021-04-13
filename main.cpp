/*
 * Main file
 * 
 * The entry point of the game, however, no game logic is implemented here, just
 * the game engine initialization. All game logic is conducted through the game
 * engine, which is distributed in several dozen files.
 *
 * A bit messy for the demo but whatever
 * Requires joystick and lcd connected to different pins than HW 3 shows.
 *
 * In case you are wondering: currently just the code for the mbed is 3,800+ lines of code
 * not including the asset data. The uLCD emulator is an additional 1,068 lines.
 *
 * (c) Daniel Cooper 2021
 */

#include "mbed.h"
#include "graphics/bitmapImage.hpp"
#include "graphics/uLCD.hpp"
#include "graphics/graphicsController.hpp"
#include "assets/assets.hpp"
#include "globals.hpp"
#include "game/sprites/bomb.hpp"
#include "graphics/gameTick.hpp"

#include "core/screenManager.hpp"
#include "screens/titleScreen.hpp"

#include "game/inputs.hpp"

UnbufferedSerial pc(USBTX, USBRX, 9600);

//ScreenManager _screenManager(&_titleScreen);

//Testing:
#include "screens/gamePlayScreen.hpp"

ScreenManager _screenManager(&_gamePlayScreen);
//End testing

int main() {
    //Initializing the graphics

    //Adding inputs
    registerInputs();

    uLCD lcd(P0_15, P0_16, p25, uLCD::BAUD_1500000);

    GraphicsController controller(&lcd);

    //Starting the game
    _screenManager.gameStart();

    //DigitalOut led(P1_23);
    bool reportedFps = false;

    while (true) {
        _screenManager.onMainLoop();
        
        if ((int)_screenManager.getTotalTime() % 10 == 9) {
            if (!reportedFps) {
                printfdbg("fps:%f\n", _screenManager.getFPS());
                reportedFps = true;
            }
        } else {
            reportedFps = false;
        }
    }
}