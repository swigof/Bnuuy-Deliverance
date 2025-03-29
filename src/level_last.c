#include "level_last.h"
#include "player.h"
#include "../obj/level_long.h"
#include "../obj/level_elevator.h"

#pragma bank 1

uint16_t level_pan_y = 0;
void level_long_init() {
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

uint8_t elevator_timer = 0;
uint8_t offset = 0;
void scanline_isr(void) { SCX_REG = offset; }
void level_elevator_update() {
    elevator_timer = 0;
    while(elevator_timer <= 30) {
        vsync();
        elevator_timer++;
    }

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

    elevator_timer = 0;
    while(elevator_timer <= 30) {
        vsync();
        elevator_timer++;
    }

    level_transition();
}

const level_t level_elevator = {
        level_elevator_HEIGHT >> 3,
        level_elevator_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_elevator_map,
        level_elevator_map_attributes,
        standard_init,
        level_elevator_update,
        &level_end,
        80, 88,
        80, 88,
        BANK(level_elevator)
};

const level_t level_long = {
        level_long_HEIGHT >> 3,
        level_long_HEIGHT - DEVICE_SCREEN_PX_HEIGHT,
        level_long_map,
        level_long_map_attributes,
        level_long_init,
        NULL,
        &level_elevator,
        128, 2024,
        16, 2024,
        BANK(level_long)
};
