#ifndef CAMERA_H
#define CAMERA_H

#include <gbdk/platform.h>

// TODO replace with level data from common header
#define MAX_CAMERA_Y ((tileset_map_height - DEVICE_SCREEN_HEIGHT) * 8)

extern uint16_t camera_y;

void init_camera(uint16_t y);
void update_camera();
void set_focus(uint16_t y);

#endif //CAMERA_H
