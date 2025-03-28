#include "level.h"
#include "player.h"

#pragma bank 1

entity_t* truck;
uint8_t truck_vel_x = 32;
uint8_t loop_flag = 0;
void* prev_update_function = NULL;

const palette_color_t title_target_colors[2] = { RGBHTML(0xD43FD6),RGB_BLACK };
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

    // Title text fade in using unused color palette
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
        if(player->frame_counter % 8 == 0) {
            fade_to_color(&title_colors[2], &title_target_colors[0]);
            fade_to_color(&title_colors[3], &title_target_colors[1]);
            set_bkg_palette(7, 1, title_colors);
        }
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
    while(current_level == &level_0) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        set_focus(MAP_COORD(player->y));
        update_entities();
        vsync();
        update_camera();
    }

    CRITICAL {
        hUGE_init(&main_track);
        add_VBL(hUGE_dosound);
    }

    truck->active =  FALSE;
}

void standard_init() {
    DISPLAY_OFF;

    player->x = current_level->start_x << 4;
    player->y = current_level->start_y << 4;
    populate_hitbox_record(player);

    box->x = player->x;
    box->y = (player->hitbox.top - 3) << 4;
    populate_hitbox_record(box);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;
}

uint16_t level_pan_y = 0;
void level_long_init() {
    DISPLAY_OFF;

    set_bkg_data(DOOR_TILE_INDEX, 4, door_tiles);

    player->x = current_level->start_x << 4;
    player->y = current_level->start_y << 4;
    populate_hitbox_record(player);

    box->x = player->x;
    box->y = (player->hitbox.top - 3) << 4;
    populate_hitbox_record(box);

    init_camera(level_pan_y);

    hide_sprites_range(0,MAX_HARDWARE_SPRITES);

    DISPLAY_ON;

    while(level_pan_y < MAP_COORD(player->y)) {
        level_pan_y += 2;
        set_focus(level_pan_y);
        vsync();
        update_camera();
    }

    set_bkg_data(DOOR_TILE_INDEX, 4, empty_tiles);
    set_focus(MAP_COORD(player->y));

    remove_door = FALSE;
}

const level_t level_end = {
        level_end_HEIGHT >> 3,
        level_end_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_end_map,
        level_end_map_attributes,
        standard_init,
        NULL,
        &level_end,
        0, 0,
        8, 136,
        BANK(level_end)
};

const level_t level_elevator = {
        level_elevator_HEIGHT >> 3,
        level_elevator_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_elevator_map,
        level_elevator_map_attributes,
        standard_init,
        NULL,
        &level_end,
        0, 0,
        80, 88,
        BANK(level_end)
};

const level_t level_long = {
        level_long_HEIGHT >> 3,
        level_long_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_long_map,
        level_long_map_attributes,
        level_long_init,
        NULL,
        &level_elevator,
        128, 2024,
        16, 2024,
        BANK(level_long)
};

const level_t level_6 = {
        level_6_HEIGHT >> 3,
        level_6_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_6_map,
        level_6_map_attributes,
        standard_init,
        NULL,
        &level_long,
        48, 32,
        16, 176,
        BANK(level_6)
};

const level_t level_5 = {
        level_5_HEIGHT >> 3,
        level_5_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_5_map,
        level_5_map_attributes,
        standard_init,
        NULL,
        &level_6,
        144, 32,
        16, 176,
        BANK(level_5)
};

const level_t level_4 = {
        level_4_HEIGHT >> 3,
        level_4_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_4_map,
        level_4_map_attributes,
        standard_init,
        NULL,
        &level_5,
        112, 32,
        144, 168,
        BANK(level_4)
};

const level_t level_3 = {
        level_3_HEIGHT >> 3,
        level_3_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_3_map,
        level_3_map_attributes,
        standard_init,
        NULL,
        &level_4,
        24, 32,
        16, 168,
        BANK(level_3)
};

const level_t level_2 = {
        level_2_HEIGHT >> 3,
        level_2_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_2_map,
        level_2_map_attributes,
        standard_init,
        NULL,
        &level_3,
        72, 32,
        32, 168,
        BANK(level_2)
};

const level_t level_1 = {
    level_1_HEIGHT >> 3,
    level_1_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_1_map,
    level_1_map_attributes,
    standard_init,
    NULL,
    &level_2,
    72,32,
    8, 168,
    BANK(level_1)
};

const level_t level_0 = {
    level_0_HEIGHT >> 3,
    level_0_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_0_map,
    level_0_map_attributes,
    level_0_init,
    level_0_update,
    &level_1,
    144, 120,
    32, 110,
    BANK(level_0)
};

const uint8_t empty_tiles[] = {
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,

        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,

        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,

        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
};
const uint8_t door_tiles[] = {
        0x7f,0x7f,0xed,0x92,
        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,

        0xff,0xff,0xff,0x80,
        0xf7,0x88,0xff,0xff,
        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,

        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,
        0xed,0x92,0xed,0x92,

        0xbf,0x59,0xbf,0x59,
        0xb7,0x49,0xb7,0x49,
        0xb7,0x49,0xb7,0x49,
        0xb7,0x49,0xb7,0x49,
};

