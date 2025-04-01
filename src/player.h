#ifndef PLAYER_H
#define PLAYER_H

#include "../obj/player_sheet.h"
#include "entity.h"
#include "common.h"
#include "box.h"
#include "color.h"
#include "level.h"

extern const state_data_t player_idle[2];
extern const state_data_t player_sigh;
extern const state_data_t player_get_up;
extern const state_data_t player_turn_around;

extern uint8_t carry;

void update_player(entity_t* player);

#endif //PLAYER_H
