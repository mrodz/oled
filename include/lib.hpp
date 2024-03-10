#pragma once

#ifndef OLED_LIB_HPP
#define OLED_LIB_HPP

#include <lvgl.h>
#include <vector>
#include <string>
#include <memory>
#include <WiFi.h>
#include <HTTPClient.h>

#include "login.hpp"

static const char *NO_ERR_STR = "no error";

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

class App
{
	std::vector<User> profiles = {};

public:
	App();
	bool add_profile(size_t index, User user);
	bool get_profile(size_t index, User **dst);
	size_t profile_count() const;
};

namespace wifi
{

	class WifiManager
	{
	private:
		std::string ssid;
		std::string password;
		bool started;

	public:
		WifiManager() = default;
		WifiManager(std::string ssid, std::string password);

		void set_ssid(std::string ssid);
		void set_password(std::string password);
		wl_status_t ensure_connection(const int timeout_ms = -1, const int delay_ms = 200);
		const std::string &get_ssid() const;
		const std::string &get_password() const;
		bool has_started() const;
	};

	struct WifiSearchResult {
		std::string ssid;
		bool password_required;
	};

	std::vector<WifiSearchResult> get_nearby_ssid();
	lv_obj_t *wifi_connection_modal(WifiManager *manager);
	lv_obj_t *wifi_modal_open_btn(lv_obj_t *parent, WifiManager &manager);
}

#endif // #ifndef OLED_LIB_HPP