#include "entity.h"

#pragma bank 1

entity_t entity_to_add = {};
uint8_t sprite_index = 0;

entity_t entities[MAX_ENTITIES];
uint8_t next_entity_index = 0;
uint8_t entity_count = 0;

uint16_t height_base_index = 0;
uint8_t tile = 0;
uint8_t* tilemap;
// Functions which use this need to be NONBANKED
inline uint8_t get_tile_type(const uint16_t x, const uint16_t y) {
    height_base_index = width_multiplication_table[y >> 3];
    tilemap = current_level->map;
    SWITCH_ROM(2);
    tile = tilemap[(x >> 3) + height_base_index];
    SWITCH_ROM(1);
    if (tile >= 112) {
        return TT_SOLID;
    } else if (tile >= 96) {
        return TT_PLATFORM;
    } else if (tile >= 80) {
        return TT_HAZARD;
    }
    return TT_NONE;
}

uint16_t edge_iterator = 0;
uint8_t tile_type = TT_NONE;
uint8_t max_tile_type = TT_NONE;
uint8_t get_vertical_edge_tile_type(edge_t* edge) NONBANKED{
    max_tile_type = TT_NONE;
    if(edge->coord > MAX_X_COORD)
        return TT_SOLID;
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
uint8_t get_horizontal_edge_tile_type(edge_t* edge) NONBANKED{
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

void populate_hitbox_record(entity_t* const e) {
    e->hitbox.top = MAP_COORD(e->y) - HEIGHT_MARGIN(e->state_data->hitbox_margin);
    e->hitbox.left = MAP_COORD(e->x) - WIDTH_MARGIN(e->state_data->hitbox_margin);
    e->hitbox.right = MAP_COORD(e->x) + WIDTH_MARGIN(e->state_data->hitbox_margin);
    e->hitbox.bottom = MAP_COORD(e->y) + HEIGHT_MARGIN(e->state_data->hitbox_margin);
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
void update_entities() {
    sprite_index = 0;
    for(entity_iterator = MAX_ENTITIES - 1; entity_iterator >= 0; entity_iterator--) {
        if(entities[entity_iterator].active) {
            // Onscreen check
            entities[entity_iterator].onscreen = MAP_COORD(entities[entity_iterator].y) > camera_y
                    && MAP_COORD(entities[entity_iterator].y) < (camera_y + DEVICE_SCREEN_PX_HEIGHT);

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
            // Entity specific update
            if(entities[entity_iterator].update_function)
                entities[entity_iterator].update_function(&entities[entity_iterator]);
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
uint16_t adjustment = 0;
edge_t edge = {};
uint16_t previous_map_coord;
uint8_t velocity_collision_move(entity_t* e) {
    moved = 0b00;
    if (e->vel_x > 0) {
        adjustment = e->x >> 4;
        e->x += e->vel_x;
        adjustment = (e->x >> 4) - adjustment;
        if(adjustment != 0) {
            e->hitbox.right += adjustment;
            e->hitbox.left += adjustment;
            moved |= 0b01;
            edge.start = e->hitbox.top;
            edge.end = e->hitbox.bottom - 1;
            edge.coord = e->hitbox.right - 1;
            if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
                adjustment = e->hitbox.right - TILE_COORD(e->hitbox.right);
                e->x -= adjustment << 4;
                e->hitbox.right -= adjustment;
                e->hitbox.left -= adjustment;
                e->vel_x = 0;
            }
        }
    } else if (e->vel_x < 0) {
        adjustment = e->x >> 4;
        e->x += e->vel_x;
        adjustment = adjustment - (e->x >> 4);
        if(adjustment != 0) {
            e->hitbox.right -= adjustment;
            e->hitbox.left -= adjustment;
            moved |= 0b01;
            edge.start = e->hitbox.top;
            edge.end = e->hitbox.bottom - 1;
            edge.coord = e->hitbox.left;
            if (get_vertical_edge_tile_type(&edge) == TT_SOLID) {
                adjustment = TILE_COORD(e->hitbox.left + 8) - e->hitbox.left;
                e->x += adjustment << 4;
                e->hitbox.right += adjustment;
                e->hitbox.left += adjustment;
                e->vel_x = 0;
            }
        }
    }
    if (e->vel_y < 0) {
        adjustment = e->y >> 4;
        e->y += e->vel_y;
        adjustment = adjustment - (e->y >> 4);
        if(adjustment != 0) {
            e->hitbox.top -= adjustment;
            e->hitbox.bottom -= adjustment;
            moved |= 0b10;
            edge.start = e->hitbox.left;
            edge.end = e->hitbox.right - 1;
            edge.coord = e->hitbox.top;
            if (get_horizontal_edge_tile_type(&edge) == TT_SOLID) {
                adjustment = TILE_COORD(e->hitbox.top + 8) - e->hitbox.top;
                e->y += adjustment << 4;
                e->hitbox.top += adjustment;
                e->hitbox.bottom += adjustment;
                e->vel_y = 0;
            }
        }
    } else if (e->vel_y > 0) {
        adjustment = e->y >> 4;
        e->y += e->vel_y;
        adjustment = (e->y >> 4) - adjustment;
        if(adjustment != 0) {
            previous_map_coord = (e->hitbox.bottom - 1) >> 3;
            e->hitbox.top += adjustment;
            e->hitbox.bottom += adjustment;
            moved |= 0b10;
            if(previous_map_coord != ((e->hitbox.bottom - 1) >> 3)) { // Prevent platform snap-ups
                edge.start = e->hitbox.left;
                edge.end = e->hitbox.right - 1;
                edge.coord = e->hitbox.bottom - 1;
                if (get_horizontal_edge_tile_type(&edge) != TT_NONE) {
                    adjustment = e->hitbox.bottom - TILE_COORD(e->hitbox.bottom);
                    e->y -= adjustment << 4;
                    e->hitbox.top -= adjustment;
                    e->hitbox.bottom -= adjustment;
                    e->vel_y = 0;
                    e->state |= GROUNDED;
                    e->state &= ~DOUBLE_JUMP;
                }
            }
        }
    }
    return moved == 0b01;
}

void check_grounding(entity_t* e) {
    edge.start = e->hitbox.left;
    edge.end = e->hitbox.right - 1;
    edge.coord = e->hitbox.bottom + 7; // Check tile below entity
    if (get_horizontal_edge_tile_type(&edge) == TT_NONE) {
        e->state &= ~GROUNDED;
    }
}

uint8_t do_hitboxes_overlap(const hitbox_record_t* const h1, const hitbox_record_t* const h2) {
    return (h1->right >= h2->left && h2->right >= h1->left)
        && (h1->bottom >= h2->top && h2->bottom >= h1->top);
}