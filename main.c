#include <gb/gb.h>
#include <gb/cgb.h>
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"
#include "helpers/input.h"

const uint8_t sprite_data[] = { 
  0x0F,0x0F,0x30,0x30,0x40,0x40,0x40,0x40,
  0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,
  0x84,0x84,0x84,0x84,0x80,0x80,0x80,0x80,
  0x44,0x44,0x43,0x43,0x30,0x30,0x0F,0x0F,
  0xF0,0xF0,0x0C,0x0C,0x02,0x02,0x02,0x02,
  0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,
  0x21,0x21,0x21,0x21,0x01,0x01,0x01,0x01,
  0x22,0x22,0xC2,0xC2,0x0C,0x0C,0xF0,0xF0
};

int main() {
    // NR52_REG = 0x80;
    // NR50_REG = 0x77;
    // NR51_REG = 0xFF;

    // DISPLAY_ON;
    // SHOW_BKG;
    // SPRITES_8x8;

    // VBK_REG = VBK_TILES;
    // set_bkg_palette(0,1,tileset_map_colors);
    // set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map);
    // VBK_REG = VBK_ATTRIBUTES;
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);
    
    //VBK_REG = VBK_TILES;
    //BGP_REG = OBP0_REG = OBP1_REG = 0xE4;
    set_sprite_data(0, 4, sprite_data);

    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    
    palette_color_t sprite_palettes[] = { RGB8(0, 0, 0),RGB8(255, 0, 0),RGB8(255, 0, 0),RGB8(255, 0, 0) };
    set_sprite_palette(0,1,sprite_palettes);

    set_sprite_prop(0, 0);
    set_sprite_prop(1, 0);
    set_sprite_prop(2, 0);
    set_sprite_prop(3, 0);

    move_sprite(0, 75, 75);
    move_sprite(1, 75, 75 + 8);
    move_sprite(2, 75 + 8, 75);
    move_sprite(3, 75 + 8, 75 + 8);

    SHOW_SPRITES;
    
    input_init();

    while(1) {
        input_process();

        wait_vbl_done();
    }

    return 0;
}