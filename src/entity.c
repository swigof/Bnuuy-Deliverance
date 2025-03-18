#include "entity.h"

entity_t entity_to_add = {};

entity_t entities[MAX_ENTITIES];
uint8_t next_entity_index = 0;
uint8_t entity_count = 0;

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
    h->top = MAP_COORD(e->y) - HEIGHT_MARGIN(e->state_data->hitbox_margin);
    h->left = MAP_COORD(e->x) - WIDTH_MARGIN(e->state_data->hitbox_margin);
    h->right = MAP_COORD(e->x) + WIDTH_MARGIN(e->state_data->hitbox_margin);
    h->bottom = MAP_COORD(e->y) + HEIGHT_MARGIN(e->state_data->hitbox_margin);
}

entity_t empty_entity = {};
uint8_t entity_index = 0;
entity_t* add_entity() {
    if(entity_count < MAX_ENTITIES) {
        entities[next_entity_index] = entity_to_add;
        entity_count++;
        entity_to_add = empty_entity;
        entity_index = next_entity_index;
        if(entity_count < MAX_ENTITIES) {
            next_entity_index = 0;
            while(next_entity_index < MAX_ENTITIES && entities[next_entity_index].active)
                next_entity_index++;
        }
        return &entities[entity_index];
    }
    return NULL;
}

int8_t entity_iterator = 0;
uint8_t sprite_index = 0;
uint8_t bottom_tile = TT_NONE;
void update_entities() {
    sprite_index = 0;
    for(entity_iterator = MAX_ENTITIES - 1; entity_iterator >= 0; entity_iterator--) {
        if(entities[entity_iterator].active) {
            // todo Entity update call

            // Move
            entities[entity_iterator].x += entities[entity_iterator].vel_x;
            populate_hitbox_record(&entities[entity_iterator], &hitbox);
            if(entities[entity_iterator].vel_x > 0) {
                if (get_edge_tile_type(&hitbox, J_RIGHT) == TT_SOLID) {
                    entities[entity_iterator].x -= ((hitbox.right - TILE_COORD(hitbox.right)) << 4);
                    entities[entity_iterator].vel_x = 0;
                }
            } else if(entities[entity_iterator].vel_x < 0) {
                if (get_edge_tile_type(&hitbox, J_LEFT) == TT_SOLID) {
                    entities[entity_iterator].x += ((TILE_COORD(hitbox.left + 8) - hitbox.left) << 4);
                    entities[entity_iterator].vel_x = 0;
                }
            }
            entities[entity_iterator].y += entities[entity_iterator].vel_y;
            populate_hitbox_record(&entities[entity_iterator], &hitbox);
            if(entities[entity_iterator].vel_y < 0) {
                bottom_tile = TT_NONE;
                if (get_edge_tile_type(&hitbox, J_UP) == TT_SOLID) {
                    entities[entity_iterator].y += ((TILE_COORD(hitbox.top + 8) - hitbox.top) << 4);
                    entities[entity_iterator].vel_y = 0;
                }
            } else {
                bottom_tile = get_edge_tile_type(&hitbox, J_DOWN);
                if ((bottom_tile == TT_SOLID || bottom_tile == TT_PLATFORM) && entities[entity_iterator].vel_y > 0) {
                    entities[entity_iterator].y -= ((hitbox.bottom - TILE_COORD(hitbox.bottom)) << 4);
                    entities[entity_iterator].vel_y = 0;
                    entities[entity_iterator].state |= GROUNDED;
                } else {
                    entities[entity_iterator].state &= !GROUNDED;
                }
            }
            // Render
            sprite_index += move_metasprite_ex(
                    entities[entity_iterator].state_data->metasprite[entities[entity_iterator].frame_counter],
                    0,
                    entities[entity_iterator].prop,
                    sprite_index,
                    MAP_COORD(entities[entity_iterator].x) + DEVICE_SPRITE_PX_OFFSET_X,
                    MAP_COORD(entities[entity_iterator].y) - get_camera_y() + DEVICE_SPRITE_PX_OFFSET_Y);

        }
    }
}