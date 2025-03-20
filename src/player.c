#include "player.h"

// Player state data is split into arrays of the not carrying and carrying versions to simplify their selection

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
    if(joypads.joy0 & J_B) {
        if(box->onscreen && !player->carry && do_hitboxes_overlap(&player->hitbox, &box->hitbox)) {
            player->carry = 1;
            box->update_function = NULL;
        }
    } else {
        if (player->carry) {
            player->carry = 0;
            box->state &= ~GROUNDED;
            populate_hitbox_record(box);
            box->update_function = (void (*)(void *)) &update_box;
            box->vel_y = player->vel_y - 30;
            if (player->state & FLIP_X)
                box->vel_x = player->vel_x - 30;
            else
                box->vel_x = player->vel_x + 30;
        }
    }

    // Position and collision detection
    velocity_direction_flip(player);
    if(velocity_collision_move(player)) {
        check_grounding(player);
    }

    // Box update if carrying
    if(player->carry) {
        box->x = player->x;
        box->y = (player->hitbox.top - 3) << 4;
    }

    // State updates
    if(!(player->state & GROUNDED)) {
        if (player->vel_y >= 0) {
            if (player->state_data != &player_fall[player->carry]) {
                player->animation_frame = 0;
                player->state_data = &player_fall[player->carry];
            }
        } else {
            if (player->state_data != &player_jump[player->carry]) {
                player->animation_frame = 0;
                player->state_data = &player_jump[player->carry];
            }
        }
    } else if(player->vel_x != 0) {
        if(player->state_data != &player_move[player->carry]) {
            player->animation_frame = 0;
            player->state_data = &player_move[player->carry];
        }
    } else {
        if(player->state_data != &player_idle[player->carry]) {
            player->animation_frame = 0;
            player->state_data = &player_idle[player->carry];
        }
    }
}

