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

User::User()
{
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

User::~User()
{
	if (this->name != nullptr)
	{
		delete[] this->name;
	}
}

const char *User::get_name() const
{
	return this->name;
}

bool User::exists() const
{
	return this->name != nullptr;
}

static lv_obj_t *mbox;

static void profile_edit_modal_cb(lv_event_t *event)
{
	if (event->code == LV_EVENT_DELETE && event->target == mbox)
	{
		/* Delete the parent modal background */
		lv_obj_del_async(lv_obj_get_parent(mbox));
		mbox = nullptr; /* happens before object is actually deleted! */
	}
	else if (event->code == LV_EVENT_VALUE_CHANGED)
	{
		Serial.printf("Button: %s\n", lv_msgbox_get_active_btn_text(event->current_target));

		/* A button was clicked */
		lv_msgbox_close(mbox);
	}
}

static char title[PROFILE_EDIT_TITLE_BUF_LEN];

static void profile_edit_click_cb(lv_event_t *event)
{
	if (event->code == LV_EVENT_CLICKED)
	{
		User *user = (User *)event->user_data;
		/* Create a base object for the modal background */
		lv_obj_t *obj = lv_obj_create(lv_scr_act());
	
		lv_obj_remove_style_all(obj);
		lv_obj_set_pos(obj, 0, 0);
		lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
		// lv_obj_set_style_opa(obj, LV_OPA_20, 0);
		lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

		const char *src = user->exists() ? user->get_name() : "Empty Profile";

		Serial.println(src);

		memset(title, 0, sizeof(title));
		snprintf(title, sizeof(title), "Edit %s", src);
		title[sizeof(title) - 1] = '\0';

		Serial.print("@@@ ");
		Serial.println(title);

		mbox = lv_msgbox_create(obj, title, "", nullptr, true);

		lv_obj_set_align(mbox, LV_ALIGN_CENTER);
		lv_obj_add_event_cb(
			mbox, profile_edit_modal_cb,
			LV_EVENT_ALL, nullptr);

		lv_obj_t *b = lv_btn_create(mbox);

		lv_obj_t *bl = lv_label_create(b);
		lv_label_set_text(bl, "cool");
	}
}

static void profile_edit_button(lv_obj_t *parent, User *user)
{
	lv_obj_t *btn = lv_btn_create(parent);
	lv_obj_set_style_bg_color(btn, lv_color_black(), 0);
	lv_obj_set_style_align(btn, LV_ALIGN_TOP_RIGHT, 0);

	lv_obj_add_event_cb(btn, profile_edit_click_cb, LV_EVENT_ALL, user);

	lv_obj_t *label = lv_label_create(btn);
	lv_label_set_text(label, LV_SYMBOL_EDIT);
}

lv_obj_t *user_profile(lv_obj_t *parent, User *user)
{
	lv_obj_t *profile_container = lv_obj_create(parent);
	lv_obj_set_style_pad_top(profile_container, 20, 1);

	static lv_coord_t row_dsc[] = {135, 50, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

	lv_obj_set_grid_dsc_array(profile_container, col_dsc, row_dsc);
	lv_obj_set_size(profile_container, 200, 250);
	lv_obj_center(profile_container);
	lv_obj_set_layout(profile_container, LV_LAYOUT_GRID);

	lv_obj_t *pfp = lv_obj_create(profile_container);
	lv_obj_set_grid_cell(pfp, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

	lv_obj_set_size(pfp, 120, 120);
	lv_obj_center(pfp);

	lv_obj_t *label = lv_label_create(profile_container);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

	const char *name;

	if (user->exists())
	{
		name = user->get_name();
	}
	else
	{
		name = "Empty Profile";
	}

	lv_label_set_text_fmt(label, "%s", name);

	profile_edit_button(profile_container, user);

	return profile_container;
}

static lv_obj_t *add_user_menu;

void add_user_modal_cb(lv_event_t * event) {
	if (event->code == LV_EVENT_DELETE && event->target == add_user_menu)
	{
		/* Delete the parent modal background */
		lv_obj_del_async(lv_obj_get_parent(add_user_menu));
		add_user_menu = nullptr; /* happens before object is actually deleted! */
	}
	else if (event->code == LV_EVENT_VALUE_CHANGED)
	{
		Serial.printf("Button: %s\n", lv_msgbox_get_active_btn_text(event->current_target));

		/* A button was clicked */
		lv_msgbox_close(add_user_menu);
	}
}

void add_user_cb(lv_event_t * event) {
	if (event->code == LV_EVENT_CLICKED) {
		/* Create a base object for the modal background */
		lv_obj_t *obj = lv_obj_create(lv_scr_act());

		lv_obj_remove_style_all(obj);
		lv_obj_set_pos(obj, 0, 0);
		lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
		// lv_obj_set_style_opa(obj, LV_OPA_20, 0);
		lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

		add_user_menu = lv_msgbox_create(obj, "Create User", "", nullptr, true);

		lv_obj_set_align(add_user_menu, LV_ALIGN_CENTER);
		lv_obj_add_event_cb(
			add_user_menu, add_user_modal_cb,
			LV_EVENT_ALL, nullptr);

		lv_obj_t *b = lv_btn_create(add_user_menu);

		lv_obj_t *bl = lv_label_create(b);
		lv_label_set_text(bl, "cool");
	}
}

lv_obj_t *add_user_button(lv_obj_t *parent)
{
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
	lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_AROUND);
	lv_style_set_flex_track_place(&style, LV_FLEX_ALIGN_CENTER);
	lv_style_set_pad_all(&style, 20);
	lv_style_set_layout(&style, LV_LAYOUT_FLEX);

	lv_obj_t *container = lv_obj_create(parent);

	lv_obj_remove_style_all(container);
	lv_obj_add_style(container, &style, 0);
	lv_obj_set_size(container, 150, 250);

	lv_obj_t *root_button = lv_btn_create(container);

	lv_obj_add_event_cb(root_button, add_user_cb, LV_EVENT_ALL, nullptr);

	lv_obj_t *label = lv_label_create(root_button);
	lv_label_set_text(label, LV_SYMBOL_PLUS);

	lv_obj_center(label);

	lv_obj_set_style_pad_all(root_button, 40, 0);
	lv_obj_set_style_radius(root_button, LV_RADIUS_CIRCLE, 0);
	lv_obj_set_style_align(root_button, LV_ALIGN_CENTER, 0);

	lv_obj_center(root_button);

	lv_obj_t *description = lv_label_create(container);

	lv_label_set_text(description, "Add user");
	lv_obj_set_style_pad_top(description, 20, 0);

	lv_obj_center(container);

	return container;
}