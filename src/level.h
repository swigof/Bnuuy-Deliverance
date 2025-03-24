#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <gbdk/platform.h>
#include "tileset_map.h"
#include "../obj/level_0.h"
#include "common.h"
#include "entity.h"
#include "../obj/box_sheet.h"
#include "../obj/player_sheet.h"
#include "../obj/truck_sheet.h"

#define MAP_WIDTH 20

typedef struct  {
    uint16_t map_height;
    uint16_t max_camera_y;
    const uint8_t* map;
    const uint8_t* attributes;
    void (*init_function)();
    void (*update_function)();
} level_t;

extern const level_t* current_level;

extern const level_t level_0;
extern const level_t level_1;

#endif //LEVEL_H
