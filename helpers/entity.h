#include <stdint.h>
#include <gb/gb.h>

#define	SUBPIXEL_THRESHOLD 0x02U

typedef struct {
    uint8_t x, y, prop, sub_x, sub_y, direction;
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;

// TODO 
// used renamed joypad direction constants?
// add subpixel thresholds constants for different entities

inline void move_entity_left(entity_t* entity) {
    if(entity->direction & J_LEFT) {
        entity->sub_x++;
    } else {
        entity->direction &= ~J_RIGHT;
        entity->direction |= J_LEFT;
        entity->sub_x = 1;
    }
    if(entity->sub_x >= 2) {
        entity->sub_x = 0;
        entity->x--;
    }
}

inline void move_entity_right(entity_t* entity) {
    if(entity->direction & J_RIGHT) {
        entity->sub_x++;
    } else {
        entity->direction &= ~J_LEFT;
        entity->direction |= J_RIGHT;
        entity->sub_x = 1;
    }
    if(entity->sub_x >= 2) {
        entity->sub_x = 0;
        entity->x++;
    }
}

inline void update_entity(entity_t entity) {
    set_sprite_prop(0, entity.prop);
    move_sprite(0, entity.x, entity.y);
}
