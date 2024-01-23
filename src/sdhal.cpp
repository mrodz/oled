#include <SD.h>
#include "sdhal.hpp"
#include "login.hpp"
#include "lib.hpp"

sd_init_err sdcard_init()
{
	Serial.print("\nInitializing SD card...");

	if (!SD.begin(SD_CS))
	{
		Serial.print(sd_init_err_msg(SD_INIT_FAIL));
		return SD_INIT_FAIL;
	}
	else
	{
		Serial.println("Wiring is correct and a card is present.");
	}

	return SD_INIT_OK;
}

const char *sd_init_err_msg(sd_init_err code)
{
	switch (code)
	{
	case SD_INIT_FAIL:
		return "Missing Filesystem\n\n"
			   "Please check:\n"
			   "* is an SD card inserted?\n"
			   "* was the card formatted correctly?\n"
			   "* is the SD card port damaged?";
	case SD_INIT_OK:
		return NO_ERR_STR;
	}
	return "sd_init_err_msg: Invalid Error Code";
}

ld_user_cfg_err load_user_config(User * user, const char *username)
{
	size_t uname_len = strlen(username);
	size_t path_len = 5 + uname_len + 5;
	char *path = new char[path_len];

	if (path == nullptr)
	{
		return LD_USER_CFG_OUT_OF_MEMORY;
	}

	size_t last_pos;

	if ((last_pos = snprintf(path, sizeof(path), "/usr/%s/data", username)) < 0)
	{
		delete[] path;
		return LD_USER_CFG_FMT_FAIL;
	}

	File attempted_load = SD.open(path);

	if (!attempted_load)
	{
		delete[] path;
		return LD_USER_CFG_FILE_NOT_FOUND;
	}

	attempted_load.find("nickname:");

	String nickname = attempted_load.readStringUntil('&');

	Serial.printf("nickname = %s", nickname);

	const char *nickname_c_str = nickname.c_str();

	*user = User(nickname_c_str);

	attempted_load.close();
	delete[] path;

	return LD_USER_CFG_OK;
}

const char *ld_user_cfg_err_msg(ld_user_cfg_err code)
{
	switch (code)
	{
	case LD_USER_CFG_OUT_OF_MEMORY:
		return "Out of Memory";
	case LD_USER_CFG_FMT_FAIL:
		return "Formatting path string failed";
	case LD_USER_CFG_FILE_NOT_FOUND:
		return "The `usr/%NAME%/data` file was not found";
	case LD_USER_CFG_OK:
		return NO_ERR_STR;
	}
	return "ld_user_cfg_err_msg: Invalid Error Code";
}

ld_users_err load_users(App *app)
{
	File user_dir = SD.open("/usr");
	uint8_t index = 0;
	while (index < PROFILE_C)
	{
		File entry = user_dir.openNextFile();
		if (!entry)
		{
			break;
		}

		const char *name = entry.name();

		if (!entry.isDirectory())
		{
			Serial.printf("Error: non-directory in /usr: %s\n", name);
			entry.close();
			user_dir.close();
			return LD_USERS_BAD_DIR;
		}

		User *tentative_profile;
		if (!app->get_profile(index, &tentative_profile)) {
			return LD_USERS_INDEX_OUT_OF_BOUNDS;
		}

		if (tentative_profile == nullptr) {
			return LD_USERS_INDEX_OUT_OF_BOUNDS;
		};

		ld_user_cfg_err config_code = load_user_config(tentative_profile, name);

		entry.close();

		if (config_code != LD_USER_CFG_OK && config_code != LD_USER_CFG_FILE_NOT_FOUND)
		{
			Serial.printf("attempting to load user config returned: %s\n", ld_user_cfg_err_msg(config_code));
			user_dir.close();
			return LD_USERS_BAD_CFG;
		}

		index++;
	}

	user_dir.close();

	return LD_USERS_OK;
}

const char *ld_users_err_msg(ld_users_err code)
{
	switch (code)
	{
		case LD_USERS_BAD_DIR:
			return "There was an unexpected file in the `/usr` directory";
		case LD_USERS_BAD_CFG:
			return "Could not load user profile. Check serial output";
		case LD_USERS_INDEX_OUT_OF_BOUNDS:
			return "Attempted to index into a user profile that was out of bounds";
		case LD_USERS_OK:
			return NO_ERR_STR;
	}
	return "ld_users_err_msg: Invalid Error Code";
}