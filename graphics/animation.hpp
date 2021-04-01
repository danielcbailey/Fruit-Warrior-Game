/*
 * Animation Class
 *
 * A simple container class for frames of an animation, intended to be able to be part of the assets collection
 * staticly defined in memory.
 */

#ifndef ANIMATION_INCLUDED
#define ANIMATION_INCLUDED

#include "bitmapImage.hpp"

class Animation {
    private:

    BitmapImage** frames;
    int numFrames;

    public:

    /*
     * Frames should already be in order that they appear
     */
    inline Animation(BitmapImage* frames[], int numFrames) : frames(frames), numFrames(numFrames) {}

    inline BitmapImage* getFrame(int frame) { return this->frames[frame]; }

    inline int getFrameCount() { return this->numFrames; }
};

#endif //ANIMATION_INCLUDED