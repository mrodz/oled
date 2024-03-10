#pragma once

#ifndef SPOTIFY_HPP
#define SPOTIFY_HPP

#include <lvgl.h>

#include <array>
#include <ctime>
#include <string>
#include <memory>

#include "lib.hpp"

#define MAX_SONG_NAME_LEN 31
#define MAX_ARTIST_NAME_LEN 31

class Song
{
public:
	Song(std::string _name, std::string _artist);

private:
	std::string name;
	std::string artist;
};

class Session
{
private:
	std::string access_token;
	time_t start;

public:
	Session(std::string _access_token, time_t _start);
	int seconds_to_expiration();
	bool expired();
};

#define ERR_NO_INTERNET 503

enum token_refresh_result_t {
	TOKEN_REFRESH_OK,
	TOKEN_REFRESH_ERR_HTTPS_APPLICATION,
	TOKEN_REFRESH_ERR_NO_INTERNET = ERR_NO_INTERNET
};

class SpotifyManager
{
private:
	std::string client_id;
	std::string client_secret;
	std::unique_ptr<Session> session;
	wifi::WifiManager& manager;

public:
	SpotifyManager(std::string client_id, std::string client_secret, wifi::WifiManager& manager);

	token_refresh_result_t refresh_token();
};


#endif // #ifndef SPOTIFY_HPP