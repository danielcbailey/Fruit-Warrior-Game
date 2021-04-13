

#ifndef COMPOST_PILE_INCLUDED
#define COMPOST_PILE_INCLUDED

#include "graphics/graphicsElement.hpp"

class CompostPile {
    private:
    int x;
    int height;

    GraphicsElement* element;

    public:

    CompostPile(int col);

    ~CompostPile();

    inline int getHeight() { return this->height; }

    void addCompost(int amt);
};

#endif //COMPOST_PILE_INCLUDED