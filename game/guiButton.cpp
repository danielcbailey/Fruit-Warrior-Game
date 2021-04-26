/*
 * GUI Button Class
 *
 * GUI Buttons are intended to give a visual effect for being hovered on.
 * This will pulsate the button's image between two colors to emphasize the
 * hover. Requires that images have specific color lookup tables.
 *
 * (c) 2021 Daniel Cooper
 */

#include "guiButton.hpp"
#include "graphics/graphicsController.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "graphics/colors.hpp"
#include "core/screenManager.hpp"
#include "graphics/gameTick.hpp"
#include "globals.hpp"

GUIButton::GUIButton(BitmapImage* image, int x, int y, std::function<const bool(void*, float)> callback, int lutSize) : temp(*image), tempColorLut(nullptr) {
    //Must register the graphics element
    this->element = new GraphicsElement(image, x, y);
    this->element->getContext()->Image.layer = 110;

    this->lutSize = lutSize;
    this->selected = false;
    this->image = image;
    this->evtHandle = 0;
    this->callback = callback;

    _globalGraphics->registerGraphicsElement(this->element);

    _tickManager.registerForTick(this);
}

GUIButton::~GUIButton() {
    _tickManager.removeRegistration(this);
    _globalGraphics->removeGraphicsElement(this->element);

    if (this->evtHandle) {
        _button1->detach(this->evtHandle);
    }
}

void GUIButton::setSelected(bool value) { 
    this->selected = value;
    if (value) {
        this->evtHandle = _button1->attach(this->callback);
    } else if (evtHandle) {
        this->element->getContext()->Image.image = this->image;
        _globalGraphics->updateGraphicsElement(this->element);
        //_button1->detach(this->evtHandle);
        this->evtHandle = 0;
    }
}

void GUIButton::onGameTick(float dt) {
    if (!this->selected) {
        return;
    }

    //The period of pulsation is once every 2 seconds
    //Creating new LUT
    memcpy(this->tempLut, this->image->getColorLUT()->lut, this->lutSize * sizeof(uint16_t));

    //The index of the value to change is 1 in this case
    this->tempLut[1] = Colors::interpColor(0xFFFF, 0x11FF, 0.5f * sinf(_screenManager.getTotalTime() * 3.1415f) + 0.5f);

    this->tempColorLut = ColorLUT(this->tempLut);
    this->temp = BitmapImage(this->image->getRawBuffer(), this->image->getWidth(), this->image->getHeight(), &this->tempColorLut);

    this->element->getContext()->Image.image = &this->temp;

    _globalGraphics->updateGraphicsElement(this->element);
}