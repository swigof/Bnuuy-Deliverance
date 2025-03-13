#include "camera.h"
#include "tileset_map.h"

uint16_t camera_y;
uint16_t old_camera_y;
uint8_t map_pos_y, old_map_pos_y;

void init_camera(const uint16_t y) {
    camera_y = y;
    if (camera_y < 8) { // camera needs to be placed atleast one tile down to allow for map buffer
        camera_y = 8;
    } else if (camera_y > MAX_CAMERA_Y) {
        camera_y = MAX_CAMERA_Y;
    }
    old_camera_y = camera_y;

    old_map_pos_y = 255;
    map_pos_y = camera_y >> 3;
    move_bkg(0, camera_y);
    set_bkg_submap(
        0,
        map_pos_y - 1, // to avoid tearing along with the +1 height, also see call in update_camera()
        DEVICE_SCREEN_WIDTH,
        DEVICE_SCREEN_HEIGHT + 1,
        tileset_map,
        DEVICE_SCREEN_WIDTH);
    set_bkg_submap_attributes(
        0,
        map_pos_y - 1,
        DEVICE_SCREEN_WIDTH,
        DEVICE_SCREEN_HEIGHT + 1,
        tileset_map_attr,
        DEVICE_SCREEN_WIDTH);
}

void update_camera() {
    if (camera_y == old_camera_y)
        return;
    move_bkg(0, camera_y);
    map_pos_y = camera_y >> 3;
    if (map_pos_y != old_map_pos_y) {
        if (camera_y < old_camera_y && map_pos_y > 0) { // prevent top line from drawing twice
            set_bkg_submap(
                0,
                map_pos_y - 1,
                DEVICE_SCREEN_WIDTH,
                1,
                tileset_map,
                DEVICE_SCREEN_WIDTH);
            set_bkg_submap_attributes(
                0,
                map_pos_y - 1,
                DEVICE_SCREEN_WIDTH,
                1,
                tileset_map_attr,
                DEVICE_SCREEN_WIDTH);
        } else {
            if ((tileset_map_height - DEVICE_SCREEN_HEIGHT) > map_pos_y) {
                set_bkg_submap(
                    0,
                    map_pos_y + DEVICE_SCREEN_HEIGHT,
                    DEVICE_SCREEN_WIDTH,
                    1,
                    tileset_map,
                    DEVICE_SCREEN_WIDTH);
                set_bkg_submap_attributes(
                    0,
                    map_pos_y + DEVICE_SCREEN_HEIGHT,
                    DEVICE_SCREEN_WIDTH,
                    1,
                    tileset_map_attr,
                    DEVICE_SCREEN_WIDTH);
            }
        }
        old_map_pos_y = map_pos_y;
    }
    old_camera_y = camera_y;
}

void set_focus(const uint16_t y) {
    if(y >= (DEVICE_SCREEN_PX_HEIGHT >> 1)) {
        camera_y = y - (DEVICE_SCREEN_PX_HEIGHT >> 1);
        if(camera_y > MAX_CAMERA_Y)
            camera_y = MAX_CAMERA_Y;
    } else
        camera_y = 0;
}

uint16_t get_camera_y() {
    return camera_y;
}
