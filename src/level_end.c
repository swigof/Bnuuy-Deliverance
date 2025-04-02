#include "level_end.h"
#include "player.h"
#include "angel.h"
#include <rand.h>
#include "cbtfx.h"
#include "../res/sfx/SFX_0A.h"
#include "../res/sfx/SFX_08.h"
#include "../res/sfx/SFX_12.h"
#include "../obj/level_final.h"
#include "../obj/truck_sheet.h"

#pragma bank 1

#define BOX_SHAKE_COUNT 4
#define STAR_TILE_INDEX 86

uint8_t true_end_flag = FALSE;

uint8_t end_cutscene_timer = 0;
uint8_t end_cutscene_flag = FALSE;
uint8_t box_shake_counter = 0;
uint8_t star_counter = 0;

entity_t* angel;

const uint8_t foregrounded_merlons[] = { 0xA0, 0x80, 0xA0, 0x80 };
const uint8_t foregrounded_floor[] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

const uint8_t star_tiles[] = {
        0x00,0x00,0x00,0x00,
        0x04,0x00,0x0e,0x00,
        0x04,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,

        0x00,0x00,0x00,0x00,
        0x00,0x00,0x02,0x00,
        0x04,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,

        0x00,0x00,0x00,0x00,
        0x00,0x00,0x04,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
};

void move_box_to_position(entity_t* e) {
    if(box_shake_counter < BOX_SHAKE_COUNT) {
        if (MAP_COORD(e->x) < 80)
            e->vel_x = ((80 - MAP_COORD(e->x)));
        else if (MAP_COORD(e->x) > 80)
            e->vel_x = -((MAP_COORD(e->x) - 80));
        if (MAP_COORD(e->y) < 224)
            e->vel_y = ((224 - MAP_COORD(e->y)));
        else if (MAP_COORD(e->y) > 224)
            e->vel_y = -((MAP_COORD(e->y) - 224));

        if(e->onscreen || e->vel_x != 0 || e->vel_y != 0) {
            if (velocity_collision_move(e)) {
                check_grounding(e);
            }
        }
    } else {
        e->vel_y++;
        e->y += e->vel_y;
    }
}

void level_end_init(void) {
    // stop music
    hUGE_mute_channel(HT_CH1,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH2,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH3,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH4,HT_CH_MUTE);
    CRITICAL {
        remove_VBL(hUGE_dosound);
    }
    // reset mutes to avoid sfx bugs
    hUGE_mute_channel(HT_CH1,HT_CH_PLAY);
    hUGE_mute_channel(HT_CH2,HT_CH_PLAY);
    hUGE_mute_channel(HT_CH3,HT_CH_PLAY);
    hUGE_mute_channel(HT_CH4,HT_CH_PLAY);
    standard_init();
    set_bkg_attributes(6, 35, 2, 2, foregrounded_merlons);
    set_bkg_attributes(12, 35, 2, 2, foregrounded_merlons);
}

