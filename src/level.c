#include "level.h"

void level_0_init() {
    DISPLAY_OFF;

    player->x = 20<<4;
    player->y = 100<<4;
    populate_hitbox_record(player);

    box->x = 40<<4;
    box->y = 100<<4;
    populate_hitbox_record(box);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;
}

void level_1_init() {
    DISPLAY_OFF;

    player->x = 20<<4;
    player->y = 300<<4;
    populate_hitbox_record(player);

    box->x = 40<<4;
    box->y = 300<<4;
    populate_hitbox_record(box);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;
}

const level_t level_0 = {
    level_0_HEIGHT >> 3,
    level_0_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_0_map,
    level_0_map_attributes,
    level_0_init,
    NULL
};

const level_t level_1 = {
    tileset_map_height,
    (tileset_map_height - DEVICE_SCREEN_HEIGHT) << 3,
    tileset_map,
    tileset_map_attr,
    level_1_init,
    NULL
};
