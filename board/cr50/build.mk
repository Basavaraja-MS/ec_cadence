# -*- makefile -*-
# Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Board-specific build requirements

# Define the SoC used by this board
CHIP:=g
CHIP_FAMILY:=cr50
CHIP_VARIANT ?= cr50_fpga

# This file is included twice by the Makefile, once to determine the CHIP info
# and then again after defining all the CONFIG_ and HAS_TASK variables. We use
# a guard so that recipe definitions and variable extensions only happen the
# second time.
ifeq ($(BOARD_MK_INCLUDED_ONCE),)
BOARD_MK_INCLUDED_ONCE=1
else

# Need to generate a .hex file
all: hex

# The simulator components have their own subdirectory
CFLAGS += -I$(realpath chip/$(CHIP)/dcrypto)
CFLAGS += -I$(realpath $(BDIR)/tpm2)
dirs-y += chip/$(CHIP)/dcrypto
dirs-y += $(BDIR)/tpm2

# Objects that we need to build
board-y =  board.o
board-y += tpm2/NVMem.o
board-y += tpm2/aes.o
board-y += tpm2/hash.o
board-y += tpm2/hash_data.o
board-y += tpm2/platform.o
board-y += tpm2/stubs.o

# Build and link with an external library
EXTLIB := $(realpath ../../third_party/tpm2)
CFLAGS += -I$(EXTLIB)
LDFLAGS_EXTRA += -L$(out)/tpm2 -ltpm2

# For the benefit of the tpm2 library.
INCLUDE_ROOT := $(abspath ./include)
CFLAGS += -I$(INCLUDE_ROOT)
# Make sure the context of the software sha256 implementation fits. If it ever
# increases, a compile time assert will fire in tpm2/hash.c.
CFLAGS += -DUSER_MIN_HASH_STATE_SIZE=210

# Add dependencies on that library
$(out)/RO/ec.RO.elf: $(out)/tpm2/libtpm2.a
$(out)/RW/ec.RW.elf: $(out)/tpm2/libtpm2.a
$(out)/RW/ec.RW_B.elf: $(out)/tpm2/libtpm2.a

# Force the external build each time, so it can look for changed sources.
.PHONY: $(out)/tpm2/libtpm2.a
$(out)/tpm2/libtpm2.a:
	$(MAKE) obj=$(realpath $(out))/tpm2 EMBEDDED_MODE=1 -C $(EXTLIB)

endif   # BOARD_MK_INCLUDED_ONCE is nonempty
