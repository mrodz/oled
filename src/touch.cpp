#include "touch.hpp"

int touch_last_x = 0;
int touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

void touch_init()
{
	Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
	ts.begin();
	ts.setRotation(TOUCH_GT911_ROTATION);
}

bool touch_has_signal()
{
	return true;
}

bool touch_touched(int16_t width, int16_t height)
{
	ts.read();
	if (ts.isTouched)
	{
#if defined(TOUCH_SWAP_XY)
		touch_last_x = map(ts.points[0].y, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, width - 1);
		touch_last_y = map(ts.points[0].x, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, height - 1);
#else
		touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, width - 1);
		touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, height - 1);
#endif
		return true;
	}
	else
	{
		return false;
	}
}

bool touch_released()
{
	return true;
}