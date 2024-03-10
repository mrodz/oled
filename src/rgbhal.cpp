#include "rgbhal.hpp"
#include "touchhal.hpp"

static Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
	DE_SIGNAL, VSYNC_IN, HSYNC_IN, PCLK_IN,
	/* START channels */
	RED_0, RED_1, RED_2, RED_3, RED_4,
	GREEN_0, GREEN_1, GREEN_2, GREEN_3, GREEN_4, GREEN_5,
	BLUE_0, BLUE_1, BLUE_2, BLUE_3, BLUE_4,
	/* END channels */
	HSYNC_POLARITY, HSYNC_FRONT_PORCH, HSYNC_PULSE_WIDTH, HSYNC_BACK_PORCH,
	VSYNC_POLARITY, VSYNC_FRONT_PORCH, VSYNC_PULSE_WIDTH, VSYNC_BACK_PORCH,
	PCLK_ACTIVE_NEG, PREFERRED_SPEED);

TFT_SmartDisplay_ESP32S3_RGB_LCD::TFT_SmartDisplay_ESP32S3_RGB_LCD(int16_t screen_width_px, int16_t screen_height_px)
	: Arduino_RGB_Display(screen_width_px, screen_height_px, rgbpanel) {}

bool TFT_SmartDisplay_ESP32S3_RGB_LCD::isUserTouching(int16_t *input_x, int16_t *input_y)
{
	bool result = touch_touched(this->width(), this->height());

	if (result)
	{
		*input_x = touch_last_x;
		*input_y = touch_last_y;
	}

	return result;
}
