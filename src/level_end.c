#include "level_end.h"
#include "player.h"
#include "../obj/level_final.h"

#pragma bank 1

const level_t level_end = {
        level_final_HEIGHT >> 3,
        level_final_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_final_map,
        level_final_map_attributes,
        standard_init,
        NULL,
        &level_end,
        0, 0,
        16, 288,
        BANK(level_final)
};
