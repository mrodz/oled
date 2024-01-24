#ifndef OLED_SDHAL_HPP
#define OLED_SDHAL_HPP

#include <SPI.h>
#include <SD.h>
#include "login.hpp"
#include "lib.hpp"

#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

typedef enum
{
	SD_INIT_OK,
	SD_INIT_FAIL
} sd_init_err;

sd_init_err sdcard_init();
const char *sd_init_err_msg(sd_init_err code);

typedef enum
{
	LD_USER_CFG_OK,
	LD_USER_CFG_OUT_OF_MEMORY,
	LD_USER_CFG_FMT_FAIL,
	LD_USER_CFG_FILE_NOT_FOUND
} ld_user_cfg_err;

ld_user_cfg_err load_user_config(User *user, const char *username);
const char *ld_user_cfg_err_msg(ld_user_cfg_err code);

typedef enum
{
	LD_USERS_OK,
	LD_USERS_BAD_DIR,
	LD_USERS_BAD_CFG,
	LD_USERS_INDEX_OUT_OF_BOUNDS,
} ld_users_err;

ld_users_err load_users(App *app);
const char *ld_users_err_msg(ld_users_err code);

#endif // #ifndef OLED_SDHAL_HPP