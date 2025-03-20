#ifndef PLAYER_H
#define PLAYER_H

#include "../obj/player_sheet.h"
#include "entity.h"
#include "common.h"

extern const state_data_t player_idle;

void update_player(entity_t* player);

#endif //PLAYER_H
