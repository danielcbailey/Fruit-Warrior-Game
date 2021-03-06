

#include "sprite.hpp"
#include "graphicsController.hpp"
#include "gameTick.hpp"
#include "globals.hpp"

//Placeholder image
const uint16_t placeholderImg_lut_buf[] = {0};
ColorLUT placeholderImg_lut = ColorLUT((uint16_t*)placeholderImg_lut_buf);
const uint8_t placeholderImg_buf[] = {0};
BitmapImage placeholderImg = BitmapImage(placeholderImg_buf, 1, 1, &placeholderImg_lut);

Sprite::Sprite() :
    GraphicsElement(GraphicsElement::ElementType::ELEMENT_IMAGE, {}), animation(this) {
    
    this->registered = false;
    this->animationRegistered = false;
    this->hidden = false;
    this->x = 0;
    this->y = 0;

    //registering for gametick updates
    _tickManager.registerForTick(this);
}

Sprite::~Sprite() {
    _tickManager.removeRegistration(this);

    //must ensure it unregisters itself from the graphics controller
    if (this->animationRegistered && !this->hidden) {
        _globalGraphics->removeAnimation(&this->animation);
    }

    if (this->registered && !this->hidden) {
        _globalGraphics->removeGraphicsElement(this);
    }
}

void Sprite::setPos(float x, float y) {
    int oldX = this->x;
    int oldY = this->y;
    this->getContext()->Image.x = x;
    this->getContext()->Image.y = y;
    this->x = x;
    this->y = y;

    if (!this->registered || this->hidden) {
        return;
    }

    if (oldX == (int)x && oldY == (int)y) {
        //Same position, can skip update
        return;
    }

    _globalGraphics->updateGraphicsElement(this);
}

float Sprite::getX() {
    return this->x;
}

float Sprite::getY() {
    return this->y;
}

void Sprite::setImage(BitmapImage* image) {
    this->getContext()->Image.image = image;
    this->getContext()->Image.width = image->getWidth();
    this->getContext()->Image.height = image->getHeight();

    if (hidden) {
        this->registered = true;
        return;
    }
    if (!this->registered) {
        //must register the element
        _globalGraphics->registerGraphicsElement(this);
        this->registered = true;
    } else {
        //must update the element
        _globalGraphics->updateGraphicsElement(this);
    }
}

void Sprite::enableAnimation() {
    if (hidden) {
        this->animationRegistered = true;
        this->registered = true;
        return;
    }

    if (!this->registered) {
        //All sprites must have some image as a placeholder
        this->setImage(&placeholderImg);
    }

    _globalGraphics->registerAnimation(&this->animation);
    this->animationRegistered = true;
}

void Sprite::disableAnimation() {
    if (hidden) {
        this->animationRegistered = false;
        return;
    }

    if (!this->animationRegistered) {
        return;
    }

    _globalGraphics->removeAnimation(&this->animation);
    this->animationRegistered = false;
}

void Sprite::show() {
    if (!this->hidden) {
        return;
    }
    if (!this->registered) {
        this->hidden = false;
        return;
    }

    _globalGraphics->registerGraphicsElement(this);

    if (this->animationRegistered) {
        _globalGraphics->registerAnimation(&this->animation);
    }

    this->hidden = false;
}

void Sprite::hide() {
    if (this->hidden) {
        return;
    }
    if (this->animationRegistered) {
        //must remove the animation prior to hiding
        _globalGraphics->removeAnimation(&this->animation);
    }
    if (this->registered) {
        _globalGraphics->removeGraphicsElement(this);
    }

    this->hidden = true;
}

//if already registered, needs to unregister the element and register it for layer changes to take effect
void Sprite::setLayer(int layer) {
    if (!this->registered) {
        this->getContext()->Image.layer = layer;
        return;
    }

    _globalGraphics->removeGraphicsElement(this);

    this->getContext()->Image.layer = layer;

    _globalGraphics->registerGraphicsElement(this);
}