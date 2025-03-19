#ifndef PLAYER_H
#define PLAYER_H

#include "../obj/player_climb.h"
#include "../obj/player_crouch.h"
#include "../obj/player_hurt.h"
#include "../obj/player_idle.h"
#include "../obj/player_fall.h"
#include "../obj/player_move.h"
#include "entity.h"

extern const state_data_t player_idle;

void update_player(entity_t* player);

#endif //PLAYER_H
