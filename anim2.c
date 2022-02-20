#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

void anim2(uint8_t *rom, char dir[255], int priority, int region) {
    region = region; // Unused
    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    uint8_t *lay_data_a = malloc(65535);
    uint8_t *lay_data_b = malloc(65535);
    int  bp_counter;
    int raw_counter;
    int lay_counter_a;
    // int lay_counter_b;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    char name[255];
    #define bp_width 512
    
    int i;
    
    bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x207174);
    decomp(raw_data, rom, &raw_counter, 0x260FB1);
    decomp(lay_data_a, rom, &lay_counter_a, 0x245ADE);
    
    // memset(bp_data, 0, 0x8000);
    // bp_counter = 0x8000;
    
    
    // int lava_tiles[] = {
        // 0x3B4C80, 0x350147, 0x3B0DA0, 0x3B35C0,
        // 0x3B4E20, 0x3502C7, 0x3B1060, 0x3B3760,
        // 0x3B4FC0, 0x350447, 0x3B1320, 0x3B3900, // 1
        // 0x3B5160, 0x3505C7, 0x3B15E0, 0x3B3AA0, // 4
        // 0x3B5300, 0x350747, 0x3B18A0, 0x3B3C40,
        // 0x3B54A0, 0x3508C7, 0x3B1B60, 0x3B3DE0,
        // 0x3B5640, 0x350A47, 0x3B1E20, 0x3B3F80,
        // 0x3B57E0, 0x350BC7, 0x3B20E0, 0x3B4120,
        // 0x3B5980, 0x350D47, 0x3B23A0, 0x3B42C0,
        // 0x3B5B20, 0x350EC7, 0x3B2660, 0x3B4460,
        // 0x3B5CC0, 0x351047, 0x3B2920, 0x3B4600, // 2
        // 0x3B5E60, 0x3511C7, 0x3B2BE0, 0x3B47A0,
        // 0x3B6000, 0x351347, 0x3B2EA0, 0x3B4940,
        // 0x3B61A0, 0x3514C7, 0x3B3160, 0x3B4AE0,
        // 0x3B6340, 0x351647, 0x3B0AE0, 0x3B3420 // 3
    // };
    
    // uint8_t fake_layout[] = {
        // 0x73, 0x00, 0x7C, 0x00, 0xA2, 0x00, 0xAD, 0x00,
        // 0xBE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        
        // 0x00, 0x00, 0x00, 0x00, 0xA5, 0x00, 0xB0, 0x00,
        // 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // };
    // memcpy(&lay_data_a[0], fake_layout, 64);
    
    for (i = 0; i < 1; i++) {
        
            memcpy(&bp_data[0x340], &rom[0x3B4FC0], 0x1A0);
            memcpy(&bp_data[0x020], &rom[0x351047], 0x180);
            memcpy(&bp_data[0x4E0], &rom[0x3B0AE0], 0x2C0);
            memcpy(&bp_data[0x1A0], &rom[0x3B3AA0], 0x1A0);
        
            // memcpy(&bp_data[0x340], &rom[lava_tiles[i*4]  ], 0x1A0);
            // memcpy(&bp_data[0x020], &rom[lava_tiles[i*4+1]], 0x180);
            // memcpy(&bp_data[0x4E0], &rom[lava_tiles[i*4+2]], 0x2C0);
            // memcpy(&bp_data[0x1A0], &rom[lava_tiles[i*4+3]], 0x1A0);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0EF0, raw_counter, bp_counter, 1, 0, priority);
        // sprintf(name, "Lava Tiles-%02d.png", i);
        // assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Lava Crush-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF5D, 0, 16, 0, dir, "Fiery Furnace.png");
        
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0DF0, raw_counter, bp_counter, 1, 0, priority);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF49, 0, 16, 0, dir, "Hot Head Hop.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF8F, 0, 16, 0, dir, "Hot Head Hop Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF67, 0, 16, 0, dir, "Hot Head Hop Bonus 2 & Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF85, 0, 16, 0, dir, "Hot Head Hop Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF53, 0, 16, 0, dir, "Red-Hot Ride.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF99, 0, 16, 0, dir, "Red-Hot Ride Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFA3, 0, 16, 0, dir, "Red-Hot Ride Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF7B, 0, 16, 0, dir, "Kleever's Kiln.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF71, 0, 16, 0, dir, "Lava Room.png"); // ?
        
    
    }
    
    
    /*
    if (region == 1) {
        // Idiot! It's not the background, it's the terrain layer...
        bp_counter = 0;
        raw_counter = 0;
        lay_counter_a = 0;
        decomp( bp_data, rom,  &bp_counter, 0x20CF31);
        decomp(raw_data, rom, &raw_counter, 0x26303A);
        decomp(lay_data_a, rom, &lay_counter_a, 0x2479EE);
        
        int j;
        char names[][20] = {"Kannon's Klaim", "Squawks's Shaft", "Windy Well"};
        int palette[] = {0x3D228E, 0x3D238E, 0x3D248E};
        int layout[] = {0x35BFEB, 0x35BFE1, 0x35BFFF};
        for (i = 1; i < 2; i++) {
            for (j = 0; j < 31; j++) {
            sprintf(name, "%s %02d.png", names[i], j);
            decode_bitplane(rom, bp_data, raw_data, bitplane, palette[i], raw_counter, bp_counter, 1, (97+j), priority);
            assemble_level(bitplane, rom, lay_data_a, lay_counter_a, layout[i], 1, 24, 0, dir, name);
            }
        }
    }
    
    if (region == 1) {
        
        bp_counter = 0;
        raw_counter = 0;
        decomp( bp_data, rom,  &bp_counter, 0x2B2B84);
        decomp(raw_data, rom, &raw_counter, 0x19F7C9);
        
        for (i = 0; i < 31; i++) {
            decode_bitplane(rom, bp_data, raw_data, bitplane, 0, raw_counter, bp_counter, 3, (66+i), 0);
            sprintf(name, "Mine Background %02X.png", i);
            assemble_screen(bitplane, raw_counter, 32, dir, name);
        }
    }
    */
    /*
    if (region == 1) {
         bp_counter = 0x400;
        raw_counter = 0;
        decomp(raw_data, rom,  &raw_counter, 0x2D8415);
    int wind_map[8] = {0x3311EB, 0x3312FB, 0x33140B, 0x33151B, 0x33162B, 0x330EBB, 0x330FCB, 0x3310DB};
        memset(bp_data, 0, 0x800);
        
        for (i = 0; i < 8; i++) {
            memcpy(&bp_data[16], &rom[wind_map[i]], 0x120);
            decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D248E, raw_counter, bp_counter, 3, 0, 0);
            sprintf(name, "Windy Well Wind-%d.png", i);
            assemble_screen(bitplane, raw_counter, 32, dir, name);
        }
        
         bp_counter = 0x820;
        raw_counter = 0;
        decomp(raw_data, rom,  &raw_counter, 0x298B07);
        int gust_map[8] = {0x356C13, 0x356E33, 0x357053, 0x357273, 0x357493, 0x3576B3, 0x3578D3, 0x357AF3};
        memset(bp_data, 0, 0x800);
        
        for (i = 0; i < 8; i++) {
            memcpy(&bp_data[32], &rom[gust_map[i]], 0x220);
            decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D25AE, raw_counter, bp_counter, 2, 0, 0);
            sprintf(name, "Gusty Glade Wind-%d.png", i);
            assemble_screen(bitplane, raw_counter, 32, dir, name);
        }
    }

     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x207174);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x260FB1);
        decomp(lay_data_a, rom, &lay_counter_a, 0x245ADE);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x260FB1);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x245ADE);
    }
    
    int lava_tiles[] = {
        0x3B4C80, 0x350147, 0x3B0DA0, 0x3B35C0,
        0x3B4E20, 0x3502C7, 0x3B1060, 0x3B3760,
        0x3B4FC0, 0x350447, 0x3B1320, 0x3B3900,
        0x3B5160, 0x3505C7, 0x3B15E0, 0x3B3AA0,
        0x3B5300, 0x350747, 0x3B18A0, 0x3B3C40,
        0x3B54A0, 0x3508C7, 0x3B1B60, 0x3B3DE0,
        0x3B5640, 0x350A47, 0x3B1E20, 0x3B3F80,
        0x3B57E0, 0x350BC7, 0x3B20E0, 0x3B4120,
        0x3B5980, 0x350D47, 0x3B23A0, 0x3B42C0,
        0x3B5B20, 0x350EC7, 0x3B2660, 0x3B4460,
        0x3B5CC0, 0x351047, 0x3B2920, 0x3B4600,
        0x3B5E60, 0x3511C7, 0x3B2BE0, 0x3B47A0,
        0x3B6000, 0x351347, 0x3B2EA0, 0x3B4940,
        0x3B61A0, 0x3514C7, 0x3B3160, 0x3B4AE0,
        0x3B6340, 0x351647, 0x3B0AE0, 0x3B3420
    }; // 15*4
    
    for (i = 0; i < 16; i++) {
    
        if (i > 0) {
            memcpy(&bp_data[0x340], &rom[lava_tiles[((i-1)*4)]  ], 0x1A0);
            memcpy(&bp_data[0x020], &rom[lava_tiles[((i-1)*4)+1]], 0x180);
            memcpy(&bp_data[0x4E0], &rom[lava_tiles[((i-1)*4)+2]], 0x2C0);
            memcpy(&bp_data[0x1A0], &rom[lava_tiles[((i-1)*4)+3]], 0x1A0);
        }
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0DF0, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Hot Head Hop & Red-Hot Ride Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Hot Head Hop-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF49, 0, 16, 0, dir, name);
        sprintf(name, "Hot Head Hop Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF8F, 0, 16, 0, dir, name);
        sprintf(name, "Hot Head Hop Bonus 2 & Warp Room-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF67, 0, 16, 0, dir, name);
        sprintf(name, "Hot Head Hop Bonus 3-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF85, 0, 16, 0, dir, name);
        sprintf(name, "Red-Hot Ride-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF53, 0, 16, 0, dir, name);
        sprintf(name, "Red-Hot Ride Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF99, 0, 16, 0, dir, name);
        sprintf(name, "Red-Hot Ride Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFA3, 0, 16, 0, dir, name);
        sprintf(name, "Kleever's Kiln-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF7B, 0, 16, 0, dir, name);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0EF0, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Fiery Furnace Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Fiery Furnace-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF5D, 0, 16, 0, dir, name);
        
    }
    
    bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x29A905);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x268077);
        decomp(lay_data_a, rom, &lay_counter_a, 0x24F714);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x268077);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x24F714);
    }
    
    memcpy(&bp_data, &rom[0x2199BE], 0x257F);
    memcpy(&bp_data[0x4840], &rom[0x21BF3E], 0x37C0);
    bp_counter = 0x8000;
    
    int castle_tiles[] = {
        0x354BCA,
        0x354F4A,
        0x3552CA,
        0x35564A,
        0x3559CA,
        0x35484A
    }; // 6
    
    for (i = 0; i < 7; i++) {
        
        if (i > 0) {
            memcpy(&bp_data[0x020], &rom[castle_tiles[i-1]], 0x380);
        }
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Castle Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Castle Crush-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0F7, 1, 16, 0, dir, name);
        sprintf(name, "Castle Crush Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C133, 1, 16, 0, dir, name);
        sprintf(name, "Castle Crush Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C13D, 1, 16, 0, dir, name);
        sprintf(name, "Chain Link Chamber-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C10B, 1, 16, 0, dir, name);
        sprintf(name, "Chain Link Chamber Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C11F, 1, 16, 0, dir, name);
        sprintf(name, "Chain Link Chamber Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C129, 1, 16, 0, dir, name);
        sprintf(name, "Toxic Tower-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C115, 1, 16, 0, dir, name);
        sprintf(name, "Stronghold Showdown-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C147, 1, 16, 0, dir, name);
    }
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1FD537);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25E0C8);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23F9B2);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25E0C8);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23F9B2);
    }
    
    int mast_tiles[] = {
        0x3A7641,
        0x3A7A01,
        0x3A5FC1,
        0x3A6381,
        0x3A6741,
        0x3A6B01,
        0x3A6EC1,
        0x3A7281
    }; // 8
    
    for (i = 0; i < 9; i++) {
        
        if (i > 0) {
            memcpy(&bp_data[0x020], &rom[mast_tiles[i-1]], 0x3C0);
        }
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1610, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Mainbrace Mayhem Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Mainbrace Mayhem-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEB1, 1, 32, 0, dir, name);
        sprintf(name, "Mainbrace Mayhem Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BECF, 1, 32, 0, dir, name);
        sprintf(name, "Mainbrace Mayhem Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BED9, 1, 32, 0, dir, name);
        sprintf(name, "Mainbrace Mayhem Bonus 3-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEF7, 1, 32, 0, dir, name);
        sprintf(name, "Mainbrace Mayhem Warp Room-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEC5, 1, 32, 0, dir, name);
        sprintf(name, "Krow's Nest-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE93, 1, 32, 0, dir, name);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D14F0, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Topsail Trouble Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Topsail Trouble-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEA7, 1, 32, 0, dir, name);
        sprintf(name, "Topsail Trouble Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEED, 1, 32, 0, dir, name);
        sprintf(name, "Topsail Trouble Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEF7, 1, 32, 0, dir, name);
        sprintf(name, "Topsail Trouble Warp Room-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEC5, 1, 32, 0, dir, name);
        sprintf(name, "Kreepy Krow-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEBB, 1, 32, 0, dir, name);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D304E, raw_counter, bp_counter, 1, 0, priority);
        sprintf(name, "Slime Climb Tiles-%02d.png", i);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        sprintf(name, "Slime Climb-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE9D, 1, 32, 0, dir, name);
        sprintf(name, "Slime Climb Bonus 1-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEE3, 1, 32, 0, dir, name);
        sprintf(name, "Slime Climb Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF0B, 1, 32, 0, dir, name);
        sprintf(name, "Rattle Battle Bonus 2-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF15, 1, 32, 0, dir, name);

    }
    
    bp_counter = 0x64C0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    // decomp( bp_data, rom,  &bp_counter, 0x1F3059);
    memset(bp_data, 0, 0x64C0);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25A71C);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23B5D4);
        decomp(lay_data_b, rom, &lay_counter_b, 0x23A1C4);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25A71C);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23B5D4);
        decomp(lay_data_b, rom, &lay_counter_b, 25 + 0x23A1C4);
    }
    
    int hive_tiles[] = {
        0x351EA7, 0x351CC7, 0x351B87,
        0x351EA7, 0x351CC7, 0x351A47,
        0x351EA7, 0x351D67, 0x351907,
        0x351EA7, 0x351E07, 0x3517C7,
        0x351E07, 0x351EA7, 0x3517C7,
        0x351D67, 0x351EA7, 0x351907,
        0x351CC7, 0x351EA7, 0x351A47,
        0x351CC7, 0x351E07, 0x351B87,
        0x351CC7, 0x351D67, 0x351B87,
        0x351CC7, 0x351CC7, 0x351A47,
        0x351D67, 0x351F47, 0x351907,
        0x351E07, 0x351FE7, 0x3517C7, // 12
        0x351EA7, 0x351F47, 0x3517C7,
        0x351EA7, 0x351CC7, 0x351907,
        0x351EA7, 0x351F47, 0x351A47,
        0x351E07, 0x351FE7, 0x351B87,
        0x351D67, 0x351FE7, 0x351B87,
        0x351CC7, 0x351F47, 0x351A47,
        0x351F47, 0x351CC7, 0x351907,
        0x351FE7, 0x351CC7, 0x3517C7,
        0x351F47, 0x351CC7, 0x3517C7,
        0x351CC7, 0x351D67, 0x351907,
        0x351F47, 0x351E07, 0x351A47,
        0x351FE7, 0x351EA7, 0x351B87, // 24
        0x351F47, 0x351EA7, 0x351A47,
        0x351CC7, 0x351EA7, 0x351907,
        0x351CC7, 0x351EA7, 0x3517C7,
        0x351CC7, 0x351E07, 0x3517C7,
        0x351D67, 0x351D67, 0x351907,
        0x351E07, 0x351CC7, 0x351A47,
    }; // 30
    lay_data_a[0x00] = 0x20;
    lay_data_a[0x01] = 0x01;
    lay_data_a[0xA0] = 0x24;
    lay_data_a[0xA1] = 0x01;
    lay_data_a[0x02] = 0xF7;
    lay_data_a[0x03] = 0x00;
    lay_data_a[0xA2] = 0xFD;
    lay_data_a[0xA3] = 0x00;
    lay_data_a[0x04] = 0x18;
    lay_data_a[0x05] = 0x01;
    lay_data_a[0xA4] = 0x1E;
    lay_data_a[0xA5] = 0x01;
    
    
    
    for (i = 0; i < 30; i++) {
        
        memcpy(&bp_data[0x160], &rom[hive_tiles[((i-1)*3)  ]], 0x0A0);
        memcpy(&bp_data[0x200], &rom[hive_tiles[((i-1)*3)+1]], 0x0A0);
        memcpy(&bp_data[0x020], &rom[hive_tiles[((i-1)*3)+2]], 0x140);
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0A10, raw_counter, bp_counter, 1, 0, priority);
        // sprintf(name, "Hive Tiles-%02d.png", i);
        // assemble_bitplane(bitplane, bp_width, raw_counter, dir, name);
        
        sprintf(name, "Rambi Rumble-%02d.png", i);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDC9, 1, 80, 0, dir, name);
        
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDD3, 1, 80, 0, dir, name);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDF1, 1, 80, 0, dir, "Hornet Hole Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFC7, 1, 16, 0, dir, "Hornet Hole Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE05, 1, 80, 0, dir, "Hornet Hole Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDC9, 1, 80, 0, dir, "Rambi Rumble.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFD1, 1, 16, 0, dir, "Rambi Rumble Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE19, 1, 80, 0, dir, "Rambi Rumble Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDDD, 1, 80, 0, dir, "Rambi Rumble Rambi Room.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFB3, 1, 16, 0, dir, "Parrot Chute Panic.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE0F, 1, 80, 0, dir, "Parrot Chute Panic Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDFB, 1, 80, 0, dir, "Parrot Chute Panic Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFBD, 1, 16, 0, dir, "King Zing Sting.png");
    }
    */
    
    free( bp_data);
    free(raw_data);
    free(lay_data_a);
    free(lay_data_b);
    free(bitplane);

} // Animated tiles (DKC2)
