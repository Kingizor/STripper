CC = gcc
CFLAGS = -O3 -Wall -Werror -Wl,-s,-flto
DFLAGS = -O0 -g -Wall -Werror
TARGET  = stripper
DTARGET = stripper_d

FILES = \
	main.o \
	decomp.o \
	decode.o \
	misc.o \
	bitplane.o \
	spec1.o \
	spec2.o \
	spec3.o \
	level1.o \
	level2.o \
	level3.o \
	raw_bit2.o \
	raw_bit3.o \
	anim2.o \
	anim3.o \
    dkc_gbc.o \
    dkc_gba.o \
    gba_misc.o \
    dkc2_decomp.o \
    dkc2_gba.o \
    dkl.o \
	lodepng.o

ODIR=obj
#_OBJ = $(FILES:.c=.o)
OBJECTS = $(patsubst %,$(ODIR)/%,$(FILES))

stripper: $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(CFLAGS)
    
$(ODIR)/%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

debug: CFLAGS = $(DFLAGS)
debug: TARGET = $(DTARGET)
debug: stripper

.PHONY: clean

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(DTARGET)
