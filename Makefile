CFLAGS += -std=c18 -Wall -Wextra -Wpedantic -Werror=vla -O3

all: mkw-bsp-parser

mkw-bsp-parser: main.c
	$(CC) $(CFLAGS) $^ -o $@
