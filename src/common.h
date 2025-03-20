#ifndef COMMON_H
#define COMMON_H

#include <gb/gb.h>
#include <hUGEDriver.h>

#define HIGH_BITS(v) (v >> 4)
#define LOW_BITS(v) (v & 0b00001111)

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

extern joypads_t joypads;
extern joypads_t prev_joypads;

extern entity_t* player;
extern entity_t* box;

extern const hUGESong_t main_track;

#endif //COMMON_H
