#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "camera.h"
#include "level.h"

#define GROUNDED 0b10000000
#define DOUBLE_JUMP 0b01000000
#define FLIP_X 0b00100000

#define MAX_X_COORD 159

#define MAP_COORD(C) (C >> 4)                    // Get map coordinate from entity x/y
#define TILE_COORD(C) (C & 0b111111111000)       // Rounds a 12bit map coordinate down to the closest 8 pixel coordinate

#define MAX_ENTITIES 5

enum tile_type {TT_NONE, TT_PLATFORM, TT_SOLID, TT_HAZARD};

typedef struct {
    uint8_t half_sprite_height;
    uint8_t hitbox_half_width;
    uint8_t hitbox_half_height;
    int8_t speed;
    uint8_t frame_duration;
    const metasprite_t* const * metasprite;
    uint8_t animation_length;
} state_data_t;

typedef struct {
    uint16_t top, bottom, left, right;
} hitbox_record_t;

typedef struct {
    uint16_t x, y;                   // bitwise 12[map position at center of the entity]4[subpixel position]
    int8_t vel_x, vel_y;
    uint8_t base_tile;
    uint8_t prop;
    uint8_t state;                   // bitwise 1[grounded]1[double jumped]1[flipx]5[unused]
    uint8_t active;                  // flag if entity is to be updated/rendered. entity can be deleted if unset
    hitbox_record_t hitbox;
    uint8_t onscreen;
    uint8_t frame_counter;
    uint8_t animation_frame;
    const state_data_t* state_data;
    void (*update_function)(void*);  // argument is entity_t self reference
} entity_t;

typedef struct {
    uint16_t start, end, coord;
} edge_t;

extern entity_t entity_to_add;
extern uint8_t sprite_index;

// From main.c
extern entity_t* player;
extern entity_t* box;

/**
 * Populates the hitbox data for an entity
 * @param e The entity to process
 */
void populate_hitbox_record(entity_t* e);

/**
 * Adds entity_to_add to the entity list
 * @return The pointer to the entity in the list or NULL if it failed to add
 */
entity_t* add_entity(void);

void update_entities(void);

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

/**
 * Check if two entity hitboxes overlap with eachother
 * @param h1 The first entity hitbox
 * @param h2 The second entity hitbox
 * @return TRUE if they overlap, FALSE otherwise
 */
inline uint8_t do_hitboxes_overlap(const hitbox_record_t* h1, const hitbox_record_t* h2) {
    return (h1->right >= h2->left && h2->right >= h1->left)
        && (h1->bottom >= h2->top && h2->bottom >= h1->top);
}

// Check if a hitbox is at a coordinate
inline uint8_t is_at_coord(hitbox_record_t* h, uint16_t x, uint16_t y) {
    return h->left < x && h->right > x && h->top < y && h->bottom > y;
}

#endif
