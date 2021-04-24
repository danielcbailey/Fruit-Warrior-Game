

#ifndef COLORS_INCLUDED
#define COLORS_INCLUDED

#include <stdint.h>

class Colors {
    public:

    //If prop is 0, then it is all c1, if prop is 1, then it is all c2
    static uint16_t interpColor(uint16_t c1, uint16_t c2, float prop);
};

#endif //COLORS_INCLUDED