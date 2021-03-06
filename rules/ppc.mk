LIBPATHS  := -L$(WUT_ROOT)/lib
CFLAGS    := -I$(WUT_ROOT)/include -fno-builtin -ffreestanding  -std=c11
CXXFLAGS  := $(CFLAGS)
LDFLAGS   := -nostdlib -nostartfiles

include $(WUT_ROOT)/rules/base.mk

%.rpx: %.elf
	@$(STRIP) $< -o $(BUILDDIR)/$(notdir $<)
