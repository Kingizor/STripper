CC = gcc
CFLAGS = -O3 -Wall -Werror
DFLAGS = -O0 -g -Wall -Werror
LDFLAGS = -flto -s
WINDRES = windres
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
    kos.o \
	lodepng.o

ODIR=obj
OBJECTS = $(patsubst %,$(ODIR)/%,$(FILES))

stripper: $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)
    
$(ODIR)/%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)
    
$(ODIR)/%.res: %.rc
	$(WINDRES) $< -O coff -o $@

debug: CFLAGS = $(DFLAGS)
debug: LDFLAGS =
debug: TARGET = $(DTARGET)
debug: stripper

.PHONY: clean

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(DTARGET)
