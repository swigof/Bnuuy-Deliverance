#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <gbdk/platform.h>
#include "tileset_map.h"

#define MAP_WIDTH 20

typedef struct  {
    uint16_t map_height;
    uint16_t max_camera_y;
    const uint8_t* map;
    const uint8_t* attributes;
    void (*init_function)();
    void (*update_function)();
} level_t;

extern const level_t level_1;

#endif //LEVEL_H
