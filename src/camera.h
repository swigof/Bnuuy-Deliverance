#ifndef CAMERA_H
#define CAMERA_H

#include <gbdk/platform.h>
#include "level.h"

extern uint16_t camera_y;

void init_camera(uint16_t y);
void update_camera();
void set_focus(uint16_t y);

#endif //CAMERA_H
