#include <gb/gb.h>
#include <gb/cgb.h>
#include "helpers/player.h"
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"

#define MAX_CAMERA_Y ((tileset_map_height - 18) * 8)
#define MAX_PLAYER_Y 152
#define MIN_PLAYER_Y 16
#define MAX_PLAYER_X 160
#define MIN_PLAYER_X 8

joypads_t joypads;

player_t player;
uint8_t map_pos_y, old_map_pos_y, redraw;
uint16_t camera_y;

void set_camera() {
    SCY_REG = camera_y;
    map_pos_y = (uint8_t)(camera_y >> 3u);
    if (map_pos_y != old_map_pos_y) {
        if (player.e.direction & J_UP) {
            set_bkg_submap(0, map_pos_y, 20, 1, tileset_map, 20);
            VBK_REG = VBK_ATTRIBUTES;
            set_bkg_submap(0, map_pos_y, 20, 18, tileset_map_attr, 20);
            VBK_REG = VBK_TILES;
        } else {
            if ((tileset_map_height - 18u) > map_pos_y)
                set_bkg_submap(0, map_pos_y + 18u, 20, 1, tileset_map, 20);
                VBK_REG = VBK_ATTRIBUTES;
                set_bkg_submap(0, map_pos_y, 20, 18, tileset_map_attr, 20);
                VBK_REG = VBK_TILES;
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

    player.e.x = MIN_PLAYER_X;
    player.e.y = MAX_PLAYER_Y;
    camera_y = MAX_CAMERA_Y;

    old_map_pos_y = 255;
    map_pos_y = (uint8_t)(camera_y >> 3u);;
    set_bkg_submap(0, map_pos_y, 20, 18, tileset_map, 20);
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_submap(0, map_pos_y, 20, 18, tileset_map_attr, 20);
    VBK_REG = VBK_TILES;
    DISPLAY_ON;

    redraw = FALSE;

    SCY_REG = camera_y;

    set_sprite_data(0, tileset_tiles_count, tileset_tiles);
    set_sprite_tile(0, 58);
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);
    set_sprite_prop(0, 0);
    move_sprite(0, player.e.x, player.e.y);

    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        // just for testing
        if(joypads.joy0 & J_UP) {
            if(player.e.y > MIN_PLAYER_Y) {
                move_entity_up(&player.e, 1);
                if (player.e.sub_y == 0 && player.e.y < 72 && camera_y > 0) {
                    player.e.y = 72;
                    camera_y--;
                    redraw = TRUE;
                }
            }
        } else if(joypads.joy0 & J_DOWN) {
            if(player.e.y < MAX_PLAYER_Y) {
                move_entity_down(&player.e, 1);
                if (player.e.sub_y == 0 && player.e.y > 72 && camera_y < MAX_CAMERA_Y) {
                    player.e.y = 72;
                    camera_y++;
                    redraw = TRUE;
                }
            }
        }

        if(joypads.joy0 & J_LEFT) {
            if (player.e.x > MIN_PLAYER_X) {
                move_entity_left(&player.e, 1); // TODO generify below for all entities, add collision check flag to entities
                player.e.true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((player.e.y + 7) >> 3) - 2)] + (player.e.x >> 3) - 1;
                if (tileset_map_attr[player.e.true_map_tile] & c) {
                    player.e.x = (((player.e.x + 7) >> 3)) << 3;
                }
            }
        } else if(joypads.joy0 & J_RIGHT) {
            if (player.e.x < MAX_PLAYER_X) {
                move_entity_right(&player.e, 1);
                player.e.true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((player.e.y + 7) >> 3) - 2)] + (player.e.x >> 3) - 1;
                if (tileset_map_attr[player.e.true_map_tile + 1] & c) {
                    player.e.x = ((player.e.x >> 3)) << 3;
                }
            }
        }

        if(joypads.joy0 & J_A) {
            if (player.air_state & GROUNDED) {
                player.air_state = (JUMPING | MAX_VELOCITY);
            } else if (player.air_state & FALLING && !(player.air_state & USED_DOUBLE)) {
                player.air_state = (JUMPING | USED_DOUBLE | MAX_VELOCITY);
            } else if (player.air_state & JUMPING && player.air_state & VELOCITY_MASK) {
                player.air_state = ((player.air_state & VELOCITY_MASK) - 1) | (player.air_state & FLAG_MASK);
            }
        } else if (!(player.air_state & GROUNDED)) {
            player.air_state &= USED_DOUBLE;
            player.air_state |= FALLING;
        }
        if (!(player.air_state & GROUNDED)) {  // TODO fix should use falling state and update to falling state when vel == 0
            if(player.air_state & VELOCITY_MASK) {
                move_entity_up(&player.e, player.air_state & VELOCITY_MASK);
                player.e.true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((player.e.y + 7) >> 3) - 2)] + (player.e.x >> 3) - 1;
                if (tileset_map_attr[player.e.true_map_tile] & c) {
                    // set to below block
                    // kill upward momentum
                }
            }
            else {
                move_entity_down(&player.e, 3); // TODO gradual increase, general accel calcs or use signed air velocity
                player.e.true_map_tile = width_multiplication_table[((uint16_t)(SCY_REG >> 3) + ((player.e.y + 7) >> 3) - 2)] + (player.e.x >> 3) - 1;
                if (player.e.y > MAX_PLAYER_Y || tileset_map_attr[player.e.true_map_tile] & c || tileset_map_attr[player.e.true_map_tile + 1] & c) {
                    player.e.y = (player.e.y >> 3) << 3;
                    player.air_state = GROUNDED;
                }
            }
        } else {
            if (player.e.y < MAX_PLAYER_Y && player.e.y & 0b00000111 == 0 && tileset_map_attr[player.e.true_map_tile + tileset_map_width] & c) {
                player.air_state = FALLING; // TODO fix repeatedly hit, thinks always grounded, condition seems utterly ignored
            }
        }

        if(joypads.joy0 & J_B) {
            // interact / attack
        }

        if(joypads.joy0 & J_SELECT) {

        }

        if(joypads.joy0 & J_START) {

        }

        if (player.e.direction & J_UP && player.e.y < 72 && camera_y > 0) {
            camera_y -= 72 - player.e.y;
            player.e.y = 72;
            redraw = TRUE;
        } else if (player.e.direction & J_DOWN && player.e.y > 72 && camera_y < MAX_CAMERA_Y) {
            camera_y += player.e.y - 72;
            player.e.y = 72;
            redraw = TRUE;
        }

        update_entity(player.e);

        if (redraw) {
            wait_vbl_done();
            set_camera();
            redraw = FALSE;
        } else wait_vbl_done();
    }

    return 0;
}