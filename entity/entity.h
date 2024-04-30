#include <stdint.h>

typedef struct {
    uint8_t x, y, prop;
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;