

#include "graphicsElement.hpp"
#include "bitmapImage.hpp"
#include "globals.hpp"
#include "graphicsController.hpp"

GraphicsElement::GraphicsElement(BitmapImage* image, int x, int y) {
    this->context = {};
    this->context.Image.image = image;
    this->context.Image.layer = 0;
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

void GraphicsElement::render(uLCD *lcd) {
    this->renderWithin(lcd, _globalGraphics->getGlobalBoundary());
}

void GraphicsElement::renderRaw(uLCD* lcd) {
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
        {
            //Cannot render images outside of the screen area, must resample any sprites that go outside
            //the screen area. However, this has already been done by renderWithin so it can just be rendered.

            this->context.Image.image->render(lcd, this->context.Image.x, this->context.Image.y, this->context.Image.layer);
        }
        
        break;
    default:
        break;
    }
}

void GraphicsElement::renderWithin(uLCD* lcd, RegionOfInfluence roi) {
    //not really concerned with maximum performance as much as not needing to rerender each thing
    //that intersects the roi of the element just rerendered.

    if (!this->isInROI(roi)) {
        //it isn't even in the region, so toss it
        return;
    }

    RegionOfInfluence intersection = intersectROIs(roi, this->getROI());
    intersection = intersectROIs(intersection, _globalGraphics->getGlobalBoundary());

    if (this->elementType == ELEMENT_IMAGE &&
        !ROIEquals(intersection, this->getROI()) &&
        this->context.Image.width * this->context.Image.height <= 2048) {
        //will software clip it to the region

        GraphicsElement* tempElem = resampleImage(intersection, this);
        tempElem->renderRaw(lcd);
        //free(tempElem->getContext()->Image.image->getRawBuffer()); no longer needed
        delete(tempElem->getContext()->Image.image);
        delete(tempElem);
    } else {
        this->renderRaw(lcd);
    }
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

bool GraphicsElement::ROIEquals(RegionOfInfluence r1, RegionOfInfluence r2) {
    return r1.x == r2.x && r1.y == r2.y && r1.x2 == r2.x2 && r1.y2 == r2.y2;
}

GraphicsElement::RegionOfInfluence GraphicsElement::adjustROIToScreen(RegionOfInfluence roi, bool* isValid) {
    *isValid = false;
    if (roi.x < 0) {
        //cannot be less than zero because of display limitations
        roi.x = 0;
    }
    if (roi.x2 < 0) {
        //Invisible ROI
        return roi;
    }
    if (roi.x2 > 127) {
        //cannot be greater than 127 because of display limitations
        roi.x2 = 127;
    }
    if (roi.x > 127) {
        //Invisible ROI
        return roi;
    }

    if (roi.y < 0) {
        //cannot be less than zero because of display limitations
        roi.y = 0;
    }
    if (roi.y2 < 0) {
        //Invisible ROI
        return roi;
    }
    if (roi.y2 > 127) {
        //cannot be greater than 127 because of display limitations
        roi.y2 = 127;
    }
    if (roi.y > 127) {
        //Invisible ROI
        return roi;
    }

    *isValid = true;
    return roi;
}

GraphicsElement* GraphicsElement::resampleImage(GraphicsElement::RegionOfInfluence roi, GraphicsElement* img) {
    //samples the image over the given roi
    //if the roi extends beyond the boundaries of the image, it simply generates an image of what it can

    GraphicsElement::RegionOfInfluence bounds = intersectROIs(roi, img->getROI());
    GraphicsElement* ret = new GraphicsElement(GraphicsElement::ELEMENT_IMAGE, {.Image={}});

    ret->getContext()->Image.x = bounds.x;
    ret->getContext()->Image.y = bounds.y;
    ret->getContext()->Image.width = bounds.x2 - bounds.x + 1;
    ret->getContext()->Image.height = bounds.y2 - bounds.y + 1;
    ret->getContext()->Image.layer = img->getLayer();

    uint16_t* buf = (uint16_t*)malloc_safe(sizeof(uint16_t) * ret->getContext()->Image.height * ret->getContext()->Image.width);
    printMalloc(buf);

    uint16_t* bufP = buf;

    for (int y = bounds.y; bounds.y2 >= y; y++) {
        for (int x = bounds.x; bounds.x2 >= x; x++) {
            BitmapImage* other = img->getContext()->Image.image;
            uint16_t color;
            if (!other->sample(x - img->getContext()->Image.x, y - img->getContext()->Image.y, &color)) {
                color = _globalGraphics->samplePixel(x, y, img->getLayer());
            }

            *(bufP++) = color;
        }
    }

    ret->getContext()->Image.image = new BitmapImage((uint8_t*)buf, ret->getContext()->Image.width, ret->getContext()->Image.height, nullptr);

    return ret;
}

GraphicsElement::RegionOfInfluence GraphicsElement::intersectROIs(RegionOfInfluence r1, RegionOfInfluence r2) {
    RegionOfInfluence ret;
    if (r1.x < r2.x) {
        ret.x = r2.x;
    } else {
        ret.x = r1.x;
    }

    if (r1.y < r2.y) {
        ret.y = r2.y;
    } else {
        ret.y = r1.y;
    }

    if (r1.x2 > r2.x2) {
        ret.x2 = r2.x2;
    } else {
        ret.x2 = r1.x2;
    }

    if (r1.y2 > r2.y2) {
        ret.y2 = r2.y2;
    } else {
        ret.y2 = r1.y2;
    }

    return ret;
}