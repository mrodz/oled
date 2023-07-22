#include "rgbhal.hpp"
#include "touch.hpp"

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

static Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
	DE_SIGNAL, VSYNC_IN, HSYNC_IN, PCLK_IN,
	/* START channels */
	RED_0, RED_1, RED_2, RED_3, RED_4,
	GREEN_0, GREEN_1, GREEN_2, GREEN_3, GREEN_4, GREEN_5,
	BLUE_0, BLUE_1, BLUE_2, BLUE_3, BLUE_4,
	/* END channels */
	HSYNC_POLARITY, HSYNC_FRONT_PORCH, HSYNC_PULSE_WIDTH, HSYNC_BACK_PORCH,
	VSYNC_POLARITY, VSYNC_FRONT_PORCH, VSYNC_PULSE_WIDTH, VSYNC_BACK_PORCH,
	PCLK_ACTIVE_NEG, PREFERRED_SPEED
);

TFT_SmartDisplay_ESP32S3_RGB_LCD::TFT_SmartDisplay_ESP32S3_RGB_LCD(int16_t screen_width_px, int16_t screen_height_px)
	: Arduino_RGB_Display(screen_width_px, screen_height_px, rgbpanel) {}

bool TFT_SmartDisplay_ESP32S3_RGB_LCD::isUserTouching(int16_t *input_x, int16_t *input_y) {
	bool result = touch_touched(this->width(), this->height());

	if (result) {
		*input_x = touch_last_x;
		*input_y = touch_last_y;
	}
	
	return result;
}
