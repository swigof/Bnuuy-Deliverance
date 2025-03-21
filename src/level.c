#include "level.h"

const level_t level_1 = {
    tileset_map_height,
    (tileset_map_height - DEVICE_SCREEN_HEIGHT) << 3,
    tileset_map,
    tileset_map_attr,
    NULL,
    NULL
};
