#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

#define MAP_WIDTH 20
#define DOOR_TILE_INDEX 12

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

extern const uint8_t door_tiles[];
extern const uint8_t empty_tiles[];

extern const level_t level_1;
extern const level_t level_2;
extern const level_t level_3;
extern const level_t level_4;
extern const level_t level_5;

void standard_init();

#endif //LEVEL_H
