#include <stdint.h>
#include <gb/gb.h>

typedef struct {
    uint8_t x, y, prop;
    uint8_t flags; // render, tile/sprite, ? ...
} entity_t;

inline void update_entity(entity_t entity) {
    set_sprite_prop(0, entity.prop);
    move_sprite(0, entity.x, entity.y);
}
