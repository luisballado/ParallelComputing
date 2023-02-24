# Makefile for building applications.

# Edit this file to compile extra C files into their own programs.

TARGETS = sum_malloc

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOK)gcc

CFLAGS = -Wall -g -std=c99 -Werror -pthread

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGETS)
