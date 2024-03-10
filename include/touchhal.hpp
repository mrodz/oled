#pragma once

#ifndef OLED_TOUCH_H
#define OLED_TOUCH_H

/*******************************************************************************
 * Touch library:
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 ******************************************************************************/

#define TOUCH_GT911
#define TOUCH_GT911_SCL 20
#define TOUCH_GT911_SDA 19
#define TOUCH_GT911_INT 40
#define TOUCH_GT911_RST 38
#define TOUCH_GT911_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 800
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

#include <Wire.h>
#include <TAMC_GT911.h>

extern int touch_last_x;
extern int touch_last_y;

void touch_init();
bool touch_has_signal();
bool touch_touched(int16_t width, int16_t height);
bool touch_released();

#endif // #ifndef OLED_TOUCH_H