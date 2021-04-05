

#include "graphicsElement.hpp"
#include "bitmapImage.hpp"

GraphicsElement::GraphicsElement(BitmapImage* image, int x, int y) {
    this->context = {};
    this->context.Image.image = image;
    this->context.Image.height = image->getHeight();
    this->context.Image.width = image->getWidth();
    this->context.Image.x = x;
    this->context.Image.y = y;

    this->elementType = ELEMENT_IMAGE;
}

bool GraphicsElement::sample(int x, int y, uint16_t *color) {
    switch (this->elementType) {
        case ELEMENT_NONE:
        {
            return false;
        }
        break;
        case ELEMENT_RECT:
        {
            if (x < this->context.Rectangle.x || x >= this->context.Rectangle.x + this->context.Rectangle.width ||
                y < this->context.Rectangle.y || y >= this->context.Rectangle.y + this->context.Rectangle.height) {
                
                return false;
            }
            if (this->context.Rectangle.outlineColor || !this->context.Rectangle.isFilled) {
                //has an outline
                if (x == this->context.Rectangle.x || y == this->context.Rectangle.y ||
                    x == this->context.Rectangle.x + this->context.Rectangle.width - 1 ||
                    y == this->context.Rectangle.y + this->context.Rectangle.height - 1) {
                    
                    //is on the outline
                    *color = this->context.Rectangle.outlineColor;
                    return true;
                }
            }
            if (this->context.Rectangle.isFilled) {
                *color = this->context.Rectangle.fillColor;
                return true;
            }
        }
        break;
        case ELEMENT_CIRCLE:
        {
            int rsq = this->context.Circle.radius;
            rsq = rsq * rsq -2;

            //testing if the point lay inside the circle
            int dx = x - this->context.Circle.x;
            int dy = y - this->context.Circle.y;
            int distSq = dx * dx + dy * dy;

            if (distSq <= rsq) {
                //contained in the circle

                if (this->context.Circle.outlineColor || !this->context.Circle.isFilled) {
                    //has an outline
                    if (rsq - distSq < 50) {
                        //on the outline
                        *color = this->context.Circle.outlineColor;
                        return true;
                    }
                }

                if (this->context.Circle.isFilled) {
                    *color = this->context.Circle.fillColor;
                    return true;
                }
            }
        }
        break;
        case ELEMENT_IMAGE:
        {
            return this->context.Image.image->sample(x - this->context.Image.x, y - this->context.Image.y, color);
        }
        break;
    }
    return false;
}

void GraphicsElement::render(uLCD* lcd) {
    switch (this->elementType) {
    case ELEMENT_RECT:
        if (this->context.Rectangle.isFilled) {
            lcd->setOutlineColor(this->context.Rectangle.outlineColor);
            lcd->drawRectangleFilled(this->context.Rectangle.x, this->context.Rectangle.y,
                                    this->context.Rectangle.x + this->context.Rectangle.width -1,
                                    this->context.Rectangle.y + this->context.Rectangle.height - 1,
                                    this->context.Rectangle.fillColor);
            lcd->setOutlineColor(0);
        } else {
            lcd->drawRectangle(this->context.Rectangle.x, this->context.Rectangle.y,
                                    this->context.Rectangle.x + this->context.Rectangle.width -1,
                                    this->context.Rectangle.y + this->context.Rectangle.height - 1,
                                    this->context.Rectangle.outlineColor);
        }
        break;
    case ELEMENT_CIRCLE:
        if (this->context.Circle.isFilled) {
            lcd->setOutlineColor(this->context.Circle.outlineColor);
            lcd->drawCircleFilled(this->context.Circle.x, this->context.Circle.y,
                                 this->context.Circle.radius, this->context.Circle.fillColor);
            lcd->setOutlineColor(0);
        } else {
            lcd->drawCircleFilled(this->context.Circle.x, this->context.Circle.y,
                                 this->context.Circle.radius, this->context.Circle.outlineColor);
        }
        break;
    case ELEMENT_IMAGE:
        this->context.Image.image->render(lcd, this->context.Image.x, this->context.Image.y, this->context.Image.layer);
        break;
    default:
        break;
    }
}

void GraphicsElement::renderWithin(uLCD* lcd, RegionOfInfluence roi) {
    //not really concerned with maximum performance as much as not needing to rerender each thing
    //that intersects the roi of the element just rerendered.
    lcd->setClippingWindow(roi.x, roi.y, roi.x2 - roi.x + 1, roi.y2 - roi.y2 + 1);
    this->render(lcd);
    lcd->setClippingWindow(0, 0, 0, 0); //disabling clipping
}

int GraphicsElement::getLayer() {
    int elemLayer = -1;

    switch (this->elementType) {
    case GraphicsElement::ELEMENT_RECT:
        elemLayer = this->context.Rectangle.layer;
        break;
    case GraphicsElement::ELEMENT_CIRCLE:
        elemLayer = this->context.Circle.layer;
        break;
    case GraphicsElement::ELEMENT_IMAGE:
        elemLayer = this->context.Image.layer;
        break;
    default:
        elemLayer = -1;
        break;
    }

    return elemLayer;
}

GraphicsElement::RegionOfInfluence GraphicsElement::getROI() {
    int elemLayer = -1;
    int roix, roiy, roix2, roiy2;

    switch (this->elementType) {
    case GraphicsElement::ELEMENT_RECT:
        elemLayer = this->context.Rectangle.layer;
        roix = this->context.Rectangle.x;
        roiy = this->context.Rectangle.y;
        roix2 = roix + this->context.Rectangle.width - 1;
        roiy2 = roiy + this->context.Rectangle.height - 1;
        break;
    case GraphicsElement::ELEMENT_CIRCLE:
        elemLayer = this->context.Circle.layer;
        roix = this->context.Circle.x - this->context.Circle.radius / 2;
        roiy = this->context.Circle.y - this->context.Circle.radius / 2;
        roix2 = roix + this->context.Circle.radius;
        roiy2 = roiy + this->context.Circle.radius;
        break;
    case GraphicsElement::ELEMENT_IMAGE:
        elemLayer = this->context.Image.layer;
        roix = this->context.Image.x;
        roiy = this->context.Image.y;
        roix2 = roix + this->context.Image.width - 1;
        roiy2 = roiy + this->context.Image.height - 1;
        break;
    default:
        elemLayer = -1;
        roix = -1;
        roiy = -1;
        roix2 = -1;
        roiy2 = -1;
        break;
    }

        return {roix, roiy, roix2, roiy2};
}

bool GraphicsElement::isInROI(RegionOfInfluence roi) {
    RegionOfInfluence croi = this->getROI();

    //x and y are always less than x2 and y2
    //Calculating if x axis overlaps
    bool xOver = (roi.x >= croi.x && roi.x <= croi.x2) ||
                 (roi.x2 >= croi.x && roi.x2 <= croi.x2) ||
                 (croi.x >= roi.x && croi.x <= roi.x2) ||
                 (croi.x2 >= roi.x && croi.x2 <= roi.x2);
    //Calculating if y axis overlaps
    bool yOver = (roi.y >= croi.y && roi.y <= croi.y2) ||
                 (roi.y2 >= croi.y && roi.y2 <= croi.y2) ||
                 (croi.y >= roi.y && croi.y <= roi.y2) ||
                 (croi.y2 >= roi.y && croi.y2 <= roi.y2);

    return xOver && yOver;
}