#include <Arduino.h>
#include <cstring>
#include <mbedtls/aes.h>
#include <lvgl.h>

#include "login.hpp"

unsigned char *encrypt_any_length_string(const char *input, uint8_t *key, uint8_t *iv)
{
	int padded_input_len = 0;
	int input_len = strlen(input) + 1;
	int modulo16 = input_len % 16;
	if (input_len < 16)
		padded_input_len = 16;
	else
		padded_input_len = (strlen(input) / 16 + 1) * 16;

	char *padded_input = new char[padded_input_len];

	if (padded_input == nullptr)
	{
		Serial.println("Failed to allocate memory");
		return nullptr;
	}

	memcpy(padded_input, input, strlen(input));
	uint8_t pkc5_value = (17 - modulo16);
	for (int i = strlen(input); i < padded_input_len; i++)
	{
		padded_input[i] = pkc5_value;
	}
	unsigned char *encrypt_output = new unsigned char[padded_input_len];
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);
	mbedtls_aes_setkey_enc(&aes, key, 256);
	mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_input_len, iv,
						  (unsigned char *)padded_input, encrypt_output);

	return encrypt_output;
}

User::User() {
	this->name = nullptr;
}

User::User(const char *name)
{
	size_t name_len = strlen(name);
	char *dst = new char[name_len + 1];

	strncpy(dst, name, name_len);
	dst[name_len] = '\0';

	this->name = dst;
}

User::~User() {
	if (this->name != nullptr) {
		delete[] this->name;
	}
}

const char * User::get_name() const {
	return this->name;
}

bool User::exists() const {
	return this->name != nullptr;
}

lv_obj_t * user_profile(lv_obj_t *parent, const User * user) {
	lv_obj_t * profile_container = lv_obj_create(parent);
	static lv_coord_t row_dsc[] = {135, 50, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };

	lv_obj_set_grid_dsc_array(profile_container, col_dsc, row_dsc);
	lv_obj_set_size(profile_container, 200, 250);
    lv_obj_center(profile_container);
    lv_obj_set_layout(profile_container, LV_LAYOUT_GRID);


	lv_obj_t * pfp = lv_obj_create(profile_container);
	lv_obj_set_grid_cell(pfp, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

	lv_obj_set_size(pfp, 120, 120);
	lv_obj_center(pfp);


	lv_obj_t * label = lv_label_create(profile_container);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

	const char * name;

	if (user->exists()) {
		name = user->get_name();
	} else {
		name = "Empty Profile";
	}

	lv_label_set_text_fmt(label, "%s " LV_SYMBOL_EDIT, name);

	return profile_container;
}