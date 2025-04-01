#include "player.h"
#include "cbtfx.h"
#include "../res/sfx/SFX_0C.h"
#include "../res/sfx/SFX_0D.h"

#pragma bank 1

uint8_t carry = 0; // If player is carrying the box

// Player state data is split into arrays of the not carrying and carrying versions to simplify their selection

const int8_t jump_velocity[2] = {-20, -16};

const state_data_t player_idle[2] = {
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        16,
        30,
        &player_sheet_metasprites[4],
        1
    },
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        8,
        30,
        &player_sheet_metasprites[6],
        1
    }
};

const state_data_t player_move[2] = {
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        16,
        10,
        &player_sheet_metasprites[0],
        2
    },
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        8,
        15,
        &player_sheet_metasprites[2],
        2
    }
};

const state_data_t player_jump[2] = {
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        16,
        30,
        &player_sheet_metasprites[0],
        1
    },
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        8,
        30,
        &player_sheet_metasprites[2],
        1
    }
};

const state_data_t player_fall[2] = {
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        16,
        30,
        &player_sheet_metasprites[12],
        1
    },
    {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        8,
        30,
        &player_sheet_metasprites[13],
        1
    }
};

const state_data_t player_sigh = {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        0,
        120,
        &player_sheet_metasprites[17],
        3
};

const state_data_t player_get_up = {
        player_sheet_HEIGHT >> 1,
        player_sheet_PIVOT_W,
        player_sheet_PIVOT_H,
        0,
        0,
        &player_sheet_metasprites[16],
        1
};

void update_player(entity_t* player) {
    // Input and velocity changes
    if(joypads.joy0 & J_A && !(prev_joypads.joy0 & J_A)) {
        if(player->state & GROUNDED) {
            CBTFX_PLAY_SFX_0C;
            player->state &= ~GROUNDED;
            player->vel_y = jump_velocity[carry];
        } else if(!(player->state & DOUBLE_JUMP) && !carry) {
            CBTFX_PLAY_SFX_0D;
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
            if(is_at_coord(&player->hitbox, current_level->door_x, current_level->door_y)) {
                level_transition(current_level->next_level);
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
            } else {
                box->vel_y = -10;
                if (player->state & FLIP_X)
                    box->vel_x = -10;
                else
                    box->vel_x = 10;
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