const uint16_t width_multiplication_table[] = {
        0 * MAP_WIDTH,
        1 * MAP_WIDTH,
        2 * MAP_WIDTH,
        3 * MAP_WIDTH,
        4 * MAP_WIDTH,
        5 * MAP_WIDTH,
        6 * MAP_WIDTH,
        7 * MAP_WIDTH,
        8 * MAP_WIDTH,
        9 * MAP_WIDTH,
        10 * MAP_WIDTH,
        11 * MAP_WIDTH,
        12 * MAP_WIDTH,
        13 * MAP_WIDTH,
        14 * MAP_WIDTH,
        15 * MAP_WIDTH,
        16 * MAP_WIDTH,
        17 * MAP_WIDTH,
        18 * MAP_WIDTH,
        19 * MAP_WIDTH,
        20 * MAP_WIDTH,
        21 * MAP_WIDTH,
        22 * MAP_WIDTH,
        23 * MAP_WIDTH,
        24 * MAP_WIDTH,
        25 * MAP_WIDTH,
        26 * MAP_WIDTH,
        27 * MAP_WIDTH,
        28 * MAP_WIDTH,
        29 * MAP_WIDTH,
        30 * MAP_WIDTH,
        31 * MAP_WIDTH,
        32 * MAP_WIDTH,
        33 * MAP_WIDTH,
        34 * MAP_WIDTH,
        35 * MAP_WIDTH,
        36 * MAP_WIDTH,
        37 * MAP_WIDTH,
        38 * MAP_WIDTH,
        39 * MAP_WIDTH,
        40 * MAP_WIDTH,
        41 * MAP_WIDTH,
        42 * MAP_WIDTH,
        43 * MAP_WIDTH,
        44 * MAP_WIDTH,
        45 * MAP_WIDTH,
        46 * MAP_WIDTH,
        47 * MAP_WIDTH,
        48 * MAP_WIDTH,
        49 * MAP_WIDTH,
        50 * MAP_WIDTH,
        51 * MAP_WIDTH,
        52 * MAP_WIDTH,
        53 * MAP_WIDTH,
        54 * MAP_WIDTH,
        55 * MAP_WIDTH,
        56 * MAP_WIDTH,
        57 * MAP_WIDTH,
        58 * MAP_WIDTH,
        59 * MAP_WIDTH,
        60 * MAP_WIDTH,
        61 * MAP_WIDTH,
        62 * MAP_WIDTH,
        63 * MAP_WIDTH,
        64 * MAP_WIDTH,
        65 * MAP_WIDTH,
        66 * MAP_WIDTH,
        67 * MAP_WIDTH,
        68 * MAP_WIDTH,
        69 * MAP_WIDTH,
        70 * MAP_WIDTH,
        71 * MAP_WIDTH,
        72 * MAP_WIDTH,
        73 * MAP_WIDTH,
        74 * MAP_WIDTH,
        75 * MAP_WIDTH,
        76 * MAP_WIDTH,
        77 * MAP_WIDTH,
        78 * MAP_WIDTH,
        79 * MAP_WIDTH,
        80 * MAP_WIDTH,
        81 * MAP_WIDTH,
        82 * MAP_WIDTH,
        83 * MAP_WIDTH,
        84 * MAP_WIDTH,
        85 * MAP_WIDTH,
        86 * MAP_WIDTH,
        87 * MAP_WIDTH,
        88 * MAP_WIDTH,
        89 * MAP_WIDTH,
        90 * MAP_WIDTH,
        91 * MAP_WIDTH,
        92 * MAP_WIDTH,
        93 * MAP_WIDTH,
        94 * MAP_WIDTH,
        95 * MAP_WIDTH,
        96 * MAP_WIDTH,
        97 * MAP_WIDTH,
        98 * MAP_WIDTH,
        99 * MAP_WIDTH,
        100 * MAP_WIDTH,
        101 * MAP_WIDTH,
        102 * MAP_WIDTH,
        103 * MAP_WIDTH,
        104 * MAP_WIDTH,
        105 * MAP_WIDTH,
        106 * MAP_WIDTH,
        107 * MAP_WIDTH,
        108 * MAP_WIDTH,
        109 * MAP_WIDTH,
        110 * MAP_WIDTH,
        111 * MAP_WIDTH,
        112 * MAP_WIDTH,
        113 * MAP_WIDTH,
        114 * MAP_WIDTH,
        115 * MAP_WIDTH,
        116 * MAP_WIDTH,
        117 * MAP_WIDTH,
        118 * MAP_WIDTH,
        119 * MAP_WIDTH,
        120 * MAP_WIDTH,
        121 * MAP_WIDTH,
        122 * MAP_WIDTH,
        123 * MAP_WIDTH,
        124 * MAP_WIDTH,
        125 * MAP_WIDTH,
        126 * MAP_WIDTH,
        127 * MAP_WIDTH,
        128 * MAP_WIDTH,
        129 * MAP_WIDTH,
        130 * MAP_WIDTH,
        131 * MAP_WIDTH,
        132 * MAP_WIDTH,
        133 * MAP_WIDTH,
        134 * MAP_WIDTH,
        135 * MAP_WIDTH,
        136 * MAP_WIDTH,
        137 * MAP_WIDTH,
        138 * MAP_WIDTH,
        139 * MAP_WIDTH,
        140 * MAP_WIDTH,
        141 * MAP_WIDTH,
        142 * MAP_WIDTH,
        143 * MAP_WIDTH,
        144 * MAP_WIDTH,
        145 * MAP_WIDTH,
        146 * MAP_WIDTH,
        147 * MAP_WIDTH,
        148 * MAP_WIDTH,
        149 * MAP_WIDTH,
        150 * MAP_WIDTH,
        151 * MAP_WIDTH,
        152 * MAP_WIDTH,
        153 * MAP_WIDTH,
        154 * MAP_WIDTH,
        155 * MAP_WIDTH,
        156 * MAP_WIDTH,
        157 * MAP_WIDTH,
        158 * MAP_WIDTH,
        159 * MAP_WIDTH,
        160 * MAP_WIDTH,
        161 * MAP_WIDTH,
        162 * MAP_WIDTH,
        163 * MAP_WIDTH,
        164 * MAP_WIDTH,
        165 * MAP_WIDTH,
        166 * MAP_WIDTH,
        167 * MAP_WIDTH,
        168 * MAP_WIDTH,
        169 * MAP_WIDTH,
        170 * MAP_WIDTH,
        171 * MAP_WIDTH,
        172 * MAP_WIDTH,
        173 * MAP_WIDTH,
        174 * MAP_WIDTH,
        175 * MAP_WIDTH,
        176 * MAP_WIDTH,
        177 * MAP_WIDTH,
        178 * MAP_WIDTH,
        179 * MAP_WIDTH,
        180 * MAP_WIDTH,
        181 * MAP_WIDTH,
        182 * MAP_WIDTH,
        183 * MAP_WIDTH,
        184 * MAP_WIDTH,
        185 * MAP_WIDTH,
        186 * MAP_WIDTH,
        187 * MAP_WIDTH,
        188 * MAP_WIDTH,
        189 * MAP_WIDTH,
        190 * MAP_WIDTH,
        191 * MAP_WIDTH,
        192 * MAP_WIDTH,
        193 * MAP_WIDTH,
        194 * MAP_WIDTH,
        195 * MAP_WIDTH,
        196 * MAP_WIDTH,
        197 * MAP_WIDTH,
        198 * MAP_WIDTH,
        199 * MAP_WIDTH,
        200 * MAP_WIDTH,
        201 * MAP_WIDTH,
        202 * MAP_WIDTH,
        203 * MAP_WIDTH,
        204 * MAP_WIDTH,
        205 * MAP_WIDTH,
        206 * MAP_WIDTH,
        207 * MAP_WIDTH,
        208 * MAP_WIDTH,
        209 * MAP_WIDTH,
        210 * MAP_WIDTH,
        211 * MAP_WIDTH,
        212 * MAP_WIDTH,
        213 * MAP_WIDTH,
        214 * MAP_WIDTH,
        215 * MAP_WIDTH,
        216 * MAP_WIDTH,
        217 * MAP_WIDTH,
        218 * MAP_WIDTH,
        219 * MAP_WIDTH,
        220 * MAP_WIDTH,
        221 * MAP_WIDTH,
        222 * MAP_WIDTH,
        223 * MAP_WIDTH,
        224 * MAP_WIDTH,
        225 * MAP_WIDTH,
        226 * MAP_WIDTH,
        227 * MAP_WIDTH,
        228 * MAP_WIDTH,
        229 * MAP_WIDTH,
        230 * MAP_WIDTH,
        231 * MAP_WIDTH,
        232 * MAP_WIDTH,
        233 * MAP_WIDTH,
        234 * MAP_WIDTH,
        235 * MAP_WIDTH,
        236 * MAP_WIDTH,
        237 * MAP_WIDTH,
        238 * MAP_WIDTH,
        239 * MAP_WIDTH,
        240 * MAP_WIDTH,
        241 * MAP_WIDTH,
        242 * MAP_WIDTH,
        243 * MAP_WIDTH,
        244 * MAP_WIDTH,
        245 * MAP_WIDTH,
        246 * MAP_WIDTH,
        247 * MAP_WIDTH,
        248 * MAP_WIDTH,
        249 * MAP_WIDTH,
        250 * MAP_WIDTH,
        251 * MAP_WIDTH,
        252 * MAP_WIDTH,
        253 * MAP_WIDTH,
        254 * MAP_WIDTH,
        255 * MAP_WIDTH,
        256 * MAP_WIDTH
};