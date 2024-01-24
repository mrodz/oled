#include <lvgl.h>
#include "../lib.hpp"

void counter_button_callback(lv_event_t *event);

class CounterButton : public Component
{
	lv_obj_t *lv_button;
	uint32_t *clicks;
	lv_obj_t *get_lv_obj() override;

public:
	CounterButton(lv_obj_t *parent);
};