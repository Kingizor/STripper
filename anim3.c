#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

void anim3(uint8_t *rom, char dir[255], int priority, int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    uint8_t *lay_data_a = malloc(65535);
    uint8_t *lay_data_b = malloc(65535);
    int  bp_counter;
    int raw_counter;
    int lay_counter;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    char name[255];
    #define bp_width 512

    int i;
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter = 0;
    decomp( bp_data, rom, &bp_counter, 0x22D9E8);
    decomp(raw_data, rom, &raw_counter, 0x22AEED);
    decomp(lay_data_a, rom, &lay_counter, 0x22849B);
    
    int jungle_tiles[] = {
        0x2BC052, 0x2BC052, 0x2BC052,
        0x2BC2D2, 0x2BC412, 0x2BC412,
        0x2BC552, 0x2BC7D2, 0x2BC7D2,
        0x2BC7D2, 0x2BC192, 0x2BC192,
        0x2BC052, 0x2BC552, 0x2BC552,
        0x2BC2D2, 0x2BC912, 0x2BC912,
        0x2BC552, 0x2BC2D2, 0x2BC2D2,
        0x2BC7D2, 0x2BC692, 0x2BC692
    }; // 8*3
    
    for (i = 0; i < 9; i++) {
        
        if (i > 0) {
            memcpy(&bp_data[0x020], &rom[jungle_tiles[((i-1)*3)  ]], 0x140);
            memcpy(&bp_data[0x160], &rom[jungle_tiles[((i-1)*3)+1]], 0x140);
            memcpy(&bp_data[0x2A0], &rom[jungle_tiles[((i-1)*3)+2]], 0x080);
        }
        
        if (region == 0) {
            decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9C39 - 77, raw_counter, bp_counter, 1, 0, priority);
        }
        else {
            decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9C39, raw_counter, bp_counter, 1, 0, priority);
        }
        sprintf(name, "Konveyor Rope Klash Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Konveyor Rope Klash-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B24B, 0, 16, 0, dir, name);
        sprintf(name, "Konveyor Rope Klash Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B273, 0, 16, 0, dir, name);
        sprintf(name, "Konveyor Rope Klash Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B27D, 0, 16, 0, dir, name);
        
    } // Jungle
    
    /*
        There is a turn-y thing that constantly updates. 0x2270 - 0x120 bytes
        
        The chemical bubbles seem to update via three separate possible writes:
        0x20F0 - 0x100 bytes
        0x2010 - 0x1C0 bytes
        0x2170 - 0x200 bytes
        
    */
    
    /*
     bp_counter = 0;
    raw_counter = 0;
    lay_counter = 0;
    decomp( bp_data, rom, &bp_counter, 0x238D56);
    decomp(raw_data, rom, &raw_counter, 0x237402);
    decomp(&lay_data_a, rom, &lay_counter, 0x233DD3);
    
    int factory_tiles[] = {
        // ?
    }
    
    for (i = 0; i < 0; i++) {
    
        if (i > 0) {
            memcpy(&bp_data[0x020], &rom[factory_tiles[((i-1)*3)  ]], 0x140);
            memcpy(&bp_data[0x160], &rom[factory_tiles[((i-1)*3)+1]], 0x140);
            memcpy(&bp_data[0x2A0], &rom[factory_tiles[((i-1)*3)+2]], 0x080);
        }
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9E39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Fire-Ball Frenzy Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2A6, 1, 80, 0, dir, "Fire-Ball Frenzy.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2CE, 1, 80, 0, dir, "Fire-Ball Frenzy Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2D8, 1, 80, 0, dir, "Fire-Ball Frenzy Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3DA039, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Blazing Bazukas Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2B0, 1, 80, 0, dir, "Blazing Bazukas.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2C4, 1, 80, 0, dir, "Blazing Bazukas Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2EC, 1, 80, 0, dir, "Blazing Bazukas Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9F39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Krack Shot Kroc Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2BA, 1, 80, 0, dir, "Krack Shot Kroc.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2D8, 1, 80, 0, dir, "Krack Shot Kroc Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2E2, 1, 80, 0, dir, "Krack Shot Kroc Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3DA139, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "KAOS Karnage Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter, 0x37B2F6, 1, 80, 0, dir, "KAOS Karnage.png");
    }
    */

    free( bp_data);
    free(raw_data);
    free(lay_data_a);
    free(lay_data_b);
    free(bitplane);

} // Animated tiles (DKC3)