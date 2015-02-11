CC = gcc
CFLAGS = -O3 -Wall -Werror -Wl,-s,-flto
TARGET = stripper
DEBUG = stripper_d

FILES = \
	main.c \
	decomp.c \
	decode.c \
	misc.c \
	bitplane.c \
	spec1.c \
	spec2.c \
	spec3.c \
	level1.c \
	level2.c \
	level3.c \
	raw_bit2.c \
	raw_bit3.c \
	anim2.c \
	anim3.c \
    dkc_gbc.c \
    dkc_gba.c \
    gba_misc.c \
    dkc2_decomp.c \
    dkc2_gba.c \
    dkl.c \
	lodepng.c

OBJECTS = $(FILES:.c=.o)

all: $(TARGET)

debug: CFLAGS = -Wall -Wextra -g
debug: $(DEBUG)

$(TARGET) $(DEBUG): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(DEBUG)
