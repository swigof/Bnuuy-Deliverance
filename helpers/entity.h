#include <stdint.h>
#include <gb/gb.h>

#define	SUBPIXEL_THRESHOLD 0x02U

typedef struct {
    uint8_t x, y, prop, sub_x, sub_y, direction, move_amount;
    uint16_t true_map_tile;
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;

// TODO 
// used renamed joypad direction constants?
// add subpixel thresholds constants for different entities

inline void move_entity_left(entity_t* entity, const uint8_t amount) {
    entity->move_amount = amount;
    while (entity->move_amount >= SUBPIXEL_THRESHOLD) {
        entity->x--;
        entity->move_amount -= SUBPIXEL_THRESHOLD;
    }
    if(entity->direction & J_LEFT) {
        entity->sub_x += entity->move_amount;
        if(entity->sub_x >= SUBPIXEL_THRESHOLD) {
            entity->sub_x -= SUBPIXEL_THRESHOLD;
            entity->x--;
        }
    } else {
        entity->direction &= ~J_RIGHT;
        entity->direction |= J_LEFT;
        entity->sub_x = entity->move_amount;
    }
}

inline void move_entity_right(entity_t* entity, const uint8_t amount) {
    entity->move_amount = amount;
    while (entity->move_amount >= SUBPIXEL_THRESHOLD) {
        entity->x++;
        entity->move_amount -= SUBPIXEL_THRESHOLD;
    }
    if(entity->direction & J_RIGHT) {
        entity->sub_x += entity->move_amount;
        if(entity->sub_x >= SUBPIXEL_THRESHOLD) {
            entity->sub_x -= SUBPIXEL_THRESHOLD;
            entity->x++;
        }
    } else {
        entity->direction &= ~J_LEFT;
        entity->direction |= J_RIGHT;
        entity->sub_x = entity->move_amount;
    }
}

inline void move_entity_up(entity_t* entity, const uint8_t amount) {
    entity->move_amount = amount;
    while (entity->move_amount >= SUBPIXEL_THRESHOLD) {
        entity->y--;
        entity->move_amount -= SUBPIXEL_THRESHOLD;
    }
    if(entity->direction & J_UP) {
        entity->sub_y += entity->move_amount;
        if(entity->sub_y >= SUBPIXEL_THRESHOLD) {
            entity->sub_y -= SUBPIXEL_THRESHOLD;
            entity->y--;
        }
    } else {
        entity->direction &= ~J_DOWN;
        entity->direction |= J_UP;
        entity->sub_y = entity->move_amount;
    }
}

inline void move_entity_down(entity_t* entity, const uint8_t amount) {
    entity->move_amount = amount;
    while (entity->move_amount >= SUBPIXEL_THRESHOLD) {
        entity->y++;
        entity->move_amount -= SUBPIXEL_THRESHOLD;
    }
    if(entity->direction & J_DOWN) {
        entity->sub_y += entity->move_amount;
        if(entity->sub_y >= SUBPIXEL_THRESHOLD) {
            entity->sub_y -= SUBPIXEL_THRESHOLD;
            entity->y++;
        }
    } else {
        entity->direction &= ~J_UP;
        entity->direction |= J_DOWN;
        entity->sub_y = entity->move_amount;
    }
}

inline void update_entity(entity_t entity) {
    set_sprite_prop(0, entity.prop);
    move_sprite(0, entity.x, entity.y);
}
