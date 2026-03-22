MODULE := common/lispbm

ifdef USE_LISPBM
MODULE_OBJS := \
	platform/scummvm/src/platform_timestamp.o \
	src/env.o \
	src/eval_cps.o \
	src/extensions.o \
	src/extensions/lbm_dyn_lib.o \
	src/fundamental.o \
	src/heap.o \
	src/lbm_channel.o \
	src/lbm_c_interop.o \
	src/lbm_custom_type.o \
	src/lbm_defrag_mem.o \
	src/lbm_flat_value.o \
	src/lbm_image.o \
	src/lbm_memory.o \
	src/lbm_prof.o \
	src/lispbm.o \
	src/print.o \
	src/stack.o \
	src/symrepr.o \
	src/tokpar.o
endif

# Include common rules
include $(srcdir)/rules.mk
