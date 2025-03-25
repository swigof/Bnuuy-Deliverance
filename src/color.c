#include "color.h"

const uint16_t black = RGB_BLACK;
const uint16_t white = RGB_WHITE;

uint8_t b,g,r;

void fade_to_color(uint16_t* color, uint16_t* target) {
    if(*color == *target)
        return;
    b = GET_BLUE(*color);
    g = GET_GREEN(*color);
    r = GET_RED(*color);

    if(b > GET_BLUE(*target))
        b--;
    else if(b < GET_BLUE(*target))
        b++;

    if(g > GET_GREEN(*target))
        g--;
    else if(g < GET_GREEN(*target))
        g++;

    if(r > GET_RED(*target))
        r--;
    else if(r < GET_RED(*target))
        r++;

    *color = RGB(r,g,b);
}
