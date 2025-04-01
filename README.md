# Bnuuy Deliverance

## Builds

The project can be built with Make using the top level Makefile.

This project uses GBDK and hUGEDriver. 
By default, they are expected to be in the directory above the project.
Alternatively, you can set `GBDK_HOME` and/or `HUGE_HOME`  in your environment to paths elsewhere.

## Assets

The project needs certain assets placed in a top level `res` folder

* `res/sprites/{x}_sheet.png` spritesheets of various entities
* `res/tiles/tileset_primary.png` tilesheet for background tiles
* `res/levels/level_{x}.png` level images built respecting tileset_primary
* `res/music/{x}_track.c` hUGETracker exported songs
* `res/sfx/SFX_{x}.[c/h]` CBT-FX exported sound effects

Care should be taken to avoid having files which match the names of those generated by the build from the resources.

## Notes on engine functionality

* If entities move more than a tile (8 pixels) at a time, the map collision detection
and camera can fall apart.
* Maps can only scroll vertically
* Maps must have a width of 20 and a height between 19 and 255 inclusive
  * `camera_y` will always start with a value of 8 or more to buffer one line lower
* The project Makefile builds spritesheets using png2asset
* Sprites should be centered horizontally and vertically on their hitboxes
  * `-pw` and `-ph` in png2asset should be set to the distance from the center to the hitbox edge
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
    * This unfortunately makes it very easy to clip through single tile thick ceilings / platforms
* Similarly to entities, levels should have a map image respecting the primary tileset and a `level_t`
* Levels have an init function which should initialize all entities and the camera as needed
* Levels have an update function pointer which, if set, allows them to override normal main loop update behavior
  * This can be used for cutscenes

## Banking

Banks 0 and 1 are used largely by default, with bank 1 being briefly switched from to access data in other banks as 
needed. 
Bank 1 is leveraged as much as possible for principle code and data to avoid filling the valuable bank 0 space.

Layout:
* ROM 0: Specifically NONBANKED code which needs to switch banks, music, and sfx
* ROM 1: All other code/data, and spritesheets
* ROM 2: Levels, and map tilesets
* ROM 3: Levels

## Resources

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf

https://laroldsretrogameyard.com/
