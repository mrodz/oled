#pragma once

#ifndef _RGBHAL_
#define _RGBHAL_

#include <Arduino_GFX_Library.h>

class TFT_SmartDisplay_ESP32S3_RGB_LCD : public Arduino_RGB_Display
{
public:
	TFT_SmartDisplay_ESP32S3_RGB_LCD(int16_t, int16_t);
	bool isUserTouching(int16_t*, int16_t*);
};

#endif // #ifndef _RGBHAL_