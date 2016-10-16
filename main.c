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
#include <stdint.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include "main.h"

static void show_usage() {
    printf("Usage: stripper ROM_FILE [OPTION]\n");
    
    printf("\nDKC 1-3 (SNES), DKC 1-3 (GBA):");
    printf("\n\t-o Use palette zero instead of transparency. (Opaque)");
    printf("\n\t-t Rip tilesets.\n");
    
    printf("\nDKC 1-3 (SNES):");
    printf("\n\t-f Only use tiles with priority bit set. (Foreground)");
    printf("\n\t-b Only use tiles with priority bit not set. (Background)");
    printf("\n\t-c Rip complete layouts.");
    printf("\n\t-s Rip special screens.");
    printf("\n\t-e Rip decompressed 8x8 tiles. (DKC2&DKC3 only)");
    printf("\n\t-d Use destructible tiles. (DKC3 only)");
    printf("\n\t-h Use destructible tiles and omit normal tiles. (DKC3 only)\n");
    
    printf("\nDKL 1-3 (GB)");
    printf("\n\t-g Use greyscale palette instead of SGB.");
    printf("\n\t-t Rip tilesets.\n");
    
    printf("\nPress enter to exit...\n");
    getchar();
}


int main(int argc, char *argv[]) {

    printf("\nKingizor's multi-Kong Decompressor\n\n");
    
    if (argc < 2) {
        show_usage();
        return 0;
    }
    
    char dir[255];
    dir[0] = '\0';
    
    #ifdef WIN32
        GetModuleFileName(NULL, dir, 255);
        char *pntr = strrchr(dir, '\\');
        dir[pntr-dir+1] = '\0';
    #endif
    
    FILE *romfile = fopen(argv[1], "rb");

    if (romfile == NULL) {
        char err_msg[255];
        sprintf(err_msg, "Error opening \"%s\"", argv[1]);
        perror(err_msg);
        printf("\nPress enter to exit...\n");
        getchar();
        return 0;
    }
    fseek(romfile, 0, SEEK_END);
    uint32_t romlen = ftell(romfile);
    rewind(romfile);
    
    uint8_t *rom = malloc(romlen);
    
    if (rom == NULL) {
        printf("Unable to allocate memory for ROM.\n");
        return -1;
    }
    if (fread(rom, 1, romlen, romfile) != romlen) {
        printf("Error reading from input file.\n");
        return -1;
    }
    fclose(romfile);
    
    char intname[25];
    char gbcname[20];
    char gbaname[10];
    char  dsname[ 5];
    memcpy(intname, &rom[0xFFC0], 21);
    memcpy(gbcname, &rom[0x0134], 16);
    memcpy(gbaname, &rom[0x0A0 ],  8);
    memcpy(dsname,   rom,          4);
    intname[21] = '\0';
    gbcname[16] = '\0';
    gbaname[ 8] = '\0';
     dsname[ 4] = '\0';
    
    int game = 0;
    int region = 3;
    
    if (!strcmp(intname, "DONKEY KONG COUNTRY 3") ||
        !strcmp(intname, "SUPER DONKEY KONG 3  ")) {
        game = 3;
        region = rom[0xFFD9];
    } // JP
    else if (!strcmp(intname, "DIDDY'S KONG QUEST   ") ||
             !strcmp(intname, "SUPER DONKEY KONG 2  ")) {
        game = 2;
        region = rom[0xFFD9];
    }
    else if (!strcmp(intname, "DONKEY KONG COUNTRY  ")) {
        if (rom[0xFFD9] != 1 || rom[0xFFDB] != 0) {
            printf("Unsupported ROM version.\n");
            getchar();
            return 0;
        }
        game = 1;
    }
    else if (!strcmp(gbcname, "DK COUNTRY\0BDDE")) {
        game = 4; // DKC GBC
    }
    else if (!strcmp(gbcname, "DONKEYKONGLAND95")) {
        game = 5; // DKL
    }
    else if (!strcmp(gbcname, "DONKEYKONGLAND 2")) {
        game = 6; // DKL2
    }
    else if (!strcmp(gbcname, "DONKEYKONGLAND 3")) {
        game = 7; // DKL3
    }
    else if (!strcmp(gbaname, "RAREDKC1")) {
        if (rom[0xAF] != 0x45) {
            printf("Unsupported ROM version.\n");
            getchar();
            return 0;
        }
        game = 8;
    }
    else if (!strcmp(gbaname, "RAREDKC2")) {
        if (rom[0xAF] != 0x45) {
            printf("Unsupported ROM version.\n");
            getchar();
            return 0;
        }
        game = 9;
    }
    else if (!strcmp(gbaname, "RAREDKC3")) {
        if (rom[0xAF] != 0x45) {
            printf("Unsupported ROM version.\n");
            getchar();
            return 0;
        }
        game = 10;
    }
    else if (!strcmp(gbaname, "KINGOFSW")) {
        if (rom[0xAF] != 0x45) {
            printf("Unsupported ROM version.\n");
            getchar();
            return 0;
        }
        game = 11;
    }
    else if (!strcmp(dsname, "DKJC")) {
        game = 12;
    }
    else {
        printf("Incorrect ROM file.\n");
        getchar();
        return 0;
    }
    
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
    
    if (argc > 3) {
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
            else if (!strcmp(argv[i], "-g")) special |= 0x08; // GB
            else if (!strcmp(argv[i], "-t")) tileset = 1;
        }
    }
    
    /*
    The "special" variable causes different modes to be enabled under certain conditions.
    
    01 - Layout Animation frames (DKC23)
    02 - Layouts (DKC123)
    04 - Raw tiles (DKC23)
    08 - Special Screens (DKC123)
    10 - Damaged Terrain (DKC3)
    20 - Empty Level (DKC3)
    any- Greyscale Mode (DKL123)
    
    */
    
    if (special == 1 || special == 4) {
        if (game == 1 || game == 4) {
            printf("This feature is currently only available for the SNES versions of DKC2 and DKC3.");
            game = 0;
        }
        if (region != 1) {
            printf("This feature is is currently only available for the USA version.");
            game = 0;
        }
    }
    
    if (game == 1 && special & 8) {
        spec1(rom, dir);
        game = 0;
    } // Special Screens (DKC)
    else if (game == 2) {
        if (special & 1) {
            anim2(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC2)
        else if (special & 8) {
            spec2(rom, dir, region);
            game = 0;
        } // Special Screens (DKC2)
        else if (special & 4) {
            raw_bitplane2(rom, dir);
            game = 0;
        } // Raw Bitplanes (DKC2)
    }
    else if (game == 3) {
        if (special & 1) {
            anim3(rom, dir, priority, region);
            game = 0;
        } // Animated (DKC3)
        else if (special & 8) {
            spec3(rom, dir, region);
            game = 0;
        } // Special Screens (DKC3)
        else if (special & 4) {
            raw_bitplane3(rom, dir);
            game = 0;
        } // Raw Bitplanes (DKC3)
    }
    
    switch (game) {
        case 1: {
            level1(rom, dir, priority, special, tileset);
            break;
        }
        case 2: {
            level2(rom, dir, priority, special, tileset, region);
            break;
        }
        case 3: {
            level3(rom, dir, priority, special, tileset, region);
            break;
        }
        case 4: {
            gbc_levels(rom, dir);
            break;
        }
        case 5: {
            dkl_levels(rom, dir, special, tileset);
            break;
        }
        case 6: {
            dkl2_levels(rom, dir, special, tileset);
            break;
        }
        case 7: {
            dkl3_levels(rom, dir, special, tileset);
            break;
        }
        case 8: {
            dkc_gba_levels(rom, dir, priority, tileset);
            break;
        }
        case 9: {
            dkc2_gba_levels(rom, dir, priority, tileset);
            break;
        }
        case 10: {
            dkc3_gba_levels(rom, dir, priority, tileset);
            break;
        }
        case 11: {
            kos_levels(rom, dir);
            break;
        }
        case 12: {
            jc_levels(rom, dir);
            break;
        }
    }
    
    free(rom);
    
    printf("\nProgram completed.\n");
    printf("\nPress enter to exit...");
    getchar();
    
    return 0;

}