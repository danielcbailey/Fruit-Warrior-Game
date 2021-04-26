/*
 * Keyboard Class
 *
 * The keyboard class creates an on-screen keyboard.
 * The keyboard contains its own text entry box and returns the entered
 * text when the static function returns. The keyboard can be preloaded
 * with a string.
 *
 * All preloaded and typed content must remain under 18 characters.
 *
 * (c) 2021 Daniel Cooper
 */

#include "keyboard.hpp"
#include "inputs.hpp"
#include <functional>
#include "graphics/graphicsController.hpp"
#include "graphics/uLCD.hpp"

/*
 * Keyboard layout
 * ------------------
 * Text Entry Here#
 * ------------------
 * 1 2 3 4 5 6 7 8 90  | ! @ # $ % ^ & * ()
 * q w e r t y u i op  | Q W E R T Y U I OP
 *  a s d f g h j k l  |  A S D F G H J K L
 *   z x c v b n m <-  |   Z X C V B N M <-
 * Shift Space Enter   | Shfit Space Enter
 */

const Keyboard::KeyboardNode Keyboard::node00 = {0, 0, '1', '!', nullptr, &node01, nullptr, &node10};
const Keyboard::KeyboardNode Keyboard::node01 = {0, 2, '2', '@', &node00, &node02, nullptr, &node11};
const Keyboard::KeyboardNode Keyboard::node02 = {0, 4, '3', '#', &node01, &node03, nullptr, &node12};
const Keyboard::KeyboardNode Keyboard::node03 = {0, 6, '4', '$', &node02, &node04, nullptr, &node13};
const Keyboard::KeyboardNode Keyboard::node04 = {0, 8, '5', '%', &node03, &node05, nullptr, &node14};
const Keyboard::KeyboardNode Keyboard::node05 = {0, 10, '6', '^', &node04, &node06, nullptr, &node15};
const Keyboard::KeyboardNode Keyboard::node06 = {0, 12, '7', '&', &node05, &node07, nullptr, &node16};
const Keyboard::KeyboardNode Keyboard::node07 = {0, 14, '8', '*', &node06, &node08, nullptr, &node17};
const Keyboard::KeyboardNode Keyboard::node08 = {0, 16, '9', '(', &node07, &node09, nullptr, &node18};
const Keyboard::KeyboardNode Keyboard::node09 = {0, 17, '0', ')', &node08, nullptr, nullptr, &node19};

const Keyboard::KeyboardNode Keyboard::node10 = {1, 0, 'q', 'Q', nullptr, &node11, &node00, &node20};
const Keyboard::KeyboardNode Keyboard::node11 = {1, 2, 'w', 'W', &node10, &node12, &node01, &node21};
const Keyboard::KeyboardNode Keyboard::node12 = {1, 4, 'e', 'E', &node11, &node13, &node02, &node22};
const Keyboard::KeyboardNode Keyboard::node13 = {1, 6, 'r', 'R', &node12, &node14, &node03, &node23};
const Keyboard::KeyboardNode Keyboard::node14 = {1, 8, 't', 'T', &node13, &node15, &node04, &node24};
const Keyboard::KeyboardNode Keyboard::node15 = {1, 10, 'y', 'Y', &node14, &node16, &node05, &node25};
const Keyboard::KeyboardNode Keyboard::node16 = {1, 12, 'u', 'U', &node15, &node17, &node06, &node26};
const Keyboard::KeyboardNode Keyboard::node17 = {1, 14, 'i', 'I', &node16, &node18, &node07, &node27};
const Keyboard::KeyboardNode Keyboard::node18 = {1, 16, 'o', 'O', &node17, &node19, &node08, &node28};
const Keyboard::KeyboardNode Keyboard::node19 = {1, 17, 'p', 'P', &node18, nullptr, &node09, &node28};

