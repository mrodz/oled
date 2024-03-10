#pragma once

#ifndef OLED_RGBHAL_HPP
#define OLED_RGBHAL_HPP

#include <Arduino_GFX_Library.h>

/*
I wish to address the specification datasheet provided with the LCD Module.
There are some SERIOUS inaccuracies on the table... besides DE, VSYNC, and HSYNC, every
other pin seems to have been mislabeled on the PDF from Shenzhen Jingcai.

The definitions below appear to work without any noticeable issues.
*/

#define DE_SIGNAL 41
#define VSYNC_IN 40
#define HSYNC_IN 39
#define PCLK_IN 42

#define RED_0 14
#define RED_1 21
#define RED_2 47
#define RED_3 48
#define RED_4 45

#define GREEN_0 9
#define GREEN_1 46
#define GREEN_2 3
#define GREEN_3 8
#define GREEN_4 16
#define GREEN_5 1

#define BLUE_0 15
#define BLUE_1 7
#define BLUE_2 6
#define BLUE_3 5
#define BLUE_4 4

#define HSYNC_POLARITY 0
#define HSYNC_FRONT_PORCH 210
#define HSYNC_PULSE_WIDTH 30
#define HSYNC_BACK_PORCH 16

#define VSYNC_POLARITY 0
#define VSYNC_FRONT_PORCH 22
#define VSYNC_PULSE_WIDTH 13
#define VSYNC_BACK_PORCH 10

#define PCLK_ACTIVE_NEG 1
#define PREFERRED_SPEED 16000000L

class TFT_SmartDisplay_ESP32S3_RGB_LCD : public Arduino_RGB_Display
{
public:
	TFT_SmartDisplay_ESP32S3_RGB_LCD(int16_t, int16_t);
	bool isUserTouching(int16_t *, int16_t *);
};

#endif // #ifndef _RGBHAL_