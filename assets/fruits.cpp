#include <stdint.h>
#include "graphics/bitmapImage.hpp"
#include "graphics/colorLUT.hpp"
//This file is autogenerated and contains asset data to be stored in ROM.
//(c) Daniel Cooper and Artists 2021

const uint16_t fruits_colorLUT_lut[] = { 0, 59249, 18748, 10598, 34513, 9889,
		 26365, 34521, 50937, 9979, 42625, 42227, 34300, 26754, 35499, 10581, 29951,
		 10750, 11519};

ColorLUT fruits_colorLUT = ColorLUT((uint16_t*)fruits_colorLUT_lut);

const uint8_t apple_0_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 4, 5, 1, 5, 2, 3, 0, 0, 0, 0, 0,
		 4, 6, 7, 5, 7, 8, 4, 0, 0, 0, 0, 0, 4, 9, 9, 8, 8, 8, 4, 0, 0, 0, 0, 0,
		 5, 8, 8, 8, 7, 5, 10, 0, 0, 0, 0, 0, 10, 7, 7, 7, 5, 10, 1, 0, 0, 0, 0,
		 0, 0, 10, 5, 10, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage apple_0 = BitmapImage((const uint8_t*)apple_0_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t apple_1_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0,
		 0, 4, 5, 1, 5, 2, 3, 0, 0, 0, 0, 0, 4, 6, 7, 5, 7, 8, 4, 0, 0, 0, 0, 0,
		 4, 9, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0,
		 5, 8, 8, 8, 7, 5, 10, 0, 0, 0, 0, 0, 10, 7, 7, 7, 5, 10, 1, 0, 0, 0, 0,
		 0, 0, 10, 5, 10, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage apple_1 = BitmapImage((const uint8_t*)apple_1_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t apple_2_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0,
		 0, 4, 5, 1, 5, 2, 3, 0, 0, 0, 0, 0, 4, 6, 7, 5, 7, 8, 4, 0, 0, 0, 0, 0,
		 4, 9, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 5, 8, 8, 8, 7, 5, 10, 0, 0, 0, 0,
		 0, 10, 7, 7, 7, 5, 10, 1, 0, 0, 0, 0, 0, 0, 10, 5, 10, 10, 1, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage apple_2 = BitmapImage((const uint8_t*)apple_2_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t orange_0_bitmap[] = { 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 3, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0,
		 3, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 11, 11, 1, 11, 11, 0, 0, 0, 0,
		 0, 0, 11, 6, 11, 11, 11, 12, 11, 0, 0, 0, 0, 0, 11, 6, 6, 12, 12, 12, 11,
		 0, 0, 0, 0, 0, 11, 6, 12, 12, 11, 11, 11, 0, 0, 0, 0, 0, 11, 12, 12, 11,
		 11, 11, 11, 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage orange_0 = BitmapImage((const uint8_t*)orange_0_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t orange_1_bitmap[] = { 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 3, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0,
		 3, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 11, 11, 1, 11, 11, 0, 0, 0, 0,
		 0, 0, 11, 6, 11, 11, 11, 12, 11, 0, 0, 0, 0, 0, 11, 6, 6, 12, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 11, 0, 0, 0, 0, 0, 11, 6, 12, 12, 11, 11,
		 11, 0, 0, 0, 0, 0, 11, 12, 12, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 11, 11,
		 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage orange_1 = BitmapImage((const uint8_t*)orange_1_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t orange_2_bitmap[] = { 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 3, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0,
		 3, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 11, 11, 1, 11, 11, 0, 0, 0, 0,
		 0, 0, 11, 6, 11, 11, 11, 12, 11, 0, 0, 0, 0, 0, 11, 6, 6, 12, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 11,
		 0, 0, 0, 0, 0, 11, 6, 12, 12, 11, 11, 11, 0, 0, 0, 0, 0, 11, 12, 12, 11,
		 11, 11, 11, 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 0, 0, 0};

BitmapImage orange_2 = BitmapImage((const uint8_t*)orange_2_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t kiwi_0_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 14, 13, 1, 0, 0, 0, 0, 0,
		 0, 1, 13, 3, 15, 3, 13, 1, 0, 0, 0, 0, 0, 13, 3, 15, 2, 15, 3, 13, 0, 0,
		 0, 0, 0, 14, 15, 2, 16, 2, 15, 14, 0, 0, 0, 0, 0, 13, 3, 15, 2, 15, 3, 13,
		 0, 0, 0, 0, 0, 1, 13, 3, 15, 3, 13, 1, 0, 0, 0, 0, 0, 0, 1, 13, 14, 13,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage kiwi_0 = BitmapImage((const uint8_t*)kiwi_0_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t kiwi_1_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 1, 13, 14, 13, 1, 0, 0, 0, 0, 0, 0, 1, 13, 3, 15, 3, 13, 1, 0, 0, 0,
		 0, 0, 13, 3, 15, 2, 15, 3, 13, 0, 0, 0, 0, 0, 14, 15, 2, 16, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 2, 15, 14, 0, 0, 0, 0, 0, 13, 3, 15, 2, 15, 3, 13,
		 0, 0, 0, 0, 0, 1, 13, 3, 15, 3, 13, 1, 0, 0, 0, 0, 0, 0, 1, 13, 14, 13,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage kiwi_1 = BitmapImage((const uint8_t*)kiwi_1_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t kiwi_2_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 1, 13, 14, 13, 1, 0, 0, 0, 0, 0, 0, 1, 13, 3, 15, 3, 13, 1, 0, 0, 0,
		 0, 0, 13, 3, 15, 2, 15, 3, 13, 0, 0, 0, 0, 0, 14, 15, 2, 16, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 15, 14,
		 0, 0, 0, 0, 0, 13, 3, 15, 2, 15, 3, 13, 0, 0, 0, 0, 0, 1, 13, 3, 15, 3,
		 13, 1, 0, 0, 0, 0, 0, 0, 1, 13, 14, 13, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage kiwi_2 = BitmapImage((const uint8_t*)kiwi_2_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t banana_0_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 17, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 6, 17, 18, 0, 0, 13, 13, 0, 0, 0, 0, 0, 6, 6, 17, 18, 18, 18, 1, 0,
		 0, 0, 0, 0, 0, 6, 6, 17, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage banana_0 = BitmapImage((const uint8_t*)banana_0_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t banana_1_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 6, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 17, 18, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 6, 6, 17, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, 6, 6, 0, 18, 18, 18, 1,
		 0, 0, 0, 0, 0, 0, 0, 0, 17, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage banana_1 = BitmapImage((const uint8_t*)banana_1_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t banana_2_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 6, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 17, 18, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 6, 6, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 13, 13,
		 0, 0, 0, 0, 0, 0, 0, 0, 18, 18, 18, 1, 0, 0, 0, 0, 0, 0, 0, 0, 17, 6, 6,
		 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage banana_2 = BitmapImage((const uint8_t*)banana_2_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t lemon_0_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 17, 17, 2, 3, 0, 0, 0, 0, 0, 0,
		 0, 17, 18, 17, 6, 0, 0, 0, 0, 0, 0, 0, 0, 18, 18, 17, 6, 0, 0, 0, 0, 0,
		 0, 0, 0, 6, 17, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage lemon_0 = BitmapImage((const uint8_t*)lemon_0_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t lemon_1_bitmap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 17, 17, 2, 3, 0, 0, 0, 0, 0, 0,
		 0, 17, 18, 17, 6, 0, 0, 0, 0, 0, 0, 0, 0, 18, 18, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 17, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 17, 6, 6, 0, 0, 0, 0, 0, 0,
		 0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage lemon_1 = BitmapImage((const uint8_t*)lemon_1_bitmap, 12, 12, &fruits_colorLUT);

const uint8_t lemon_2_bitmap[] = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0,
		 0, 0, 17, 17, 2, 3, 0, 0, 0, 0, 0, 0, 0, 17, 18, 17, 6, 0, 0, 0, 0, 0, 0,
		 0, 0, 18, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 17, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 17, 6, 6, 0, 0, 0, 0, 0,
		 0, 0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BitmapImage lemon_2 = BitmapImage((const uint8_t*)lemon_2_bitmap, 12, 12, &fruits_colorLUT);