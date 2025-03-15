#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "tileset_map.h"
#include "player.h"
#include "camera.h"

#define GROUNDED       0b00010000u
#define JUMPING        0b00100000u
#define FALLING        0b01000000u
#define USED_DOUBLE    0b10000000u
#define FLAG_MASK      0b11110000u
#define VELOCITY_MASK  0b00001111u
#define MAX_VELOCITY   0b00000111u
#define MIN_VELOCITY   0b00000000u

#define SPRITE_WIDTH(D) ((D >> 4) << 3)          // Get width of a sprite from entity sprite_dimensions
#define SPRITE_HEIGHT(D) ((D & 0b00001111) << 3) // Get height of a sprite from entity sprite_dimensions
#define WIDTH_MARGIN(P) (P >> 4)                 // Get width margin from entity hitbox_margin
#define HEIGHT_MARGIN(P) (P & 0b00001111)        // Get height margin from entity hitbox_margin
#define MAP_COORD(C) (C >> 4)                    // Get map coordinate from entity x/y
#define TILE_COORD(C) (C & 0b111111111000)       // Rounds a 12bit map coordinate down to the closest 8 pixel coordinate

extern uint8_t sprite_index;

typedef struct {
    uint16_t x, y;             // bitwise 12[map position at center of the entity]4[subpixel position]
    uint8_t prop, direction;
    uint8_t air_state;         // bitwise 1[double_jump]1[falling]1[jumping]1[grounded]4[speed]
    uint8_t sprite_dimensions; // bitwise 4[8 multiples for width]4[8 multiples for height]
    uint8_t hitbox_margin;     // bitwise 4[margin to horizontal hitbox edges from center]4[same vertically]
    uint8_t flags;             // render, tile/sprite, ? ...
} entity_t;

typedef struct {
    uint16_t top, bottom, left, right;
} hitbox_record_t;

void move_entity_up(entity_t* entity, uint8_t amount);
void move_entity_down(entity_t* entity, uint8_t amount);
void move_entity_left(entity_t* entity, uint8_t amount);
void move_entity_right(entity_t* entity,  uint8_t amount);

void render_entity(const entity_t* entity);

#endif
