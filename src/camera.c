#include "camera.h"

#pragma bank 1

uint16_t camera_y;

uint16_t old_camera_y;
uint8_t map_pos_y, old_map_pos_y;
uint8_t *bank_map, *bank_attributes;

void init_camera(const uint16_t y) NONBANKED{
    camera_y = y;
    if (camera_y < 8) { // camera needs to be placed atleast one tile down to allow for map buffer
        camera_y = 8;
    } else if (camera_y > current_level->max_camera_y) {
        camera_y = current_level->max_camera_y;
    }
    old_camera_y = camera_y;

    old_map_pos_y = -1;
    map_pos_y = camera_y >> 3;
    move_bkg(0, camera_y);
    bank_map = current_level->map;
    bank_attributes = current_level->attributes;
    SWITCH_ROM(current_level->bank);
    set_bkg_submap(
        0,
        map_pos_y - 1, // to avoid tearing along with the +1 height, also see call in update_camera()
        DEVICE_SCREEN_WIDTH,
        DEVICE_SCREEN_HEIGHT + 1,
        bank_map,
        MAP_WIDTH);
    set_bkg_submap_attributes(
        0,
        map_pos_y - 1,
        DEVICE_SCREEN_WIDTH,
        DEVICE_SCREEN_HEIGHT + 1,
        bank_attributes,
        MAP_WIDTH);
    SWITCH_ROM(1);
}

void update_camera() NONBANKED{
    if (camera_y == old_camera_y)
        return;
    move_bkg(0, camera_y);
    map_pos_y = camera_y >> 3;
    if (map_pos_y != old_map_pos_y) {
        if (camera_y < old_camera_y && map_pos_y > 0) { // prevent top line from drawing twice
            bank_map = current_level->map;
            bank_attributes = current_level->attributes;
            SWITCH_ROM(current_level->bank);
            set_bkg_submap(
                0,
                map_pos_y - 1,
                DEVICE_SCREEN_WIDTH,
                1,
                bank_map,
                MAP_WIDTH);
            set_bkg_submap_attributes(
                0,
                map_pos_y - 1,
                DEVICE_SCREEN_WIDTH,
                1,
                bank_attributes,
                MAP_WIDTH);
            SWITCH_ROM(1);
        } else {
            if ((current_level->map_height - DEVICE_SCREEN_HEIGHT) > map_pos_y) {
                bank_map = current_level->map;
                bank_attributes = current_level->attributes;
                SWITCH_ROM(current_level->bank);
                set_bkg_submap(
                    0,
                    map_pos_y + DEVICE_SCREEN_HEIGHT,
                    DEVICE_SCREEN_WIDTH,
                    1,
                    bank_map,
                    MAP_WIDTH);
                set_bkg_submap_attributes(
                    0,
                    map_pos_y + DEVICE_SCREEN_HEIGHT,
                    DEVICE_SCREEN_WIDTH,
                    1,
                    bank_attributes,
                    MAP_WIDTH);
                SWITCH_ROM(1);
            }
        }
        old_map_pos_y = map_pos_y;
    }
    old_camera_y = camera_y;
}

void set_focus(const uint16_t y) {
    if(y >= (DEVICE_SCREEN_PX_HEIGHT >> 1)) {
        camera_y = y - (DEVICE_SCREEN_PX_HEIGHT >> 1);
        if(camera_y > current_level->max_camera_y)
            camera_y = current_level->max_camera_y;
    } else
        camera_y = 0;
}
