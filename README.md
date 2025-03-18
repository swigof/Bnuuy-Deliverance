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

### Structure Notes

Each screen will define tiles to load for entities.

Each player or entity who only appears once (or those which share a common state) will have a 
reserved portion of tile memory which adapts to its current state for a given screen.

Other entities will require having their entire tileset in memory for a given screen.

Current state and previous state will be tracked and checked per cycle to see if a tileset 
switch needs to occur and, if one does, which metasprite data to switch too. 

Ex:
* The players tiledata from 00-3F will be loaded with an idle animation by default
* When the player enters the moving state, tiles for the running animation will be placed in 
those addresses

Variables that would normally be locals to a function are defined as globals next to the function.

### TODOS

* add entity state / animation
  * fix entity render function to support more than just player idle metasprite
  * emotes
* tile hazards
* de-duplicate and streamline code in entity movement / collision detection
  * make movement / hitbox edge iteration based on signed inputs and start/finish coords
    * get_edge_tile_type can take a fixed coordinate and two bounds for any edge check
  * only calc collision if tile changes? prev vs new hitbox coords/tiles? just store old in entity, new in global?
* put get_edge_tile_type into tilespace to optimize it? would still be 16bit.
* levels
* carrying things
    * slowed movement, lower jumps
    * throwing
    * dropping on hit
    * balance
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
