

#include "knifeTrajectoryMarker.hpp"
#include "assets/assets.hpp"

KnifeTrajectoryMarker::KnifeTrajectoryMarker(int index) {
    this->hide();
    this->setLayer(100);
    this->getAnimationPlayer()->setAnimation(&knife_marker_animation, 4, AnimationPlayer::PLAYBACK_REPEAT_BOTH_WAYS);
    this->enableAnimation();
}

void KnifeTrajectoryMarker::onGameTick(float dt) {

}