#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "rgbhal.hpp"

#define TFT_BL 2
#define SERIAL_BAUD_RATE 115200

const int16_t SCREEN_WIDTH_PX = 800;
const int16_t SCREEN_HEIGHT_PT = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH_PX * 10];
TFT_SmartDisplay_ESP32S3_RGB_LCD *gfx = new TFT_SmartDisplay_ESP32S3_RGB_LCD(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PT);

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

void counter_button_callback(lv_event_t *event)
{
	int *clicks = (int *)event->user_data;
	switch (event->code)
	{
	case LV_EVENT_CLICKED:
	{
		(*clicks)++;

		lv_obj_t *button = lv_event_get_target(event);
		lv_obj_t *label = lv_obj_get_child(button, 0);

		lv_label_set_text_fmt(label, "You've clicked the button %u times!", clicks);

		break;
	}
	case LV_EVENT_DELETE:
	{
		delete (int *)event->user_data;
		break;
	}
	}
}

void counter_button(void)
{
	static uint32_t *clicks = new uint32_t{0};

	static lv_obj_t *button = lv_btn_create(NULL);
	lv_obj_add_event_cb(button, counter_button_callback, LV_EVENT_ALL, clicks);
	lv_obj_align(button, LV_ALIGN_CENTER, 0, 0);

	static lv_obj_t *label = lv_label_create(button);
	lv_label_set_text_fmt(label, "You've clicked this button %u times", clicks);
	lv_obj_center(label);
}

static void btn_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *btn = lv_event_get_target(e);
	if (code == LV_EVENT_CLICKED)
	{
		static uint8_t cnt = 0;
		cnt++;
		// int *cnt = (int *)e->user_data;
		// (*cnt)++;

		/*Get the first child of the button which is the label and change its text*/
		lv_obj_t *label = lv_obj_get_child(btn, 0);
		lv_label_set_text_fmt(label, "Button: %d", cnt);
	}
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_2(void)
{
	lv_obj_t *btn = lv_btn_create(lv_scr_act());				  /*Add a button the current screen*/
	lv_obj_set_pos(btn, 10, 10);								  /*Set its position*/
	lv_obj_set_size(btn, 120, 50);								  /*Set its size*/
	lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

	lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
	lv_label_set_text(label, "Button");		/*Set the labels text*/
	lv_obj_center(label);
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
	disp_drv.ver_res = SCREEN_HEIGHT_PT;
	disp_drv.flush_cb = display_driver;
	disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);

	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = capacitive_touch_driver;
	lv_indev_drv_register(&indev_drv);

	lv_example_get_started_2();
}

void loop(void)
{
	uint32_t delay_ms = lv_timer_handler();
	delay(delay_ms);
}