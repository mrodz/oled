#pragma once

#ifndef OLED_LIB_HPP
#define OLED_LIB_HPP

#include <lvgl.h>

struct Position
{
	lv_coord_t x;
	lv_coord_t y;

	Position(lv_coord_t _x, lv_coord_t _y) : x(_x), y(_y) {}
};

struct Dimension
{
	lv_coord_t width;
	lv_coord_t height;

	Dimension(lv_coord_t _width, lv_coord_t _height) : width(_width), height(_height) {}
};

class Component
{
	virtual lv_obj_t *get_lv_obj();

public:
	void set_pos(Position position);
	void set_size(Dimension size);
};

#endif // #ifndef OLED_LIB_HPP