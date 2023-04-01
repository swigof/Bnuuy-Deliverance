#include <gb/gb.h>
#include <gb/cgb.h>
#include "tile_data/tileset_tiles.h"
#include "tile_data/tileset_map.h"

#define	I_NORMAL    0x00U
#define I_MENU      0x01U
#define I_INVENTORY 0x02U

const uint8_t sprite_data[] = { 
  0x0F,0x0F,0x30,0x30,0x40,0x40,0x40,0x40,
  0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,
  0x84,0x84,0x84,0x84,0x80,0x80,0x80,0x80,
  0x44,0x44,0x43,0x43,0x30,0x30,0x0F,0x0F,
  0xF0,0xF0,0x0C,0x0C,0x02,0x02,0x02,0x02,
  0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,
  0x21,0x21,0x21,0x21,0x01,0x01,0x01,0x01,
  0x22,0x22,0xC2,0xC2,0x0C,0x0C,0xF0,0xF0
};

joypads_t joypads;
uint8_t input_mode = I_NORMAL;
uint8_t x, y = 75;

int main() {
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_ON;
    SHOW_BKG;
    SPRITES_8x8;
    SHOW_SPRITES;

    // VBK_REG = VBK_TILES;
    // set_bkg_palette(0,1,tileset_map_colors);
    // set_bkg_data(0,tileset_tiles_count,tileset_tiles);
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map);
    // VBK_REG = VBK_ATTRIBUTES;
    // set_bkg_tiles(0,0,tileset_map_width,tileset_map_height,tileset_map_attr);
    //VBK_REG = VBK_TILES;

    set_sprite_data(0, 4, sprite_data);

    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    
    palette_color_t sprite_palettes[] = { RGB8(0, 0, 0),RGB8(255, 0, 0),RGB8(255, 0, 0),RGB8(255, 0, 0) };
    set_sprite_palette(0,1,sprite_palettes);

    set_sprite_prop(0, 0);
    set_sprite_prop(1, 0);
    set_sprite_prop(2, 0);
    set_sprite_prop(3, 0);

    move_sprite(0, x, y);
    move_sprite(1, x, y + 8);
    move_sprite(2, x + 8, y);
    move_sprite(3, x + 8, y + 8);

    joypad_init(1, &joypads);

    while(1) {

      joypad_ex(&joypads);

      if(joypads.joy0 & J_UP) {
        y--;
      } else if(joypads.joy0 & J_DOWN) {
        y++;
      }

      if(joypads.joy0 & J_LEFT) {
        x--;
      } else if(joypads.joy0 & J_RIGHT) {
        x++;
      }

      if(joypads.joy0 & J_A) {

      }

      if(joypads.joy0 & J_B) {

      }

      if(joypads.joy0 & J_SELECT) {

      }

      if(joypads.joy0 & J_START) {

      }

      sprite_palettes[1] += 1;
      sprite_palettes[2] += 1;
      sprite_palettes[3] += 1;
      if(sprite_palettes[1] > 32767) sprite_palettes[1] = 0;
      if(sprite_palettes[2] > 32767) sprite_palettes[2] = 0;
      if(sprite_palettes[3] > 32767) sprite_palettes[3] = 0;
      set_sprite_palette(0, 1, sprite_palettes);

      move_sprite(0, x, y);
      move_sprite(1, x, y + 8);
      move_sprite(2, x + 8, y);
      move_sprite(3, x + 8, y + 8);

      wait_vbl_done();
    }

    return 0;
}