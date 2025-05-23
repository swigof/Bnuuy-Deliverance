#include "box.h"
#include "cbtfx.h"
#include "../res/sfx/SFX_0A.h"

#pragma bank 1

const state_data_t box_base = {
        box_sheet_HEIGHT >> 1,
        box_sheet_PIVOT_W,
        box_sheet_PIVOT_H,
        15,
        30,
        &box_sheet_metasprites[0],
        1
};

uint8_t was_grounded;
void update_box(entity_t* box) {
    // Velocities
    if(!(box->state & GROUNDED) && box->vel_y < 100) {
        box->vel_y += 2;
    }
    if(box->state & GROUNDED) {
        was_grounded = TRUE;
        if(box->vel_x > 1) {
            box->vel_x -= 2;
        } else if(box->vel_x < -1) {
            box->vel_x += 2;
        } else {
            box->vel_x = 0;
        }
    } else {
        was_grounded = FALSE;
        if(box->vel_x > 0) {
            box->vel_x -= 1;
        } else if(box->vel_x < 0) {
            box->vel_x += 1;
        }
    }

    // Position and collision detection
    if(box->onscreen || box->vel_x != 0 || box->vel_y != 0) {
        if(velocity_collision_move(box)) {
            check_grounding(box);
        }
    }

    if(!was_grounded && (box->state & GROUNDED)) {
        CBTFX_PLAY_SFX_0A;
    }
}
