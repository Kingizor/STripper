#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

void anim3(uint8_t *rom, char dir[255], int priority, int region) {
    region = region; // Unused
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

    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter = 0;
    decomp( bp_data, rom, &bp_counter, 0x22D9E8);
    decomp(raw_data, rom, &raw_counter, 0x22AEED);
    decomp(lay_data_a, rom, &lay_counter, 0x22849B);
    
    int jungle_tiles[] = {
        // 0x2BC2D2, 0x2BC552, 0x2BC7D2, 0x2BC052
        0x2BC412, 0x2BC7D2, 0x2BC192, 0x2BC552, 0x2BC912, 0x2BC2D2, 0x2BC692, 0x2BC052
    };
    memset(bp_data, 0, 0xFFFF);
    
    for (size_t i = 0; i < sizeof(jungle_tiles) / sizeof(int); i++) {
        
        // memcpy(&bp_data[0x020], &rom[jungle_tiles[i]], 0x140);
        // memcpy(&bp_data[0x160], &rom[jungle_tiles[i]], 0x140);
        memcpy(&bp_data[0x2A0], &rom[jungle_tiles[i]], 0x080);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9C39, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "KRK-%02zd", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
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
    decomp(lay_data_a, rom, &lay_counter, 0x233DD3);
    
    int factory_tiles[] = {
        // 0x345600, 0x345720, 0x345840, 0x345960, 0x345A80, 0x345BA0 // Wheel
        // 0x343380, 0x343540, 0x343700, 0x3438C0, 0x343A80, 0x343C40, 0x343000 // Lava1
        // 0x344000, 0x344100, 0x344200, 0x344300, 0x344400, 0x344500, 0x343E00 // Lava2
        0x344A00, 0x344C00, 0x344E00, 0x345000, 0x345200, 0x345400, 0x344600 // Lava3
    };
    
    int len = sizeof(factory_tiles) / sizeof(int);
    
    for (int i = 0; i < len; i++) {
        
        memset(bp_data, 0, 0xFFFF);
        
        // memcpy(&bp_data[0x4E0], &rom[factory_tiles[i]], 0x120); // Wheel
        // memcpy(&bp_data[0x020], &rom[factory_tiles[i]], 0x1C0); // Lava1
        // memcpy(&bp_data[0x1E0], &rom[factory_tiles[i]], 0x100); // Lava2
        memcpy(&bp_data[0x2E0], &rom[factory_tiles[i]], 0x200); // Lava3
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3DA039, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "bb-%02d", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D9F39, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "ks-%02d", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        
    }
    */

    free( bp_data);
    free(raw_data);
    free(lay_data_a);
    free(lay_data_b);
    free(bitplane);

} // Animated tiles (DKC3)
