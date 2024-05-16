#include <gb/gb.h>
#include <gb/cgb.h>
#include "helpers/entity.h"
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"

joypads_t joypads;

entity_t player;
uint8_t map_pos_y, old_map_pos_y, redraw;

void set_camera() {
    SCY_REG = player.y; 
    map_pos_y = (uint8_t)(player.y >> 3u);
    if (map_pos_y != old_map_pos_y) { 
        if (player.direction & J_UP) {
            set_bkg_submap(0, map_pos_y, 20, 1, tileset_map, 20);
        } else {
            if ((tileset_map_height - 18u) > map_pos_y) 
                set_bkg_submap(0, map_pos_y + 18u, 20, 1, tileset_map, 20);     
        }
        old_map_pos_y = map_pos_y; 
    }
}

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_ON;
    SHOW_BKG;
    SPRITES_8x8;
    SHOW_SPRITES;

    VBK_REG = VBK_TILES;
    set_bkg_palette(0,1,tileset_map_colors);
    set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    set_bkg_submap(0, 0, 20, 18, tileset_map, 20);
    // VBK_REG = VBK_ATTRIBUTES;
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);
    // VBK_REG = VBK_TILES;

    set_sprite_data(0, tileset_tiles_count, tileset_tiles);

    set_sprite_tile(0, 58);
    
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);

    set_sprite_prop(0, 0);

    player.x = 100;
    player.y = 100;
    move_sprite(0, player.x, player.y);

    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        // just for testing
        if(joypads.joy0 & J_UP) {
            move_entity_up(&player);
            set_camera();
        } else if(joypads.joy0 & J_DOWN) {
            move_entity_down(&player);
            set_camera();
        }

        if(joypads.joy0 & J_LEFT) {
            move_entity_left(&player);
        } else if(joypads.joy0 & J_RIGHT) {
            move_entity_right(&player);
        }

        if(joypads.joy0 & J_A) {
            // jump
        }

        if(joypads.joy0 & J_B) {
            // interact / attack
        }

        if(joypads.joy0 & J_SELECT) {

        }

        if(joypads.joy0 & J_START) {

        }

        update_entity(player);

        wait_vbl_done();
    }

    return 0;
}