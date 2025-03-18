#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "tileset_map.h"
#include "camera.h"

#define GROUNDED 0b10000000
#define DOUBLE_JUMP 0b01000000
#define JUMP_VELOCITY (-20)

#define SPRITE_WIDTH(D) ((D >> 4) << 3)          // Get width of a sprite from entity sprite_dimensions
#define SPRITE_HEIGHT(D) ((D & 0b00001111) << 3) // Get height of a sprite from entity sprite_dimensions
#define WIDTH_MARGIN(P) (P >> 4)                 // Get width margin from entity hitbox_margin
#define HEIGHT_MARGIN(P) (P & 0b00001111)        // Get height margin from entity hitbox_margin
#define MAP_COORD(C) (C >> 4)                    // Get map coordinate from entity x/y
#define TILE_COORD(C) (C & 0b111111111000)       // Rounds a 12bit map coordinate down to the closest 8 pixel coordinate

#define MAX_ENTITIES 10

enum tile_type {TT_NONE, TT_PLATFORM, TT_SOLID, TT_HAZARD};

typedef struct {
    uint8_t sprite_dimensions; // bitwise 4[8 multiples for width]4[8 multiples for height]
    uint8_t hitbox_margin;     // bitwise 4[margin to horizontal hitbox edges from center]4[same vertically]
    int8_t speed;
    const uint8_t* tiles;
    uint8_t tile_count;
    uint8_t frame_duration;
    const metasprite_t* const * metasprite;
    uint8_t animation_length;
} state_data_t;

typedef struct {
    uint16_t x, y;             // bitwise 12[map position at center of the entity]4[subpixel position]
    int8_t vel_x, vel_y;
    uint8_t prop;
    uint8_t state;             // entity dependant
    uint8_t active;            // flag if entity is to be updated/rendered
    uint8_t frame_counter;
    const state_data_t* state_data;
    void* update_function;
} entity_t;

typedef struct {
    uint16_t top, bottom, left, right;
} hitbox_record_t;

extern entity_t entity_to_add;

/**
 * Adds entity_to_add to the entity list
 * @return The pointer to the entity in the list or NULL if it failed to add
 */
entity_t* add_entity();

void update_entities();

#endif
