#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "tileset_map.h"
#include "player.h"
#include "camera.h"

#define	SUBPIXEL_THRESHOLD 0x02U

#define GROUNDED       0b00010000u
#define JUMPING        0b00100000u
#define FALLING        0b01000000u
#define USED_DOUBLE    0b10000000u
#define FLAG_MASK      0b11110000u
#define VELOCITY_MASK  0b00001111u
#define MAX_VELOCITY   0b00000111u
#define MIN_VELOCITY   0b00000000u

#define SPRITE_WIDTH_MASK   0b11110000u
#define SPRITE_HEIGHT_MASK  0b00001111u
#define HITBOX_WIDTH_MASK   0b11110000u
#define HITBOX_HEIGHT_MASK  0b00001111u

extern uint16_t current_entity_true_map_tile;
extern uint8_t sprite_index;

typedef struct { // TODO optimize direction
    uint16_t x, y;             // map position
    uint8_t prop, direction;
    uint8_t sub_x, sub_y;
    uint8_t air_state;         // bitwise 1[double_jump]1[falling]1[jumping]1[grounded]4[speed]
    uint8_t sprite_dimensions; // bitwise 4[8 multiples for width]4[8 multiples for height]
    uint8_t hitbox_padding;    // bitwise 4[width padding on both ends]4[height padding on top]
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;

void check_collision(entity_t* entity);

void move_entity_up(entity_t* entity, uint8_t amount);
void move_entity_down(entity_t* entity, uint8_t amount);
void move_entity_left(entity_t* entity, uint8_t amount);
void move_entity_right(entity_t* entity,  uint8_t amount);

void render_entity(const entity_t* entity);

// TODO 
// used renamed joypad direction constants?
// add subpixel thresholds constants for different entities

#endif
