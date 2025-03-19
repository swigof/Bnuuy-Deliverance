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

