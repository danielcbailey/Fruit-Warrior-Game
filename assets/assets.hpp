/*
 * The header file for all assets
 */

#ifndef ASSETS_INCLUDED
#define ASSETS_INCLUDED

#include "graphics/bitmapImage.hpp"
#include "graphics/animation.hpp"

//fruits

extern BitmapImage apple_0;
extern BitmapImage apple_1;
extern BitmapImage apple_2;

extern BitmapImage orange_0;
extern BitmapImage orange_1;
extern BitmapImage orange_2;

extern BitmapImage kiwi_0;
extern BitmapImage kiwi_1;
extern BitmapImage kiwi_2;

extern BitmapImage banana_0;
extern BitmapImage banana_1;
extern BitmapImage banana_2;

extern BitmapImage lemon_0;
extern BitmapImage lemon_1;
extern BitmapImage lemon_2;

//ninja
extern BitmapImage ninja_0;
extern BitmapImage ninja_1;
extern BitmapImage ninja_2;
extern BitmapImage ninja_3;

//knife
extern BitmapImage knife_0;
extern BitmapImage knife_1;
extern BitmapImage knife_2;
extern BitmapImage knife_3;
extern BitmapImage knife_4;
extern BitmapImage knife_5;
extern BitmapImage knife_6;
extern BitmapImage knife_7;

//backgrounds
extern BitmapImage backgroundGame_0;
extern BitmapImage idleBackground_0;

//bomb
extern BitmapImage bomb_0;
extern BitmapImage bomb_1;
extern BitmapImage bomb_2;
extern BitmapImage bomb_3;

//knife markers
extern BitmapImage knifeMarker_0;
extern BitmapImage knifeMarker_1;
extern BitmapImage knifeMarker_2;
extern BitmapImage knifeMarker_3;

//characters
extern BitmapImage* characters[];

//logos
extern BitmapImage studioLogo_0;
extern BitmapImage studioLogo_1;
extern BitmapImage samuraiLogo_0;
extern BitmapImage samuraiLogo_1;
extern BitmapImage fruitLogo_0;
extern BitmapImage highScoreText_0;

//GUI elements
extern BitmapImage startButton_0;
extern BitmapImage highScoresButton_0;
extern BitmapImage backButton_0;

//Animations
extern Animation bomb_animation;

extern Animation apple_cut_animation;
extern Animation banana_cut_animation;
extern Animation orange_cut_animation;
extern Animation kiwi_cut_animation;
extern Animation lemon_cut_animation;

extern Animation knife_marker_animation;

#endif //ASSETS_INCLUDED