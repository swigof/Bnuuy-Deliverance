#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "tileset_map.h"

#define MAX_CAMERA_Y ((tileset_map_height - 18) * 8)

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
    SPRITES_8x16;
    SHOW_SPRITES;
    VBK_REG = VBK_TILES;
    set_bkg_data(0,tileset_primary_TILE_COUNT,tileset_primary_tiles);
    set_bkg_palette(0,1,tileset_map_colors);

    player.x = MIN_PLAYER_X;
    player.y = MAX_PLAYER_Y;
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

    player.sprite_dimensions = ((player_idle_WIDTH >> 3) << 4) | (player_idle_HEIGHT >> 3);
    set_sprite_data(0, player_idle_TILE_COUNT, player_idle_tiles);
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);

    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        // just for testing
        if(joypads.joy0 & J_UP) {
            if(player.y > MIN_PLAYER_Y) {
                move_entity_up(&player, 4);
                if (player.sub_y == 0 && player.y < 72 && camera_y > 0) {
                    player.y = 72;
                    camera_y--;
                    redraw = TRUE;
                }
            }
        } else if(joypads.joy0 & J_DOWN) {
            if(player.y < MAX_PLAYER_Y) {
                move_entity_down(&player, 4);
                if (player.sub_y == 0 && player.y > 72 && camera_y < MAX_CAMERA_Y) {
                    player.y = 72;
                    camera_y++;
                    redraw = TRUE;
                }
            }
        }

        if(joypads.joy0 & J_LEFT) {
            if (player.x > MIN_PLAYER_X) { // TODO move screen guard into move function? have entity flag?
                move_entity_left(&player, 1);
            }
        } else if(joypads.joy0 & J_RIGHT) {
            if (player.x < MAX_PLAYER_X) {
                move_entity_right(&player, 1);
            }
        }
        // check_collision(&player, 0); // TODO collision flag for entity

        if(joypads.joy0 & J_A) {
            if (player.air_state & GROUNDED) {
                player.air_state = (JUMPING | MAX_VELOCITY);
            } else if (player.air_state & FALLING && !(player.air_state & USED_DOUBLE)) {
                player.air_state = (JUMPING | USED_DOUBLE | MAX_VELOCITY);
            } else if (player.air_state & JUMPING && player.air_state & VELOCITY_MASK) {
                player.air_state = ((player.air_state & VELOCITY_MASK) - 1) | (player.air_state & FLAG_MASK);
            }
        } else if (!(player.air_state & GROUNDED)) {
            // TODO preserve vel
            player.air_state &= USED_DOUBLE;
            player.air_state |= FALLING;
        }
        if (!(player.air_state & GROUNDED)) {  // TODO fix should use falling state and update to falling state when vel == 0
            if(player.air_state & VELOCITY_MASK) {
                move_entity_up(&player, player.air_state & VELOCITY_MASK);
            }
            else {
                move_entity_down(&player, 3); // TODO gradual increase, general accel calcs or use signed air velocity
            }
        } else {
            if (player.y < MAX_PLAYER_Y && player.y & 0b00000111 == 0 && tileset_map_attr[current_entity_true_map_tile + tileset_map_width] & COLLIDABLE) {
                player.air_state = FALLING; // TODO fix repeatedly hit, thinks always grounded, condition seems utterly ignored
            }
        }

        check_collision(&player, 0);

        if(joypads.joy0 & J_B) {
            // interact / attack
        }

        if(joypads.joy0 & J_SELECT) {

        }

        if(joypads.joy0 & J_START) {

        }
        // TODO adjust cam value to prioritize up
        if (player.direction & J_UP && player.y < 72 && camera_y > 0) {
            camera_y -= 72 - player.y;
            player.y = 72;
            redraw = TRUE;
        } else if (player.direction & J_DOWN && player.y > 72 && camera_y < MAX_CAMERA_Y) {
            camera_y += player.y - 72;
            player.y = 72;
            redraw = TRUE;
        }

        update_entity(player);

        sprite_index = 0;

        if (redraw) {
            vsync();
            set_camera();
            redraw = FALSE;
        } else vsync();
    }

    return 0;
}