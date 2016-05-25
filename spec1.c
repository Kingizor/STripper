#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"

struct Level {
    uint16_t bp_len;
    uint16_t raw_len;
    uint16_t bp_ofs;
    uint16_t raw_ofs;
    uint32_t bp_addr;
    uint32_t raw_addr;
    uint32_t palette;
    uint8_t palette_fix;
    uint8_t mode;
    uint8_t opacity;
    char *name;
};


void spec1(uint8_t *rom, char dir[255]) {
    
    struct Level levels[] = {
        {0x7000, 0x700, 0, 0, 0x0116F1, 0x010FF0, 0x39BE03, 0, 2, 1, "DKC Overworld"},
        {0x7000, 0x700, 0, 0, 0x0116F1, 0x010FF0, 0x39BE03, 0, 2, 1, "DKC Overworld"},
        {0x7000, 0x700, 0, 0, 0x051012, 0x050912, 0x39C103, 0, 2, 1, "Kongo Jungle (Left)"},
        {0x7000, 0x700, 0, 0, 0x061757, 0x061057, 0x39C003, 0, 2, 1, "Kongo Jungle (Right)"},
        {0x7000, 0x700, 0, 0, 0x067CF7, 0x0675F7, 0x39C523, 0, 2, 1, "Monkey Mines (Left)"},
        {0x7000, 0x700, 0, 0, 0x0C8BB4, 0x0C84B4, 0x39C423, 0, 2, 1, "Monkey Mines (Right)"},
        {0x7000, 0x700, 0, 0, 0x0A0701, 0x0A0000, 0x39CF43, 0, 2, 1, "Vine Valley (Left)"},
        {0x7000, 0x700, 0, 0, 0x0828E4, 0x0821E4, 0x39D043, 0, 2, 1, "Vine Valley (Right)"},
        {0x7000, 0x700, 0, 0, 0x370000, 0x040000, 0x39C323, 0, 2, 1, "Gorilla Glacier (Left)"},
        {0x7000, 0x700, 0, 0, 0x341558, 0x340E57, 0x39C223, 0, 2, 1, "Gorilla Glacier (Right)"},
        {0x7000, 0x700, 0, 0, 0x320700, 0x320000, 0x39CC83, 0, 2, 1, "Kremkroc Industries Inc (Left)"},
        {0x7000, 0x700, 0, 0, 0x3F0701, 0x3F0000, 0x39CD63, 0, 2, 1, "Kremkroc Industries Inc (Right)"},
        {0x7000, 0x700, 0, 0, 0x110701, 0x110000, 0x39D143, 0, 2, 1, "Chimp Caverns (Left)"},
        {0x7000, 0x700, 0, 0, 0x116DC3, 0x1166C2, 0x39D243, 0, 2, 1, "Chimp Caverns (Right)"},
        {0x7000, 0x700, 0, 0, 0x34A0FE, 0x3499FD, 0x39B983, 0, 2, 1, "Funky's Flights"},
        {0x6EE0, 0x700, 0, 0, 0x001A41, 0x001340, 0x39BAC3, 0, 2, 1, "Candy's Save Point"},
        {0x5EE0, 0x6C0, 0, 0, 0x329DD2, 0x329712, 0x39BCA3, 0, 2, 0, "Cranky's Cabin BG1"},
        {0x36E0, 0x240, 0, 0, 0x27095E, 0x2EFC5D, 0x39BCA3, 0, 2, 1, "Cranky's Cabin BG2"},
        {0x7000, 0x700, 0, 0, 0x0468AA, 0x387728, 0x39B7E3, 0, 2, 1, "Game Over Screen"},
        {0x7000, 0x700, 0, 0, 0x2F0700, 0x2F0000, 0x39B4A3, 0, 2, 1, "Title Screen"},
        {0x2600, 0x700, 0, 0, 0x2F8DBE, 0x2F86BE, 0x39D463, 0, 2, 0, "Mode Screen BG"},
        {0x3400, 0x700, 0, 0, 0x096A77, 0x2F7FBE, 0x39D563, 0, 3, 1, "File Screen BG3"},
        {0x2000, 0x700, 0, 0x200, 0x240690, 0x240450, 0x39C203, 0, 2, 1, "Nintendo 1994"},
        {0x8000, 0x700, 0, 0, 0x0F84A0, 0x0F7DA0, 0x39D683, 0, 2, 1, "DK's Treehouse"},
        {0x400, 0x2000, 0, 0, 0x27FCE9, 0x27DCE9, 0x39A01C, 0, 3, 0, "Cave FG3"},
        {0x400, 0x2000, 0, 0, 0x27FCE9, 0x27DCE9, 0x39AD65, 0, 3, 0, "Cave FG3 (Alt)"},
        {0x1000, 0x700, 0, 0, 0x3B6ED8, 0x3B67D8, 0x399C1C, 0, 2, 0, "Water BG2"},
        {0x1000, 0x800, 0, 0, 0x287A4A, 0x288A2A, 0x39A2DC, 0, 2, 0, "Walkway BG2 (Left)"},
        {0x1000, 0x800, 0, 0, 0x287A4A, 0x28922A, 0x39A2DC, 0, 2, 0, "Walkway BG2 (Right)"},
        {0xF00, 0x800, 0, 0, 0x34831D, 0x3491FD, 0x399A14, 0, 3, 1, "Mine Cart Carnage BG3"},
        {0x1300, 0x800, 0, 0, 0x14EB52, 0x14DF52, 0x39B3A3, 0, 3, 1, "Temple BG3"},
        {0xFE0, 0x800, 0, 0, 0x22896D, 0x22816D, 0x39B0A3, 0, 3, 1, "Oil Drum Alley BG3"},
        {0xFE0, 0x800, 0, 0, 0x22896D, 0x22816D, 0x39B0A3, 63, 3, 1, "Blackout Basement BG3"},
        {0x1800, 0x800, 0, 0, 0x238BFB, 0x2383FB, 0x39C623, 0, 3, 1, "Forest BG3"},
        {0x21A0, 0x800, 0xE60, 0, 0xC3BFE, 0xC33FE, 0x39B2A3, 0, 2, 0, "Tree Top Town BG2"},
        {0x21A0, 0x800, 0xE60, 0, 0xC3BFE, 0xC33FE, 0x39B1A3, 0, 2, 0, "Rope Bridge Rumble BG2"},
        {0x2000, 0x800, 0x1000, 0, 0x318502, 0x317D02, 0x39B2A3, 0, 3, 1, "Tree Top Town BG3"},
        {0x2000, 0x800, 0x1000, 0, 0x318502, 0x317D02, 0x39B1A3, 0, 3, 1, "Rope Bridge Rumble BG3"}
    };
    
    int length = sizeof(levels) / sizeof(struct Level);
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < length; i++) {
        
        uint8_t *bp_data = calloc(0x8000, 1);
        uint8_t *raw_data = calloc(0x2000, 1);
        uint8_t *bitplane = malloc(0x100000);
        
        memcpy(&bp_data[levels[i].bp_ofs], &rom[levels[i].bp_addr], levels[i].bp_len);
        memcpy(&raw_data[levels[i].raw_ofs], &rom[levels[i].raw_addr], levels[i].raw_len);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, levels[i].palette, levels[i].raw_len, levels[i].bp_len, levels[i].mode, levels[i].palette_fix, levels[i].opacity);
        assemble_screen(bitplane, levels[i].raw_len, 32, dir, levels[i].name);
        
        free(bp_data);
        free(raw_data);
        free(bitplane);
    }
    
    return;
    
}
