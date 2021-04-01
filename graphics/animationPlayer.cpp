

#include "animationPlayer.hpp"

BitmapImage* AnimationPlayer::getFrame(float dt) {
    this->ellapsedTime += dt;

    if (!this->currentAnimation) {
        return nullptr;
    }

    int frameCounter = ellapsedTime * framesPerSecond;

    BitmapImage* ret;

    switch (this->playbackType) {
    case PLAYBACK_ONCE_SINGLE_WAY:
        if (frameCounter >= this->currentAnimation->getFrameCount()) {
            //animation finished already
            ret = this->currentAnimation->getFrame(this->currentAnimation->getFrameCount() - 1);
        } else {
            ret = this->currentAnimation->getFrame(frameCounter);
        }
        break;
    case PLAYBACK_ONCE_BOTH_WAYS:
        if (frameCounter >= this->currentAnimation->getFrameCount() * 2 - 1) {
            //animation finished
            //to explain the math to calculate this condition:
            //suppose an animation consists of a, b, c, and d. That is 4 frames.
            //it is played: a, b, c, d, c, b, a which is 7 frames, or 2 * n - 1

            ret = this->currentAnimation->getFrame(0);
        } else if (frameCounter >= this->currentAnimation->getFrameCount()) {
            //reverse direction
            ret = this->currentAnimation->getFrame((this->currentAnimation->getFrameCount() - 1) * 2 - frameCounter);
        } else {
            ret = this->currentAnimation->getFrame(frameCounter);
        }
        break;
    case PLAYBACK_REPEAT_SINGLE_WAY:
        ret = this->currentAnimation->getFrame(frameCounter % this->currentAnimation->getFrameCount());
        break;
    case PLAYBACK_REPEAT_BOTH_WAYS:
        if (frameCounter % (this->currentAnimation->getFrameCount() * 2 - 1) >= this->currentAnimation->getFrameCount()) {
            //reverse direction
            ret = this->currentAnimation->getFrame((this->currentAnimation->getFrameCount() - 1) * 2 -
                  frameCounter % (this->currentAnimation->getFrameCount() * 2 - 1));
        } else {
            ret = this->currentAnimation->getFrame(frameCounter % (this->currentAnimation->getFrameCount() * 2 - 1));
        }
        break;
    default:
        return nullptr;
    }

    return ret;
}