const Keyboard::KeyboardNode Keyboard::node20 = {2, 1, 'a', 'A', nullptr, &node21, &node10, &node30};
const Keyboard::KeyboardNode Keyboard::node21 = {2, 3, 's', 'S', &node20, &node22, &node11, &node31};
const Keyboard::KeyboardNode Keyboard::node22 = {2, 5, 'd', 'D', &node21, &node23, &node12, &node32};
const Keyboard::KeyboardNode Keyboard::node23 = {2, 7, 'f', 'F', &node22, &node24, &node13, &node33};
const Keyboard::KeyboardNode Keyboard::node24 = {2, 9, 'g', 'G', &node23, &node25, &node14, &node34};
const Keyboard::KeyboardNode Keyboard::node25 = {2, 11, 'h', 'H', &node24, &node26, &node15, &node35};
const Keyboard::KeyboardNode Keyboard::node26 = {2, 13, 'j', 'J', &node25, &node27, &node16, &node36};
const Keyboard::KeyboardNode Keyboard::node27 = {2, 15, 'k', 'K', &node26, &node28, &node17, &nodeBack};
const Keyboard::KeyboardNode Keyboard::node28 = {2, 17, 'l', 'L', &node27, nullptr, &node19, &nodeBack};

const Keyboard::KeyboardNode Keyboard::node30 = {3, 2, 'z', 'Z', nullptr, &node31, &node20, &nodeShift};
const Keyboard::KeyboardNode Keyboard::node31 = {3, 4, 'x', 'X', &node30, &node32, &node21, &nodeShift};
const Keyboard::KeyboardNode Keyboard::node32 = {3, 6, 'c', 'C', &node31, &node33, &node22, &nodeSpace};
const Keyboard::KeyboardNode Keyboard::node33 = {3, 8, 'v', 'V', &node32, &node34, &node23, &nodeSpace};
const Keyboard::KeyboardNode Keyboard::node34 = {3, 10, 'b', 'B', &node33, &node35, &node24, &nodeSpace};
const Keyboard::KeyboardNode Keyboard::node35 = {3, 12, 'n', 'N', &node34, &node36, &node25, &nodeEnter};
const Keyboard::KeyboardNode Keyboard::node36 = {3, 14, 'm', 'M', &node35, &nodeBack, &node26, &nodeEnter};
const Keyboard::KeyboardNode Keyboard::nodeBack = {3, 16, 8, 8, &node36, nullptr, &node28, &nodeEnter};

const Keyboard::KeyboardNode Keyboard::nodeShift = {4, 0, 15, 14, nullptr, &nodeSpace, &node30, nullptr};
const Keyboard::KeyboardNode Keyboard::nodeSpace = {4, 6, ' ', ' ', &nodeShift, &nodeEnter, &node33, nullptr};
const Keyboard::KeyboardNode Keyboard::nodeEnter = {4, 12, '\n', '\n', &nodeSpace, nullptr, &node36, nullptr};

const Keyboard::KeyboardNode Keyboard::nodes[] = {node00, node01, node02, node03, node04, node05, node06, node07, node08, node09,
                                                  node10, node11, node12, node13, node14, node15, node16, node17, node18, node19,
                                                  node20, node21, node22, node23, node24, node25, node26, node27, node28,
                                                  node30, node31, node32, node33, node34, node35, node36, nodeBack,
                                                  nodeShift, nodeSpace, nodeEnter};

Keyboard::KeyboardNode* Keyboard::currentNode = nullptr;
std::string Keyboard::currentText = "";
volatile bool Keyboard::done = false;
bool Keyboard::shift = false;
bool Keyboard::joyActive = false;
bool Keyboard::keyboardRendered = false;
bool Keyboard::entryRendered = false;

void Keyboard::renderKeyboard() {
    //Making a rectangle where the keyboard occupies the screen
    uLCD* lcd = _globalGraphics->getLCD();
    if (!keyboardRendered) {
        lcd->drawRectangleFilled(0, 88, 127, 127, 53908);
        keyboardRendered = true;
    }

    lcd->setTextColor(0x0);
    
    for (const KeyboardNode node: nodes) {
        //Setting cursor to location
        lcd->locate(node.x, node.y + 11);

        //Testing if the node is the current node
        if (currentNode->x == node.x && currentNode->y == node.y) {
            //This is the current node, must set the background to match it
            lcd->setTextBackground(0xFFFF);
        } else {
            lcd->setTextBackground(53908);
        }

        //Testing for the special cases
        switch (node.lowerValue) {
        case ' ':
            //Space
            lcd->print((char*)"Space");
            break;
        case '\n':
            //Enter
            lcd->print((char*)"Enter");
            break;
        case 15:
            //Shift
            lcd->print((char*)"Shift");
            break;
        case 8:
            //Backspace
            lcd->print((char*)"<-");
            break;
        default:
            //Everything else

            if (shift) {
                lcd->print(node.shiftValue);
            } else {
                lcd->print(node.lowerValue);
            }

            break;
        }
    }

    //Just restoring it to the default
    lcd->setTextBackground(0x0);
    lcd->setTextColor(0xFFFF);
}

