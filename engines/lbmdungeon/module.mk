MODULE := engines/lbmdungeon

MODULE_OBJS := \
	detection.o \
	metaengine.o

# When we start modifying EoB engine files, add them here and remove
# the corresponding files from the kyra ENABLE_EOB build.
#
# EOB_OBJS := \
#	chargen.o \
#	eobcommon.o \
#	eob.o \
#	darkmoon.o \
#	items_eob.o \
#	magic_eob.o \
#	scene_eob.o \
#	sprites_eob.o \
#	timer_eob.o \
#	kyra_rpg.o \
#	scene_rpg.o \
#	sprites_rpg.o \
#	timer_rpg.o \
#	chargen.o \
#	gui_eob.o \
#	gui_eob_segacd.o \
#	gui_rpg.o \
#	saveload_eob.o \
#	saveload_rpg.o \
#	resource_segacd.o \
#	staticres_eob.o \
#	staticres_rpg.o \
#	script_eob.o \
#	seqplayer_eob_segacd.o \
#	sequences_eob.o \
#	sequences_darkmoon.o \
#	screen_eob.o \
#	screen_eob_amiga.o \
#	screen_eob_pc98.o \
#	screen_eob_segacd.o \
#	screen_eob_towns.o \
#	sound_amiga_eob.o \
#	sound_pc98_eob.o \
#	sound_pc98_darkmoon.o \
#	sound_segacd_eob.o \
#	sound_towns_darkmoon.o \
#	text_rpg.o \
#	audiomaster2.o \
#	mlalf98.o \
#	capcom98.o \
#	pcspeaker_v1.o \
#	segacd.o

# This module can be built as a plugin
ifeq ($(ENABLE_LBMDUNGEON), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
