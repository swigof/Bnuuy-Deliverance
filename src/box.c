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

}
