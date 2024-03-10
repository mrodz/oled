#include <Arduino.h>
#include <lvgl.h>
#include <WiFi.h>
#include <string>
#include <vector>
#include <HTTPClient.h>

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

size_t App::profile_count() const
{
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

bool App::add_profile(size_t index, User user)
{
	if (index != this->profile_count())
	{
		return false;
	}

	this->profiles.push_back(user);

	return true;
}

namespace wifi
{
	WifiManager::WifiManager(std::string _ssid, std::string _password) : ssid(_ssid), password(_password) {}

	void WifiManager::set_ssid(std::string ssid)
	{
		this->ssid = ssid;
		this->started = false;
	}

	void WifiManager::set_password(std::string password)
	{
		this->password = password;
		this->started = false;
	}

	const std::string &WifiManager::get_ssid() const
	{
		return this->ssid;
	}

	const std::string &WifiManager::get_password() const
	{
		return this->password;
	}

	wl_status_t WifiManager::ensure_connection(const int timeout_ms /*= -1*/, const int delay_ms /*= 200*/)
	{
		wl_status_t status;

		for (int i = 0; (status = WiFi.status()) != WL_CONNECTED && (i <= timeout_ms || timeout_ms == -1); i += delay_ms)
		{
			delay(delay_ms);
		}

		return status;
	}

	static lv_obj_t *wifi_mbox;

	static void wifi_modal_cb(lv_event_t *event)
	{
		if (event->code == LV_EVENT_DELETE && event->target == wifi_mbox)
		{
			/* Delete the parent modal background */
			lv_obj_del_async(lv_obj_get_parent(wifi_mbox));
			wifi_mbox = nullptr; /* happens before object is actually deleted! */
		}
		else if (event->code == LV_EVENT_VALUE_CHANGED)
		{
			Serial.printf("Button: %s\n", lv_msgbox_get_active_btn_text(event->current_target));

			/* A button was clicked */
			lv_msgbox_close(wifi_mbox);
		}
	}

	lv_obj_t *wifi_connection_modal(WifiManager *manager)
	{
		lv_obj_t *obj = lv_obj_create(lv_scr_act());

		lv_obj_remove_style_all(obj);
		lv_obj_set_pos(obj, 0, 0);
		lv_obj_set_style_bg_color(obj, lv_color_black(), 0);

		lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

		wifi_mbox = lv_msgbox_create(obj, "Wifi Settings", "", nullptr, true);

		lv_obj_set_align(wifi_mbox, LV_ALIGN_CENTER);
		lv_obj_add_event_cb(
			wifi_mbox, wifi_modal_cb,
			LV_EVENT_ALL, nullptr);
		lv_obj_set_size(wifi_mbox, 400, 300);


		static lv_style_t style;
		lv_style_init(&style);
		lv_style_set_flex_flow(&style, LV_FLEX_FLOW_COLUMN);
		lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_AROUND);
		lv_style_set_flex_track_place(&style, LV_FLEX_ALIGN_CENTER);
		lv_style_set_layout(&style, LV_LAYOUT_FLEX);

		lv_obj_t *col = lv_obj_create(wifi_mbox);
		lv_obj_remove_style_all(col);

		lv_obj_set_size(col, 350, 220);
		lv_obj_center(col);
		lv_obj_add_style(col, &style, 0);
		lv_obj_set_scrollbar_mode(col, LV_SCROLLBAR_MODE_OFF);

		lv_obj_t *wifi_connections = lv_list_create(col);
		lv_obj_set_size(wifi_connections, 320, 200);


		for (auto &network : get_nearby_ssid())
		{
			auto label = network.ssid;

			if (network.password_required) {
				label += " *";
			}

			lv_list_add_btn(wifi_connections, LV_SYMBOL_WIFI, label.c_str());
		}

		return obj;
	}

	static void wifi_modal_open_btn_click(lv_event_t *e)
	{
		if (e->code == LV_EVENT_CLICKED)
		{
			WifiManager *manager = static_cast<WifiManager *>(e->user_data);
			(void)wifi_connection_modal(manager);
		}
	}

	lv_obj_t *wifi_modal_open_btn(lv_obj_t *parent, WifiManager &manager)
	{
		lv_obj_t *btn = lv_btn_create(parent);

		lv_obj_t *btn_text = lv_label_create(btn);

		lv_label_set_text(btn_text, "Open Wifi Settings");

		lv_obj_add_event_cb(btn, wifi_modal_open_btn_click, LV_EVENT_ALL, static_cast<void *>(&manager));

		return btn;
	}

	std::vector<WifiSearchResult> get_nearby_ssid()
	{
		int network_max = WiFi.scanNetworks();

		std::vector<WifiSearchResult> result;
		result.reserve(network_max);

		for (int i = 0; i < network_max; i++)
		{
			WifiSearchResult network_entry{WiFi.SSID(i).c_str(), WiFi.encryptionType(i) == WIFI_AUTH_OPEN};

			result.push_back(network_entry);
		}

		return result;
	}
}