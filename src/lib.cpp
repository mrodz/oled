#include <Arduino.h>
#include <lvgl.h>
#include <WiFi.h>
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

WifiManager &WifiManager::get_instance()
{
	static WifiManager instance;

	if (!instance.ssid.empty() && !instance.password.empty() && !instance.started)
	{
		WiFi.begin(instance.ssid.c_str(), instance.password.c_str());
	}

	(void)instance.ensure_connection(600, 50);

	return instance;
}

void WifiManager::set_ssid(std::string ssid)
{
	this->ssid = ssid;
}

void WifiManager::set_password(std::string password)
{
	this->password = password;
}

wl_status_t WifiManager::ensure_connection(const int timeout_ms = -1, const int delay_ms = 200)
{
	wl_status_t status;

	for (int i = 0; (status = WiFi.status()) != WL_CONNECTED && (i <= timeout_ms || timeout_ms == -1); i += delay_ms)
	{
		delay(delay_ms);
	}

	return status;
}