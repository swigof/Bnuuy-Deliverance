# GBDK path
ifndef GBDK_HOME
    GBDK_HOME = gbdk/
endif

# Tools
LCC = $(GBDK_HOME)bin/lcc
PNG2ASSET = $(GBDK_HOME)bin/png2asset

# LCC flags
LCCFLAGS += -I$(OBJDIR)    # Add generated asset files to includes
LCCFLAGS += -Wm-yC         # Gameboy color mode
LCCFLAGS += -Wf-MMD        # Header file dependency output for Makefile use
LCCFLAGS += -Wf-Wp-MP      # Per-header phony rules
ifdef GBDK_DEBUG
    LCCFLAGS += -debug -v  # Add debug flags and command logging
endif

PROJECTNAME = eigg

BINDIR      = bin
SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
MKDIRS      = $(OBJDIR) $(BINDIR) # See bottom of Makefile for directory auto-creation

BINS	    = $(BINDIR)/$(PROJECTNAME).gbc
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))

# Images in res/ will be converted to source files with png2asset
IMAGES      = $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.png)))
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
$(OBJDIR)/%.c: $(RESDIR)/%.png
	$(PNG2ASSET) $< -spr8x8 -no_palettes -noflip -tiles_only -o $@

# Prevent make from deleting intermediary generated asset C source files
.SECONDARY: $(IMAGE_SRC)

# Compile image .c files in "obj/" to .o object files
$(OBJDIR)/%.o:	$(OBJDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)


clean:
	rm -f $(OBJDIR)/*.* $(BINDIR)/*.*

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
$(info $(shell mkdir -p $(MKDIRS)))
