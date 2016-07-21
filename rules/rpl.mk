LIBPATHS  := -L$(WUT_ROOT)/lib -L$(DEVKITPPC)/lib -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2
CFLAGS    := -I$(WUT_ROOT)/include -fno-builtin -ffreestanding -mno-sdata
CXXFLAGS  := $(CFLAGS)
LDFLAGS   := -nostartfiles -T $(WUT_ROOT)/rules/rpl.ld -pie -fPIE -z common-page-size=64 -z max-page-size=64 -lcrt
ELF2RPL   := $(WUT_ROOT)/bin/elf2rpl

include $(WUT_ROOT)/rules/base.mk

%.rpx: %.elf
	@echo "[RPX] $(notdir $@)"
	@$(ELF2RPL) $(BUILDDIR)/$(notdir $<) $@
