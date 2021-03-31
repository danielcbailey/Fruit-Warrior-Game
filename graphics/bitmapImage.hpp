/*
 * Bitmap Image Class
 *
 * Bitmap images are immutable and are created from static memory or are loaded dynamically.
 * If loaded dynamically, the constructor expects a file to load from.
 *
 * Bitmap images can be stored in 8-bit or 16-bit color depth. If using 8-bit color depth, a
 * color look up table must be specified. To control which color mode is used, the lookup table
 * is a pointer, if left nullptr, then 16-bit color will be used.
 *
 * (c) Daniel Cooper 2021
 */

#include "mbed.h"
#include <cstdint>
#include <stdint.h>
#include "colorLUT.hpp"
#include "uLCD.hpp"
#include "graphicsElement.hpp"

#ifndef BITMAP_IMAGE_INCLUDED
#define BITMAP_IMAGE_INCLUDED

class BitmapImage {
    private:

    uint8_t* bitmap;
    int width;
    int height;
    ColorLUT* lut;
    GraphicsElement* receipt;

    public:

    inline BitmapImage(const uint8_t* bitmap, int width, int height, ColorLUT* lut) :
         bitmap((uint8_t*)bitmap), width(width), height(height), lut(lut) {}

    BitmapImage(FileHandle bitmapFile);

    inline int getWidth() { return this->width; }

    inline int getHeight() { return this->height; }

    inline uint8_t* getRawBuffer() { return this->bitmap; }

    void render(uLCD* lcd, int x, int y, int layer);

    bool sample(int x, int y, uint16_t* color);

    inline GraphicsElement* getGraphicsElement() { return this->receipt; }

    inline void setGraphicsElement(GraphicsElement* element) { this->receipt = element; }
};

#endif //BITMAP_IMAGE_INCLUDED