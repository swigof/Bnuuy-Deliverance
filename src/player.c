#include "player.h"

const state_data_t player_idle = {
    ((player_idle_WIDTH >> 3) << 4) | (player_idle_HEIGHT >> 3),
    (player_idle_PIVOT_W << 4) | (player_idle_PIVOT_H),
    15,
    player_idle_tiles,
    player_idle_TILE_COUNT,
    30,
    player_idle_metasprites,
    2
};
