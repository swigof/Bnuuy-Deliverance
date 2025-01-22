#include "entity.h"

// TODO remove redundancies across moves

void move_entity_up(entity_t* const entity, const uint8_t amount) {
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

void move_entity_down(entity_t* const entity, const uint8_t amount) {
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

void move_entity_left(entity_t* const entity, const uint8_t amount) {
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

void move_entity_right(entity_t* const entity, const uint8_t amount) {
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