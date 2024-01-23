#include <lvgl.h>
#include "lib.hpp"

void Component::set_pos(Position position)
{
	lv_obj_set_pos(this->get_lv_obj(), position.x, position.y);
}

void Component::set_size(Dimension size)
{
	lv_obj_set_pos(this->get_lv_obj(), size.width, size.height);
}

App::App() = default;

bool App::get_profile(size_t index, User **dst)
{
	if (index > 3)
	{
		return false;
	}

	*dst = this->profiles + index;

	return true;
}