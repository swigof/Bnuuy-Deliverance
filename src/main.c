#include <gb/gb.h>
#include <gb/cgb.h>
#include "entity.h"
#include "../obj/tileset_primary.h"
#include "player.h"
#include "camera.h"
#include <gbdk/emu_debug.h>

joypads_t joypads;
joypads_t prev_joypads;
entity_t* player;
const state_data_t* pprev_state = 0;
uint8_t prev_frame = 0;

void print_message(void) {
    if(LY_REG == 143) {
        EMU_printf("line\n");
    }
}

const palette_color_t sprite_palettes[] = {
        RGB8(0xFF,0xEE,0xBB),RGB8(0xFF,0x66,0xCC),RGB8(0x77,0x33,0x66),RGB8(0,0,0)
};
const palette_color_t tile_palettes[] = {
        RGB8(0xFA,0xE6,0xCD),RGB8(0xF3,0xC0,0xCE),RGB8(0x97,0x9B,0xC7),RGB8(0,0,0)
};
int main() {
//    CRITICAL {
//        STAT_REG = STATF_MODE00;
//        add_LCD(print_message);
//        add_LCD(nowait_int_handler);
//    }
//    set_interrupts(IE_REG | LCD_IFLAG);

    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_OFF;
    SHOW_BKG;
    SPRITES_8x16;
    SHOW_SPRITES;

    set_bkg_data(0,tileset_primary_TILE_COUNT,tileset_primary_tiles);
    set_bkg_palette(0,1,tile_palettes);

    entity_to_add.x = 20<<4;
    entity_to_add.y = 344<<4;
    entity_to_add.active = TRUE;
    entity_to_add.state_data = &player_idle;
    entity_to_add.update_function = (void (*)(void *)) &update_player;
    set_sprite_data(0, player_idle_TILE_COUNT, player_idle_tiles);
    set_sprite_palette(0, 1, sprite_palettes);
    player = add_entity();

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
        pprev_state = player->state_data;
        prev_frame = player->animation_frame;
        if(prev_frame == 1 && pprev_state == &player_move) {
            EMU_printf("preupdate %u\n", player->state_data->metasprite[player->animation_frame]);
        }
        update_entities();
        if(prev_frame == 1 && pprev_state == &player_move && player->state_data == &player_idle) {
            EMU_printf("postupdate %u\n", player->state_data->metasprite[player->animation_frame]);
            //EMU_BREAKPOINT;
        }
        vsync();
        if(prev_frame == 1 && pprev_state == &player_move && player->state_data == &player_idle) {
            EMU_printf("postsync %u\n", player->state_data->metasprite[player->animation_frame]);
            //EMU_BREAKPOINT;
        }
        update_camera();
    }

    return 0;
}
