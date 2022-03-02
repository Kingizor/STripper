/*

This file contains the frontend code for the decompressor and the debitplanifier.


Not-going-to-do-in-a-hurry:

Non USA versions are not yet supported.
Animated tile offsets may be different in other versions.
Floodlit Fish uses window clipping and other shenanigans.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include "main.h"

static void show_usage() {
    puts("Usage: stripper ROM_FILE [OPTION]");

    puts("\nDKC 1-3 (SNES), DKC 1-3 (GBA):");
    puts("\t-o Use palette zero instead of transparency. (Opaque)");
    puts("\t-t Rip tilesets.");

    puts("\nDKC 1-3 (SNES):");
    puts("\t-f Only use tiles with priority bit set. (Foreground)");
    puts("\t-b Only use tiles with priority bit not set. (Background)");
    puts("\t-c Rip complete layouts.");
    puts("\t-s Rip special screens.");
    puts("\t-e Rip decompressed 8x8 tiles. (DKC2&DKC3 only)");
    puts("\t-d Use destructible tiles. (DKC3 only)");
    puts("\t-h Show destructible tiles only. (DKC3 only)");
    puts("\t-n Show non-destructible tiles only. (DKC3 only)");

    puts("\nDKL 1-3 (GB)");
    puts("\t-g Use greyscale palette instead of SGB.");
    puts("\t-t Rip tilesets.\n");
}


enum GAME_LIST {
    UNKNOWN,
    SNES_DKC, SNES_DKC2, SNES_DKC3,
    GBC_DKC, GB_DKL, GB_DKL2, GB_DKL3,
    GBA_DKC, GBA_DKC2, GBA_DKC3,
    GBA_DKKOS, DS_DKJC
};

struct ROM_META {
    long unsigned hash;
    size_t size;
    enum GAME_LIST game;
    int region;
    int version;
};

static const struct ROM_META romlist[] = {
    { 0x195B60FE,  0x400000,  SNES_DKC,   1,0 },
    { 0x32E11AF8,  0x400000,  SNES_DKC2,  2,0 },
    { 0xD73850D8,  0x400000,  SNES_DKC2,  2,0 },
    { 0xA5B0964D,  0x400000,  SNES_DKC2,  2,1 },
    { 0x76612943,  0x400000,  SNES_DKC2,  1,0 },
    { 0x7B4D0D69,  0x400000,  SNES_DKC2,  1,1 },
    { 0xE2A62365,  0x400000,  SNES_DKC2,  0,0 },
    { 0x2AF4ED4E,  0x400000,  SNES_DKC2,  0,1 },
//  { 0xC4599C84,  0x400000,  SNES_DKC3,  2,0 },
    { 0x57592E47,  0x400000,  SNES_DKC3,  1,0 },
//  { 0xB11D0174,  0x400000,  SNES_DKC3,  0,0 },
//  { 0x3E2BD924,  0x400000,  SNES_DKC3,  0,1 },
    { 0xCB6CAE97,  0x400000,   GBC_DKC,   0,0 },
    { 0x48D7B8C4,   0x50000,    GB_DKL,   0,0 },
    { 0x53B97DBF,   0x50000,    GB_DKL2,  0,0 },
    { 0x43673D7C,   0x50000,    GB_DKL3,  0,0 },
//  { 0xC7AB9EA1,  0x100000,   GBC_DKL3,  0,0 },
    { 0xF86CB732,  0x800000,   GBA_DKC,   0,0 },
    { 0x7967734C,  0x800000,   GBA_DKC2,  0,0 },
    { 0xFD5900BF,  0x800000,   GBA_DKKOS, 0,0 },
    { 0x7781F51A, 0x1000000,   GBA_DKC3,  0,0 },
    { 0xAF640C57, 0x4000000,    DS_DKJC,  0,0 }
};
static const int romcount = sizeof(romlist) / sizeof(struct ROM_META);



int main (int argc, char *argv[]) {

    puts("\nKingizor's multi-Kong Decompressor\n");

    if (argc < 2) {
        show_usage();
        return 0;
    }

    char dir[2048];
    dir[0] = '\0';

    #ifdef WIN32
        GetModuleFileName(NULL, dir, 2048);
        char *pntr = strrchr(dir, '\\');
        dir[pntr-dir+1] = '\0';
    #endif

    FILE *romfile = fopen(argv[1], "rb");

    if (romfile == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    size_t romlen;
    if ((fseek(romfile, 0, SEEK_END) == -1)
    || ((romlen = ftell(romfile))    == -1u)
    ||  (fseek(romfile, 0, SEEK_SET) == -1)) {
        fprintf(stderr, "Error seeking input file.\n");
        return 1;
    }

    unsigned char *rom = malloc(romlen);

    if (rom == NULL) {
        fprintf(stderr, "Unable to allocate memory for input file.\n");
        return 1;
    }
    if (fread(rom, 1, romlen, romfile) != romlen) {
        fprintf(stderr, "Error reading from input file.\n");
        return 1;
    }
    fclose(romfile);

    long unsigned hash = XXH32(rom, romlen, 0x50434441);
    int i;
    for (i = 0; i < romcount; i++)
        if (romlist[i].hash == hash)
            break;
    if (i == romcount) {
        puts("Unsupported ROM version.");
        return 1;
    }
    const struct ROM_META *rl = &romlist[i];
    int game   = rl->game;
    int region = rl->region;





    /*
        Priority flag:
            0 - Both layers with transparency (default)
            1 - Both layers without transparency (special fix for screens)
            2 - Foreground with transparency
            3 - Background with transparency

        Many levels/areas will only contain tiles that do not have the priority bit set. Using option 3 in these cases will result in empty images.

    */

    int special = 0;
    int priority = 0;
    int tileset = 0;

    for (int i = 2; i < argc; i++) {
             if (!strcmp(argv[i], "-f")) priority = 2; // Foreground (Priority set)
        else if (!strcmp(argv[i], "-b")) priority = 3; // Background (Priority not set)
        else if (!strcmp(argv[i], "-o")) priority = 1; // Opaque (Use palette zero)
        else if (!strcmp(argv[i], "-a")) special |= 0x01;
        else if (!strcmp(argv[i], "-c")) special |= 0x02;
        else if (!strcmp(argv[i], "-e")) special |= 0x04;
        else if (!strcmp(argv[i], "-s")) special |= 0x08;
        else if (!strcmp(argv[i], "-d")) special |= 0x10;
        else if (!strcmp(argv[i], "-h")) special |= 0x30;
        else if (!strcmp(argv[i], "-n")) special |= 0x40;
        else if (!strcmp(argv[i], "-g")) special |= 0x08; // GB
        else if (!strcmp(argv[i], "-t")) tileset = 1;
    }

    /*
    The "special" variable causes different modes to be enabled under certain conditions.

    01 - Layout Animation frames (DKC23)
    02 - Layouts (DKC123)
    04 - Raw tiles (DKC23)
    08 - Special Screens (DKC123)
    10 - Damaged Terrain (DKC3)
    20 - Empty Level (DKC3)
    40 - Non-damaged terrain (DKC3)
    any- Greyscale Mode (DKL123)

    */

    if (special == 1 || special == 4) {
        if (game == SNES_DKC || game == GBC_DKC) {
            printf("This feature is currently only available for the SNES versions of DKC2 and DKC3.");
            game = 0;
        }
        if (region != 1) {
            printf("This feature is is currently only available for the USA version.");
            game = 0;
        }
    }

    if (game == SNES_DKC && special & 8) {
        spec1(rom, dir);
        game = 0;
    } // Special Screens (DKC)
    else if (game == SNES_DKC2) {
        if (special & 1) {
            // anim2(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC2)
        else if (special & 8) {
            spec2(rom, romlen, dir, region);
            game = 0;
        } // Special Screens (DKC2)
        else if (special & 4) {
            raw_bitplane2(rom, dir);
            game = 0;
        } // Raw Bitplanes (DKC2)
    }
    else if (game == SNES_DKC3) {
        if (special & 1) {
            // anim3(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC3)
        else if (special & 8) {
            spec3(rom, romlen, dir, region);
            game = 0;
        } // Special Screens (DKC3)
        else if (special & 4) {
            raw_bitplane3(rom, dir);
            game = 0;
        } // Raw Bitplanes (DKC3)
    }

    switch (game) {
        case SNES_DKC:   { level1(rom, romlen, dir, priority, special, tileset);         break; }
        case SNES_DKC2:  { level2(rom, romlen, dir, priority, special, tileset, region); break; }
        case SNES_DKC3:  { level3(rom, romlen, dir, priority, special, tileset, region); break; }
        case  GBC_DKC:   {  gbc_levels(rom, romlen, dir); break; }
        case   GB_DKL:   {  dkl_levels(rom, romlen, dir, special, tileset); break; }
        case   GB_DKL2:  { dkl2_levels(rom, romlen, dir, special, tileset); break; }
        case   GB_DKL3:  { dkl3_levels(rom, romlen, dir, special, tileset); break; }
        case  GBA_DKC:   {  dkc_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKC2:  { dkc2_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKC3:  { dkc3_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKKOS: { kos_levels(rom, dir); break; }
        case   DS_DKJC:  {  jc_levels(rom, dir); break; }
    }

    free(rom);

    printf("\nProgram completed.\n");

    return 0;

}
