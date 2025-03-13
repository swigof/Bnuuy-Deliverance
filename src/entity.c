#include "entity.h"

uint8_t sprite_index = 0;

void check_collision(entity_t* const entity) {

}

void move_entity_up(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_UP;
    if(entity->direction & J_DOWN) {
        entity->direction &= ~J_DOWN;
        entity->sub_y = 0;
    }
    entity->sub_y += amount;
    while (entity->sub_y >= SUBPIXEL_THRESHOLD) {
        entity->y--;
        entity->sub_y -= SUBPIXEL_THRESHOLD;
    }
    // TODO check collision immediately
}

void move_entity_down(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_DOWN;
    if(entity->direction & J_UP) {
        entity->direction &= ~J_UP;
        entity->sub_y = 0;
    }
    entity->sub_y += amount;
    while (entity->sub_y >= SUBPIXEL_THRESHOLD) {
        entity->y++;
        entity->sub_y -= SUBPIXEL_THRESHOLD;
    }
    // TODO check collision immediately
}

void move_entity_left(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_LEFT;
    if(entity->direction & J_RIGHT) {
        entity->direction &= ~J_RIGHT;
        entity->sub_x = 0;
    }
    entity->sub_x += amount;
    while (entity->sub_x >= SUBPIXEL_THRESHOLD) {
        entity->x--;
        entity->sub_x -= SUBPIXEL_THRESHOLD;
    }
    // TODO check collision immediately
}

void move_entity_right(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_RIGHT;
    if(entity->direction & J_LEFT) {
        entity->direction &= ~J_LEFT;
        entity->sub_x = 0;
    }
    entity->sub_x += amount;
    while (entity->sub_x >= SUBPIXEL_THRESHOLD) {
        entity->x++;
        entity->sub_x -= SUBPIXEL_THRESHOLD;
    }
    // TODO check collision immediately
}

void render_entity(const entity_t* const entity) {
    sprite_index = move_metasprite_ex(
            player_idle_metasprites[0],
            0,
            entity->prop,
            sprite_index,
            entity->x,
            entity->y - get_camera_y()
    );
}
