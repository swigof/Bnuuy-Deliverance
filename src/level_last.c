#include "level_last.h"
#include "level_end.h"
#include "player.h"
#include "../obj/level_long.h"
#include "../obj/level_elevator.h"

#pragma bank 1

uint16_t level_pan_y = 0;
void level_long_init(void) {
    DISPLAY_OFF;

    set_bkg_data(DOOR_TILE_INDEX, 4, door_tiles);

    player->x = current_level->start_x << 4;
    player->y = current_level->start_y << 4;
    populate_hitbox_record(player);

    box->x = player->x;
    box->y = (player->hitbox.top - 3) << 4;
    populate_hitbox_record(box);

    init_camera(level_pan_y);

    hide_sprites_range(0,MAX_HARDWARE_SPRITES);

    DISPLAY_ON;

    while(level_pan_y < MAP_COORD(player->y)) {
        level_pan_y += 2;
        set_focus(level_pan_y);
        vsync();
        update_camera();
    }

    set_bkg_data(DOOR_TILE_INDEX, 4, empty_tiles);
    set_focus(MAP_COORD(player->y));

    remove_door = FALSE;
}

void level_long_update(void) {
    while(current_level == &level_long) {
        prev_joypads = joypads;
        joypad_ex(&joypads);
        set_focus(MAP_COORD(player->y));
        update_entities();
        vsync();
        update_camera();
        if (carry && (player->state & GROUNDED) && (joypads.joy0 & J_UP)) {
            if (is_at_coord(&player->hitbox, 72, 56)) {
                remove_door = TRUE;
                true_end_flag = TRUE;
                level_transition_alt_coord(&level_end, 72, 56);
            }
        }
    }
}

const uint8_t elevator_closed[] = { 0x5B, 0x5C, 0x5F, 0x5F, 0x5D, 0x5E };
const uint8_t elevator_open[] = { 0x59, 0x4A, 0x4A, 0x4A, 0x5A, 0x7E };
void level_elevator_init(void) {
    // swap to elevator track
    hUGE_mute_channel(HT_CH1,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH2,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH3,HT_CH_MUTE);
    hUGE_mute_channel(HT_CH4,HT_CH_MUTE);
    hUGE_init(&elevator_track);
    standard_init();
}

uint8_t elevator_timer = 0;
uint8_t offset = 0;
void scanline_isr(void) { SCX_REG = offset; }
void level_elevator_update(void) {
    joypads.joy0 = J_LEFT | J_B;
    while(MAP_COORD(player->x) > 80) {
        update_entities();
        vsync();
    }

    set_bkg_tiles(12, 7, 1, 6, elevator_closed);

    CRITICAL {
        STAT_REG = STATF_MODE00;
        add_LCD(scanline_isr);
        add_LCD(nowait_int_handler);
    }
    set_interrupts(IE_REG | LCD_IFLAG);
    elevator_timer = 0;
    while(elevator_timer <= 254) {
        offset = (elevator_timer & 0b00000100) >> 2;
        sprite_index = 0;
        sprite_index += move_metasprite_ex(
                box->state_data->metasprite[box->animation_frame],
                box->base_tile,
                box->prop,
                sprite_index,
                MAP_COORD(box->x) + DEVICE_SPRITE_PX_OFFSET_X + offset,
                MAP_COORD(box->y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);
        sprite_index += move_metasprite_ex(
                *player_idle[1].metasprite,
                player->base_tile,
                player->prop,
                sprite_index,
                MAP_COORD(player->x) + DEVICE_SPRITE_PX_OFFSET_X + offset,
                MAP_COORD(player->y) - camera_y + DEVICE_SPRITE_PX_OFFSET_Y);
        hide_sprites_range(sprite_index,MAX_HARDWARE_SPRITES);
        vsync();
        elevator_timer++;
    }
    CRITICAL {
        remove_LCD(nowait_int_handler);
        remove_LCD(scanline_isr);
        STAT_REG = 0;
    }

    set_bkg_tiles(12, 7, 1, 6, elevator_open);

    joypads.joy0 = J_RIGHT | J_B;
    while(MAP_COORD(player->x) < 104) {
        update_entities();
        vsync();
    }

    level_transition(current_level->next_level);
}

const level_t level_elevator = {
        level_elevator_HEIGHT >> 3,
        level_elevator_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_elevator_map,
        level_elevator_map_attributes,
        level_elevator_init,
        level_elevator_update,
        &level_end,
        104, 88,
        104, 88,
        BANK(level_elevator)
};

const level_t level_long = {
        level_long_HEIGHT >> 3,
        level_long_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_long_map,
        level_long_map_attributes,
        level_long_init,
        level_long_update,
        &level_elevator,
        128, 2024,
        16, 2024,
        BANK(level_long)
};
