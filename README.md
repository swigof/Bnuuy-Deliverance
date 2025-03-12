# eigg

### Constraints

* If entities move more than a tile (8 pixels) at a time, the map collision detection
and camera can fall apart.
* Maps can only scroll vertically
* Maps must have a width of 20 and height of atleast 19
* camera_y should always start with values greater than 8 as it assumes it can buffer one line 
lower (currently this defaults to the bottom of a map anyway)

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


### TODOS

* fix tile collision
  * determine method to retrieve map data using big maps
  * decide on method of storing entity coords to facilitate map tile coordinate conversion
* adjust sprite spacing and use extra room for emotes
* add entity state / animation
* store static entity(-state) data in rom arrays (heights, paddings, etc)
* make conversion operation #defines / inlines
* make move directions -1,0,1 for x and y
* add horizontal scrolling
* verify tile data memory isn't duplicated
* fix entity movement
* add simple sfx and music
  * on separate channels?
* levels
* automate level/tilemap builds
* simple opening cinematic
* tile hazards
* moving hazards / enemies & projectiles
* carrying things
  * slowed movement, lower jumps 
  * throwing
  * dropping on hit
  * balance
* menu
* auto saving per screen
* replace tile 0 with most common background tile

### Resources

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf
