#include <SD.h>
#include "sdhal.hpp"
#include "login.hpp"

const char *sdcard_error_message(int code)
{
	switch (code)
	{
	case 1:
		return "Missing Filesystem\n\n"
			   "Please check:\n"
			   "* is an SD card inserted?\n"
			   "* was the card formatted correctly?\n"
			   "* is the SD card port damaged?";
	case 0:
		return "No error";
	}
	return "Invalid Error Code";
}

void sdcard_init_fs()
{
	if (!SD.exists("/usr"))
	{
		SD.mkdir("usr");
	}
}

int load_user_config(uint8_t index, const char *username)
{
	size_t uname_len = strlen(username);
	size_t path_len = 5 + uname_len + 5;
	char *path = new char[path_len];

	if (path == nullptr)
	{
		return 1;
	}

	size_t last_pos;

	if ((last_pos = snprintf(path, sizeof(path), "usr/%s/data", username)) < 0)
	{
		delete[] path;
		return 2;
	}

	File attempted_load = SD.open(path);

	if (!attempted_load)
	{
		delete[] path;
		return 3;
	}

	uint8_t username_buffer[MAX_USERNAME_LEN] = {0};

	attempted_load.readBytesUntil(':', username_buffer, sizeof(username_buffer));

	Serial.printf("username: %s", username_buffer);

	attempted_load.close();
	delete[] path;
}

void load_users()
{
	File user_dir = SD.open("usr");
	uint8_t index = 0;
	while (1)
	{
		File entry = user_dir.openNextFile();
		if (!entry)
		{
			break;
		}

		if (!entry.isDirectory())
		{
			Serial.println("Warning: non-directory in /usr");
			continue;
		}

		load_user_config(index, entry.name());
	}
}

int sdcard_init()
{
	Serial.print("\nInitializing SD card...");

	if (!SD.begin(SD_CS))
	{
		Serial.print(sdcard_error_message(1));
		return 1;
	}
	else
	{
		Serial.println("Wiring is correct and a card is present.");
	}

	return 0;
}