#ifndef PLAYER_H
#define PLAYER_H

#include "../obj/player_sheet.h"
#include "entity.h"
#include "common.h"
#include "box.h"
#include "color.h"

extern const state_data_t player_idle[2];

void update_player(entity_t* player);

#endif //PLAYER_H
