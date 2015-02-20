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
    printf("\n\t-o Use palette index zero instead of transparent zero. (Opaque)\n");
    
    printf("\nDKC 1-3 (SNES):");
    printf("\n\t-f Only use tiles with priority bit set. (Foreground)");
    printf("\n\t-b Only use tiles with priority bit not set. (Background)");
    printf("\n\t-c Rip complete layouts.");
    printf("\n\t-s Rip special screens.\n");
    
    printf("\nDKC 2-3 (SNES):");
    printf("\n\t-a Rip level & tileset frames.");
    printf("\n\t-e Rip decompressed 8x8 tiles.\n");
    
    printf("\nDKL 1-3 (GB)");
    printf("\n\t-g Use greyscale palette.\n");
    
    printf("\nPress enter to continue...\n");
    getchar();
}


int main(int argc, char *argv[]) {

    
    uint8_t *rom;
    unsigned int romlen;
    FILE * romfile;
    
    printf("\nKingizor's DKC123/DKL123/DKCGBC/DKCGBA123 Decompressor\n\n");
    
    if (argc < 2 || argc > 3) {
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
    
    romfile = fopen(argv[1], "r");

    if (romfile != NULL) {
        fseek(romfile, 0, SEEK_END);
        romlen = ftell(romfile);
        rewind(romfile);
        
        if (romlen > 16777216) {
            printf("Invalid ROM file.\n");
            getchar();
            return 0;
        }
        
        
        rom = malloc(romlen);
        fread(rom, 1, romlen, romfile);
        fclose(romfile);
        // printf("\nROM file loaded.\n");
    }
    else {
        perror("\nError\n");
        getchar();
        return 0;
    }
    
    char intname[25];
    memcpy(intname, &rom[0xFFC0], 21);
    intname[21] = '\0';
    char gbcname[20];
    memcpy(gbcname, &rom[0x0134], 16);
    gbcname[16] = '\0';
    char gbaname[10];
    memcpy(gbaname, &rom[0x0A0], 8);
    gbaname[8] = '\0';
    
    int game = 0;
    int region = 3;
    
    if (!strcmp(intname, "DONKEY KONG COUNTRY 3")) {
        game = 3;
        region = rom[0xFFD9];
    }
    else if (!strcmp(intname, "SUPER DONKEY KONG 3  ")) {
        game = 3;
        region = rom[0xFFD9];
    } // JP
    else if (!strcmp(intname, "SUPER DONKEY KONG 2  ")) {
        game = 2;
        region = rom[0xFFD9];
    } // JP
    else if (!strcmp(intname, "DIDDY'S KONG QUEST   ")) {
        game = 2;
        region = rom[0xFFD9]; // 0 = JP, 1 = US, 2 = EU
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
    else {
        printf("Incorrect ROM file.\n");
        getchar();
        return 0;
    }
    
    int special = 0;
    
    int priority = 0;
    /*
        Priority flag:
            0 - Both layers with transparency (default)
            1 - Both layers without transparency (special fix for screens)
            2 - Foreground with transparency
            3 - Background with transparency
            
        Many levels/areas will only contain tiles that do not have the priority bit set. Using option 3 in these cases will result in empty images.
            
    */
    
    int i;
    
    for (i = 2; i < 4; i++) {
        if (argc == i+1) {
            if (!strcmp(argv[i], "-f")) {
                priority = 2;
                printf("Foreground mode enabled.\n\n");
            }
            else if (!strcmp(argv[i], "-b")) {
                priority = 3;
                printf("Background mode enabled.\n\n");
            }
            else if (!strcmp(argv[i], "-o")) {
                priority = 1;
                printf("Opaque mode enabled.\n\n");
            }
            else if (!strcmp(argv[i], "-a")) {
                special = 1;
                printf("Alternate tile mode.\n\n");
            }
            else if (!strcmp(argv[i], "-c")) {
                special = 2;
                printf("Full layout mode.\n\n");
            }
            else if (!strcmp(argv[i], "-s")) {
                special = 3;
                printf("Special screen mode.\n\n");
            }
            else if (!strcmp(argv[i], "-e")) {
                special = 4;
                printf("Raw bitplane mode.\n\n");
            }
            else if (!strcmp(argv[i], "-g")) {
                special = 5;
                printf("Gameboy mode.\n\n");
            }
            else {
                printf("Unknown command \"%s\"\n", argv[2]);
                getchar();
                return 0;
            }
        }
    }
    
    /*
    For each map we need to decompress 
    
    1) Bitplane Data.
    2) Tile Assembly Data. (8x8 -> 32x32)
    3) Level Layout Data.  (32x32 -> Level)
    
    The data from steps one and two is then used to decode the bitplane data.
    
    The resulting bitplane data is stored row by row, tile by tile. The assemble_bitplane function can be called to 
    
    The new image data can then be passed to the assembler function along with the level palette and the magic dimension.
    
    No need for free/malloc since the data can just be overwritten each time. Should only need to reset each counter to zero.
    
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
    
    if (special == 1 && game == 2) {
        anim2(rom, dir, priority, region);
        game = 0;
    } // Animated (DKC2)
    
    if (special == 1 && game == 3) {
        anim3(rom, dir, priority, region);
        game = 0;
    } // Animated (DKC3)
    
    if (special == 3 && game == 1) {
        spec1(rom, dir);
        game = 0;
    } // Special Screens (DKC)
    
    if (special == 3 && game == 2) {
        spec2(rom, dir, region);
        game = 0;
    } // Special Screens (DKC2)
    
    if (special == 3 && game == 3) {
        spec3(rom, dir, region);
        game = 0;
    } // Special Screens (DKC3)
    
    if (special == 4 && game == 2) {
        raw_bitplane2(rom, dir);
        game = 0;
    } // Raw Bitplanes (DKC2)
    
    if (special == 4 && game == 3) {
        raw_bitplane3(rom, dir);
        game = 0;
    } // Raw Bitplanes (DKC3)
    
    if (game == 1) {
        level1(rom, dir, priority, special);
        game = 0;
    } // DKC
    
    if (game == 2) {
        level2(rom, dir, priority, special, region);
        game = 0;
    } // DKC2
    
    if (game == 3) {
        level3(rom, dir, priority, special, region);
        game = 0;
    } // DKC3
    
    if (game == 4) {
        gbc_levels(rom, dir);
        game = 0;
    } // DKC GBC
    if (game == 5) {
        dkl_levels(rom, dir, special);
        game = 0;
    } // DKL
    if (game == 6) {
        dkl2_levels(rom, dir, special);
        game = 0;
    } // DKL2
    if (game == 7) {
        dkl3_levels(rom, dir, special);
        game = 0;
    } // DKL3
    if (game == 8) {
        dkc_gba_levels(rom, dir, priority, special);
        game = 0;
    } // DKC GBA
    if (game == 9) {
        dkc2_gba_levels(rom, dir, priority, special);
        game = 0;
    } // DKC2 GBA
    if (game == 10) {
        dkc3_gba_levels(rom, dir, priority, special);
        game = 0;
    } // DKC3 GBA
    
    free(rom);
    
    printf("\nProgram completed.\n");
    printf("\nPress enter to continue...");
    getchar();
    
    return 0;

}