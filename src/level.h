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
    void (*init_function)(void);
    void (*update_function)(void);
    const level_t* next_level;
    const uint16_t door_x, door_y;
    const uint16_t start_x, start_y;
    const uint8_t bank;
};

extern const uint16_t width_multiplication_table[];

extern const level_t* current_level;

extern uint8_t remove_door;
extern uint16_t door_x, door_y;

extern const uint8_t door_tiles[];
extern const uint8_t empty_tiles[];

extern const level_t level_1;
extern const level_t level_2;
extern const level_t level_3;
extern const level_t level_4;
extern const level_t level_5;

void standard_init(void);
void apply_level_transition(const level_t* level);
inline void level_transition_alt_coord(const level_t* level, uint16_t x, uint16_t y) {
    door_x = x;
    door_y = y;
    apply_level_transition(level);
}
inline void level_transition(const level_t* level) {
    door_x = current_level->door_x;
    door_y = current_level->door_y;
    apply_level_transition(level);
}

#endif //LEVEL_H
