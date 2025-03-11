# eigg

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

* adjust sprite spacing and use extra room for emotes
* verify tile data memory isn't duplicated
* fix entity movement
* fix tile collision
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

### Resources

https://gbdev.io/

https://gbdk-2020.github.io/gbdk-2020/docs/api/index.html

https://sdcc.sourceforge.net/doc/sdccman.pdf
