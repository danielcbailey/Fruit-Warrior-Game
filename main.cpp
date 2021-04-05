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

UnbufferedSerial pc(USBTX, USBRX, 9600);

//ScreenManager _screenManager(&_titleScreen);

//Testing:
#include "screens/gamePlayScreen.hpp"

ScreenManager _screenManager(&_gamePlayScreen);
//End testing

int main() {
    //Initializing the graphics

    uLCD lcd(P0_15, P0_16, p25, uLCD::BAUD_1500000);

    GraphicsController controller(&lcd);

    //Starting the game
    _screenManager.gameStart();

    GraphicsElement::ElementContext context;
    context.Image.layer = 5;
    context.Image.width = apple_0.getWidth();
    context.Image.height = apple_0.getHeight();
    context.Image.x = 20;
    context.Image.y = 20;
    context.Image.image = &apple_0;

    GraphicsElement::ElementContext context2;
    context2.Image.layer = 6;
    context2.Image.width = orange_2.getWidth();
    context2.Image.height = orange_2.getHeight();
    context2.Image.x = 20;
    context2.Image.y = 100;
    context2.Image.image = &orange_2;

    GraphicsElement::ElementContext backCtx;
    backCtx.Image.layer = 0;
    backCtx.Image.x = 0;
    backCtx.Image.y = 0;
    backCtx.Image.width = backgroundGame_0.getWidth();
    backCtx.Image.height = backgroundGame_0.getHeight();
    backCtx.Image.image = &backgroundGame_0;

    GraphicsElement testElement(GraphicsElement::ElementType::ELEMENT_IMAGE, context);
    GraphicsElement testElement2(GraphicsElement::ElementType::ELEMENT_IMAGE, context2);
    GraphicsElement background(GraphicsElement::ElementType::ELEMENT_IMAGE, backCtx);

    controller.registerGraphicsElement(&testElement);

    printfdbg("test0\n");

    controller.registerGraphicsElement(&testElement2);

    controller.registerGraphicsElement(&background);

    printfdbg("test\n");

    DigitalOut led(P1_23);

    Bomb testBomb;

    int direction = 1;
    int pos = 20;
    int count = 0;
    while (true) {
        if (direction == 1 && pos >= 100) {
            direction = -1;
        } else if (direction == -1 && pos <= 20) {
            direction = 1;
        }

        pos += direction;

        testElement.getContext()->Image.y = pos;
        testElement2.getContext()->Image.y = 130 - pos;

        controller.updateGraphicsElement(&testElement2);
        controller.updateGraphicsElement(&testElement);

        wait_us(100000);
        _tickManager.dispatchGameTick(0.1f);
        count++;
    }
}