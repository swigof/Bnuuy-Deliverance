#include "entity.h"

uint16_t current_entity_true_map_tile = 0;
uint8_t sprite_index = 0;

// TODO fix for variable sprite size, split collision flags for sides to be able to pass through platforms
void check_collision(entity_t* const entity, const uint8_t level) {
    current_entity_true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((entity->y + 7) >> 3) - 2)] + ((entity->x - entity->hitbox_padding) >> 3);
    if(entity->direction & J_LEFT) {
        if (tileset_map_attr[current_entity_true_map_tile] & COLLIDABLE) {
            entity->x = (((entity->x + 7) >> 3)) << 3;
        }
    } else if(entity->direction & J_RIGHT) {
        if (tileset_map_attr[current_entity_true_map_tile] & COLLIDABLE) {
            entity->x = (((entity->x >> 3)) << 3) + entity->hitbox_padding;
        }
    }

    current_entity_true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((entity->y + 7) >> 3) - 2)] + ((entity->x - entity->hitbox_padding) >> 3);
    if (entity->direction & J_UP) {
        if (tileset_map_attr[current_entity_true_map_tile] & COLLIDABLE) {
            // set to below block
            // kill upward momentum
        }
    } else if (entity->direction & J_DOWN) {
        if (entity->y > MAX_PLAYER_Y || tileset_map_attr[current_entity_true_map_tile] & COLLIDABLE || tileset_map_attr[current_entity_true_map_tile + 1] & COLLIDABLE) {
            entity->y = (entity->y >> 3) << 3;
            entity->air_state = GROUNDED;
        }
    }
}

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