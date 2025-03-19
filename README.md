# eigg

### Constraints

* If entities move more than a tile (8 pixels) at a time, the map collision detection
and camera can fall apart.
* Maps can only scroll vertically
* Maps must have a width of 20 and a height between 19 and 8191 inclusive
  * `camera_y` will always start with a value of 8 or more to buffer one line lower
* Sprites should be centered horizontally and vertically on their hitboxes
  * `-pw` and `-ph` in png2asset should be set to the distance from the center to the hitbox edge
  * Hitboxes have a size limit of 30x30 (4 bits for pw/ph)
  * Hitboxes should always remain fully inside the mapped area

### TODOS

* tile hazards / hurt
* emotes / more animations
* fix player move to idle state change having single frame where metasprites point at wrong tile data
* levels
* fix entity updates function to support more than just player tile location 0
* carrying things
    * slowed movement, lower jumps
    * throwing
    * dropping on hit
    * balance
* only calc collision if tile changes? prev vs new hitbox coords/tiles? just store old in entity, new in global?
* put get_edge_tile_type into tilespace to optimize it? would still be 16bit.
* make conversion operation #defines / inlines
* automate level/tilemap builds
* add horizontal scrolling
* camera safety measure for if old vs new position is more than 1 tile difference
* possibly adjust camera framing to make look ahead comfortable
* add simple sfx and music
  * on separate channels?
* simple opening cinematic
* moving hazards / enemies & projectiles
* menu
* auto saving per screen
* replace tile 0 with most common background tile
* make tile type to tile number range based tileset
* do a pointer access vs direct access performance test. function parameter overhead.
  * can just check instruction conversion
  * can use inlines that set a global then make call instead of parameters
* make entity x,y signed to better handle offscreen behavior?
* stop scrolls earlier to allow map space to be traversed offscreen
* find a way to render differently sized metasprites so their x,y always corresponds to their center
  * one 8x16 sprite: 0,0 <-> 4,8
  * two horizontal:  0,0 <-> 8,8
  * ...
* rename and used direction constants that aren't joypad ones
* migrate entities linked lists of active vs inactive instead of flag to avoid wasted cycles on loops
* fix screen scroll sprite move jitter

### Resources

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf
