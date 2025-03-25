#include "level.h"

entity_t* truck;
uint8_t truck_vel_x = 32;
uint8_t loop_flag = 0;
void* prev_update_function = NULL;

uint8_t title_color_counter = 0;
const palette_color_t text_start_color = RGB8(0xFA,0xE6,0xCD); // Matches the background color
palette_color_t title_colors[] = {RGB8(0xFA,0xE6,0xCD),RGB8(0,0,0),RGB8(0xFA,0xE6,0xCD),RGB8(0xFA,0xE6,0xCD)};
const uint8_t title_attr_map[] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

const state_data_t truck_base = {
        ((truck_sheet_WIDTH >> 3) << 4) | (truck_sheet_HEIGHT >> 3),
        (truck_sheet_PIVOT_W << 4) | (truck_sheet_PIVOT_H),
        0,
        0,
        &truck_sheet_metasprites[0],
        1
};

const state_data_t player_sigh = {
    ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
    (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
    0,
    150,
    &player_sheet_metasprites[17],
    3
};

const state_data_t player_get_up = {
    ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
    (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
    0,
    0,
    &player_sheet_metasprites[16],
    1
};

void level_0_init() {
    DISPLAY_OFF;

    player->active = FALSE;
    box->active = FALSE;

    set_sprite_data(player_sheet_TILE_COUNT + box_sheet_TILE_COUNT, truck_sheet_TILE_COUNT, truck_sheet_tiles);
    set_sprite_palette(2, 1, truck_sheet_palettes);
    entity_to_add.base_tile = player_sheet_TILE_COUNT + box_sheet_TILE_COUNT;
    entity_to_add.prop = 2;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &truck_base;
    truck = add_entity();

    truck->x = -256;   // Start wrapped offscreen
    truck->y = 113<<4;

    init_camera(19);

    set_bkg_palette(7,1, title_colors);
    set_bkg_attributes(1, 4, 5, 1, title_attr_map);
    set_bkg_attributes(1, 5, 11, 1, title_attr_map);

    DISPLAY_ON;
}
void level_0_update() {
    // move in truck
    while(MAP_COORD(truck->x) != 16) {
        truck->x += truck_vel_x;
        truck_vel_x--;
        truck->y += 16;
        update_entities();
        vsync();
        truck->y -= 16;
        update_entities();
        vsync();
        vsync();
    }
    loop_flag = 0;
    while(loop_flag < 20) {
        truck->y += 16;
        update_entities();
        vsync();
        vsync();
        truck->y -= 16;
        update_entities();
        vsync();
        vsync();
        loop_flag++;
    }
    loop_flag = 0;
    while(loop_flag < 120) {
        vsync();
        loop_flag++;
    }

    // bnuuy enters scene
    player->active = TRUE;
    player->x = 32 << 4;
    player->y = 110 << 4;
    populate_hitbox_record(player);
    box->active = TRUE;
    box->x = 32 << 4;
    box->y = 110 << 4;
    populate_hitbox_record(box);
    joypads.joy0 = J_B | J_A | J_RIGHT;
    update_entities();
    vsync();
    joypads.joy0 = J_B | J_RIGHT;
    loop_flag = 0;
    while(loop_flag < 26) {
        update_entities();
        vsync();
        loop_flag++;
    }
    joypads.joy0 = J_RIGHT;
    loop_flag = 0;
    update_entities();
    vsync();
    loop_flag++;

    // have a sit
    prev_update_function = player->update_function;
    player->update_function = NULL;
    player->state_data = &player_sigh;
    player->animation_frame = 0;
    player->frame_counter = 0;
    while(player->frame_counter != 149 || player->animation_frame != 2) {
        if(player->frame_counter % 32 == 0) {
            title_color_counter++;
            title_colors[2] = text_start_color - title_color_counter;
            title_colors[3] = text_start_color - title_color_counter * 2;
        }
        set_bkg_palette(7, 1, title_colors);
        update_entities();
        vsync();
        loop_flag++;
    }
    player->state_data = &player_get_up;
    player->animation_frame = 0;
    player->frame_counter = 0;
    loop_flag = 0;
    while(loop_flag < 60) {
        update_entities();
        vsync();
        loop_flag++;
    }

    // give player control
    player->update_function = prev_update_function;
    level_0.update_function = NULL;
}

void level_1_init() {
    DISPLAY_OFF;

    CRITICAL {
        hUGE_init(&main_track);
        add_VBL(hUGE_dosound);
    }

    player->x = 20<<4;
    player->y = 300<<4;
    populate_hitbox_record(player);

    box->x = 40<<4;
    box->y = 300<<4;
    populate_hitbox_record(box);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;
}

level_t level_1 = {
    tileset_map_height,
    (tileset_map_height - DEVICE_SCREEN_HEIGHT) << 3,
    tileset_map,
    tileset_map_attr,
    level_1_init,
    NULL,
    NULL,
    0,
    0
};

level_t level_0 = {
    level_0_HEIGHT >> 3,
    level_0_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_0_map,
    level_0_map_attributes,
    level_0_init,
    level_0_update,
    &level_1,
    144,
    120
};