void level_end_update(void) {
    // wait for player to reach center
    while(!end_cutscene_flag) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        update_entities();
        if(MAP_COORD(player->x) >= 80) {
            end_cutscene_flag = TRUE;
        }
        vsync();
    }

    if(!true_end_flag) {
        // be grounded
        joypads.joy0 = 0;
        prev_joypads = joypads;
        while(!(GROUNDED & player->state)) {
            update_entities();
            vsync();
        }
        update_entities();
        vsync();
        while(!(GROUNDED & box->state)) {
            update_entities();
            vsync();
        }

        // sigh
        player->update_function = NULL;
        player->animation_frame = 0;
        player->frame_counter = 0;
        player->state_data = &player_sigh;
        while(player->frame_counter != 100 || player->animation_frame != 2) {
            update_entities();
            vsync();
        }

        //pan up
        while(camera_y > 8) {
            if(camera_y % 4 == 0) {
                fade_to_color(&tile_palettes[0], &black);
                set_bkg_palette(0, 1, tile_palettes);
            }
            update_entities();
            camera_y--;
            update_camera();
            vsync();
        }
        player->active = FALSE;
        box->active = FALSE;
    } else {
        // background dropping box and angel sprites
        set_bkg_attributes(6, 37, 8, 1, foregrounded_floor);

        // prevent player from carrying box, move box into position
        carry = FALSE;
        box->update_function = (void (*)(void *)) &move_box_to_position;
        end_cutscene_timer = 0;
        while (end_cutscene_timer < 120) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            joypads.joy0 &= ~J_B;
            update_entities();
            vsync();
            end_cutscene_timer++;
        }

        // shake box
        initrand(0); // use static seed to preserve same behavior
        while (box_shake_counter < BOX_SHAKE_COUNT) {
            end_cutscene_timer = rand();
            if (end_cutscene_timer < 30)
                end_cutscene_timer = 30;
            else if (end_cutscene_timer > 120)
                end_cutscene_timer = 120;
            CBTFX_PLAY_SFX_0A;
            box->x += (int8_t) rand();
            box->y += (int8_t) rand();
            while (end_cutscene_timer != 0) {
                prev_joypads = joypads;
                joypad_ex(&joypads);
                joypads.joy0 &= ~J_B;
                update_entities();
                vsync();
                end_cutscene_timer--;
            }
            box_shake_counter++;
        }

        // drop box and despawn it
        while (MAP_COORD(box->y) < 305) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            joypads.joy0 &= ~J_B;
            update_entities();
            vsync();
        }
        box->active = FALSE;
        end_cutscene_timer = 0;
        while (end_cutscene_timer < 60) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            update_entities();
            vsync();
            end_cutscene_timer++;
        }
        CBTFX_PLAY_SFX_08;
        end_cutscene_timer = 0;
        while (end_cutscene_timer < 60) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            update_entities();
            vsync();
            end_cutscene_timer++;
        }

        // spawn angel, fly it up
        entity_to_add.base_tile = player_sheet_TILE_COUNT + box_sheet_TILE_COUNT + truck_sheet_TILE_COUNT;
        entity_to_add.prop = 3;
        entity_to_add.active = TRUE;
        entity_to_add.state_data = &angel_tpose;
        entity_to_add.x = 80 << 4;
        entity_to_add.y = 336 << 4;
        entity_to_add.update_function = (void (*)(void *)) &update_angel;
        angel = add_entity();
        while (MAP_COORD(angel->y) > 212) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            if (MAP_COORD(angel->y) > 273)
                angel->vel_y--;
            else
                angel->vel_y++;
            angel->y += angel->vel_y;
            update_entities();
            vsync();
        }
        while (MAP_COORD(angel->y) < 216) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            angel->vel_y++;
            angel->y += angel->vel_y;
            update_entities();
            vsync();
        }

        // idle
        angel->state_data = &angel_idle;
        end_cutscene_timer = 0;
        while (end_cutscene_timer < 120) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            update_entities();
            vsync();
            end_cutscene_timer++;
        }

        //clap
        angel->state_data = &angel_clap;
        end_cutscene_timer = 0;
        while (end_cutscene_timer < 180) {
            if(angel->animation_frame == 1 && angel->frame_counter == 0)
                CBTFX_PLAY_SFX_12;
            prev_joypads = joypads;
            joypad_ex(&joypads);
            update_entities();
            vsync();
            end_cutscene_timer++;
        }

        //fly away and pan up
        angel->state_data = &angel_tpose;
        while(camera_y > 8) {
            if(camera_y % 4 == 0) {
                fade_to_color(&tile_palettes[0], &black);
                set_bkg_palette(0, 1, tile_palettes);
            }
            prev_joypads = joypads;
            joypad_ex(&joypads);
            angel->vel_y--;
            angel->y += angel->vel_y;
            update_entities();
            camera_y--;
            update_camera();
            vsync();
        }
        angel->active = FALSE;
        player->active = FALSE;
    }
    end_cutscene_timer = 0;
    while(1) {
        if(end_cutscene_timer % 8 == 0) {
            set_bkg_data(STAR_TILE_INDEX + 2, 1, &star_tiles[star_counter % 48]);
            set_bkg_data(STAR_TILE_INDEX + 1, 1, &star_tiles[(star_counter + 16) % 48]);
            set_bkg_data(STAR_TILE_INDEX, 1, &star_tiles[(star_counter + 32) % 48]);
            star_counter += 16;
        }
        vsync();
        end_cutscene_timer++;
    }
}

const level_t level_end = {
        level_final_HEIGHT >> 3,
        level_final_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_final_map,
        level_final_map_attributes,
        level_end_init,
        level_end_update,
        &level_end,
        0, 0,
        16, 288,
        BANK(level_final)
};
