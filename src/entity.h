#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "tileset_map.h"
#include "camera.h"

#define GROUNDED 0b10000000
#define DOUBLE_JUMP 0b01000000
#define FLIP_X 0b00100000
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
    uint8_t frame_duration;
    const metasprite_t* const * metasprite;
    uint8_t animation_length;
} state_data_t;

typedef struct {
    uint16_t x, y;                   // bitwise 12[map position at center of the entity]4[subpixel position]
    int8_t vel_x, vel_y;
    uint8_t base_tile;
    uint8_t prop;
    uint8_t state;                   // bitwise 1[grounded]1[double jumped]1[flipx]5[unused]
    uint8_t active;                  // flag if entity is to be updated/rendered. entity can be deleted if unset
    uint8_t onscreen;
    uint8_t frame_counter;
    uint8_t animation_frame;
    const state_data_t* state_data;
    void (*update_function)(void*);  // argument is entity_t self reference
} entity_t;

typedef struct {
    uint16_t top, bottom, left, right;
} hitbox_record_t;

typedef struct {
    uint16_t start, end, coord;
} edge_t;

extern entity_t entity_to_add;

/**
 * Adds entity_to_add to the entity list
 * @return The pointer to the entity in the list or NULL if it failed to add
 */
entity_t* add_entity();

void update_entities();

/**
 * Flags the entity state for which horizontal direction it should be rendered in based on velocity
 * @param e Entity to process
 */
void velocity_direction_flip(entity_t* e);

/**
 * Processes velocity based movement and collision
 * @param e Entity to process
 * @return If the entity moved strictly horizontally
 */
uint8_t velocity_collision_move(entity_t* e);

/**
 * Makes air state adjustments to an entity based on if it is on a solid tile
 * @param e Entity to process
 */
void check_grounding(entity_t* e);

#endif
