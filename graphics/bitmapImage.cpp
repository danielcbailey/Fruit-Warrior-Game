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
 * If the Look Up Table is used, a reserved value of zero is used to designate transparency.
 *
 * (c) Daniel Cooper 2021
 */

#include "bitmapImage.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include "graphicsController.hpp"

BitmapImage::BitmapImage(FileHandle bitmapFile) {
    /*
     * The organization of bitmap files is as follows:
     * Byte 0: The width in pixels
     * Byte 1: The height in pixels
     * Byte 2: The color lut ID, if 0 then no LUT is used
     * Byte 3-n: The bitmap pixel data
     * Byte n+1-k: The lut data if used
     */
    
    //TODO
}

void BitmapImage::render(uLCD* lcd, int xs, int ys, int layer) {

    if (this->lut != nullptr) {
        //Must generate a temporary buffer for the blit process
        uint16_t* blitBuf = (uint16_t*)malloc(this->width * this->height * 2);

        uint16_t* bufSeeker = blitBuf;
        uint8_t* imgSeeker = this->bitmap;
        for (int y = 0; this->height > y; y++) {
            for (int x = 0; this->width > x; x++) {
                if (*imgSeeker == 0) {
                    //Reserved value for transparency
                    *(bufSeeker++) = _globalGraphics->samplePixel(xs + x, ys + y, layer);
                    imgSeeker++;
                } else {
                    *(bufSeeker++) = this->lut->lut[*(imgSeeker++)];
                }
            }
        }

        lcd->BLIT(xs, ys, this->width, this->height, blitBuf);
        free(blitBuf);
    } else {
        //Just has to call the blit function since no color conversion is needed
        lcd->BLIT(xs, ys, this->width, this->height, (uint16_t*)this->bitmap);
    }
}

bool BitmapImage::sample(int x, int y, uint16_t* color) {
    if (x < 0 || y < 0 || x >= this->width || y >= this->height) {
        return false;
    }

    if (this->lut != nullptr) {
        uint8_t rawColor = this->bitmap[this->width * y + x];

        if (!rawColor) {
            //Is transparent
            return false;
        }

        *color = this->lut->lut[rawColor];
        return true;
    } else {
        *color = ((uint16_t*)this->bitmap)[this->width * y + x];
        return true;
    }
}