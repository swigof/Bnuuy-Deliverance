The entity handling system was initially meant to be able to swap metasprites and their tile data on the fly based on
the entity state.

Unfortunately, loading a small number of tiles into VRAM was slower than anticipated, and caused synchronization
problems with the entity state, tile data, and metasprites between screen rendering passes.

There is certainly a way to fix this, but I had no time to consider solutions and ultimately did not need the 
functionality.

What remains from before is on this branch.
