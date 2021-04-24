

#include "assets.hpp"

BitmapImage* bomb_animation_frames[] = {&bomb_0, &bomb_1, &bomb_2, &bomb_3};
Animation bomb_animation = Animation(bomb_animation_frames, 4);

BitmapImage* apple_cut_animation_frames[] = {&apple_1, &apple_2};
Animation apple_cut_animation = Animation(apple_cut_animation_frames, 2);

BitmapImage* orange_cut_animation_frames[] = {&orange_1, &orange_2};
Animation orange_cut_animation = Animation(orange_cut_animation_frames, 2);

BitmapImage* banana_cut_animation_frames[] = {&banana_1, &banana_2};
Animation banana_cut_animation = Animation(banana_cut_animation_frames, 2);

BitmapImage* kiwi_cut_animation_frames[] = {&kiwi_1, &kiwi_2};
Animation kiwi_cut_animation = Animation(kiwi_cut_animation_frames, 2);

BitmapImage* lemon_cut_animation_frames[] = {&lemon_1, &lemon_2};
Animation lemon_cut_animation = Animation(lemon_cut_animation_frames, 2);

BitmapImage* knife_marker_animation_frames[] = {&knifeMarker_0, &knifeMarker_1, &knifeMarker_2, &knifeMarker_3};
Animation knife_marker_animation = Animation(knife_marker_animation_frames, 4);