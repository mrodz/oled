#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "rgbhal.hpp"
#include "components/counter.hpp"

#define TFT_BL 2
#define SERIAL_BAUD_RATE 115200

const int16_t SCREEN_WIDTH_PX = 800;
const int16_t SCREEN_HEIGHT_PX = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH_PX * 10];
TFT_SmartDisplay_ESP32S3_RGB_LCD *gfx = new TFT_SmartDisplay_ESP32S3_RGB_LCD(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX);

#include "touch.hpp"

void debug_driver(const char *buf)
{
	Serial.printf(buf);
	Serial.flush();
}

void display_driver(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	uint16_t *first_pixel = (uint16_t *)&color_p->full;

	gfx->draw16bitRGBBitmap((uint32_t)area->x1, (uint32_t)area->y1, first_pixel, w, h);

	lv_disp_flush_ready(disp);
}

void capacitive_touch_driver(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
	int16_t touchX, touchY;

	bool touched = gfx->isUserTouching(&touchX, &touchY);

	if (!touched)
	{
		data->state = LV_INDEV_STATE_REL;
	}
	else
	{
		data->state = LV_INDEV_STATE_PR;

		data->point.x = touchX;
		data->point.y = touchY;

		Serial.print("Data x ");
		Serial.println(touchX);

		Serial.print("Data y ");
		Serial.println(touchY);
	}
}

void setup(void)
{
	Serial.begin(SERIAL_BAUD_RATE);

#ifdef TFT_BL
	pinMode(TFT_BL, OUTPUT);
	digitalWrite(TFT_BL, HIGH);
#endif

	gfx->begin();

	touch_init();

	Serial.printf("Using LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());

	lv_init();

	lv_log_register_print_cb(debug_driver); /* register print function for debugging */

	lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH_PX * 10);

	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);

	disp_drv.hor_res = SCREEN_WIDTH_PX;
	disp_drv.ver_res = SCREEN_HEIGHT_PX;
	disp_drv.flush_cb = display_driver;
	disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);

	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = capacitive_touch_driver;
	lv_indev_drv_register(&indev_drv);

	lv_obj_t * screen = lv_scr_act();

	CounterButton counter_button(screen);

	counter_button.set_pos(Position{50, 50});
}

void loop(void)
{
	uint32_t delay_ms = lv_timer_handler();
	delay(delay_ms);
}