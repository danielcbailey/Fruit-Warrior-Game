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

class Sprite : GraphicsElement {
    private:
    
    bool registered;
    bool animationRegistered;
    bool hidden;
    AnimationPlayer animation;

    public:

    Sprite();

    ~Sprite();

    void setPos(int x, int y);

    int getX();

    int getY();

    void setImage(BitmapImage* image);

    inline AnimationPlayer* getAnimationPlayer() { return &this->animation; }

    void enableAnimation();

    void disableAnimation();

    //sprites show by default
    void show();

    void hide();

    //if already registered, needs to unregister the element and register it for layer changes to take effect
    void setLayer(int layer);

    //to be implemented by the class that implements Sprite.
    //called on every game tick with the time delta passed to the function
    virtual void onGameTick(float dt) = 0;
};

#endif //SPRITE_INCLUDED