void Keyboard::renderKeyboardTextEntry() {
    //Making a rectangle where the keyboard occupies the screen
    uLCD* lcd = _globalGraphics->getLCD();
    if (!entryRendered) {
        lcd->drawRectangleFilled(0, 64, 127, 87, 53908);
        lcd->drawRectangle(0, 68, 127, 68, 0xFFFF);
        lcd->drawRectangle(0, 83, 127, 83, 0xFFFF);
        entryRendered = true;
    }

    lcd->setTextBackground(53908);

    lcd->locate(0, 9);

    lcd->print((char*) (currentText + ' ').c_str());
    lcd->setTextBackground(0x0);
    lcd->setTextColor(0xFFFF);
}

bool Keyboard::onButton(void* evtDetails, float dt) {
    ButtonInput::ButtonEvent evt = *((ButtonInput::ButtonEvent*) evtDetails);

    if (evt.eventType != ButtonInput::BUTTON_PRESS) {
        return false;
    }

    //Testing for special cases
    switch (currentNode->lowerValue) {
    case ' ':
        //Space
        if (currentText.length() >= 18) return false;
        currentText += ' ';
        return false; //No need to render a space as it is invisible
    case '\n':
        //Enter
        done = true;
        return false;
    case 15:
        //Shift
        shift = !shift;
        renderKeyboard();
        return false;
    case 8:
        //Backspace
        if (currentText.length() > 0) {
            currentText = currentText.substr(0, currentText.length() - 1);
        }
        break;
    default:
        //Everything else

        if (currentText.length() >= 18) return false;

        if (shift) {
            currentText += currentNode->shiftValue;
        } else {
            currentText += currentNode->lowerValue;
        }

        break;
    }

    renderKeyboardTextEntry();

    return false;
}

bool Keyboard::onJoyInput(void* evtDetails, float dt) {
    JoystickInput::JoystickEvent evt = *((JoystickInput::JoystickEvent*) evtDetails);

    switch (evt.direction) {
    case JoystickInput::JOYSTICK_LEFT:
        if (currentNode->left && !joyActive) {
            currentNode = (KeyboardNode*) currentNode->left;
            joyActive = true;
        }
        break;
    case JoystickInput::JOYSTICK_RIGHT:
        if (currentNode->right && !joyActive) {
            currentNode = (KeyboardNode*) currentNode->right;
            joyActive = true;
        }
        break;
    case JoystickInput::JOYSTICK_UP:
        if (currentNode->up && !joyActive) {
            currentNode = (KeyboardNode*) currentNode->up;
            joyActive = true;
        }
        break;
    case JoystickInput::JOYSTICK_DOWN:
        if (currentNode->down && !joyActive) {
            currentNode = (KeyboardNode*) currentNode->down;
            joyActive = true;
        }
        break;
    case JoystickInput::JOYSTICK_NONE:

        joyActive = false;
        wait_us(1000); //Making sure it doesn't trigger too quickly after the value is reset

        //Fall through to the default case
    default:
        return false;
    }

    //Updating the keyboard
    renderKeyboard();

    return false;
}

std::string Keyboard::keyboardInput(std::string startText) {
    currentText = startText;

    //Must render the keyboard
    currentNode = (KeyboardNode*) &node00;
    done = false;
    shift = false;
    joyActive = false;
    keyboardRendered = false;
    entryRendered = false;

    renderKeyboard();
    renderKeyboardTextEntry();

    //Attaching to button and joystick
    _button1->attach(std::bind(&Keyboard::onButton, placeholders::_1, placeholders::_2));
    _joystick->attach(std::bind(&Keyboard::onJoyInput, placeholders::_1, placeholders::_2));

    //Manually polling inputs because this function is a blocking call
    while (!done) {
        _inputArbiter->pollInputs(0.01f); //The delta time isn't used by any inputs the keyboard cares about
    }

    //Filling the space the keyboard used to occupy
    _globalGraphics->getLCD()->drawRectangleFilled(0, 64, 127, 127, 0x0);

    return currentText;
}