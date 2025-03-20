# eigg

### Notes on engine functionality

* If entities move more than a tile (8 pixels) at a time, the map collision detection
and camera can fall apart.
* Maps can only scroll vertically
* Maps must have a width of 20 and a height between 19 and 511 inclusive
  * `camera_y` will always start with a value of 8 or more to buffer one line lower
* The project Makefile builds spritesheets using png2asset
* Sprites should be centered horizontally and vertically on their hitboxes
  * `-pw` and `-ph` in png2asset should be set to the distance from the center to the hitbox edge
  * Hitboxes have a size limit of 30x30 (4 bits for pw/ph)
  * Hitboxes should always remain fully inside the mapped area
* Each entity should have a source definition and spritesheet
  * The source definition details its various states and the metasprites to use in each state
  * The spritesheet should have its animations placed in sequence in left to right, top to bottom order
  * Sprite data for all an entity's metasprites should be fully set before being made active
    * This would usually be done on level load
* Entities update mostly through their own pre-defined functions
  * There are generic functions that can be used for velocity movement and collision
    * Acceleration / velocity changes need to be handled by the entity's update function however
* The box when carried by the player has its movement handled by the player update function

### TODOS

* carrying things
    * slowed movement, lower jumps
    * throwing
    * dropping on hit
    * balance?
* add simple sfx and music
    * on separate channels?
* levels
* fix screen scroll sprite move jitter
* tile hazards / hurt
* simple opening cinematic
* emotes / more animations
* add collision/grounding flag to basic movement routine
* make conversion operation #defines / inlines
* automate level/tilemap builds
* add horizontal scrolling
* camera safety measure for if old vs new position is more than 1 tile difference
* possibly adjust camera framing to make look ahead comfortable
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
* consolidate entity flags further into state to save ram if needed
  * alternatively, expand flags into variables if ram is fine

### Resources

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf
