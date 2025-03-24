# GBDK path
ifndef GBDK_HOME
    GBDK_HOME = ../gbdk/
endif
# hUGEDriver path
ifndef HUGE_HOME
    HUGE_HOME = ../hUGEDriver/
endif

# Tools
LCC = $(GBDK_HOME)bin/lcc
PNG2ASSET = $(GBDK_HOME)bin/png2asset

# LCC flags
CFLAGS += -I$(OBJDIR)    # Add generated asset files to includes
CFLAGS += -Wm-yC         # Gameboy color mode
CFLAGS += -Wf-MMD        # Header file dependency output for Makefile use
CFLAGS += -Wf-Wp-MP      # Per-header phony rules
ifdef GBDK_DEBUG
    CFLAGS += -debug -v  # Add debug flags and command logging
endif

# Add hUGEDriver
CFLAGS += -I$(HUGE_HOME)include
CFLAGS += -Wl-l$(HUGE_HOME)gbdk/hUGEDriver.lib

PROJECTNAME = eigg

BINDIR      = bin
SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
IMAGEDIRS   = $(RESDIR)/*
MKDIRS      = $(OBJDIR) $(BINDIR) # See bottom of Makefile for directory auto-creation

BINS	    = $(BINDIR)/$(PROJECTNAME).gbc
CSOURCES    = $(foreach dir,$(SRCDIR) $(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))

# Images in res/ will be converted to source files with png2asset
IMAGES      = $(foreach dir,$(IMAGEDIRS),$(notdir $(wildcard $(dir)/*.png)))
IMAGE_SRC   = $(IMAGES:%.png=$(OBJDIR)/%.c)

# png2asset objs first for dependants
OBJS        = $(IMAGES:%.png=$(OBJDIR)/%.o) $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all: $(BINS)

# Dependencies (using output from -Wf-MMD -Wf-Wp-MP)
DEPS = $(OBJS:%.o=%.d)

-include $(DEPS)

# Convert png images in res/ to .c files
# The resulting C files will get compiled to object files afterward
.SECONDEXPANSION:
$(OBJDIR)/%.c: $(RESDIR)/tiles/%.png
	$(PNG2ASSET) $< -spr8x8 -sprite_no_optimize -no_palettes -noflip -tiles_only -o $@
$(OBJDIR)/player_sheet.c: $(RESDIR)/sprites/player_sheet.png
	$(PNG2ASSET) $< -spr8x16 -no_palettes -sw 16 -sh 32 -pw 4 -ph 8 -o $@
$(OBJDIR)/truck_sheet.c: $(RESDIR)/sprites/truck_sheet.png
	$(PNG2ASSET) $< -spr8x16 -sw 32 -sh 32 -pw 15 -ph 15 -o $@
$(OBJDIR)/box_sheet.c: $(RESDIR)/sprites/box_sheet.png
	$(PNG2ASSET) $< -spr8x16 -sw 16 -sh 16 -pw 7 -ph 7 -o $@
$(OBJDIR)/%.c: $(RESDIR)/levels/%.png
	$(PNG2ASSET) $< -sprite_no_optimize -map -use_map_attributes -maps_only \
        -source_tileset $(RESDIR)\tiles\tileset_primary.png -o $@

# Prevent make from deleting intermediary generated asset C source files
.SECONDARY: $(IMAGE_SRC)

# Compile image .c files in "obj/" to .o object files
$(OBJDIR)/%.o:	$(OBJDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c assembly files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(LCC) $(CFLAGS) -o $(BINS) $(OBJS)


clean:
	rm -f $(OBJDIR)/*.* $(BINDIR)/*.*

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
$(info $(shell mkdir -p $(MKDIRS)))
