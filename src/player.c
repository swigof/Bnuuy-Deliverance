#include "player.h"

const state_data_t player_idle = {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[4],
        1
};

const state_data_t player_move = {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        10,
        &player_sheet_metasprites[0],
        2
};

const state_data_t player_jump = {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[0],
        1
};

const state_data_t player_fall = {
        ((player_sheet_WIDTH >> 3) << 4) | (player_sheet_HEIGHT >> 3),
        (player_sheet_PIVOT_W << 4) | (player_sheet_PIVOT_H),
        15,
        30,
        &player_sheet_metasprites[12],
        1
};

void update_player(entity_t* player) {
    // Input and velocity changes
    if(joypads.joy0 & J_A && !(prev_joypads.joy0 & J_A)) {
        if(player->state & GROUNDED) {
            player->state &= ~GROUNDED;
            player->vel_y = JUMP_VELOCITY;
        } else if(!(player->state & DOUBLE_JUMP)) {
            player->state |= DOUBLE_JUMP;
            player->vel_y = JUMP_VELOCITY;
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

    // Position and collision detection
    velocity_direction_flip(player);
    if(velocity_collision_move(player)) {
        check_grounding(player);
    }

    // State updates
    if(!(player->state & GROUNDED)) {
        if (player->vel_y >= 0) {
            if (player->state_data != &player_fall) {
                player->animation_frame = 0;
                player->state_data = &player_fall;
            }
        } else {
            if (player->state_data != &player_jump) {
                player->animation_frame = 0;
                player->state_data = &player_jump;
            }
        }
    } else if(player->vel_x != 0) {
        if(player->state_data != &player_move) {
            player->animation_frame = 0;
            player->state_data = &player_move;
        }
    } else {
        if(player->state_data != &player_idle) {
            player->animation_frame = 0;
            player->state_data = &player_idle;
        }
    }
}

