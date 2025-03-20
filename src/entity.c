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
void update_entities() {
    sprite_index = 0;
    for(entity_iterator = MAX_ENTITIES - 1; entity_iterator >= 0; entity_iterator--) {
        if(entities[entity_iterator].active) {
            // Onscreen check
            entities[entity_iterator].onscreen = MAP_COORD(entities[entity_iterator].y) > (camera_y - 8)
                    && MAP_COORD(entities[entity_iterator].y) < (camera_y + DEVICE_SCREEN_PX_HEIGHT + 8);

            // Entity specific update
            if(entities[entity_iterator].update_function)
                entities[entity_iterator].update_function(&entities[entity_iterator]);

            if(entities[entity_iterator].onscreen) {
                // Animation
                if(entities[entity_iterator].state_data->animation_length > 1) {
                    entities[entity_iterator].frame_counter++;
                    if(entities[entity_iterator].frame_counter >= entities[entity_iterator].state_data->frame_duration) {
                        entities[entity_iterator].frame_counter = 0;
                        entities[entity_iterator].animation_frame++;
                        if(entities[entity_iterator].animation_frame
                                >= entities[entity_iterator].state_data->animation_length)
                            entities[entity_iterator].animation_frame = 0;
                    }
                }

                // Render
                if(entities[entity_iterator].state & FLIP_X) {
                    sprite_index += move_metasprite_flipx(
                            entities[entity_iterator].state_data->metasprite[entities[entity_iterator].animation_frame],
                            entities[entity_iterator].base_tile,
                            entities[entity_iterator].prop,
                            sprite_index,
                            MAP_COORD(entities[entity_iterator].x) + DEVICE_SPRITE_PX_OFFSET_X,
                            MAP_COORD(entities[entity_iterator].y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);
                } else {
                    sprite_index += move_metasprite_ex(
                            entities[entity_iterator].state_data->metasprite[entities[entity_iterator].animation_frame],
                            entities[entity_iterator].base_tile,
                            entities[entity_iterator].prop,
                            sprite_index,
                            MAP_COORD(entities[entity_iterator].x) + DEVICE_SPRITE_PX_OFFSET_X,
                            MAP_COORD(entities[entity_iterator].y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);
                }
            }
        }
    }
    hide_sprites_range(sprite_index,MAX_HARDWARE_SPRITES);
}

void velocity_direction_flip(entity_t* e) {
    if (e->vel_x > 0) {
        e->state &= ~FLIP_X;
    } else if (e->vel_x < 0) {
        e->state |= FLIP_X;
    }
}

uint8_t moved = 0; // bitwise 6[unused]1[vertical]1[horizontal]
hitbox_record_t hitbox = {};
edge_t edge = {};
uint8_t velocity_collision_move(entity_t* e) {
    moved = 0b00;
    e->x += e->vel_x;
    if (e->vel_x > 0) {
        populate_hitbox_record(e, &hitbox);
        moved |= 0b01;
        edge.start = hitbox.top;
        edge.end = hitbox.bottom - 1;
        edge.coord = hitbox.right - 1;
        if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
            e->x -= ((hitbox.right - TILE_COORD(hitbox.right)) << 4);
            e->vel_x = 0;
        }
    } else if (e->vel_x < 0) {
        populate_hitbox_record(e, &hitbox);
        moved |= 0b01;
        edge.start = hitbox.top;
        edge.end = hitbox.bottom - 1;
        edge.coord = hitbox.left;
        if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
            e->x += ((TILE_COORD(hitbox.left + 8) - hitbox.left) << 4);
            e->vel_x = 0;
        }
    }
    e->y += e->vel_y;
    if (e->vel_y < 0) {
        moved |= 0b10;
        populate_hitbox_record(e, &hitbox);
        edge.start = hitbox.left;
        edge.end = hitbox.right - 1;
        edge.coord = hitbox.top;
        if (get_horizontal_edge_tile_type(&edge) == TT_SOLID) {
            e->y += ((TILE_COORD(hitbox.top + 8) - hitbox.top) << 4);
            e->vel_y = 0;
        }
    } else if (e->vel_y > 0) {
        moved |= 0b10;
        populate_hitbox_record(e, &hitbox);
        edge.start = hitbox.left;
        edge.end = hitbox.right - 1;
        edge.coord = hitbox.bottom - 1;
        if (get_horizontal_edge_tile_type(&edge) != TT_NONE) {
            e->y -= ((hitbox.bottom - TILE_COORD(hitbox.bottom)) << 4);
            e->vel_y = 0;
            e->state |= GROUNDED;
            e->state &= ~DOUBLE_JUMP;
        }
    }
    return moved == 0b01;
}

void check_grounding(entity_t* e) {
    populate_hitbox_record(e, &hitbox);
    edge.start = hitbox.left;
    edge.end = hitbox.right - 1;
    edge.coord = hitbox.bottom + 7; // Check tile below entity
    if (get_horizontal_edge_tile_type(&edge) == TT_NONE) {
        e->state &= ~GROUNDED;
    }
}