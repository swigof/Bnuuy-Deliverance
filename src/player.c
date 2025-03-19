#include "player.h"

const state_data_t player_idle = {
        ((player_idle_WIDTH >> 3) << 4) | (player_idle_HEIGHT >> 3),
        (player_idle_PIVOT_W << 4) | (player_idle_PIVOT_H),
        15,
        player_idle_tiles,
        player_idle_TILE_COUNT,
        30,
        player_idle_metasprites,
        1
};

const state_data_t player_move = {
        ((player_move_WIDTH >> 3) << 4) | (player_move_HEIGHT >> 3),
        (player_move_PIVOT_W << 4) | (player_move_PIVOT_H),
        15,
        player_move_tiles,
        player_move_TILE_COUNT,
        10,
        player_move_metasprites,
        2
};

const state_data_t player_jump = {
        ((player_move_WIDTH >> 3) << 4) | (player_move_HEIGHT >> 3),
        (player_move_PIVOT_W << 4) | (player_move_PIVOT_H),
        15,
        player_move_tiles,
        player_move_TILE_COUNT,
        30,
        player_move_metasprites,
        1
};

const state_data_t player_fall = {
        ((player_fall_WIDTH >> 3) << 4) | (player_fall_HEIGHT >> 3),
        (player_fall_PIVOT_W << 4) | (player_fall_PIVOT_H),
        15,
        player_fall_tiles,
        player_fall_TILE_COUNT,
        30,
        player_fall_metasprites,
        1
};

void update_player(entity_t* player) {
    if(!(player->state & GROUNDED)) {
        if (player->vel_y >= 0) {
            if (player->state_data != &player_fall) {
                player->animation_frame = 0;
                player->state_data = &player_fall;
                //set_sprite_data(0, player_fall.tile_count, player_fall.tiles);
            }
        } else {
            if (player->state_data != &player_jump) {
                player->animation_frame = 0;
                player->state_data = &player_jump;
                //set_sprite_data(0, player_jump.tile_count, player_jump.tiles);
            }
        }
    } else if(player->vel_x != 0) {
        if(player->state_data != &player_move) {
            player->animation_frame = 0;
            player->state_data = &player_move;
            //set_sprite_data(0, player_move.tile_count, player_move.tiles);
        }
    } else {
        if(player->state_data != &player_idle) {
            player->animation_frame = 0;
            player->state_data = &player_idle;
            //set_sprite_data(0, player_idle.tile_count, player_idle.tiles);
        }
    }
}

