#include "box.h"

const state_data_t box_base = {
        ((box_sheet_WIDTH >> 3) << 4) | (box_sheet_HEIGHT >> 3),
        (box_sheet_PIVOT_W << 4) | (box_sheet_PIVOT_H),
        15,
        30,
        &box_sheet_metasprites[0],
        1
};

void update_box(entity_t* box) {
    // Velocities
    if(!(box->state & GROUNDED) && box->vel_y < 100) {
        box->vel_y += 2;
    }
    if(box->state & GROUNDED) {
        if(box->vel_x > 1) {
            box->vel_x -= 2;
        } else if(box->vel_x < -1) {
            box->vel_x += 2;
        } else {
            box->vel_x = 0;
        }
    } else {
        if(box->vel_x > 0) {
            box->vel_x -= 1;
        } else if(box->vel_x < 0) {
            box->vel_x += 1;
        }
    }

    // Position and collision detection
    velocity_direction_flip(box);
    if(velocity_collision_move(box)) {
        check_grounding(box);
    }
}
