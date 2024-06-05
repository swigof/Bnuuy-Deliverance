#include <gb/gb.h>
#include <gb/cgb.h>
#include "helpers/entity.h"
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"

#define MAX_CAMERA_Y ((tileset_map_height - 18) * 8)
#define MAX_PLAYER_Y 152
#define MIN_PLAYER_Y 16
#define MAX_PLAYER_X 160
#define MIN_PLAYER_X 8

joypads_t joypads;

entity_t player;
uint8_t map_pos_y, old_map_pos_y, redraw;
uint16_t camera_y;

void set_camera() {
    SCY_REG = camera_y;
    map_pos_y = (uint8_t)(camera_y >> 3u);
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

    DISPLAY_OFF;
    SHOW_BKG;
    SPRITES_8x8;
    SHOW_SPRITES;
    VBK_REG = VBK_TILES;
    set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    set_bkg_palette(0,1,tileset_map_colors);

    player.x = MIN_PLAYER_X;
    player.y = MAX_PLAYER_Y; // TODO place player at bottom, make map longer to see if max camera y breaks?
    camera_y = MAX_CAMERA_Y;

    old_map_pos_y = 255;
    map_pos_y = (uint8_t)(camera_y >> 3u);;
    set_bkg_submap(0, map_pos_y, 20, 18, tileset_map, 20);
    DISPLAY_ON;

    redraw = FALSE;

    SCY_REG = camera_y;

    // VBK_REG = VBK_ATTRIBUTES;
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);
    // VBK_REG = VBK_TILES;

    set_sprite_data(0, tileset_tiles_count, tileset_tiles);
    set_sprite_tile(0, 58);
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);
    set_sprite_prop(0, 0);
    move_sprite(0, player.x, player.y);

    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        // just for testing
        if(joypads.joy0 & J_UP) {
            if(player.y > MIN_PLAYER_Y) {
                move_entity_up(&player);
                if (player.sub_y == 0 && player.y < 72 && camera_y > 0) {
                    player.y = 72;
                    camera_y--;
                    redraw = TRUE;
                }
            }
        } else if(joypads.joy0 & J_DOWN) {
            if(player.y < MAX_PLAYER_Y) {
                move_entity_down(&player);
                if (player.sub_y == 0 && player.y > 72 && camera_y < MAX_CAMERA_Y) {
                    player.y = 72;
                    camera_y++;
                    redraw = TRUE;
                }
            }
        }

        if(joypads.joy0 & J_LEFT) {
            if (player.x > MIN_PLAYER_X)
                move_entity_left(&player);
        } else if(joypads.joy0 & J_RIGHT) {
            if (player.x < MAX_PLAYER_X)
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

        if (redraw) {
            wait_vbl_done();
            set_camera();
            redraw = FALSE;
        } else wait_vbl_done();
    }

    return 0;
}