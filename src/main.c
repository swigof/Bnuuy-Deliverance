#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "tileset_map.h"
#include "camera.h"

joypads_t joypads;
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
    entity_to_add.sprite_dimensions = ((player_idle_WIDTH >> 3) << 4) | (player_idle_HEIGHT >> 3);
    entity_to_add.hitbox_margin = (player_idle_PIVOT_W << 4) | (player_idle_PIVOT_H);
    entity_to_add.active = TRUE;
    player = add_entity();

    set_sprite_data(0, player_idle_TILE_COUNT, player_idle_tiles);
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;

    joypad_init(1, &joypads);

    while(1) {
        joypad_ex(&joypads);

        if(joypads.joy0 & J_UP) {
            move_entity_up(player, 60);
        } else if(joypads.joy0 & J_DOWN) {
            move_entity_down(player, 60);
        }
        if(joypads.joy0 & J_RIGHT) {
            move_entity_right(player, 60);
        } else if(joypads.joy0 & J_LEFT) {
            move_entity_left(player, 60);
        }

        set_focus(MAP_COORD(player->y));

        update_entities();

        vsync();
        update_camera();
    }

    return 0;
}
