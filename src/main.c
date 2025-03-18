#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "tileset_map.h"
#include "camera.h"

joypads_t joypads;
joypads_t prev_joypads;
entity_t* player;

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_OFF;
    SHOW_BKG;
    SPRITES_8x16;
    SHOW_SPRITES;

    set_bkg_data(0,tileset_primary_TILE_COUNT,tileset_primary_tiles);
    set_bkg_palette(0,1,tileset_map_colors);

    entity_to_add.x = 20;
    entity_to_add.y = 350<<4;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &player_idle;
    player = add_entity();

    set_sprite_data(0, player_idle_TILE_COUNT, player_idle_tiles);
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;

    joypad_init(1, &joypads);

    while(1) {
        prev_joypads = joypads;
        joypad_ex(&joypads);

        if(joypads.joy0 & J_A && !(prev_joypads.joy0 & J_A)) {
            if(player->state & GROUNDED) {
                player->state &= ~GROUNDED;
                player->vel_y = JUMP_VELOCITY;
            } else if(!(player->state & DOUBLE_JUMP)) {
                player->state |= DOUBLE_JUMP;
                player->vel_y = JUMP_VELOCITY;
            }
        } else if(!(player->state & GROUNDED) && player->vel_y < 100) {
            if(joypads.joy0 & J_A)
                player->vel_y += 1;
            else
                player->vel_y += 2;
        }
        if(joypads.joy0 & J_LEFT) {
            player->vel_x = -player->state_data->speed;
        } else if(joypads.joy0 & J_RIGHT) {
            player->vel_x = player->state_data->speed;
        } else {
            player->vel_x = 0;
        }

        set_focus(MAP_COORD(player->y));

        update_entities();

        vsync();
        update_camera();
    }

    return 0;
}
