#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"

void spec1(uint8_t *rom, char dir[255]) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    int bp_counter;
    int raw_counter;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    // Special Screens
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0116F1], bp_counter);
    memcpy(raw_data, &rom[0x010FF0], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39BE03, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "DKC Overworld");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x051012], bp_counter);
    memcpy(raw_data, &rom[0x050912], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C103, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kongo Jungle (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x061757], bp_counter);
    memcpy(raw_data, &rom[0x061057], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C003, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kongo Jungle (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x067CF7], bp_counter);
    memcpy(raw_data, &rom[0x0675F7], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C523, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Monkey Mines (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0C8BB4], bp_counter);
    memcpy(raw_data, &rom[0x0C84B4], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C423, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Monkey Mines (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0A0701], bp_counter);
    memcpy(raw_data, &rom[0x0A0000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39CF43, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Vine Valley (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0828E4], bp_counter);
    memcpy(raw_data, &rom[0x0821E4], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D043, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Vine Valley (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x370000], bp_counter);
    memcpy(raw_data, &rom[0x040000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C323, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gorilla Glacier (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x341558], bp_counter);
    memcpy(raw_data, &rom[0x340E57], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C223, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gorilla Glacier (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x320700], bp_counter);
    memcpy(raw_data, &rom[0x320000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39CC83, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kremkroc Industries Inc (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x3F0701], bp_counter);
    memcpy(raw_data, &rom[0x3F0000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39CD63, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kremkroc Industries Inc (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x110701], bp_counter);
    memcpy(raw_data, &rom[0x110000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D143, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Chimp Caverns (Left)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x116DC3], bp_counter);
    memcpy(raw_data, &rom[0x1166C2], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D243, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Chimp Caverns (Right)");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x34A0FE], bp_counter);
    memcpy(raw_data, &rom[0x3499FD], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B983, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Funky's Flights");
    
     bp_counter = 0x6EE0;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x001A41], bp_counter);
    memcpy(raw_data, &rom[0x001340], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39BAC3, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Candy's Save Point");
    
     bp_counter = 0x5EE0;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x329DD2], bp_counter);
    memset(raw_data, 0, 0x40);
    memcpy(&raw_data[0x40], &rom[0x329712], 0x6C0);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39BCA3, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cranky's Cabin BG1");
    
     bp_counter = 0x36E0;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x27095E], bp_counter);
    memset(raw_data, 0, raw_counter);
    memcpy(raw_data, &rom[0x2EFC5D], 0x240);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39BCA3, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cranky's Cabin BG2");
    
     bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0468AA], bp_counter);
    memcpy(raw_data, &rom[0x387728], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B7E3, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Game Over Screen");
    
    bp_counter = 0x7000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x2F0700], bp_counter);
    memcpy(raw_data, &rom[0x2F0000], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B4A3, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Title Screen");
    
    bp_counter = 0x2600;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x2F8DBE], bp_counter);
    memcpy(raw_data, &rom[0x2F86BE], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D463, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mode Screen BG");
    
    bp_counter = 0x3400;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x096A77], bp_counter);
    memcpy(raw_data, &rom[0x2F7FBE], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D563, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "File Screen BG3");
    
    bp_counter = 0x2000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x240690], bp_counter);
    memset(raw_data, 0, raw_counter);
    memcpy(&raw_data[0x200], &rom[0x240450], 0x240);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C203, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Nintendo 1994");
    
    bp_counter = 0x8000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x0F84A0], bp_counter);
    memcpy(raw_data, &rom[0x0F7DA0], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39D683, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "DK's Treehouse");
    
    bp_counter = 0x400;
    raw_counter = 0x2000;
    memcpy(bp_data, &rom[0x27FCE9], bp_counter);
    memcpy(raw_data, &rom[0x27DCE9], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39A01C, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cave FG3");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39AD65, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cave FG3 (Alt)");
    
    bp_counter = 0x1000;
    raw_counter = 0x700;
    memcpy(bp_data, &rom[0x3B6ED8], bp_counter);
    memcpy(raw_data, &rom[0x3B67D8], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x399C1C, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Water BG2");
    
    bp_counter = 0x1000;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x287A4A], bp_counter);
    memcpy(raw_data, &rom[0x288A2A], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39A2DC, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Walkway BG2 (Left)");
    
    raw_counter = 0x800;
    memcpy(raw_data, &rom[0x28922A], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39A2DC, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Walkway BG2 (Right)");
    
    bp_counter = 0xF00;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x34831D], bp_counter);
    memcpy(raw_data, &rom[0x3491FD], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x399A14, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mine Cart Carnage BG3");
    
    bp_counter = 0x1300;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x14EB52], bp_counter);
    memcpy(raw_data, &rom[0x14DF52], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B3A3, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Temple BG3");
    
    bp_counter = 0xFE0;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x22896D], bp_counter);
    memcpy(raw_data, &rom[0x22816D], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B0A3, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Oil Drum Alley BG3");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B0A3, raw_counter, bp_counter, 3, 63, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Blackout Basement BG3");
    
    bp_counter = 0x1800;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x238BFB], bp_counter);
    memcpy(raw_data, &rom[0x2383FB], raw_counter);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39C623, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Forest BG3");
    
    bp_counter = 0x21A0;
    raw_counter = 0x800;
    memcpy(&bp_data[0xE60], &rom[0x0C3BFE], bp_counter);
    memcpy(raw_data, &rom[0x0C33FE], raw_counter);
    bp_counter = 0x3000;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B2A3, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tree Top Town BG2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B1A3, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rope Bridge Rumble BG2");
    
    bp_counter = 0x1000;
    raw_counter = 0x800;
    memcpy(&bp_data[0x1000], &rom[0x318502], bp_counter);
    memcpy(raw_data, &rom[0x317D02], raw_counter);
    bp_counter = 0x2000;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B2A3, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tree Top Town BG3");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x39B1A3, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rope Bridge Rumble BG3");
    
    free( bp_data);
    free(raw_data);
    free(bitplane);

} // Special Screens (DKC)