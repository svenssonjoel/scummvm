MODULE := engines/lbmdungeon

MODULE_OBJS := \
	detection.o \
	metaengine.o \
	lbmdungeon_lispbm.o \
	audiomaster2.o \
	capcom98.o \
	chargen.o \
	darkmoon.o \
	eob.o \
	eobcommon.o \
	gui_eob.o \
	gui_eob_segacd.o \
	gui_rpg.o \
	items_eob.o \
	kyra_rpg.o \
	magic_eob.o \
	mlalf98.o \
	pcspeaker_v1.o \
	resource_segacd.o \
	saveload_eob.o \
	saveload_rpg.o \
	scene_eob.o \
	scene_rpg.o \
	screen_eob.o \
	screen_eob_amiga.o \
	screen_eob_pc98.o \
	screen_eob_segacd.o \
	screen_eob_towns.o \
	script_eob.o \
	segacd.o \
	seqplayer_eob_segacd.o \
	sequences_darkmoon.o \
	sequences_eob.o \
	sound_amiga_eob.o \
	sound_pc98_darkmoon.o \
	sound_pc98_eob.o \
	sound_segacd_eob.o \
	sound_towns_darkmoon.o \
	sprites_eob.o \
	sprites_rpg.o \
	staticres_eob.o \
	staticres_rpg.o \
	text_eob_segacd.o \
	text_rpg.o \
	timer_eob.o \
	timer_rpg.o

# Define ENABLE_EOB so kyra's resource.h exposes EoB enum constants
# (lbmdungeon owns all EoB code; kyra does not compile EoB files)
# LBM_SINGLE_THREADED enables lbm_eval_step() for cooperative stepping
CPPFLAGS += -DENABLE_EOB -DLBM_SINGLE_THREADED -DLBM64

# This module can be built as a plugin
ifeq ($(ENABLE_LBMDUNGEON), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
