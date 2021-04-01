

#ifndef ANIMATION_PLAYER_INCLUDED
#define ANIMATION_PLAYER_INCLUDED

#include "animation.hpp"

class GraphicsElement;

class AnimationPlayer {
    public:

    enum PlaybackType { PLAYBACK_ONCE_SINGLE_WAY, PLAYBACK_ONCE_BOTH_WAYS, PLAYBACK_REPEAT_SINGLE_WAY, PLAYBACK_REPEAT_BOTH_WAYS };
    
    private:

    Animation* currentAnimation;
    float framesPerSecond;
    PlaybackType playbackType;
    float ellapsedTime;
    GraphicsElement* element;

    public:

    inline AnimationPlayer(GraphicsElement* parent) :
     currentAnimation(nullptr), framesPerSecond(0), playbackType(PLAYBACK_ONCE_SINGLE_WAY), ellapsedTime(0), element(parent) {}

    inline void setAnimation(Animation* animation, float fps, PlaybackType type) {
        this->currentAnimation = animation;
        this->framesPerSecond = fps;
        this->playbackType = type;
    }

    inline void setSpeed(float fps) { this->framesPerSecond = fps; }

    inline float getSpeed() { return this->framesPerSecond; }

    inline void reset() { this->ellapsedTime = 0; }

    inline GraphicsElement* getElement() { return this->element; }

    //Where dt is the time since the previous frame
    BitmapImage* getFrame(float dt);
};

#endif //ANIMATION_PLAYER_INCLUDED