# CECS 326 Lab 1: The Torn Map
#
# You should not need to change this file. `make` builds ./fetch.
#
#   make           build ./fetch
#   make example   build ./example, the worked example
#   make warmup    build ./warmup, the Phase 0 toolchain check
#   make check     build ./fetch and run the self-check
#   make clean     delete everything that was built

CC      = gcc
CFLAGS  = -Wall -Werror -pthread -g
LDFLAGS = -pthread -no-pie

# Pick the scriptoria object file that matches this machine.
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_M),x86_64)
MIRROR = lib/mirror_x86_64.o
else ifeq ($(UNAME_S)-$(UNAME_M),Linux-aarch64)
MIRROR = lib/mirror_arm64_linux.o
else ifeq ($(UNAME_S)-$(UNAME_M),Darwin-arm64)
MIRROR = lib/mirror_arm64_macos.o
else
$(error No scriptoria build for $(UNAME_S)/$(UNAME_M). Use the Codespace -- see the README.)
endif

.PHONY: all check clean
all: fetch

fetch: fetch.c mirror.h $(MIRROR)
	$(CC) $(CFLAGS) fetch.c $(MIRROR) -o $@ $(LDFLAGS)

example: example_serial.c mirror.h $(MIRROR)
	$(CC) $(CFLAGS) example_serial.c $(MIRROR) -o $@ $(LDFLAGS)

warmup: warmup.c
	$(CC) -Wall -Werror warmup.c -o $@

check: fetch
	./selfcheck.sh

clean:
	rm -f fetch example warmup out.map
