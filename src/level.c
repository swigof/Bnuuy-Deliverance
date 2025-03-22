#include "level.h"

const level_t level_0 = {
    level_0_HEIGHT >> 3,
    level_0_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_0_map,
    level_0_map_attributes,
    NULL,
    NULL
};

const level_t level_1 = {
    tileset_map_height,
    (tileset_map_height - DEVICE_SCREEN_HEIGHT) << 3,
    tileset_map,
    tileset_map_attr,
    NULL,
    NULL
};
