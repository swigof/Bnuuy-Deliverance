#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "angel.h"
#include "box.h"
#include "camera.h"
#include "common.h"
#include "level_zero.h"
#include "truck_sheet.h"

#pragma bank 1

joypads_t joypads;
joypads_t prev_joypads;

entity_t* player;
entity_t* box;

const level_t* current_level;

const palette_color_t tile_palettes[] = {
        RGB8(0xFA,0xE6,0xCD),RGB8(0xF3,0xC0,0xCE),RGB8(0x97,0x9B,0xC7),RGB8(0,0,0)
};

int main(void) NONBANKED{
    DISPLAY_OFF;

    NR52_REG = 0x80;
    NR50_REG = 0xFF;
    NR51_REG = 0xFF;

    SHOW_BKG;
    SPRITES_8x16;
    SHOW_SPRITES;

    // Set sprite and tile data as they don't change
    SWITCH_ROM(BANK(tileset_primary));
    set_bkg_data(0,tileset_primary_TILE_COUNT,tileset_primary_tiles);
    set_bkg_palette(0,1,tile_palettes);
    SWITCH_ROM(1);
    set_sprite_data(0, player_sheet_TILE_COUNT, player_sheet_tiles);
    set_sprite_palette(0, 1, player_sheet_palettes);
    set_sprite_data(player_sheet_TILE_COUNT, box_sheet_TILE_COUNT, box_sheet_tiles);
    set_sprite_palette(1, 1, box_sheet_palettes);
    set_sprite_data(player_sheet_TILE_COUNT + box_sheet_TILE_COUNT, truck_sheet_TILE_COUNT, truck_sheet_tiles);
    set_sprite_palette(2, 1, truck_sheet_palettes);
    set_sprite_data(player_sheet_TILE_COUNT + box_sheet_TILE_COUNT + truck_sheet_TILE_COUNT,
                    angel_sheet_TILE_COUNT, angel_sheet_tiles);
    set_sprite_palette(3, angel_sheet_PALETTE_COUNT, angel_sheet_palettes);

    // Create our box and player since they will persist through levels
    entity_to_add.base_tile = 0;
    entity_to_add.prop = 0;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &player_idle[0];
    entity_to_add.update_function = (void (*)(void *)) &update_player;
    player = add_entity();
    entity_to_add.base_tile = player_sheet_TILE_COUNT;
    entity_to_add.prop = 1; // Set to palette 1
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &box_base;
    entity_to_add.update_function = (void (*)(void *)) &update_box;
    box = add_entity();

    joypad_init(1, &joypads);

    current_level = &level_0;

    current_level->init_function();

    while(1) {
        if (current_level->update_function) {
            current_level->update_function();
        } else {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            set_focus(MAP_COORD(player->y));
            update_entities();
            vsync();
            update_camera();
        }
    }

    return 0;
}
