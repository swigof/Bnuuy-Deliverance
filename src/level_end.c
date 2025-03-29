#include "level_end.h"
#include "player.h"
#include "angel.h"
#include <rand.h>
#include "../obj/level_final.h"
#include "../obj/truck_sheet.h"

#pragma bank 1

#define BOX_SHAKE_COUNT 4

uint8_t true_end_flag = FALSE;

uint8_t end_cutscene_timer = 0;
uint8_t end_cutscene_flag = FALSE;
uint8_t box_shake_counter = 0;

entity_t* angel;

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

void level_end_update() {
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

    // prevent player from carrying box, move box into position
    carry = FALSE;
    box->update_function = (void (*)(void *)) &move_box_to_position;
    end_cutscene_timer = 0;
    while(end_cutscene_timer < 120) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        joypads.joy0 &= ~J_B;
        update_entities();
        vsync();
        end_cutscene_timer++;
    }

    // shake box
    initrand(0); // use static seed to preserve same behavior
    while(box_shake_counter < BOX_SHAKE_COUNT) {
        end_cutscene_timer = rand();
        if(end_cutscene_timer < 30)
            end_cutscene_timer = 30;
        else if(end_cutscene_timer > 120)
            end_cutscene_timer = 120;
        box->x += (int8_t)rand();
        box->y += (int8_t)rand();
        while(end_cutscene_timer != 0) {
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
    while(MAP_COORD(box->y) < 305) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        joypads.joy0 &= ~J_B;
        update_entities();
        vsync();
    }
    box->active = FALSE;
    end_cutscene_timer = 0;
    while(end_cutscene_timer < 120) {
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
    while(MAP_COORD(angel->y) > 212) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        if(MAP_COORD(angel->y) > 273)
            angel->vel_y--;
        else
            angel->vel_y++;
        angel->y += angel->vel_y;
        update_entities();
        vsync();
    }
    while(MAP_COORD(angel->y) < 216) {
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
    while(end_cutscene_timer < 120) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        update_entities();
        vsync();
        end_cutscene_timer++;
    }

    //clap for the true gamer
    if(true_end_flag) {
        angel->state_data = &angel_clap;
        end_cutscene_timer = 0;
        while(end_cutscene_timer < 180) {
            prev_joypads = joypads;
            joypad_ex(&joypads);
            update_entities();
            vsync();
            end_cutscene_timer++;
        }
    }

    //fly away and pan up
    angel->state_data = &angel_tpose;
    while(camera_y > 8) {
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

const level_t level_end = {
        level_final_HEIGHT >> 3,
        level_final_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_final_map,
        level_final_map_attributes,
        standard_init,
        level_end_update,
        &level_end,
        0, 0,
        16, 288,
        BANK(level_final)
};
