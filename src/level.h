#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <gbdk/platform.h>
#include "../obj/level_0.h"
#include "../obj/level_1.h"
#include "../obj/level_2.h"
#include "../obj/level_3.h"
#include "../obj/level_4.h"
#include "../obj/level_5.h"
#include "../obj/level_6.h"
#include "../obj/level_end.h"
#include "../obj/level_long.h"
#include "../obj/level_elevator.h"
#include "common.h"
#include "entity.h"
#include "../obj/box_sheet.h"
#include "../obj/player_sheet.h"
#include "../obj/truck_sheet.h"
#include "color.h"

#define MAP_WIDTH 20

typedef struct level_t level_t;
struct level_t {
    uint16_t map_height;
    uint16_t max_camera_y;
    const uint8_t* map;
    const uint8_t* attributes;
    void (*init_function)();
    void (*update_function)();
    const level_t* next_level;
    const uint16_t door_x, door_y;
    const uint16_t start_x, start_y;
    const uint8_t bank;
};

extern const uint16_t width_multiplication_table[];

extern const level_t* current_level;

extern const level_t level_0;

#endif //LEVEL_H
