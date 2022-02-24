CFLAGS += -Wall -Wextra -Werror -Wno-unknown-pragmas
CFLAGS += -Og -g
CFLAGS += -fsanitize=address
CFLAGS += -fsanitize=leak
CFLAGS += -fsanitize=undefined
CFLAGS += -fstack-protector-all
CFLAGS += -fopenmp
CFLAGS += -DLODEPNG
# LDFLAGS = -flto -s -fopenmp
LDFLAGS = -lxxhash -lbd_comp -lsd_comp -llodepng
TARGET = stripper

OBJ = main.o \
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

stripper: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET)
	
# deps
main.c: 								 main.h
level1.c level3.c spec3.c: 				 misc.h
dkc_gba.c dkc2_gba.c kos.c jc.c: 		 gba_misc.h
dkc2_gba.c gba_misc.c: 					 dkc2_decomp.h
level1.c level2.c level3.c:              bitplane.h
raw_bit2.c raw_bit3.c:                   bitplane.h
spec1.c spec2.c spec3.c:                 bitplane.h
anim2.c anim3.c: 		                 bitplane.h
bitplane.c misc.c gba_misc.c jc.c kos.c: bitplane.h

clean:
	rm $(OBJ) $(TARGET)
