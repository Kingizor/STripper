CFLAGS += -Wall -Wextra -Werror -Wno-unknown-pragmas
CFLAGS += -Og -g
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=leak
CFLAGS += -fsanitize=undefined
CFLAGS += -fstack-protector-all
CFLAGS += -fopenmp
CFLAGS += -DLODEPNG
# LDFLAGS = -flto -s -fopenmp
LDFLAGS = -lxxhash -llodepng -ldkcomp
CLI_TARGET = stripper_cli
GTK_TARGET = stripper_gtk
WIN_TARGET = stripper_w.exe

OBJ = rom.o \
	misc.o bitplane.o \
	spec1.o spec2.o spec3.o \
	level1.o level2.o level3.o \
	raw_bit2.o raw_bit3.o \
	dkc_gbc.o dkl.o \
	dkc_gba.o dkc2_decomp.o \
	dkc2_gba.o \
	gba_misc.o \
	kos.o jc.o \
    png.o
#   anim2.o anim3.o
CLI_OBJ = cli.o
GTK_OBJ = gtk.o
WIN_OBJ = win.o

all: $(OBJ) $(CLI_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(CLI_OBJ) -o $(CLI_TARGET)

gtk: CFLAGS += `pkg-config --cflags --libs gtk+-3.0`
gtk: $(OBJ) $(GTK_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(GTK_OBJ) -o $(GTK_TARGET)
	
# deps
cli.o: 								     main.h rom.h
gtk.o:									 main.h rom.h
win.o:									 main.h rom.h
rom.o:									 rom.h
level1.c level3.c spec3.c: 				 misc.h
dkc_gba.c dkc2_gba.c kos.c jc.c: 		 gba_misc.h
dkc2_gba.c gba_misc.c: 					 dkc2_decomp.h
level1.c level2.c level3.c:              bitplane.h
raw_bit2.c raw_bit3.c:                   bitplane.h
spec1.c spec2.c spec3.c:                 bitplane.h
anim2.c anim3.c: 		                 bitplane.h
bitplane.c misc.c gba_misc.c jc.c kos.c: bitplane.h

clean:
	rm -f $(OBJ) $(CLI_OBJ) $(GTK_OBJ) $(WIN_OBJ) $(TARGET) $(GTK_TARGET) $(WIN_TARGET)
