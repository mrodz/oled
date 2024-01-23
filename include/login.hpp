#ifndef OLED_LOGIN_HPP
#define OLED_LOGIN_HPP

#include <lvgl.h>

class User
{
	char * name;

public:
	User();
	User(const char *name);
	virtual ~User();

	bool exists() const;
	const char * get_name() const;
};

unsigned char *encrypt_any_length_string(const char *input, uint8_t *key, uint8_t *iv);
lv_obj_t * user_profile(lv_obj_t *parent, const User * user);

#endif