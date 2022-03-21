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

#ifdef WIN32
    #include <windows.h>
#endif

#include "main.h"
#include "rom.h"

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

    struct ROM_DATA rom_data;
    char *error = NULL;
    if ((error = verify_rom(&rom_data, argv[1])) != NULL) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    unsigned char *rom = rom_data.buf;
    size_t size        = rom_data.meta->size;
    int    game        = rom_data.meta->game;
    int    region      = rom_data.meta->region;


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
             if (!strcmp(argv[i], "-f")) priority |= 2; // No BG (Priority set)
        else if (!strcmp(argv[i], "-b")) priority |= 4; // No FG (Priority not set)
        else if (!strcmp(argv[i], "-o")) priority |= 1; // Opaque (Use palette zero)
        else if (!strcmp(argv[i], "-a")) special |= 0x01; // animated?
        else if (!strcmp(argv[i], "-c")) special |= 0x02; // complete layouts
        else if (!strcmp(argv[i], "-e")) special |= 0x04; // raw 8x8
        else if (!strcmp(argv[i], "-s")) special |= 0x08; // special screens
        else if (!strcmp(argv[i], "-d")) special |= 0x10; // use destructibles
        else if (!strcmp(argv[i], "-h")) special |= 0x30; // dest only
        else if (!strcmp(argv[i], "-n")) special |= 0x40; // non-dest only
        else if (!strcmp(argv[i], "-g")) special |= 0x08; // GB greyscale
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
            fprintf(stderr, "This feature is currently only available for the SNES versions of DKC2 and DKC3.");
            game = 0;
        }
        if (region != 1) {
            fprintf(stderr, "This feature is is currently only available for the USA version.");
            game = 0;
        }
    }

    if (game == SNES_DKC && special & 8) {
        spec1(rom, size, dir);
        game = 0;
    } // Special Screens (DKC)
    else if (game == SNES_DKC2) {
        if (special & 1) {
            // anim2(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC2)
        else if (special & 8) {
            spec2(rom, size, dir, region);
            game = 0;
        } // Special Screens (DKC2)
        else if (special & 4) {
            raw_bitplane2(rom, size, dir);
            game = 0;
        } // Raw Bitplanes (DKC2)
    }
    else if (game == SNES_DKC3) {
        if (special & 1) {
            // anim3(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC3)
        else if (special & 8) {
            spec3(rom, size, dir, region);
            game = 0;
        } // Special Screens (DKC3)
        else if (special & 4) {
            raw_bitplane3(rom, size, dir);
            game = 0;
        } // Raw Bitplanes (DKC3)
    }

    unsigned palette[] = { /* DKL { 16, 88, 160, 232 } */
        0xE8E8E8, 0xA0A0A0, 0x585858, 0x101010
    };
    switch (game) {
        case SNES_DKC:   {          level1(rom, size, dir, priority, special, tileset);         break; }
        case SNES_DKC2:  {          level2(rom, size, dir, priority, special, tileset, region); break; }
        case SNES_DKC3:  {          level3(rom, size, dir, priority, special, tileset, region); break; }
        case  GBC_DKC:   {      gbc_levels(rom, size, dir); break; }
        case   GB_DKL:   {      dkl_levels(rom, size, dir, special, palette, tileset); break; }
        case   GB_DKL2:  {     dkl2_levels(rom, size, dir, special, palette, tileset); break; }
        case   GB_DKL3:  {     dkl3_levels(rom, size, dir, special, palette, tileset); break; }
        case  GBA_DKC:   {  dkc_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKC2:  { dkc2_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKC3:  { dkc3_gba_levels(rom, dir, priority, tileset); break; }
        case  GBA_DKKOS: {      kos_levels(rom, dir); break; }
        case   DS_DKJC:  {       jc_levels(rom, dir); break; }
    }

    free(rom);

    printf("\nProgram completed.\n");

    return 0;

}
