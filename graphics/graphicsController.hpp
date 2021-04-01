

#ifndef GRAPHICS_CONTROLLER_INCLUDED
#define GRAPHICS_CONTROLLER_INCLUDED

#include "uLCD.hpp"
#include "graphicsElement.hpp"
#include "animationPlayer.hpp"
#include <list>

class GraphicsController; //forward declaration
extern GraphicsController* _globalGraphics;

class GraphicsController {
    private:

    uLCD* lcd;

    //head is lower layer, tail is top layer of overall list
    //head is first to draw in a layer, tail is the last element to draw in inner list
    std::list<std::list<GraphicsElement*>> elements;

    std::list<AnimationPlayer*> animations;

    //used for rendering new placement of element
    void renderAllAbove(GraphicsElement* element, bool add);

    //used for restoring a place that an element used to occupy
    void renderAllIn(GraphicsElement::RegionOfInfluence roi);

    GraphicsElement* resampleImage(GraphicsElement::RegionOfInfluence roi, GraphicsElement* img);

    public:

    inline GraphicsController(uLCD* lcd) : lcd(lcd) { _globalGraphics = this; }

    uint16_t samplePixel(int x, int y, int layer);

    /*
     * Draws the element on the screen, making sure to respect layers and transparency.
     * To draw, it has to redraw each element that appears on top of the given element.
     */
    void registerGraphicsElement(GraphicsElement* element);

    /*
     * Draws the element on the screen following a similar proceedure to the above, but
     * optimizes for movement so that it only restores what it absolutely must.
     */
    void updateGraphicsElement(GraphicsElement* element);

    /*
     * Completely removes the element from the screen, will calculate what must be replaced
     * in the space it used to occupy and redraws the above.
     */
    void removeGraphicsElement(GraphicsElement* element);

    /*
     * Registers an animation for the graphics controller to update
     * Automatically updates the frames and renders them to the screen
     */
    void registerAnimation(AnimationPlayer* player);

    /*
     * Removes a registered animation from the graphics controller's management
     * This must be called prior to deleting an element.
     */
    void removeAnimation(AnimationPlayer* player);

    /*
     * Processes all animations registered with the graphics controller
     */
    void handleGraphicsTick(float dt);
};

#endif //GRAPHICS_CONTROLLER_INCLUDED