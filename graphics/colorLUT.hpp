/*
 * Color LUT Class
 *
 * Color Look Up Tables are used for bitmap color compression down to 8 bits.
 * Bitmap images can indivdiually specify a lookup table. Lookup tables can be
 * reused, but is not necessary for a memory advantage.
 * In order for a Color LUT to be advantageous, the bitmap has to be larger than
 * 384 pixels (less than 20x20) if it doesn't repurpose an existing Color LUT.
 *
 * This is a very simple class for defining some default color palletes at compile
 * time.
 *
 * (c) Daniel Cooper 2021
 */

#ifndef COLOR_LUT_DEFINED
#define COLOR_LUT_DEFINED

#include <stdint.h>

class ColorLUT {
    public:

    uint16_t* lut;

    inline ColorLUT(uint16_t* lut) : lut(lut) {}
};

#endif //COLOR_LUT_DEFINED