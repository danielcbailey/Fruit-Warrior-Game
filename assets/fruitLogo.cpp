#include <stdint.h>
#include "graphics/bitmapImage.hpp"
#include "graphics/colorLUT.hpp"
//This file is autogenerated and contains asset data to be stored in ROM.
//(c) Daniel Cooper and Artists 2021

const uint16_t fruitLogo_colorLUT_lut[] = { 0, 65535, 26602, 34786, 4085, 49964,
		 17313, 26850};

ColorLUT fruitLogo_colorLUT = ColorLUT((uint16_t*)fruitLogo_colorLUT_lut);

const uint8_t fruitLogo_0_bitmap[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0,
		 0, 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 5, 5, 1, 0, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 4, 4, 1, 1,
		 2, 2, 1, 0, 0, 0, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 1,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6,
		 2, 2, 6, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 1, 1, 2, 2, 1, 0, 0, 0,
		 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
		 1, 1, 1, 0, 1, 2, 2, 7, 7, 7, 7, 2, 1, 4, 4, 1, 2, 2, 2, 1, 1, 4, 2, 1,
		 0, 0, 0, 1, 4, 4, 1, 1, 2, 2, 4, 4, 2, 2, 4, 4, 4, 1, 1, 2, 2, 6, 6, 6,
		 6, 6, 1, 4, 2, 2, 6, 6, 2, 2, 1, 4, 2, 1, 0, 0, 0, 1, 4, 2, 1, 1, 2, 2,
		 6, 6, 2, 2, 6, 6, 6, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 6, 1, 1, 6, 2,
		 1, 4, 2, 1, 0, 0, 0, 1, 4, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 0, 1, 2,
		 2, 1, 0, 0, 0, 0, 1, 2, 2, 1, 1, 1, 6, 6, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2,
		 1, 1, 2, 2, 1, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 1, 2, 2, 1,
		 0, 0, 1, 1, 1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 0,
		 0, 0, 1, 2, 6, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
		 0, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 6, 1, 0, 0, 0, 1, 2, 6, 1, 0, 0, 0, 0,
		 1, 2, 2, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 1, 2, 6, 1, 1, 2, 2, 1, 1,
		 2, 6, 1, 0, 0, 0, 1, 2, 6, 1, 0, 0, 0, 0, 1, 2, 6, 1, 0, 0, 0, 0, 1, 2,
		 2, 2, 1, 1, 1, 1, 2, 6, 6, 1, 2, 6, 1, 1, 2, 6, 1, 0, 0, 0, 1, 6, 6, 1,
		 0, 0, 0, 0, 1, 2, 6, 1, 0, 0, 0, 0, 1, 1, 2, 2, 2, 1, 1, 2, 2, 6, 6, 1,
		 2, 6, 1, 1, 6, 6, 6, 1, 0, 0, 1, 6, 6, 1, 0, 0, 0, 0, 1, 6, 6, 1, 0, 0,
		 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 6, 6, 6, 1, 6, 6, 1, 0, 1, 6, 6, 1, 0, 0,
		 1, 6, 6, 1, 0, 0, 0, 0, 1, 6, 6, 1, 0, 0, 0, 0, 0, 1, 1, 1, 6, 6, 6, 6,
		 1, 6, 6, 1, 6, 6, 1, 0, 1, 6, 6, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1,
		 1, 0, 0, 0};

BitmapImage fruitLogo_0 = BitmapImage((const uint8_t*)fruitLogo_0_bitmap, 38, 20, &fruitLogo_colorLUT);