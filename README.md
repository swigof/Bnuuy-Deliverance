# eigg

## Builds

The project can be built with Make using the top level Makefile.

This project uses GBDK and hUGEDriver. 
By default, they are expected to be in the directory above the project.
Alternatively, you can set `GBDK_HOME` and/or `HUGE_HOME`  in your environment to paths elsewhere.

## Resources

The project needs certain resources placed in a top level `res` folder

* `res/sprites/player_sheet.png` spritesheet of the player entity
* `res/sprites/truck_sheet.png` spritesheet of the truck entity
* `res/sprites/box_sheet.png` spritesheet of the box entity
* `res/tiles/tileset_primary.png` tilesheet for background tiles
* `res/main_track.c` hUGETracker exported song
* `res/levels/level_{x}.png` level images built respecting tileset_primary

## Notes on engine functionality

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
* Each entity should have a source definition (`entity_t`) and spritesheet
  * The source definition details its various states and the metasprites to use in each state
  * The spritesheet should have its animations placed in sequence in left to right, top to bottom order
  * Sprite data for all an entity's metasprites should be fully set before being made active
    * This would usually be done on level load
* Entities update mostly through their own pre-defined functions
  * There are generic functions that can be used for velocity movement and collision
    * Acceleration / velocity changes need to be handled by the entity's update function however
* New entities are added via the `add_entity` function. 
  * There is a dedicated array in RAM for entities which normally allows a maximum of 10 at a time
  * Entities which have the active flag unset may be removed when adding a new entity
* The box when carried by the player has its movement handled by the player update function
  * Collision checks aren't made for the box in this case
* Similarly to entities, levels should have a map image respecting the primary tileset and a `level_t`
* Levels have an init function which should initialize all entities and the camera as needed
* Levels have an update function pointer which, if set, allows them to override normal main loop update behavior
  * This can be used for cutscenes

## TODOS

* add levels
* fix player sprite colors and level colors
* background decorations
* simple closing cinematic
* use advancing pointer for loops instead of index
* camera pan upon level load
    * extremely long level gag with elevator at bottom
* add simple sfx
    * adjust music start to feel more natural
* signs / text popups
* adjust intro cinematic pacing
* tile hazards / hurt
    * dropping on hit
* emotes / more animations
* timer?
* crouch state
* custom track or arrangement
* fix top spawning preventing first tile row load 
* influence throw on up hold
* screenshake with scanline interrupts and scroll shifting
* possibly adjust camera framing to make look ahead comfortable
* auto saving per screen
* adjust offscreen checks to prevent disappearance
* fix box collision while held / adjust player collision accordingly
* moving hazards / enemies & projectiles
* momentum driven air movement
* add collision/grounding flag to basic movement routine
* make conversion operation #defines / inlines
* add horizontal scrolling
* carrying balance mechanics?
* camera safety measure for if old vs new position is more than 1 tile difference
* menu
* do a pointer access vs direct access performance test. function parameter overhead.
  * can just check instruction conversion
  * can use inlines that set a global then make call instead of parameters
* stop scrolls earlier to allow map space to be traversed offscreen
* find a way to render differently sized metasprites so their x,y always corresponds to their center
  * one 8x16 sprite: 0,0 <-> 4,8
  * two horizontal:  0,0 <-> 8,8
  * ...
* migrate entities linked lists of active vs inactive instead of flag to avoid wasted cycles on loops
* consolidate entity flags further into state to save ram if needed
  * alternatively, expand flags into variables if ram is fine

## References

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf
