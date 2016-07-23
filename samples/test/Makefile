.SUFFIXES:

ifeq ($(strip $(WUT_ROOT)),)
$(error "Please ensure WUT_ROOT is in your environment.")
endif

ifeq ($(shell uname -o),Cygwin)
ROOT := $(shell cygpath -w ${CURDIR})
WUT_ROOT := $(shell cygpath -w ${WUT_ROOT})
else
ROOT := $(CURDIR)
endif

include $(WUT_ROOT)/rules/rpl.mk

TARGET   := $(notdir $(CURDIR))
BUILD    := build
SOURCE   := src
INCLUDE  := include
DATA     := data
LIBS     := -lgcc -lcoreinit

CFLAGS   += -std=gnu11 -mno-eabi -mhard-float -ffast-math \
		    -O3 -Wall -Wextra -Wno-unused-parameter -Wno-strict-aliasing
CXXFLAGS += -std=gnu++11 -mno-eabi -mhard-float -ffast-math \
		    -O3 -Wall -Wextra -Wno-unused-parameter -Wno-strict-aliasing
LDFLAGS  += -Wl,-Map,$(notdir $@).map,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size,-wrap,_malloc_r,-wrap,_free_r,-wrap,_realloc_r,-wrap,_calloc_r,-wrap,_memalign_r,-wrap,_malloc_usable_size_r,-wrap,valloc,-wrap,_valloc_r,-wrap,_pvalloc_r,-wrap,__eabi,-wrap,_wcsrtombs_r 

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT   := $(ROOT)/$(TARGET)
export VPATH    := $(foreach dir,$(SOURCE),$(ROOT)/$(dir)) \
                   $(foreach dir,$(DATA),$(ROOT)/$(dir))
export BUILDDIR := $(ROOT)
export DEPSDIR  := $(BUILDDIR)

CFILES    := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CXXFILES  := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES    := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.S)))

ifeq ($(strip $(CXXFILES)),)
export LD := $(CC)
else
export LD := $(CXX)
endif

export OFILES := $(CFILES:.c=.o) \
                 $(CXXFILES:.cpp=.o) \
                 $(SFILES:.S=.o)
export INCLUDES := $(foreach dir,$(INCLUDE),-I$(ROOT)/$(dir)) \
                   -I$(ROOT)/$(BUILD)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(ROOT)/Makefile

clean:
	@echo "[RM]  $(notdir $(OUTPUT))"
	@rm -rf $(BUILD) $(OUTPUT).elf $(OUTPUT).rpx $(OUTPUT).a

else

DEPENDS	:= $(OFILES:.o=.d)

$(OUTPUT).rpx: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif
