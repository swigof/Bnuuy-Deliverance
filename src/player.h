#ifndef PLAYER_H
#define PLAYER_H

#include "../obj/player_sheet.h"
#include "entity.h"
#include "common.h"
#include "box.h"
#include "color.h"
#include "level.h"

extern const state_data_t player_idle[2];

extern uint8_t remove_door;

void update_player(entity_t* player);
void level_transition();

#endif //PLAYER_H
