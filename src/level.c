#include "level.h"

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
    level_0.update_function = NULL;
}

void level_1_init() {
    DISPLAY_OFF;

    CRITICAL {
        hUGE_init(&main_track);
        add_VBL(hUGE_dosound);
    }

    truck->active =  FALSE;

    player->x = 8<<4;
    player->y = 168<<4;
    populate_hitbox_record(player);

    box->x = player->x;
    box->y = (player->hitbox.top - 3) << 4;
    populate_hitbox_record(box);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;
}

level_t level_1 = {
    level_1_HEIGHT >> 3,
    level_1_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
    level_1_map,
    level_1_map_attributes,
    level_1_init,
    NULL,
    &level_1,
    72,
    32
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
        256 * MAP_WIDTH,
        257 * MAP_WIDTH,
        258 * MAP_WIDTH,
        259 * MAP_WIDTH,
        260 * MAP_WIDTH,
        261 * MAP_WIDTH,
        262 * MAP_WIDTH,
        263 * MAP_WIDTH,
        264 * MAP_WIDTH,
        265 * MAP_WIDTH,
        266 * MAP_WIDTH,
        267 * MAP_WIDTH,
        268 * MAP_WIDTH,
        269 * MAP_WIDTH,
        270 * MAP_WIDTH,
        271 * MAP_WIDTH,
        272 * MAP_WIDTH,
        273 * MAP_WIDTH,
        274 * MAP_WIDTH,
        275 * MAP_WIDTH,
        276 * MAP_WIDTH,
        277 * MAP_WIDTH,
        278 * MAP_WIDTH,
        279 * MAP_WIDTH,
        280 * MAP_WIDTH,
        281 * MAP_WIDTH,
        282 * MAP_WIDTH,
        283 * MAP_WIDTH,
        284 * MAP_WIDTH,
        285 * MAP_WIDTH,
        286 * MAP_WIDTH,
        287 * MAP_WIDTH,
        288 * MAP_WIDTH,
        289 * MAP_WIDTH,
        290 * MAP_WIDTH,
        291 * MAP_WIDTH,
        292 * MAP_WIDTH,
        293 * MAP_WIDTH,
        294 * MAP_WIDTH,
        295 * MAP_WIDTH,
        296 * MAP_WIDTH,
        297 * MAP_WIDTH,
        298 * MAP_WIDTH,
        299 * MAP_WIDTH,
        300 * MAP_WIDTH,
        301 * MAP_WIDTH,
        302 * MAP_WIDTH,
        303 * MAP_WIDTH,
        304 * MAP_WIDTH,
        305 * MAP_WIDTH,
        306 * MAP_WIDTH,
        307 * MAP_WIDTH,
        308 * MAP_WIDTH,
        309 * MAP_WIDTH,
        310 * MAP_WIDTH,
        311 * MAP_WIDTH,
        312 * MAP_WIDTH,
        313 * MAP_WIDTH,
        314 * MAP_WIDTH,
        315 * MAP_WIDTH,
        316 * MAP_WIDTH,
        317 * MAP_WIDTH,
        318 * MAP_WIDTH,
        319 * MAP_WIDTH,
        320 * MAP_WIDTH,
        321 * MAP_WIDTH,
        322 * MAP_WIDTH,
        323 * MAP_WIDTH,
        324 * MAP_WIDTH,
        325 * MAP_WIDTH,
        326 * MAP_WIDTH,
        327 * MAP_WIDTH,
        328 * MAP_WIDTH,
        329 * MAP_WIDTH,
        330 * MAP_WIDTH,
        331 * MAP_WIDTH,
        332 * MAP_WIDTH,
        333 * MAP_WIDTH,
        334 * MAP_WIDTH,
        335 * MAP_WIDTH,
        336 * MAP_WIDTH,
        337 * MAP_WIDTH,
        338 * MAP_WIDTH,
        339 * MAP_WIDTH,
        340 * MAP_WIDTH,
        341 * MAP_WIDTH,
        342 * MAP_WIDTH,
        343 * MAP_WIDTH,
        344 * MAP_WIDTH,
        345 * MAP_WIDTH,
        346 * MAP_WIDTH,
        347 * MAP_WIDTH,
        348 * MAP_WIDTH,
        349 * MAP_WIDTH,
        350 * MAP_WIDTH,
        351 * MAP_WIDTH,
        352 * MAP_WIDTH,
        353 * MAP_WIDTH,
        354 * MAP_WIDTH,
        355 * MAP_WIDTH,
        356 * MAP_WIDTH,
        357 * MAP_WIDTH,
        358 * MAP_WIDTH,
        359 * MAP_WIDTH,
        360 * MAP_WIDTH,
        361 * MAP_WIDTH,
        362 * MAP_WIDTH,
        363 * MAP_WIDTH,
        364 * MAP_WIDTH,
        365 * MAP_WIDTH,
        366 * MAP_WIDTH,
        367 * MAP_WIDTH,
        368 * MAP_WIDTH,
        369 * MAP_WIDTH,
        370 * MAP_WIDTH,
        371 * MAP_WIDTH,
        372 * MAP_WIDTH,
        373 * MAP_WIDTH,
        374 * MAP_WIDTH,
        375 * MAP_WIDTH,
        376 * MAP_WIDTH,
        377 * MAP_WIDTH,
        378 * MAP_WIDTH,
        379 * MAP_WIDTH,
        380 * MAP_WIDTH,
        381 * MAP_WIDTH,
        382 * MAP_WIDTH,
        383 * MAP_WIDTH,
        384 * MAP_WIDTH,
        385 * MAP_WIDTH,
        386 * MAP_WIDTH,
        387 * MAP_WIDTH,
        388 * MAP_WIDTH,
        389 * MAP_WIDTH,
        390 * MAP_WIDTH,
        391 * MAP_WIDTH,
        392 * MAP_WIDTH,
        393 * MAP_WIDTH,
        394 * MAP_WIDTH,
        395 * MAP_WIDTH,
        396 * MAP_WIDTH,
        397 * MAP_WIDTH,
        398 * MAP_WIDTH,
        399 * MAP_WIDTH,
        400 * MAP_WIDTH,
        401 * MAP_WIDTH,
        402 * MAP_WIDTH,
        403 * MAP_WIDTH,
        404 * MAP_WIDTH,
        405 * MAP_WIDTH,
        406 * MAP_WIDTH,
        407 * MAP_WIDTH,
        408 * MAP_WIDTH,
        409 * MAP_WIDTH,
        410 * MAP_WIDTH,
        411 * MAP_WIDTH,
        412 * MAP_WIDTH,
        413 * MAP_WIDTH,
        414 * MAP_WIDTH,
        415 * MAP_WIDTH,
        416 * MAP_WIDTH,
        417 * MAP_WIDTH,
        418 * MAP_WIDTH,
        419 * MAP_WIDTH,
        420 * MAP_WIDTH,
        421 * MAP_WIDTH,
        422 * MAP_WIDTH,
        423 * MAP_WIDTH,
        424 * MAP_WIDTH,
        425 * MAP_WIDTH,
        426 * MAP_WIDTH,
        427 * MAP_WIDTH,
        428 * MAP_WIDTH,
        429 * MAP_WIDTH,
        430 * MAP_WIDTH,
        431 * MAP_WIDTH,
        432 * MAP_WIDTH,
        433 * MAP_WIDTH,
        434 * MAP_WIDTH,
        435 * MAP_WIDTH,
        436 * MAP_WIDTH,
        437 * MAP_WIDTH,
        438 * MAP_WIDTH,
        439 * MAP_WIDTH,
        440 * MAP_WIDTH,
        441 * MAP_WIDTH,
        442 * MAP_WIDTH,
        443 * MAP_WIDTH,
        444 * MAP_WIDTH,
        445 * MAP_WIDTH,
        446 * MAP_WIDTH,
        447 * MAP_WIDTH,
        448 * MAP_WIDTH,
        449 * MAP_WIDTH,
        450 * MAP_WIDTH,
        451 * MAP_WIDTH,
        452 * MAP_WIDTH,
        453 * MAP_WIDTH,
        454 * MAP_WIDTH,
        455 * MAP_WIDTH,
        456 * MAP_WIDTH,
        457 * MAP_WIDTH,
        458 * MAP_WIDTH,
        459 * MAP_WIDTH,
        460 * MAP_WIDTH,
        461 * MAP_WIDTH,
        462 * MAP_WIDTH,
        463 * MAP_WIDTH,
        464 * MAP_WIDTH,
        465 * MAP_WIDTH,
        466 * MAP_WIDTH,
        467 * MAP_WIDTH,
        468 * MAP_WIDTH,
        469 * MAP_WIDTH,
        470 * MAP_WIDTH,
        471 * MAP_WIDTH,
        472 * MAP_WIDTH,
        473 * MAP_WIDTH,
        474 * MAP_WIDTH,
        475 * MAP_WIDTH,
        476 * MAP_WIDTH,
        477 * MAP_WIDTH,
        478 * MAP_WIDTH,
        479 * MAP_WIDTH,
        480 * MAP_WIDTH,
        481 * MAP_WIDTH,
        482 * MAP_WIDTH,
        483 * MAP_WIDTH,
        484 * MAP_WIDTH,
        485 * MAP_WIDTH,
        486 * MAP_WIDTH,
        487 * MAP_WIDTH,
        488 * MAP_WIDTH,
        489 * MAP_WIDTH,
        490 * MAP_WIDTH,
        491 * MAP_WIDTH,
        492 * MAP_WIDTH,
        493 * MAP_WIDTH,
        494 * MAP_WIDTH,
        495 * MAP_WIDTH,
        496 * MAP_WIDTH,
        497 * MAP_WIDTH,
        498 * MAP_WIDTH,
        499 * MAP_WIDTH,
        500 * MAP_WIDTH,
        501 * MAP_WIDTH,
        502 * MAP_WIDTH,
        503 * MAP_WIDTH,
        504 * MAP_WIDTH,
        505 * MAP_WIDTH,
        506 * MAP_WIDTH,
        507 * MAP_WIDTH,
        508 * MAP_WIDTH,
        509 * MAP_WIDTH,
        510 * MAP_WIDTH,
        511 * MAP_WIDTH,
        512 * MAP_WIDTH
};