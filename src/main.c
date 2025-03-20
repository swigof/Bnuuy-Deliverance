#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "box.h"
#include "camera.h"
#include "common.h"

joypads_t joypads;
joypads_t prev_joypads;

entity_t* player;
entity_t* box;

const palette_color_t sprite_palettes[] = {
        RGB8(0xFF,0xEE,0xBB),RGB8(0xFF,0x66,0xCC),RGB8(0x77,0x33,0x66),RGB8(0,0,0)
};
const palette_color_t tile_palettes[] = {
        RGB8(0xFA,0xE6,0xCD),RGB8(0xF3,0xC0,0xCE),RGB8(0x97,0x9B,0xC7),RGB8(0,0,0)
};

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0xFF;
    NR51_REG = 0xFF;

    CRITICAL {
        hUGE_init(&main_track);
        add_VBL(hUGE_dosound);
    }

    DISPLAY_OFF;
    SHOW_BKG;
    SPRITES_8x16;
    SHOW_SPRITES;

    set_bkg_data(0,tileset_primary_TILE_COUNT,tileset_primary_tiles);
    set_bkg_palette(0,1,tile_palettes);

    set_sprite_data(0, player_sheet_TILE_COUNT, player_sheet_tiles);
    set_sprite_palette(0, 1, sprite_palettes);
    entity_to_add.base_tile = 0;
    entity_to_add.prop = 0;
    entity_to_add.x = 20<<4;
    entity_to_add.y = 330<<4;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &player_idle[0];
    entity_to_add.update_function = (void (*)(void *)) &update_player;
    populate_hitbox_record(&entity_to_add);
    player = add_entity();

    set_sprite_data(player_sheet_TILE_COUNT, box_sheet_TILE_COUNT, box_sheet_tiles);
    set_sprite_palette(1, 1, box_sheet_palettes);
    entity_to_add.base_tile = player_sheet_TILE_COUNT;
    entity_to_add.prop = 1; // Set to palette 1
    entity_to_add.x = 40<<4;
    entity_to_add.y = 330<<4;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &box_base;
    populate_hitbox_record(&entity_to_add);
    entity_to_add.update_function = (void (*)(void *)) &update_box;
    box = add_entity();

    init_camera(MAP_COORD(player->y));

    DISPLAY_ON;

    joypad_init(1, &joypads);

    while(1) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        set_focus(MAP_COORD(player->y));
        update_entities();
        vsync();
        update_camera();
    }

    return 0;
}
