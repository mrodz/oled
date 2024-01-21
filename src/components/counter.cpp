#include <lvgl.h>
#include "components/counter.hpp"

void counter_button_callback(lv_event_t *event)
{
	auto clicks = (int *)(event->user_data);

	switch (event->code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_t *button = lv_event_get_target(event);
		lv_obj_t *label = lv_obj_get_child(button, 0);

		*clicks += 1;

		lv_label_set_text_fmt(label, "You've clicked the %u times!", *clicks);

		break;
	}
	case LV_EVENT_DELETE:
	{
		delete clicks;
		break;
	}
	}
}

CounterButton::CounterButton(lv_obj_t *parent)
{
	this->clicks = new uint32_t{0};
	this->lv_button = lv_btn_create(parent);

	lv_obj_add_event_cb(this->lv_button, counter_button_callback, LV_EVENT_ALL, this->clicks); /*Assign a callback to the button*/

	lv_obj_t *label = lv_label_create(this->lv_button); /*Add a label to the button*/
	lv_label_set_text(label, "Button");					/*Set the labels text*/
	lv_obj_center(label);
}

lv_obj_t *CounterButton::get_lv_obj()
{
	return this->lv_button;
}