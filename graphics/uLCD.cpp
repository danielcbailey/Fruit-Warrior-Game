/*
 * uLCD Class
 * 
 * This is a psuedo-library for 4D Systems's Goldelox Processor powered displays.
 * The library only implements features that this game engine uses.
 * These features include most text features and most graphics commands.
 *
 * This library is loosely related to the uLCD_4DGL library, but is better optimized
 * and is updated to MBed OS 6
 *
 * (c) Daniel Cooper 2021
 */

#include "uLCD.hpp"
#include <cstdio>
#include <cstring>

#include "globals.hpp"

/*
     * With the baud rate, the library will always use 9600 for initial communication with the uLCD
     * It sets the new baud rate after starting initial communication.
     */
    uLCD::uLCD(PinName tx, PinName rx, PinName reset, uLCDBaud baud) : serial(tx, rx, 9600), resetSignal(reset) {
        
        this->resetSignal.write(true);

        this->reset();
        this->cls();
        
        //Set baud
        this->purgeBuffer();

        char buf[4];
        buf[0] = 0x0;
        buf[1] = 0xB;
        this->addIntToBuf(&buf[2], (int)baud);

        this->serial.write(buf, 4);

        int baudv = 9600;
        switch (baud) {
        case BAUD_9600:
            baudv = 9600;
            break;
        case BAUD_56000:
            baudv = 56000;
            break;
        case BAUD_115200:
            baudv = 115200;
            break;
        case BAUD_128000:
            baudv = 128000;
            break;
        case BAUD_300000:
            baudv = 300000;
            break;
        case BAUD_600000:
            baudv = 600000;
            break;
        case BAUD_1000000:
            baudv = 1000000;
            break;
        case BAUD_1500000:
            baudv = 1500000;
            break;
        }

        wait_us(4096); //waiting for all bytes to transmit because the mbed api doesn't

        this->serial.baud(baudv);

        //going to ignore the result because it may have been recieved before the baud switch
        char resp;
        while(!this->serial.readable());
        this->serial.read(&resp, sizeof(char));

        wait_us(1000); //testing

        this->purgeBuffer();

        this->cls(); //Just to send a command post-baud change

        this->setTextColor(0xFFFF); //making text foreground white
    }

    void uLCD::addIntToBuf(char* buf, int v) {
        int16_t sv = (int16_t)v;
        buf[0] = (unsigned char)(sv >> 8);
        buf[1] = (unsigned char)sv;
    }

    //General functions
    
    void uLCD::purgeBuffer() {
        char trash;
        while (this->serial.readable()) {
            this->serial.read(&trash, sizeof(char));
        }
    }

    void uLCD::awaitResponse() {
        char resp;
        while(!this->serial.readable());
        this->serial.read(&resp, sizeof(char));

        if (resp != 0x6) {
            printf("Display did not respond with Status OK.\n");
        }
    }

    /** Clear the screen and fills the screen with the set background color. Defaults to black (0x0)*/
    void uLCD::cls() {
        this->purgeBuffer();
        char buf[2];
        buf[0] = 0xFF;
        buf[1] = 0xD7;
        this->serial.write(buf, 2);

        this->awaitResponse();
    }

    /** Resets the screen, blocks for 3 seconds */
    void uLCD::reset() {
        this->resetSignal.write(false);
        wait_us(50);
        this->resetSignal.write(true);
        wait_us(3000000);
    }

    //Text Functions

    /**
     * Sets text foreground color
     * @param color The 4DGL color to set the text foreground color
     */
    void uLCD::setTextColor(uint16_t color) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x7F;
        
        //color already accounts for the big-endian style for transmission
        buf[2] = color;
        buf[3] = color >> 8;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets text background color
     * @param color The 4DGL color to set the text background color
     */
    void uLCD::setTextBackground(uint16_t color) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x7E;
        
        //color already accounts for the big-endian style for transmission
        buf[2] = color;
        buf[3] = color >> 8;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets the font scalar values, scales by integral amounts only.
     * @param width The width scalar for the font
     * @param height The height scalar for the font
     */
    void uLCD::setFontSize(int width, int height) {
        //sending two separate commands
        this->purgeBuffer();

        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x7C;
        this->addIntToBuf(&buf[2], width);
        this->serial.write(buf, 4);

        this->awaitResponse();

        this->purgeBuffer();

        buf[0] = 0xFF;
        buf[1] = 0x7B;
        this->addIntToBuf(&buf[2], height);
        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets whether the next text sent to the display gets bolded.
     * IMPORTANT: Text format commands are reset after the next text sent.
     * @param bold True to bold next text, false to reset manually
     */
    void uLCD::setTextBold(bool bold) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x76;
        buf[2] = 0x0;
        buf[3] = bold;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets whether the next text sent to the display gets italicized
     * IMPORTANT: Text format commands are reset after the next text sent.
     * @param italic True to italicize next text, false to reset manually
     */
    void uLCD::setTextItalic(bool italic) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x75;
        buf[2] = 0x0;
        buf[3] = italic;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets whether the next text sent to the display gets inverted colors
     * IMPORTANT: Text format commands are reset after the next text sent.
     * @param invert True to invert colors for next text, false to reset manually
     */
    void uLCD::setTextInverted(bool invert) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x74;
        buf[2] = 0x0;
        buf[3] = invert;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets whether the next text sent to the display gets underlined
     * IMPORTANT: Text format commands are reset after the next text sent.
     * @param underline True to underline next text, false to reset manually
     */
    void uLCD::setTextUnderline(bool underline) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x73;
        buf[2] = 0x0;
        buf[3] = underline;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Prints one char to the display at the location set with locate()
     * @param c The character to print
     */
    void uLCD::print(char c) {
        this->purgeBuffer();
        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0xFE;
        buf[2] = 0x0;
        buf[3] = c;

        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Prints a string to the lcd at the location set with locate()
     * IMPORTANT: Will not automatically line wrap, this must be done manually
     * @param str The null-terminated string to print
     */
    void uLCD::print(char* str) {
        this->purgeBuffer();
        char buf[2];
        buf[0] = 0x0;
        buf[1] = 0x6;

        this->serial.write(buf, 2);

        wait_us(25); //delay to process the command header

        //getting string length
        char length = strlen(str);

        //This is a very expensive operation for the display, so will have to slow down by
        //sending only one byte at a time

        char* bufP = str;

        for (int i = 0; length + 1 > i; i++) {
            this->serial.write(bufP++, 1);
            wait_us(20);
        }

        this->awaitResponse();
    }

    /**
     * Prints a formatted string to the lcd at the location set with locate()
     * IMPORTANT: Will not automatically line wrap, this must be done manually
     * @param format A string with the appropriate formatting codes
     */
    void uLCD::printf(const char* str, ...) {
        va_list args;
        va_start(args, str);

        this->purgeBuffer();

        char buf[258];
        buf[0] = 0x0;
        buf[1] = 0x6;

        //Generating final string
        int length = std::vsnprintf(&buf[2], 256, str, args);

        //This is a very expensive operation for the display, so will have to slow down by
        //sending only one byte at a time

        char* bufP = buf;

        for (int i = 0; length + 3 > i; i++) {
            this->serial.write(bufP++, 1);
            wait_us(20);
        }

        this->awaitResponse();

        va_end(args);
    }

    /**
     * Locates the text cursor to the given text coordinates.
     * IMPORTANT: Text coordinates are not the same as screen coordinates
     * @param x The x text coordinate
     * @param y The y text coordinate
     */
    void uLCD::locate(int x, int y) {
        this->purgeBuffer();

        char buf[6];
        buf[0] = 0xFF;
        buf[1] = 0xE4;
        this->addIntToBuf(&buf[2], y);
        this->addIntToBuf(&buf[4], x);
        this->serial.write(buf, 6);

        this->awaitResponse();
    }

    //Graphics Functions

    /**
     *  Draws a circle with no fill centered at (x, y)
     * @param x The x pixel coordinate
     * @param y The y pixel coordinate
     * @param r The radius measured in pixels
     * @param color The 4DGL color for the circle's outline
     */
    void uLCD::drawCircle(int x, int y, int radius, uint16_t color) {
        this->purgeBuffer();

        char buf[10];
        buf[0] = 0xFF;
        buf[1] = 0xCD;
        this->addIntToBuf(&buf[2], x);
        this->addIntToBuf(&buf[4], y);
        this->addIntToBuf(&buf[6], radius);
        buf[8] = color;
        buf[9] = color >> 8;
        this->serial.write(buf, 10);

        this->awaitResponse();
    }

    /**
     *  Draws a filled circle centered at (x, y)
     * @param x The x pixel coordinate
     * @param y The y pixel coordinate
     * @param r The radius measured in pixels
     * @param color The 4DGL color for the circle's fill
     */
    void uLCD::drawCircleFilled(int x, int y, int radius, uint16_t color) {
        this->purgeBuffer();

        char buf[10];
        buf[0] = 0xFF;
        buf[1] = 0xCC;
        this->addIntToBuf(&buf[2], x);
        this->addIntToBuf(&buf[4], y);
        this->addIntToBuf(&buf[6], radius);
        buf[8] = color;
        buf[9] = color >> 8;
        this->serial.write(buf, 10);

        this->awaitResponse();
    }

    /**
     * Draws a triange with no fill
     * @param x1 The x coordinate of the first vertex
     * @param y2 The y coordinate of the first vertex
     * @param x2 The x coordinate of the second vertex
     * @param y2 The y coordinate of the second vertex
     * @param x3 The x coordinate of the third vertex
     * @param y3 The y coordinate of the third vertex
     * @param color The 4DGL color for the triangle's outline
     */
    void uLCD::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color) {
        this->purgeBuffer();

        char buf[16];
        buf[0] = 0xFF;
        buf[1] = 0xC9;
        this->addIntToBuf(&buf[2], x1);
        this->addIntToBuf(&buf[4], y1);
        this->addIntToBuf(&buf[6], x2);
        this->addIntToBuf(&buf[8], y2);
        this->addIntToBuf(&buf[10], x3);
        this->addIntToBuf(&buf[12], y3);
        buf[14] = color;
        buf[15] = color >> 8;
        this->serial.write(buf, 16);

        this->awaitResponse();
    }

    /**
     * Draws a line between the two vertices
     * @param x1 The x coordinate of the first vertex
     * @param y2 The y coordinate of the first vertex
     * @param x2 The x coordinate of the second vertex
     * @param y2 The y coordinate of the second vertex
     * @param color The 4DGL color for the line
     */
    void uLCD::drawLine(int x1, int y1, int x2, int y2, uint16_t color) {
        this->purgeBuffer();

        char buf[12];
        buf[0] = 0xFF;
        buf[1] = 0xD2;
        this->addIntToBuf(&buf[2], x1);
        this->addIntToBuf(&buf[4], y1);
        this->addIntToBuf(&buf[6], x2);
        this->addIntToBuf(&buf[8], y2);
        buf[10] = color;
        buf[11] = color >> 8;
        this->serial.write(buf, 12);

        this->awaitResponse();
    }

    /**
     * Draws a rectangle with no fill between two vertices
     * @param x1 The x coordinate of the first vertex
     * @param y2 The y coordinate of the first vertex
     * @param x2 The x coordinate of the second vertex
     * @param y2 The y coordinate of the second vertex
     * @param color The 4DGL color for the rectangle's outline
     */
    void uLCD::drawRectangle(int x1, int y1, int x2, int y2, uint16_t color) {
        this->purgeBuffer();

        char buf[12];
        buf[0] = 0xFF;
        buf[1] = 0xCF;
        this->addIntToBuf(&buf[2], x1);
        this->addIntToBuf(&buf[4], y1);
        this->addIntToBuf(&buf[6], x2);
        this->addIntToBuf(&buf[8], y2);
        buf[10] = color;
        buf[11] = color >> 8;
        this->serial.write(buf, 12);

        this->awaitResponse();
    }

    /**
     * Draws a filled rectangle between two vertices
     * @param x1 The x coordinate of the first vertex
     * @param y2 The y coordinate of the first vertex
     * @param x2 The x coordinate of the second vertex
     * @param y2 The y coordinate of the second vertex
     * @param color The 4DGL color for the rectangle's fill
     */
    void uLCD::drawRectangleFilled(int x1, int y1, int x2, int y2, uint16_t color) {
        this->purgeBuffer();

        char buf[12];
        buf[0] = 0xFF;
        buf[1] = 0xCE;
        this->addIntToBuf(&buf[2], x1);
        this->addIntToBuf(&buf[4], y1);
        this->addIntToBuf(&buf[6], x2);
        this->addIntToBuf(&buf[8], y2);
        buf[10] = color;
        buf[11] = color >> 8;
        this->serial.write(buf, 12);

        this->awaitResponse();
    }

    /**
     * Sets an individual pixel's color
     * @param x The x coordinate
     * @param y The y coordinate
     * @param color The 4DGL color for the pixel
     */
    void uLCD::setPixel(int x, int y, uint16_t color) {
        this->purgeBuffer();

        char buf[8];
        buf[0] = 0xFF;
        buf[1] = 0xCB;
        this->addIntToBuf(&buf[2], x);
        this->addIntToBuf(&buf[4], y);
        buf[6] = color;
        buf[7] = color >> 8;
        this->serial.write(buf, 8);

        this->awaitResponse();
    }

    /**
     * Draws a bitmap at the given coordinate pair from a buffer.
     * @param x The x coordinate of the top left corner
     * @param y The y coordinate of the top left corner
     * @param width The width of the bitmap
     * @param height The height of the bitmap
     * @param image The buffer containing the bitmap. Must be at least of size width * height
     */
    void uLCD::BLIT(int x, int y, int width, int height, uint16_t* image) {
        this->purgeBuffer();

        char buf[10];
        buf[0] = 0x0;
        buf[1] = 0xA;
        this->addIntToBuf(&buf[2], x);
        this->addIntToBuf(&buf[4], y);
        this->addIntToBuf(&buf[6], width);
        this->addIntToBuf(&buf[8], height);
        this->serial.write(buf, 10);

        wait_us(2500); //Giving the display time to process the command header before the payload

        this->serial.write(image, width * height * 2);

        this->awaitResponse();
    }

    /**
     * Sets the outline color for applicable shapes.
     * @param color The color for the outline, zero disables outlines
     */
    void uLCD::setOutlineColor(uint16_t color) {
        this->purgeBuffer();

        char buf[4];
        buf[0] = 0xFF;
        buf[1] = 0x67;
        buf[2] = color;
        buf[3] = color >> 8;
        this->serial.write(buf, 4);

        this->awaitResponse();
    }

    /**
     * Sets the clipping window. If all parameters are zero, then clipping is disabled.
     * @param x The x coordinate of the top left corner
     * @param y The y coordinate of the top left corner
     * @param width The width of the clipping window
     * @param height The height of the clipping window
     */
     void uLCD::setClippingWindow(int x, int y, int width, int height) {
        if (!x && !y && !width && !height) {
            //disable clipping
            this->purgeBuffer();
            char buf[4];
            buf[0] = 0xFF;
            buf[1] = 0x6C;
            buf[2] = 0x0;
            buf[3] = 0x0;

            this->serial.write(buf, 4);

            this->awaitResponse();

            return;
        }

        //enable clipping
        this->purgeBuffer();
        char buf[10];
        buf[0] = 0xFF;
        buf[1] = 0x6C;
        buf[2] = 0x0;
        buf[3] = 0x1;

        this->serial.write(buf, 4);

        this->awaitResponse();

        //set clipping region
        this->purgeBuffer();

        buf[0] = 0xFF;
        buf[1] = 0xBF;
        this->addIntToBuf(&buf[2], x);
        this->addIntToBuf(&buf[4], y);
        this->addIntToBuf(&buf[6], x + width - 1);
        this->addIntToBuf(&buf[8], y + height - 1);
        this->serial.write(buf, 10);

        this->awaitResponse();
     }