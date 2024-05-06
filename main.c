#include <gb/gb.h>
#include <gb/cgb.h>
#include "helpers/entity.h"
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"

#define	I_NORMAL    0x00U
#define I_MENU      0x01U
#define I_INVENTORY 0x02U

joypads_t joypads;
uint8_t input_mode = I_NORMAL;

entity_t ship;
uint8_t flip_counter = 0;
uint8_t firing_charge = 0;
uint8_t next_shot_speed = 2;

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_ON;
    SHOW_BKG;
    SPRITES_8x8;
    SHOW_SPRITES;

    VBK_REG = VBK_TILES;
    set_bkg_palette(0,1,tileset_map_colors);
    set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map);
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);
    VBK_REG = VBK_TILES;

    set_sprite_data(0, tileset_tiles_count, tileset_tiles);

    set_sprite_tile(0, 61);
    
    palette_color_t sprite_palettes[] = { RGB8(255, 0, 0),RGB8(0, 255, 0),RGB8(0, 0, 255),RGB8(0, 0, 0) };
    set_sprite_palette(0, 1, sprite_palettes);

    set_sprite_prop(0, 0);

    move_sprite(0, ship.x, ship.y);

    joypad_init(1, &joypads);

    while(1) {

      joypad_ex(&joypads);

      if(joypads.joy0 & J_UP) {
        ship.y--;
        next_shot_speed = 3;
      } else if(joypads.joy0 & J_DOWN) {
        ship.y++;
        next_shot_speed = 1;
      }

      if(joypads.joy0 & J_LEFT) {
        ship.x--;
      } else if(joypads.joy0 & J_RIGHT) {
        ship.x++;
      }

      if(joypads.joy0 & J_A) {
        // Render sprite / palette change and sound for charging
        firing_charge++;
      } else if (firing_charge != 0) {
        if(firing_charge < 15) {
          // Failure
        }
        if(firing_charge == 15) {
          // Special shot (with aoe)?
        } else  {
          // Normal shot
        }
        next_shot_speed = 2;
        firing_charge = 0;
      }

      if(joypads.joy0 & J_B) {
        // lock to change face / movement?

        // sprite_palettes[1] -= 1;
        // sprite_palettes[2] -= 1;
        // sprite_palettes[3] -= 1;
        // if(sprite_palettes[1] < 0) sprite_palettes[1] = 32767;
        // if(sprite_palettes[2] < 0) sprite_palettes[2] = 32767;
        // if(sprite_palettes[3] < 0) sprite_palettes[3] = 32767;
        // set_sprite_palette(0, 1, sprite_palettes);
      }

      if(joypads.joy0 & J_SELECT) {

      }

      if(joypads.joy0 & J_START) {

      }

      flip_counter++;
      if(flip_counter == 3) {
        ship.prop ^= S_FLIPX;
        flip_counter = 0;
      }

      update_entity(ship);

      // scroll_sprite() ?

      // fix blurring on movement?

      wait_vbl_done();
    }

    return 0;
}