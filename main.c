#include <gb/gb.h>
#include <gb/cgb.h>
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"
#include "helpers/input.h"

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    set_bkg_palette(0,1,tileset_map_colors);
    VBK_REG = 0;
    set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map);
    VBK_REG = 1;
    set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);

    joypads_t joypads;
    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        if(joypads.joy0 & J_UP) {
            
        } else if(joypads.joy0 & J_DOWN) {

        }

        if(joypads.joy0 & J_LEFT) {

        } else if(joypads.joy0 & J_RIGHT) {

        }

        if(joypads.joy0 & J_A) {

        }

        if(joypads.joy0 & J_B) {

        }

        if(joypads.joy0 & J_SELECT) {

        }

        if(joypads.joy0 & J_START) {

        }
    }

    return 0;
}