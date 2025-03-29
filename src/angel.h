#ifndef ANGEL_H
#define ANGEL_H

#include "../obj/angel_sheet.h"
#include "entity.h"

extern const state_data_t angel_idle;
extern const state_data_t angel_tpose;
extern const state_data_t angel_clap;

void update_angel(entity_t* e);

#endif //ANGEL_H
