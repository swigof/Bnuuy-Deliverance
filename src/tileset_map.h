#ifndef TILESET_MAP_H
#define TILESET_MAP_H

#include <gb/cgb.h>

#define tileset_map_width 20
#define tileset_map_height 45
#define tileset_map_tiles 900
#define tileset_map_bytes 900

extern const uint8_t tileset_map[];
extern const palette_color_t tileset_map_colors[];
extern const uint8_t tileset_map_attr[];
extern const uint16_t width_multiplication_table[];

#endif