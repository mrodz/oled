#include <Arduino.h>
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

size_t App::profile_count() const {
	return this->profiles.size();
}

bool App::get_profile(size_t index, User **dst)
{
	Serial.printf("Loading user profile #%u\n", index);

	if (index >= this->profile_count())
	{
		return false;
	}

	*dst = &this->profiles[index];

	return true;
}

bool App::add_profile(size_t index, User user) {
	if (index != this->profile_count()) {
		return false;
	}

	this->profiles.push_back(user);

	return true;
}
