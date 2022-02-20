CC = gcc
CFLAGS = -fopenmp -O3 -Wall -Werror -Wextra
DFLAGS = -fopenmp -O0 -g -Wall -Werror -Wextra
LDFLAGS = -flto -s -fopenmp
DLDFLAGS = -fopenmp
LIBS = -L.
WINDRES = windres
TARGET  := stripper
DTARGET := stripper_d

ifeq ($(OS),Windows_NT)
	TARGET = stripper.exe
	DTARGET = stripper_d.exe
endif

ifdef LIBPNG
	LIBS += -llibpng16
	CFLAGS +=-DLIBPNG
	DFLAGS +=-DLIBPNG
else ifndef BITMAP
	LIBS += -llodepng
	CFLAGS += -DLODEPNG
	DFLAGS += -DLODEPNG
endif

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
	jc.o \
    png.o

ODIR=obj
OBJECTS = $(patsubst %,$(ODIR)/%,$(FILES))

stripper: $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LIBS) $(LDFLAGS)
	
$(ODIR)/%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)
	
$(ODIR)/%.res: %.rc
	$(WINDRES) $< -O coff -o $@

debug: CFLAGS = $(DFLAGS)
debug: LDFLAGS = $(DLDFLAGS)
debug: TARGET = $(DTARGET)
debug: stripper

.PHONY: clean

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(DTARGET)
