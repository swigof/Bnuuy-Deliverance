#include <gb/gb.h>

#define	I_NORMAL    0x00U
#define I_MENU      0x01U
#define I_INVENTORY 0x02U

joypads_t joypads;
uint8_t input_mode = I_NORMAL;

inline void input_init() {
    joypad_init(1, &joypads);
}

inline void input_process() {
    if(joypads.joy0 & J_UP) {
        
    } else if(joypads.joy0 & J_DOWN) {

    }

    if(joypads.joy0 & J_LEFT) {

    } else if(joypads.joy0 & J_RIGHT) {

    }

    if(joypads.joy0 & J_A) {

    }

    if(joypads.joy0 & J_B) {

    }

    if(joypads.joy0 & J_SELECT) {

    }

    if(joypads.joy0 & J_START) {

    }
}