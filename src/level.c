#include "level.h"
#include "entity.h"
#include "color.h"
#include "level_last.h"
#include "cbtfx.h"
#include "../res/sfx/SFX_0F.h"
#include "../obj/level_1.h"
#include "../obj/level_2.h"
#include "../obj/level_3.h"
#include "../obj/level_4.h"
#include "../obj/level_5.h"
#include "../obj/player_sheet.h"
#include "../obj/box_sheet.h"

#pragma bank 1

uint8_t remove_door = TRUE; // flag to remove doors on level transition
uint8_t play_door_close_sfx = TRUE;

uint16_t player_fade_colors[4];
uint16_t box_fade_colors[4];
uint16_t door_x, door_y;
void apply_level_transition(const level_t* level) {
    // remove door by replacing its tiles with blanks
    if(remove_door) {
        CBTFX_PLAY_SFX_0F;
        set_bkg_data(DOOR_TILE_INDEX, 4, empty_tiles);
    }

    // set player to turn around sprite, hide box
    hide_sprites_range(
            move_metasprite_ex(
                    player_sheet_metasprites[21],
                    player->base_tile,
                    player->prop,
                    0,
                    door_x + DEVICE_SPRITE_PX_OFFSET_X,
                    door_y - camera_y + DEVICE_SPRITE_PX_OFFSET_Y),
            MAX_HARDWARE_SPRITES);

    // fade out player
    player_fade_colors[0] = RGB8(255,255,255); // set initial colors to match those in player sheet
    player_fade_colors[1] = RGB8(255,228,194);
    player_fade_colors[2] = RGB8(154,114,72);
    player_fade_colors[3] = RGB8(0,0,0);
    while(player_fade_colors[1] != black) {
        DECREMENT_COLOR(&player_fade_colors[1]);
        DECREMENT_COLOR(&player_fade_colors[2]);
        set_sprite_palette(0, 1, player_fade_colors);
        vsync();
    }

    // change level
    current_level = level;
    current_level->init_function();

    sprite_index = 0;

    // show box blacked out
    box_fade_colors[0] = black;
    box_fade_colors[1] = black;
    box_fade_colors[2] = black;
    box_fade_colors[3] = black;
    set_sprite_palette(1, 1, box_fade_colors);
    sprite_index += move_metasprite_ex(
            box_sheet_metasprites[0],
            box->base_tile,
            box->prop,
            sprite_index,
            MAP_COORD(box->x) + DEVICE_SPRITE_PX_OFFSET_X,
            MAP_COORD(box->y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);

    // change player to carry animation
    sprite_index += move_metasprite_ex(
            player_sheet_metasprites[3],
            player->base_tile,
            player->prop,
            sprite_index,
            MAP_COORD(player->x) + DEVICE_SPRITE_PX_OFFSET_X,
            MAP_COORD(player->y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);

    hide_sprites_range(sprite_index, MAX_HARDWARE_SPRITES);

    // fade in player and box
    while(player_fade_colors[1] != player_sheet_palettes[1]) {
        fade_to_color(&player_fade_colors[1], &player_sheet_palettes[1]);
        fade_to_color(&player_fade_colors[2], &player_sheet_palettes[2]);
        fade_to_color(&box_fade_colors[1], &box_sheet_palettes[1]);
        fade_to_color(&box_fade_colors[2], &box_sheet_palettes[2]);
        set_sprite_palette(0, 1, player_fade_colors);
        set_sprite_palette(1, 1, box_fade_colors);
        vsync();
    }

    // re-place door tiles
    set_bkg_data(DOOR_TILE_INDEX, 4, door_tiles);
    if(play_door_close_sfx)
        CBTFX_PLAY_SFX_0F;
}

void standard_init(void) {
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

const level_t level_5 = {
        level_5_HEIGHT >> 3,
        level_5_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_5_map,
        level_5_map_attributes,
        standard_init,
        NULL,
        &level_long,
        144, 40,
        16, 184,
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
        104, 40,
        144, 176,
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
        24, 40,
        16, 176,
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
        72, 40,
        32, 176,
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
    72,40,
    8, 176,
    BANK(level_1)
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