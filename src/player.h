#ifndef PLAYER_H
#define PLAYER_H

// TODO place struct definition and player state related management here and in player.c

// TODO replace with level data from common header
#define MAX_PLAYER_Y tileset_map_height * 8
#define MIN_PLAYER_Y 8
#define MAX_PLAYER_X 154
#define MIN_PLAYER_X 6

#include "../obj/player_climb.h"
#include "../obj/player_crouch.h"
#include "../obj/player_hurt.h"
#include "../obj/player_idle.h"
#include "../obj/player_jump.h"
#include "../obj/player_run.h"

// TODO place struct definition and player state related management here and in player.c

#endif //PLAYER_H
