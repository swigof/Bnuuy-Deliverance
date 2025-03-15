#include "entity.h"

enum tile_type {TT_NONE, TT_PLATFORM, TT_SOLID, TT_HAZARD};

uint8_t sprite_index = 0;

uint8_t tile = 0;
inline uint8_t get_tile_type(const uint16_t x, const uint16_t y) {
    tile = tileset_map[(x >> 3) + width_multiplication_table[y >> 3]];
    if (tile == 54) {
        return TT_SOLID;
    }
    return TT_NONE;
}

hitbox_record_t hitbox = {0,0,0,0}; // TODO possibly place in and update for each entity
uint16_t edge_iterator = 0;
uint8_t tile_type = TT_NONE;
uint8_t max_tile_type = TT_NONE;
/**
    Get the highest priority tile type along the hitbox edge of an entity for the current map
    @param h Pointer to hitbox record of the entity
    @param edge The edge to check (one of J_RIGHT, J_LEFT, J_UP, J_DOWN)
    @return returns The largest value tile type of those found on the edge
                    (TT_NONE < TT_PLATFORM < TT_SOLID < TT_HAZARD)
*/
uint8_t get_edge_tile_type(const hitbox_record_t* const h, uint8_t edge) {
    max_tile_type = TT_NONE;

    if (edge == J_UP) {
        for (edge_iterator = h->left; edge_iterator < (h->right - 1); edge_iterator += 8) {
            tile_type = get_tile_type(edge_iterator, h->top);
            if (tile_type > max_tile_type)
                max_tile_type = tile_type;
        }
        tile_type = get_tile_type((h->right - 1), h->top);
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    } else if (edge == J_DOWN) {
        for (edge_iterator = h->left; edge_iterator < (h->right - 1); edge_iterator += 8) {
            tile_type = get_tile_type(edge_iterator, (h->bottom - 1));
            if (tile_type > max_tile_type)
                max_tile_type = tile_type;
        }
        tile_type = get_tile_type((h->right - 1), (h->bottom - 1));
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    } else if (edge == J_LEFT) {
        for (edge_iterator = h->top; edge_iterator < (h->bottom - 1); edge_iterator += 8) {
            tile_type = get_tile_type(h->left, edge_iterator);
            if (tile_type > max_tile_type)
                max_tile_type = tile_type;
        }
        tile_type = get_tile_type(h->left, (h->bottom - 1));
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    } else if (edge == J_RIGHT) {
        for (edge_iterator = h->top; edge_iterator < (h->bottom - 1); edge_iterator += 8) {
            tile_type = get_tile_type((h->right - 1), edge_iterator);
            if (tile_type > max_tile_type)
                max_tile_type = tile_type;
        }
        tile_type = get_tile_type((h->right - 1), (h->bottom - 1));
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    }

    return max_tile_type;
}

void populate_hitbox_record(const entity_t* const e, hitbox_record_t* const h) {
    h->top = MAP_COORD(e->y) - HEIGHT_MARGIN(e->hitbox_margin);
    h->left = MAP_COORD(e->x) - WIDTH_MARGIN(e->hitbox_margin);
    h->right = MAP_COORD(e->x) + WIDTH_MARGIN(e->hitbox_margin);
    h->bottom = MAP_COORD(e->y) + HEIGHT_MARGIN(e->hitbox_margin);
}

void move_entity_up(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_UP;
    if(entity->direction & J_DOWN) {
        entity->direction &= ~J_DOWN;
        entity->y |= 0b0000000000001111;
    }
    entity->y -= amount;
    populate_hitbox_record(entity, &hitbox);
    if(get_edge_tile_type(&hitbox, J_UP) == TT_SOLID) {
        entity->y += ((TILE_COORD(hitbox.top + 8) - hitbox.top) << 4);
    }
}

void move_entity_down(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_DOWN;
    if(entity->direction & J_UP) {
        entity->direction &= ~J_UP;
        entity->y &= 0b1111111111110000;
    }
    entity->y += amount;
    populate_hitbox_record(entity, &hitbox);
    if(get_edge_tile_type(&hitbox, J_DOWN) == TT_SOLID) {
        entity->y -= ((hitbox.bottom - TILE_COORD(hitbox.bottom)) << 4);
    }
}

void move_entity_left(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_LEFT;
    if(entity->direction & J_RIGHT) {
        entity->direction &= ~J_RIGHT;
        entity->x |= 0b0000000000001111;
    }
    entity->x -= amount;
    populate_hitbox_record(entity, &hitbox);
    if(get_edge_tile_type(&hitbox, J_LEFT) == TT_SOLID) {
        entity->x += ((TILE_COORD(hitbox.left + 8) - hitbox.left) << 4);
    }
}

void move_entity_right(entity_t* const entity, const uint8_t amount) {
    entity->direction |= J_RIGHT;
    if(entity->direction & J_LEFT) {
        entity->direction &= ~J_LEFT;
        entity->x &= 0b1111111111110000;
    }
    entity->x += amount;
    populate_hitbox_record(entity, &hitbox);
    if(get_edge_tile_type(&hitbox, J_RIGHT) == TT_SOLID) {
        entity->x -= ((hitbox.right - TILE_COORD(hitbox.right)) << 4);
    }
}

void render_entity(const entity_t* const entity) {
    sprite_index += move_metasprite_ex(
        player_idle_metasprites[0],
        0,
        entity->prop,
        sprite_index,
        MAP_COORD(entity->x) + DEVICE_SPRITE_PX_OFFSET_X,
        MAP_COORD(entity->y) - get_camera_y() + DEVICE_SPRITE_PX_OFFSET_Y);
}
