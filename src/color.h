#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <gb/cgb.h>

#define INCREMENT_COLOR(c) fade_to_color(c, &white)
#define DECREMENT_COLOR(c) fade_to_color(c, &black)
#define GET_BLUE(c) (c >> 10)
#define GET_GREEN(c) ((c & 0b1111111111) >> 5)
#define GET_RED(c) (c & 0b11111)

extern const uint16_t black;
extern const uint16_t white;

void fade_to_color(uint16_t* color, uint16_t* target);

#endif //COLOR_H
