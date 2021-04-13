

#include "compostPile.hpp"
#include "graphics/graphicsController.hpp"
#include "screens/gamePlayScreen.hpp"
#include "core/screenManager.hpp"
#include "screens/gameOverScreen.hpp"

CompostPile::CompostPile(int col) {
    this->height = 2; //starting height

    this->x = col * 11 + 3;
    this->element = new GraphicsElement(GraphicsElement::ELEMENT_RECT, {});
    this->element->getContext()->Rectangle.x = this->x;
    this->element->getContext()->Rectangle.height = this->height;
    this->element->getContext()->Rectangle.y = 127 - this->height;
    this->element->getContext()->Rectangle.isFilled = true;
    this->element->getContext()->Rectangle.fillColor = 0xF00F; //greenish
    this->element->getContext()->Rectangle.width = 11;
    this->element->getContext()->Rectangle.layer = 2; //Compost layer

    _globalGraphics->registerGraphicsElement(this->element);
}

CompostPile::~CompostPile() {
    _globalGraphics->removeGraphicsElement(this->element);
}

void CompostPile::addCompost(int amt) {
    this->height += amt;

    this->element->getContext()->Rectangle.height = this->height;
    this->element->getContext()->Rectangle.y = 127 - this->height;

    //Testing for game over condition
    if (this->element->getContext()->Rectangle.y < COMPOST_MIN_Y) {
        //Game over, too much compost
        _screenManager.changeScreen(&_gameOverScreen);
    } else {
        _globalGraphics->updateGraphicsElement(this->element);
    }
}