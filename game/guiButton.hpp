/*
 * GUI Button Class
 *
 * GUI Buttons are intended to give a visual effect for being hovered on.
 * This will pulsate the button's image between two colors to emphasize the
 * hover. Requires that images have specific color lookup tables.
 *
 * (c) 2021 Daniel Cooper
 */

#ifndef GUI_BUTTON_INCLUDED
#define GUI_BUTTON_INCLUDED

#include "graphics/bitmapImage.hpp"
#include "graphics/tickable.hpp"
#include "graphics/graphicsElement.hpp"
#include <cstdint>
#include <functional>
#include "inputs.hpp"

class GUIButton : public Tickable {
    private:

    bool selected;
    std::function<const bool(void*, float)> callback;
    GraphicsElement* element;
    BitmapImage* image;
    BitmapImage temp;
    uint16_t tempLut[16];
    ColorLUT tempColorLut;
    int lutSize;
    int evtHandle;

    public:
    
    GUIButton(BitmapImage* image, int x, int y, std::function<const bool(void*, float)> callback, int lutSize);

    ~GUIButton();

    void setSelected(bool value);

    inline bool isSelected() { 
        return this->selected; 
    }

    void onGameTick(float dt);
};

#endif //GUI_BUTTON_INCLUDED