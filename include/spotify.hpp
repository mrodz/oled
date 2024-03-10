#ifndef SPOTIFY_HPP
#define SPOTIFY_HPP

#include <array>
#include <ctime>
#include <string>
#include <memory>

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

class SpotifyManager
{
private:
	std::string client_id;
	std::string client_secret;
	std::unique_ptr<Session> session;
	WifiManager& manager;

public:
	SpotifyManager(std::string client_id, std::string client_secret, WifiManager& manager);

	void refresh_token();
};

#endif // #ifndef SPOTIFY_HPP