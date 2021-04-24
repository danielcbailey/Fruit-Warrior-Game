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
#include "core/files.hpp"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "screens/titleScreen.hpp"

#include "game/inputs.hpp"
#include "screens/introScreen.hpp"

UnbufferedSerial pc(USBTX, USBRX, 115200);

//ScreenManager _screenManager(&_titleScreen);

//Testing:
#include "screens/gamePlayScreen.hpp"
#include "audio/audioPlayer.hpp"
#include "audio/audioManager.hpp"

#include "serialAsync.hpp"

ScreenManager _screenManager(&_introScreen);
//End testing

int main() {
    //Adding inputs
    registerInputs();

    //Creating the global file system
    SDBlockDevice sdCard(p5, p6, p7, p8, 12000000);

    _globalFileSystem = new FATFileSystem("fs", &sdCard);

    AudioManager audioMgr;

    uLCD lcd(P0_15, P0_16, p25, uLCD::BAUD_1500000);

    //Testing audio system

    AudioPlayer test("titleScreen.wav");

    test.play(200);

    GraphicsController controller(&lcd);
    controller.setGlobalBoundary({0, 0, 127, 127});

    //Starting the game
    _screenManager.gameStart();

    //DigitalOut led(P1_23);
    bool reportedFps = false;

    while (true) {
        _screenManager.onMainLoop();
       // _audioManager->updateBuffers(); // Repeated here in case the display doesn't have an update in several frames
        
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

FileSystem* _globalFileSystem;