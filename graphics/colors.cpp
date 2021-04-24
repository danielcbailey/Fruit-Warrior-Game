

#include "colors.hpp"

uint16_t Colors::interpColor(uint16_t c1, uint16_t c2, float prop) {
    //Breaking up c1 and c2 into components
    char red1 = (c1 & 0xF8) >> 3;
    char green1 = ((c1 & 0x7) << 3) | ((c1 & 0xE000) >> 13);
    char blue1 = (c1 & 0x1F00) >> 8;

    char red2 = (c2 & 0xF8) >> 3;
    char green2 = ((c2 & 0x7) << 3) | ((c2 & 0xE000) >> 13);
    char blue2 = (c2 & 0x1F00) >> 8;

    char red = (red2 - red1) * prop + red1;
    char green = (green2 - green1) * prop + green1;
    char blue = (blue2 - blue1) * prop + blue1;

    uint8_t colorLower = ((((uint8_t) green) & 0x7) << 5) | (((uint8_t) blue) & 0x1F);
    uint8_t colorUpper = ((((uint8_t) red) & 0x1F) << 3) | ((((uint8_t) green) & 0x38) >> 3);

    return ((uint16_t) colorLower << 8) | colorUpper;
}