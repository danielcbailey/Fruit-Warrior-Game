/*
 * Sprite Abstract Class
 *
 * Abstracts many features useful to creating sprite objects such as:
 * - Animations
 * - Movement
 * - Changing appearanace
 * - Gametick updates
 */

#ifndef SPRITE_INCLUDED
#define SPRITE_INCLUDED

#include "graphicsElement.hpp"
#include "animationPlayer.hpp"
#include "tickable.hpp"

class Sprite : public GraphicsElement, public Tickable {
    private:
    
    bool registered;
    bool animationRegistered;
    bool hidden;
    float x, y;
    AnimationPlayer animation;

    public:

    Sprite();

    ~Sprite();

    void setPos(float x, float y);

    float getX();

    float getY();

    void setImage(BitmapImage* image);

    inline AnimationPlayer* getAnimationPlayer() { return &this->animation; }

    void enableAnimation();

    void disableAnimation();

    //sprites show by default
    void show();

    void hide();

    //if already registered, needs to unregister the element and register it for layer changes to take effect
    void setLayer(int layer);
};

#endif //SPRITE_INCLUDED