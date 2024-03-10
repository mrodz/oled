#include "spotify.hpp"
#include "lib.hpp"

#include <string>
#include <ctime>
#include <memory>

Song::Song(std::string _name, std::string _artist) : name(_name), artist(_artist) {}

Session::Session(std::string _access_token, time_t _start) : access_token(_access_token), start(_start) {}

SpotifyManager::SpotifyManager(std::string _client_id, std::string _client_secret, WifiManager &_manager) : client_id(_client_id), client_secret(_client_secret), manager(_manager) {}

static const char SPOTIFY_ROOT_CERT[] =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIGyTCCBbGgAwIBAgIQAYb/D7YM2sg7QNnLmPmXRzANBgkqhkiG9w0BAQsFADBZ\n"
	"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTMwMQYDVQQDEypE\n"
	"aWdpQ2VydCBHbG9iYWwgRzIgVExTIFJTQSBTSEEyNTYgMjAyMCBDQTEwHhcNMjQw\n"
	"MjA1MDAwMDAwWhcNMjUwMjA0MjM1OTU5WjBOMQswCQYDVQQGEwJTRTESMBAGA1UE\n"
	"BxMJU3RvY2tob2xtMRMwEQYDVQQKEwpTcG90aWZ5IEFCMRYwFAYDVQQDDA0qLnNw\n"
	"b3RpZnkuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0m+b8wxn\n"
	"lRzCtGq6M5CnGN+sSiKrQPwOjQ/pu3I8q2JZXy6h1hiPSm8XY9yU9r/1PLOUlS02\n"
	"qXFIk9yH9H06XReHWEbSpLkG/ewdNA+bw43ct7wIcSyZlazVE8VwTK5tRXd7t/dK\n"
	"uvzA+vH2yfLko13+dpQZX8+wpKNN6yFNC8RWpEjiki7FuX6KxBAjJ2+zuN3ZY97+\n"
	"YcD2nR5ukZQkJU0N67nkR3F2qKRdSxvsBqtD7kquFhEMBzC6maF01DuAlmBgbisu\n"
	"UmvUcqeFGhCUTto5B4nrZsGAWuaONjPsRDXBdOB4mC9h8nw7jZThMml/Qdbr4oTC\n"
	"8g1ZLdfDRzX1oQIDAQABo4IDljCCA5IwHwYDVR0jBBgwFoAUdIWAwGbH3zfez70p\n"
	"N6oDHb7tzRcwHQYDVR0OBBYEFOKOubTXdmj+gEu7rZkN0PkcSLfTMCUGA1UdEQQe\n"
	"MByCDSouc3BvdGlmeS5jb22CC3Nwb3RpZnkuY29tMD4GA1UdIAQ3MDUwMwYGZ4EM\n"
	"AQICMCkwJwYIKwYBBQUHAgEWG2h0dHA6Ly93d3cuZGlnaWNlcnQuY29tL0NQUzAO\n"
	"BgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMIGf\n"
	"BgNVHR8EgZcwgZQwSKBGoESGQmh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdp\n"
	"Q2VydEdsb2JhbEcyVExTUlNBU0hBMjU2MjAyMENBMS0xLmNybDBIoEagRIZCaHR0\n"
	"cDovL2NybDQuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsRzJUTFNSU0FTSEEy\n"
	"NTYyMDIwQ0ExLTEuY3JsMIGHBggrBgEFBQcBAQR7MHkwJAYIKwYBBQUHMAGGGGh0\n"
	"dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBRBggrBgEFBQcwAoZFaHR0cDovL2NhY2Vy\n"
	"dHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsRzJUTFNSU0FTSEEyNTYyMDIw\n"
	"Q0ExLTEuY3J0MAwGA1UdEwEB/wQCMAAwggF+BgorBgEEAdZ5AgQCBIIBbgSCAWoB\n"
	"aAB1AE51oydcmhDDOFts1N8/Uusd8OCOG41pwLH6ZLFimjnfAAABjXgRLFMAAAQD\n"
	"AEYwRAIgZ+iR8MCCPwofdJ8YXmIAnutuz5WboRtVzUDqzTiAnA8CIDieWqZlu/SS\n"
	"breL5DVHfOUU/MHIp5tZllSYznuT/wcsAHcAfVkeEuF4KnscYWd8Xv340IdcFKBO\n"
	"lZ65Ay/ZDowuebgAAAGNeBEskgAABAMASDBGAiEAnveJl9iS9cEcl3lVa2NNMSNH\n"
	"wQI8ey3NOYEGnpQNaa4CIQC8NqHR6q2ZBehlAWkx9K7L3qYGly9GkCeU73UpkEwO\n"
	"NAB2AObSMWNAd4zBEEEG13G5zsHSQPaWhIb7uocyHf0eN45QAAABjXgRLLoAAAQD\n"
	"AEcwRQIgDc9TGFKQpZb/SRM52w2hObxiUJBKiNLZ4MUAXMINMXUCIQDEKQl4yZbj\n"
	"IGG1ewCfAbDJKrR6mIuXlt9dLTCH8q+RXDANBgkqhkiG9w0BAQsFAAOCAQEAZY1F\n"
	"9XNlkcRjs1+Fp9PNAO61A6b40Exph1/Vuy+ewVsJQPpcF1Zj7Qz1jL8J0rJQI+QN\n"
	"QLNZy7gzozvtD14HEnDU+H0joxwmgLZfkUeQ7JCczFLZW1vaDvslAlNJUNtvadE8\n"
	"YM9LrKfWNHMHAdZcc+NbW7PEWFG9IcY5W6v3UClJO27hbyS+qBlWVfGtGAq0e9br\n"
	"cBtQBL2QgG5Q9fkdycLghOBuMXvUrmoLNUO/hi+wAeJ+wSJI54ORZkumNjn6dAWi\n"
	"kqoZDD4oKt3c4SYEb1pMl02PtFuZAERj3aHqpFSp28bwy/CIPNPC1F3UicAuNu5I\n"
	"eopoyfdMjajyt/H63g==\n"
	"-----END CERTIFICATE-----\n";

void SpotifyManager::refresh_token()
{
	(void)this->manager.ensure_connection();

	static const char SPOTIFY_URL[] = "https://accounts.spotify.com/api/token";

	WiFiClientSecure client;
	client.setCACert(SPOTIFY_ROOT_CERT);
	HTTPClient https;

	WiFiClientSecure &c = client;

	if (https.begin(c, SPOTIFY_URL))
	{
		int httpCode = https.GET();
	} else {
		goto cleanup;
	}

cleanup:
	https.end();
}