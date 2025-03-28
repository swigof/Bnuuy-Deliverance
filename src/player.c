#include "player.h"

#pragma bank 1

uint8_t carry = 0; // If player is carrying the box

// Check if player is at the door on the current level
inline uint8_t is_at_door(hitbox_record_t* h) {
    return h->left < current_level->door_x &&
           h->right > current_level->door_x &&
           h->top < current_level->door_y &&
           h->bottom > current_level->door_y;
}
#define DOOR_TILE_INDEX 12
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

// Player state data is split into arrays of the not carrying and carrying versions to simplify their selection

const int8_t jump_velocity[2] = {-20, -16};

const state_data_t player_idle[2] = {
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[4],
        1
    },
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        10,
        30,
        &player_sheet_metasprites[6],
        1
    }
};

const state_data_t player_move[2] = {
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        10,
        &player_sheet_metasprites[0],
        2
    },
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        10,
        15,
        &player_sheet_metasprites[2],
        2
    }
};

const state_data_t player_jump[2] = {
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[0],
        1
    },
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        12,
        30,
        &player_sheet_metasprites[2],
        1
    }
};

const state_data_t player_fall[2] = {
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[12],
        1
    },
    {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        12,
        30,
        &player_sheet_metasprites[13],
        1
    }
};

uint16_t player_fade_colors[4];
uint16_t box_fade_colors[4];
void level_transition() {
    // remove door by replacing its tiles with blanks
    set_bkg_data(DOOR_TILE_INDEX, 4, empty_tiles);

    // set player to turn around sprite, hide box
    hide_sprites_range(
            move_metasprite_ex(
                    player_sheet_metasprites[21],
                    player->base_tile,
                    player->prop,
                    0,
                    current_level->door_x + DEVICE_SPRITE_PX_OFFSET_X,
                    current_level->door_y - camera_y + DEVICE_SPRITE_PX_OFFSET_Y),
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
    current_level = current_level->next_level;
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
}

void update_player(entity_t* player) {
    // Input and velocity changes
    if(joypads.joy0 & J_A && !(prev_joypads.joy0 & J_A)) {
        if(player->state & GROUNDED) {
            player->state &= ~GROUNDED;
            player->vel_y = jump_velocity[carry];
        } else if(!(player->state & DOUBLE_JUMP) && !carry) {
            player->state |= DOUBLE_JUMP;
            player->vel_y = jump_velocity[carry];
        }
    } else if(!(player->state & GROUNDED) && player->vel_y < 100) {
        if(joypads.joy0 & J_A)
            player->vel_y += 1;
        else
            player->vel_y += 2;
    }
    if(joypads.joy0 & J_LEFT) {
        player->vel_x = -player->state_data->speed;
    } else if(joypads.joy0 & J_RIGHT) {
        player->vel_x = player->state_data->speed;
    } else {
        player->vel_x = 0;
    }
    if(joypads.joy0 & J_B) {
        if(box->onscreen && !carry && do_hitboxes_overlap(&player->hitbox, &box->hitbox)) {
            carry = 1;
            box->update_function = NULL;
        }
        if(carry && (player->state & GROUNDED) && (joypads.joy0 & J_UP)) {
            if(is_at_door(&player->hitbox)) {
                level_transition();
            }
        }
    } else {
        if (carry) {
            carry = 0;
            box->state &= ~GROUNDED;
            populate_hitbox_record(box);
            box->update_function = (void (*)(void *)) &update_box;
            box->vel_x = player->vel_x;
            box->vel_y = player->vel_y;
            if (!(joypads.joy0 & J_DOWN)) {
                box->vel_y -= 30;
                if (player->state & FLIP_X)
                    box->vel_x -= 30;
                else
                    box->vel_x += 30;
            }
        }
    }

    // Position and collision detection
    velocity_direction_flip(player);
    if(velocity_collision_move(player)) {
        check_grounding(player);
    }

    // Box update if carrying
    if(carry) {
        box->x = player->x;
        if(joypads.joy0 & J_DOWN)
            box->y = player->hitbox.top << 4;
        else
            box->y = (player->hitbox.top - 3) << 4;
    }

    // State updates
    if(!(player->state & GROUNDED)) {
        if (player->vel_y >= 0) {
            if (player->state_data != &player_fall[carry]) {
                player->animation_frame = 0;
                player->state_data = &player_fall[carry];
            }
        } else {
            if (player->state_data != &player_jump[carry]) {
                player->animation_frame = 0;
                player->state_data = &player_jump[carry];
            }
        }
    } else if(player->vel_x != 0) {
        if(player->state_data != &player_move[carry]) {
            // Start at the end of frame 1 to prevent walking into walls causing animation jitter
            player->animation_frame = 1;
            player->frame_counter = 8;
            player->state_data = &player_move[carry];
        }
    } else {
        if(player->state_data != &player_idle[carry]) {
            player->animation_frame = 0;
            player->state_data = &player_idle[carry];
        }
    }
}

