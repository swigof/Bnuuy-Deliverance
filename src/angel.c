#include "angel.h"

#pragma bank 1

uint8_t float_timer = 0;

void update_angel(entity_t* e) {
    float_timer++;
    if(float_timer % 64 == 0) {
        if(float_timer % 128)
            e->y += 16;
        else
            e->y -= 16;
    }
}

const state_data_t angel_tpose = {
    angel_sheet_HEIGHT >> 1,
    angel_sheet_PIVOT_W,
    angel_sheet_PIVOT_H,
    16,
    15,
    &angel_sheet_metasprites[0],
    1
};

const state_data_t angel_idle = {
    angel_sheet_HEIGHT >> 1,
    angel_sheet_PIVOT_W,
    angel_sheet_PIVOT_H,
    16,
    15,
    &angel_sheet_metasprites[1],
    1
};

const state_data_t angel_clap = {
    angel_sheet_HEIGHT >> 1,
    angel_sheet_PIVOT_W,
    angel_sheet_PIVOT_H,
    16,
    15,
    &angel_sheet_metasprites[2],
    2
};
