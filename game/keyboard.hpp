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

#ifndef KEYBOARD_INCLUDED
#define KEYBOARD_INCLUDED

#include <string>

/*
 * Keyboard layout
 * --------------------
 * Text Entry Here#
 * --------------------
 * 1 2 3 4 5 6 7 8 9 0 | ! @ # $ % ^ & * ( )
 * q w e r t y u i o p | Q W E R T Y U I O P
 *  a s d f g h j k l  |  A S D F G H J K L
 *   z x c v b n m <-  |   Z X C V B N M <-
 * Shift Space Enter   | Shfit Space Enter
 */

class Keyboard {

    private:

    typedef struct KeyboardNode_S {
        int y; //Where 0, 0 is the top left key
        int x;
        char lowerValue;
        char shiftValue;
        const KeyboardNode_S* left;
        const KeyboardNode_S* right;
        const KeyboardNode_S* up;
        const KeyboardNode_S* down;
    } KeyboardNode;

    static const KeyboardNode node00;
    static const KeyboardNode node01;
    static const KeyboardNode node02;
    static const KeyboardNode node03;
    static const KeyboardNode node04;
    static const KeyboardNode node05;
    static const KeyboardNode node06;
    static const KeyboardNode node07;
    static const KeyboardNode node08;
    static const KeyboardNode node09;

    static const KeyboardNode node10;
    static const KeyboardNode node11;
    static const KeyboardNode node12;
    static const KeyboardNode node13;
    static const KeyboardNode node14;
    static const KeyboardNode node15;
    static const KeyboardNode node16;
    static const KeyboardNode node17;
    static const KeyboardNode node18;
    static const KeyboardNode node19;

    static const KeyboardNode node20;
    static const KeyboardNode node21;
    static const KeyboardNode node22;
    static const KeyboardNode node23;
    static const KeyboardNode node24;
    static const KeyboardNode node25;
    static const KeyboardNode node26;
    static const KeyboardNode node27;
    static const KeyboardNode node28;

    static const KeyboardNode node30;
    static const KeyboardNode node31;
    static const KeyboardNode node32;
    static const KeyboardNode node33;
    static const KeyboardNode node34;
    static const KeyboardNode node35;
    static const KeyboardNode node36;
    static const KeyboardNode nodeBack;

    static const KeyboardNode nodeShift;
    static const KeyboardNode nodeSpace;
    static const KeyboardNode nodeEnter;

    static const KeyboardNode nodes[];

    static KeyboardNode* currentNode;
    static std::string currentText;
    static volatile bool done;
    static bool shift;
    static bool joyActive;
    static bool keyboardRendered;
    static bool entryRendered;

    static void renderKeyboard();

    static void renderKeyboardTextEntry();

    static bool onButton(void* evtDetails, float dt);

    static bool onJoyInput(void* evtDetails, float dt);

    public:

    static std::string keyboardInput(std::string startText);
};

#endif //KEYBOARD_INCLUDED