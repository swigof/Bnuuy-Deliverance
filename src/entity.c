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

uint16_t edge_iterator = 0;
uint8_t tile_type = TT_NONE;
uint8_t max_tile_type = TT_NONE;
uint8_t get_vertical_edge_tile_type(edge_t* edge) {
    max_tile_type = TT_NONE;
    for(edge_iterator = edge->start; edge_iterator < edge->end; edge_iterator += 8) {
        tile_type = get_tile_type(edge->coord, edge_iterator);
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    }
    tile_type = get_tile_type(edge->coord, edge->end);
    if (tile_type > max_tile_type)
        max_tile_type = tile_type;
    return max_tile_type;
}
uint8_t get_horizontal_edge_tile_type(edge_t* edge) {
    max_tile_type = TT_NONE;
    for(edge_iterator = edge->start; edge_iterator < edge->end; edge_iterator += 8) {
        tile_type = get_tile_type(edge_iterator, edge->coord);
        if (tile_type > max_tile_type)
            max_tile_type = tile_type;
    }
    tile_type = get_tile_type(edge->end, edge->coord);
    if (tile_type > max_tile_type)
        max_tile_type = tile_type;
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
uint8_t moved = 0; // bitwise 6[unused]1[verical]1[horizontal]
hitbox_record_t hitbox = {};
edge_t edge = {};
void update_entities() {
    sprite_index = 0;
    for(entity_iterator = MAX_ENTITIES - 1; entity_iterator >= 0; entity_iterator--) {
        if(entities[entity_iterator].active) {
            // Move
            moved = 0b00;
            entities[entity_iterator].x += entities[entity_iterator].vel_x;
            if(entities[entity_iterator].vel_x > 0) {
                populate_hitbox_record(&entities[entity_iterator], &hitbox);
                moved |= 0b01;
                edge.start = hitbox.top;
                edge.end = hitbox.bottom-1;
                edge.coord = hitbox.right-1;
                if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
                    entities[entity_iterator].x -= ((hitbox.right - TILE_COORD(hitbox.right)) << 4);
                    entities[entity_iterator].vel_x = 0;
                }
            } else if(entities[entity_iterator].vel_x < 0) {
                populate_hitbox_record(&entities[entity_iterator], &hitbox);
                moved |= 0b01;
                edge.start = hitbox.top;
                edge.end = hitbox.bottom-1;
                edge.coord = hitbox.left;
                if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
                    entities[entity_iterator].x += ((TILE_COORD(hitbox.left + 8) - hitbox.left) << 4);
                    entities[entity_iterator].vel_x = 0;
                }
            }
            entities[entity_iterator].y += entities[entity_iterator].vel_y;
            if(entities[entity_iterator].vel_y < 0) {
                moved |= 0b10;
                populate_hitbox_record(&entities[entity_iterator], &hitbox);
                edge.start = hitbox.left;
                edge.end = hitbox.right-1;
                edge.coord = hitbox.top;
                if (get_horizontal_edge_tile_type(&edge) == TT_SOLID) {
                    entities[entity_iterator].y += ((TILE_COORD(hitbox.top + 8) - hitbox.top) << 4);
                    entities[entity_iterator].vel_y = 0;
                }
            } else if(entities[entity_iterator].vel_y > 0) {
                moved |= 0b10;
                populate_hitbox_record(&entities[entity_iterator], &hitbox);
                edge.start = hitbox.left;
                edge.end = hitbox.right-1;
                edge.coord = hitbox.bottom-1;
                if (get_horizontal_edge_tile_type(&edge) != TT_NONE) {
                    entities[entity_iterator].y -= ((hitbox.bottom - TILE_COORD(hitbox.bottom)) << 4);
                    entities[entity_iterator].vel_y = 0;
                    entities[entity_iterator].state |= GROUNDED;
                    entities[entity_iterator].state &= ~DOUBLE_JUMP;
                }
            }

            // Check grounding
            if(moved == 0b01) {
                populate_hitbox_record(&entities[entity_iterator], &hitbox);
                edge.start = hitbox.left;
                edge.end = hitbox.right-1;
                edge.coord = hitbox.bottom+7; // Check tile below entity
                if (get_horizontal_edge_tile_type(&edge) == TT_NONE) {
                    entities[entity_iterator].state &= ~GROUNDED;
                }
            }

            // Entity update?

            // Animation
            entities[entity_iterator].frame_counter++;
            if(entities[entity_iterator].frame_counter >= entities[entity_iterator].state_data->frame_duration) {
                entities[entity_iterator].frame_counter = 0;
                entities[entity_iterator].animation_frame++;
                if(entities[entity_iterator].animation_frame >= entities[entity_iterator].state_data->animation_length)
                    entities[entity_iterator].animation_frame = 0;
            }

            // Render
            sprite_index += move_metasprite_ex(
                    entities[entity_iterator].state_data->metasprite[entities[entity_iterator].animation_frame],
                    0,
                    entities[entity_iterator].prop,
                    sprite_index,
                    MAP_COORD(entities[entity_iterator].x) + DEVICE_SPRITE_PX_OFFSET_X,
                    MAP_COORD(entities[entity_iterator].y) - get_camera_y() + DEVICE_SPRITE_PX_OFFSET_Y);
        }
    }
}