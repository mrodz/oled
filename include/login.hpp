#pragma once

#ifndef OLED_LOGIN_HPP
#define OLED_LOGIN_HPP

#include <lvgl.h>

class User
{
	char *name;

public:
	User();
	User(const char *name);
	virtual ~User();

	bool exists() const;
	const char *get_name() const;
};

#define PROFILE_EDIT_TITLE_BUF_LEN 64

unsigned char *encrypt_any_length_string(const char *input, uint8_t *key, uint8_t *iv);

namespace ui
{
	lv_obj_t *user_profile(lv_obj_t *parent, User *user);
	lv_obj_t *add_user_button(lv_obj_t *parent);
}

#endif