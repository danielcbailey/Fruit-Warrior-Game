/*
 * Graphics Element Class
 *
 * Graphic elements serve as reciepts issued by the graphics controller for a screen draw.
 * They are stored in the graphics controller until revoked and it is the responsibility of
 * the reciept holder to revoke the element when it is no longer supposed to be tracked.
 *
 * Graphics Elements allow the graphics controller to track what needs to be updated and
 * sent to the display. They also allow it to recreate the display without the need to store
 * a separate screen buffer.
 */

#ifndef GRAPHICS_ELEMENT_INCLUDED
#define GRAPHICS_ELEMENT_INCLUDED

#include <cstdint>
#include <stdint.h>
#include "uLCD.hpp"

class BitmapImage;

class GraphicsElement {
    public:

    enum ElementType {ELEMENT_NONE, ELEMENT_RECT, ELEMENT_CIRCLE, ELEMENT_IMAGE};

    typedef union {
        struct {
            int x;
            int y;
            int layer;
            int width;
            int height;
            uint16_t fillColor;
            uint16_t outlineColor;
            bool isFilled;
        } Rectangle;
        struct {
            int x;
            int y;
            int layer;
            int radius;
            uint16_t fillColor;
            uint16_t outlineColor;
            bool isFilled;
        } Circle;
        struct {
            int x;
            int y;
            int layer;
            int width;
            int height;
            BitmapImage* image;
        } Image;
    } ElementContext;
    
    typedef struct {
        int x;
        int y;
        int x2;
        int y2;
    } RegionOfInfluence;

    private:

    ElementType elementType;

    ElementContext context;

    RegionOfInfluence renderedROI; //used for keeping track of its existing position on the display before an update

    public:

    inline GraphicsElement(ElementType eType, ElementContext context) : context(context), elementType(eType), renderedROI({0,0,0,0}) {}

    GraphicsElement(BitmapImage* image, int x, int y);

    inline ElementContext* getContext() { return &this->context; }

    inline ElementType getType() { return this->elementType; }

    //These coordinates are global coordinates
    bool sample(int x, int y, uint16_t* color);

    void render(uLCD* lcd);

    void renderRaw(uLCD* lcd);

    void renderWithin(uLCD* lcd, RegionOfInfluence roi);

    int getLayer();

    RegionOfInfluence getROI();

    bool isInROI(RegionOfInfluence roi);

    inline void setRenderedROI(RegionOfInfluence roi) { this->renderedROI = roi; }

    inline RegionOfInfluence getRenderedROI() { return this->renderedROI; }

    static bool ROIEquals(RegionOfInfluence r1, RegionOfInfluence r2);

    static RegionOfInfluence adjustROIToScreen(RegionOfInfluence roi, bool* isValid);

    static GraphicsElement* resampleImage(GraphicsElement::RegionOfInfluence roi, GraphicsElement* img);

    static RegionOfInfluence intersectROIs(RegionOfInfluence r1, RegionOfInfluence r2);
};

#endif //GRAPHICS_ELEMENT_INCLUDED