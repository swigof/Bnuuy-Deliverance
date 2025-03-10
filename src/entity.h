#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <gb/gb.h>
#include <gb/metasprites.h>
#include "tileset_map.h"
#include "player.h"

#define	SUBPIXEL_THRESHOLD 0x02U

#define GROUNDED       0b00010000u
#define JUMPING        0b00100000u
#define FALLING        0b01000000u
#define USED_DOUBLE    0b10000000u
#define FLAG_MASK      0b11110000u
#define VELOCITY_MASK  0b00001111u
#define MAX_VELOCITY   0b00000111u
#define MIN_VELOCITY   0b00000000u

typedef struct {
    uint8_t x, y, prop, sub_x, sub_y, direction, move_amount;
    uint16_t true_map_tile;
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;

typedef struct {
    uint8_t air_state;
    entity_t e;
} player_t;

void move_entity_up(entity_t* const entity, const uint8_t amount);
void move_entity_down(entity_t* const entity, const uint8_t amount);
void move_entity_left(entity_t* const entity, const uint8_t amount);
void move_entity_right(entity_t* const entity, const uint8_t amount);

// TODO 
// used renamed joypad direction constants?
// add subpixel thresholds constants for different entities

inline void update_entity(entity_t entity) {
    set_sprite_prop(0, entity.prop);
    move_metasprite_ex(player_idle_metasprites[0], 0, 0, 0, entity.x, entity.y);
}

#endif