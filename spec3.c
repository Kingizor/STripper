#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"
#include "misc.h"

void spec3(uint8_t *rom, char dir[255], int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    int bp_counter;
    int raw_counter;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    int i;
    
    
    bp_counter = 0x520;
    raw_counter = 0x800;
    memset(raw_data, 0, 0x800);
    if (region == 1) {
        memcpy(bp_data, &rom[0x298030], 0x520);
        memcpy(&raw_data[0x340], &rom[0x297FB0], 0x80);
    }
    else if (region == 2) {
        memcpy(bp_data, &rom[0x298030 - 1], 0x520);
        memcpy(&raw_data[0x340], &rom[0x297FB0 - 1], 0x80);
    }
    else {
        memcpy(bp_data, &rom[0x298030 - 700], 0x520);
        memcpy(&raw_data[0x340], &rom[0x297FB0 - 700], 0x80);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7901, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Nintendo 1996");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x330200);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xD74, 0x330000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xDC1, 0x330000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7681, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Nintendo Presents 1");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x360B11);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xD89, 0x360000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xDD6, 0x360000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7681, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Nintendo Presents 2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0CFE3D);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xD98, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xDE5, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7701, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Title Screen");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29342E);
        decode(rom, raw_data, &raw_counter, 0xDF3, 0x290000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29342E - 1);
        decode(rom, raw_data, &raw_counter, 0xDF3, 0x290000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x293400);
        decode(rom, raw_data, &raw_counter, 0xDA6, 0x290000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7701, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Title Screen Text");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x13FE2A);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x5F5, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x63B, 0x2A0000);
    }
    raw_counter = 0x700;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8641, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "File Screen");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A077B);
        decode(rom, raw_data, &raw_counter, 0x649, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A077B - 1);
        decode(rom, raw_data, &raw_counter, 0x649, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A04B3);
        decode(rom, raw_data, &raw_counter, 0x603, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8641, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "File Screen VHS");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2AC294);
        decode(rom, raw_data, &raw_counter, 0xC9E, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2AC294 - 1);
        decode(rom, raw_data, &raw_counter, 0xC9E, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2ABFCC);
        decode(rom, raw_data, &raw_counter, 0xC51, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8841, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bonus Screen");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2AA8DC);
        decode(rom, raw_data, &raw_counter, 0xC63, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2AA8DC - 1);
        decode(rom, raw_data, &raw_counter, 0xC63, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2AA614);
        decode(rom, raw_data, &raw_counter, 0xC16, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8A01, raw_counter, bp_counter, 2, 14, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Grab 15 Bananas");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8A01, raw_counter, bp_counter, 2, 51, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Grab 15 Bananas 2P");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A9DCC);
        decode(rom, raw_data, &raw_counter, 0xA77, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A9DCC - 1);
        decode(rom, raw_data, &raw_counter, 0xA77, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A9B04);
        decode(rom, raw_data, &raw_counter, 0xA2A, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8841, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Collect the Stars");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8841, raw_counter, bp_counter, 2, 50, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Collect the Stars 2P");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2AB4DE);
        decode(rom, raw_data, &raw_counter, 0xC79, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2AB4DE - 1);
        decode(rom, raw_data, &raw_counter, 0xC79, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2AB216);
        decode(rom, raw_data, &raw_counter, 0xC2C, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8AE1, raw_counter, bp_counter, 2, 14, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Find the Coin");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8AE1, raw_counter, bp_counter, 2, 51, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Find the Coin 2P");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A8675);
        decode(rom, raw_data, &raw_counter, 0xC8F, 0x2A0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A8675 - 1);
        decode(rom, raw_data, &raw_counter, 0xC8F, 0x2A0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A83AD);
        decode(rom, raw_data, &raw_counter, 0xC42, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8A01, raw_counter, bp_counter, 2, 14, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bash the Baddies");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8A01, raw_counter, bp_counter, 2, 51, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bash the Baddies 2P");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0BFE6D);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x977, 0x1F0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x9C4, 0x1F0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7521, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Game Over Screen");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0EFD21);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x557, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x58F, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8461, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swanky's Sideshow (Default)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8461, raw_counter, bp_counter, 2, 45, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swanky's Sideshow (1)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8461, raw_counter, bp_counter, 2, 46, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swanky's Sideshow (2)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8461, raw_counter, bp_counter, 2, 47, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swanky's Sideshow (3)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x08FE75);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x3C6, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x3F0, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5B01, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cranky's Sideshow");
    
    // BG Scroll
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0A0000);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x3CD, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x3F7, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5B01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cranky's Sideshow Border");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0DFED2);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x49D, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x4C7, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8561, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Funky's Rentals");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x07FD8A);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x793, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x7E0, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6401, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Wrinkly's Save Cave (1)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6501, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Wrinkly's Save Cave (2)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6601, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Wrinkly's Save Cave (3)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6701, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Wrinkly's Save Cave (4)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x382E96);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xA06, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xA53, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 16, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - K's Kache");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 17, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Hill-Top Hoard");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 18, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Undercover Cove");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 19, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Kong Cave");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 20, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Bounty Beach");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 21, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Belcha's Burrow");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 22, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Smuggler's Cove");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 23, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Arich's Hoard");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 24, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Bounty Bay");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 25, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Sky-High Secret");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 26, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Glacial Grotto");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 27, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Clifftop Cache");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 28, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Sewer Stockpile");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2B04B0);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2B04B0 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2B01E8);
    }
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xA0D, 0x2B0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xA5A, 0x2B0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 16, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - K's Kache Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 17, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Hill-Top Hoard Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 18, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Undercover Cove Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 19, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Kong Cave Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 20, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Bounty Beach Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 21, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Belcha's Burrow Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 22, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Smuggler's Cove Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 23, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Arich's Hoard Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 24, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Bounty Bay Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 25, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Sky-High Secret Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 26, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Glacial Grotto Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 27, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Clifftop Cache Ice");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8741, raw_counter, bp_counter, 2, 28, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "BBC - Sewer Stockpile Ice");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x331DED);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xC92, 0x330000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xCDF, 0x330000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA239, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Banana Queen BG");
    
    bp_counter = 0; // Uses deductive 
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2B8538);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2B8538 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2B8270);
    }
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xCA0, 0x2B0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xCED, 0x2B0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA239, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Banana Queen Block & Sunlight");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x334FAD);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xCB6, 0x330000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xD03, 0x330000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA839, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Dixie's Photo Album BG");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 0) {
        decomp(bp_data, rom, &bp_counter, 0x293400);
        decode(rom, raw_data, &raw_counter, 0xCC4, 0x290000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x29342E); // ?
        decode(rom, raw_data, &raw_counter, 0xD11, 0x290000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA839, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Dixie's Photo Album Text");
    
    bp_counter = 0; // Palette?
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x334FAD);
    decomp(raw_data, rom, &raw_counter, 0x334C21);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA839, raw_counter, bp_counter, 2, 40, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph Category BG");
    
    uint8_t *temp_data = malloc(65535);
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 0) {
        decode(rom, temp_data, &raw_counter, 0xD15, 0x2B0000);
    }
    else {
        decode(rom, temp_data, &raw_counter, 0xD62, 0x2B0000);
    }
    boss_photo_layout(raw_data, temp_data, 0);
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2B1BC4);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2B1BC4 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2B18FC);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB139, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Belcha");
    boss_photo_layout(raw_data, temp_data, 1);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB239, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Arich");
    boss_photo_layout(raw_data, temp_data, 2);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB339, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Squirt");
    boss_photo_layout(raw_data, temp_data, 3);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB439, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - KAOS");
    boss_photo_layout(raw_data, temp_data, 4);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB539, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Bleak");
    boss_photo_layout(raw_data, temp_data, 5);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB639, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Barbos");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x380000);
    if (region == 0) {
        decode(rom, temp_data, &raw_counter, 0xCF0, 0x2B0000);
    }
    else {
        decode(rom, temp_data, &raw_counter, 0xD3D, 0x2B0000);
    }
    raw_counter = 0x800;
    bear_photo_layout(raw_data, temp_data, 0);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA999, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Bazaar & Barnacle");
    bear_photo_layout(raw_data, temp_data, 1);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DAC99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Brash & Blunder");
    bear_photo_layout(raw_data, temp_data, 2);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DAB99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Blue & Bazooka");
    bear_photo_layout(raw_data, temp_data, 3);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DAA99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Blizzard & Bramble");
    bear_photo_layout(raw_data, temp_data, 4);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DAD99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Benny, Bjorn & Barter");
    bear_photo_layout(raw_data, temp_data, 5);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA899, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Photograph - Baffle & Boomer");
    
    free(temp_data);
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x25968A);
    decode(rom, raw_data, &raw_counter, 0x1DD, 0x250000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lakeside Limbo BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBA99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tidal Trouble BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB999, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kreeping Klasps BG1");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x2594A2);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Boardwalk BG1-3");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x259533);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lakeside Limbo BG1-2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBA99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tidal Trouble BG1-2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB999, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kreeping Klasps BG1-2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x1CFF76);
    memcpy(&bp_data[0x1220], bp_data, bp_counter);
    bp_counter += 0x1220;
    decode(rom, raw_data, &raw_counter, 0x1F9, 0x1C0000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lakeside Limbo BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBA99, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tidal Trouble BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB999, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kreeping Klasps BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2BAB57);
        decode(rom, raw_data, &raw_counter, 0xD63, 0x2B0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2BAB57 - 1);
        decode(rom, raw_data, &raw_counter, 0xD63, 0x2B0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2BA88F);
        decode(rom, raw_data, &raw_counter, 0xD16, 0x2B0000);
    }
    memcpy(&raw_data[0x100], raw_data, raw_counter);
    memset(raw_data, 0, 0x100);
    raw_counter = 0x800;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 2, 42, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "K.Rool in Hovercraft");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x1DFE8F);
    memcpy(&bp_data[0x14A0], bp_data, bp_counter);
    bp_counter += 0x14A0;
    decomp(raw_data, rom, &raw_counter, 0x1DF873);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D61C1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Doorstop Dash BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D60C1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Squeals on Wheels BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Murky Mill BG1");
    
    bp_counter = 0;
    raw_counter = 0;
    memcpy(&bp_data[0x1720], &rom[0x1F04D4], 0x8D0);
    bp_counter = 0x8D0;
    bp_counter += 0x1720;
    memcpy(raw_data, &rom[0x1EFF14], 0x5C0);
    raw_counter = 0x5C0;
    lights_layout(raw_data, raw_counter);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6901, raw_counter, bp_counter, 3, 39, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Murky Mill Lights");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x370000);
    memcpy(&bp_data[0x1140], bp_data, bp_counter);
    bp_counter += 0x1140;
    memcpy(raw_data, &rom[0x36254B], 0x700);
    raw_counter = 0x700;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5FC1, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Skidda's Row BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBC99, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tearaway Toboggan BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBB99, raw_counter, bp_counter, 2, 29, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lemguin Lunge BG");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x299BFB);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x299BFB - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x299BFB - 700);
    }
    memcpy(&bp_data[0x1E40], bp_data, bp_counter);
    bp_counter += 0x1E40;
    decode(rom, raw_data, &raw_counter, 0x22C, 0x290000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6301, raw_counter, bp_counter, 2, 15, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Belcha");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29B6C0);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29B6C0 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x29B6C0 - 700);
    }
    memcpy(&bp_data[0x11C0], bp_data, bp_counter);
    bp_counter += 0x11C0;
    decode(rom, raw_data, &raw_counter, 0x23A, 0x290000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6301, raw_counter, bp_counter, 3, 15, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Belcha's Barn BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x2717A6);
    memcpy(&bp_data[0xBE0], bp_data, bp_counter);
    bp_counter += 0xBE0;
    decode(rom, raw_data, &raw_counter, 0x21D, 0x270000);
    memcpy(raw_data, &raw_data[0xC0], raw_counter-0xC0);
    raw_counter -= 0xC0; // THIS AWE
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5D01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Shield Bust-Up BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBD99, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Springin' Spiders BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBE79, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ripsaw Rage BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBF79, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swoopy Salvo BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5DE1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swoopy Salvo BG1 (AK)");
    // decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5EC1, raw_counter, bp_counter, 2, 0, 0);
    // assemble_screen(bitplane, raw_counter, 32, dir, "Arich's Ambush BG1"); // Not used in game.

    decode(rom, raw_data, &raw_counter, 0x21D, 0x270000);
    raw_counter = 0xC0;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBE79, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ripsaw Rage Saw");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBE79, raw_counter, bp_counter, 2, 64, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ripsaw Rage Saw (Alt)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x2703F1);
    decomp(raw_data, rom, &raw_counter, 0x26FCEA);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5D01, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Shield Bust-Up BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBD99, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Springin' Spiders BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBE79, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ripsaw Rage BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBF79, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swoopy Salvo BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5DE1, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swoopy Salvo BG2 (AK)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5EC1, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Arich's Ambush BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    memset(bp_data, 0, 0x2000);
    memset(raw_data, 0, 0x800);
    if (region == 1) {
        memcpy( &bp_data[0xBE0], &rom[0x298D12], 0xDA2);
        memcpy(&raw_data[0x21C], &rom[0x298550], 0x8A);
        memcpy(&raw_data[0x2D8], &rom[0x2985DA], 0x1CE);
    }
    else if (region == 2) {
        memcpy( &bp_data[0xBE0], &rom[0x298D12 - 1], 0xDA2);
        memcpy(&raw_data[0x21C], &rom[0x298550 - 1], 0x8A);
        memcpy(&raw_data[0x2D8], &rom[0x2985DA - 1], 0x1CE);
    }
    else {
        memcpy( &bp_data[0xBE0], &rom[0x298D12 - 700], 0xDA2);
        memcpy(&raw_data[0x21C], &rom[0x298550 - 700], 0x8A);
        memcpy(&raw_data[0x2D8], &rom[0x2985DA - 700], 0x1CE);
    }
    bp_counter = 0xDA2 + 0xBE0;
    raw_counter = 0x800;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5EC1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Arich");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x28861B);
    memcpy(&bp_data[0x6080], bp_data, bp_counter);
    bp_counter += 0x6080;
    decode(rom, raw_data, &raw_counter, 0x18E, 0x280000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7921, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Riverside Race BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7B21, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bobbing Barrel Brawl BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning Look-Out BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7C21, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning Look-Out BG1 (Bright)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x28A874);
    memcpy(&bp_data[0xEA0], bp_data, bp_counter);
    bp_counter += 0xEA0;
    memcpy(raw_data, &rom[0x28A634], 0x240);
    raw_counter = 0x240;
    surface_layout(raw_data, raw_counter);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7921, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Riverside Race BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7B21, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bobbing Barrel Brawl BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning Look-Out & Pothole Panic BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x28A874);
    memcpy(&bp_data[0x1EA0], bp_data, bp_counter);
    bp_counter += 0x1EA0;
    decomp(raw_data, rom, &raw_counter, 0x28A412);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 30, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 31, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 32, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-3");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 33, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-4");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 34, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-5");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 35, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-6");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 36, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-7");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 37, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-8");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 3, 38, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lightning-9");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        memcpy(&bp_data[0x1E00], &rom[0x2A160B], 0x1900);
    }
    else if (region == 2) {
        memcpy(&bp_data[0x1E00], &rom[0x2A160B - 1], 0x1900);
    }
    else {
        memcpy(&bp_data[0x1E00], &rom[0x2A1343], 0x1900);
    }
    bp_counter = 0x1900 + 0x1E00;
    decode(rom, raw_data, &raw_counter, 0x2F5, 0x2A0000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rocket Barrel Ride BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tracker Barrel Trek BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Drop Bounce BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5A01, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Squirt's Showdown BG1");
    
    bp_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A1349);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A1349 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A1081);
    }
    memcpy(&raw_data[0x200], bp_data, 0x300);
    if (region == 1) {
        memcpy(&bp_data[0x1E00], &rom[0x2A160B], 0x1900);
    }
    else if (region == 2) {
        memcpy(&bp_data[0x1E00], &rom[0x2A160B - 1], 0x1900);
    }
    else {
        memcpy(&bp_data[0x1E00], &rom[0x2A1343], 0x1900);
    }
    bp_counter = 0x1900 + 0x1E00;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5A01, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Squirt's Showdown BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A21AB);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A21AB - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A1EE3);
    }
    memcpy(&bp_data[0x1520], bp_data, bp_counter);
    bp_counter += 0x1520;
    raw_counter = 0x800;
    
    waterfall_layout(rom, raw_data, 0);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall SS (128)");
    
    waterfall_layout(rom, raw_data, 1);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall RBR (192)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall BDB (192)");
    
    waterfall_layout(rom, raw_data, 2);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (256L)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall BDB (256L)");
    
    waterfall_layout(rom, raw_data, 3);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (256R)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall BDB (256R)");
    
    waterfall_layout(rom, raw_data, 4);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (320L)");
    
    waterfall_layout(rom, raw_data, 5);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (320R)");
    
    waterfall_layout(rom, raw_data, 6);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (384L)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall BDB (384L)");
    
    waterfall_layout(rom, raw_data, 7);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (384R)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall BDB (384R)");
    
    waterfall_layout(rom, raw_data, 8);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (448&576L)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall RBR (576L)");
    
    waterfall_layout(rom, raw_data, 9);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (448&576M)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall RBR (576M)");
    
    waterfall_layout(rom, raw_data, 10);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (448R)");
    
    waterfall_layout(rom, raw_data, 11);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall TBT (576R)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Waterfall RBR (576R)");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2B14E2);
        decode(rom, raw_data, &raw_counter, 0xE3C, 0x2B0000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2B14E2 - 1);
        decode(rom, raw_data, &raw_counter, 0xE3C, 0x2B0000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2B121A);
        decode(rom, raw_data, &raw_counter, 0xDEF, 0x2B0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gleamin' Bream Underlay");
    
    bp_counter = 0;
    raw_counter = 0;
    bp_counter = 0xC70;
    if (region == 1) {
        memcpy(bp_data, &rom[0x29F818], 0xC70);
        decomp(raw_data, rom, &raw_counter, 0x29F346);
    }
    else if (region == 2) {
        memcpy(bp_data, &rom[0x29F818 - 1], 0xC70);
        decomp(raw_data, rom, &raw_counter, 0x29F346 - 1);
    }
    else {
        memcpy(bp_data, &rom[0x29F55C], 0xC70);
        decomp(raw_data, rom, &raw_counter, 0x29F08A);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8161, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bazza's Blockade BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB899, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Fish Food Frenzy BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 3, 9, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Floodlit Fish Dark BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 3, 10, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Floodlit Fish Light BG");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB799, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barbos' Barrier BG");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29BFBC);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29BFBC - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x29BD00);
    }
    decode(rom, raw_data, &raw_counter, 0x250, 0x290000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB799, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barbos");
    
    bp_counter = 0;
    raw_counter = 0;
    bp_counter = 0x1F60;
    raw_counter = 0x800;
    if (region == 1) {
        memcpy(bp_data, &rom[0x2C576C], 0x1F60);
        memcpy(raw_data, &rom[0x2C4F6C], 0x800);
    }
    else if (region == 2) {
        memcpy(bp_data, &rom[0x2C576C - 1], 0x1F60);
        memcpy(raw_data, &rom[0x2C4F6C - 1], 0x800);
    }
    else {
        memcpy(bp_data, &rom[0x2C54A4], 0x1F60);
        memcpy(raw_data, &rom[0x2C4CA4], 0x800);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9E39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Fire-Ball Frenzy BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA039, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Blazing Bazukas BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9F39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krack-Shot Kroc BG1");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29B6C0);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29B6C0 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x29B404);
    }
    memcpy(&bp_data[0x11C0], bp_data, bp_counter);
    bp_counter += 0x11C0;
    decode(rom, raw_data, &raw_counter, 0x154, 0x290000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9E39, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Factory BG2");

    bp_counter = 0;
    raw_counter = 0;
    bp_counter = 0x1200;
    raw_counter = 0x3E0;
    if (region == 1) {
        memcpy(bp_data, &rom[0x29E146], 0x1200);
        memcpy(raw_data, &rom[0x29DD86], 0x3E0);
    }
    else if (region == 2) {
        memcpy(bp_data, &rom[0x29E146 - 1], 0x1200);
        memcpy(raw_data, &rom[0x29DD86 - 1], 0x3E0);
    }
    else {
        memcpy(bp_data, &rom[0x29DE8A], 0x1200);
        memcpy(raw_data, &rom[0x29DACA], 0x3E0);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA139, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "KAOS");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A68F2);
        memcpy(raw_data, &rom[0x2A7D8A], 0x800);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A68F2 - 1);
        memcpy(raw_data, &rom[0x2A7D8A - 1], 0x800);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A662A);
        memcpy(raw_data, &rom[0x2A7AC2], 0x800);
    }
    raw_counter = 0x800;
    pipe_layout(raw_data, raw_counter);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7F61, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Low-G Labyrinth Smoke");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8061, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Poisonous Pipeline Smoke");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A6712);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A6712 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A644A);
    }
    memcpy(&bp_data[0x1D80], bp_data, bp_counter);
    bp_counter += 0x1D80;
    decode(rom, raw_data, &raw_counter, 0x2DF, 0x2A0000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8061, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Poisonous Pipeline BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A426A);
        decomp(raw_data, rom, &raw_counter, 0x2A5F1C);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A426A - 1);
        decomp(raw_data, rom, &raw_counter, 0x2A5F1C - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A3FA2);
        decomp(raw_data, rom, &raw_counter, 0x2A5C54);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D94F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krevice Kreepers BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D91F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kong-Fused Cliffs BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D93F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ropey Rumpus BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D95F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Criss Kross Cliffs BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D96F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rocket Rush BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D92F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cliffs BG1 (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2A3387);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2A3387 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2A30BF);
    }
    decode(rom, raw_data, &raw_counter, 0x105, 0x2A0000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D94F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krevice Kreepers BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D91F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kong-Fused Cliffs BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D93F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ropey Rumpus BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D95F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Criss Kross Cliffs BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D96F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rocket Rush BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D92F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cliffs BG2 (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x32587C);
    decode(rom, raw_data, &raw_counter, 0x0C4, 0x320000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA439, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Buzzer Barrage BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA639, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Creepy Caverns BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA539, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Pot Hole Panic BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA339, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tyrant Twin Tussle BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA739, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Tyrant Twin Tussle BG1 (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x32275C);
    decomp(raw_data, rom, &raw_counter, 0x324170);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9C39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Konveyor Rope Klash BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9B39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Koindozer Klamber BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9A39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Stampede Sprint BG1");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9D39, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Stampede Sprint BG1 (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x32470A);
    decode(rom, raw_data, &raw_counter, 0x130, 0x320000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9C39, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Konveyor Rope Klash BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9B39, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Koindozer Klamber BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9A39, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Stampede Sprint BG2");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9D39, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Stampede Sprint BG2 (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29481D);
        decode(rom, raw_data, &raw_counter, 0xE0A, 0x290000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29481D - 1);
        decode(rom, raw_data, &raw_counter, 0xE0A, 0x290000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x294561);
        decode(rom, raw_data, &raw_counter, 0xDBD, 0x290000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bleak BG1");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x29481D);
        decode(rom, raw_data, &raw_counter, 0xE11, 0x290000);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x29481D - 1);
        decode(rom, raw_data, &raw_counter, 0xE11, 0x290000);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x294561);
        decode(rom, raw_data, &raw_counter, 0xDC4, 0x290000);
    }
    i = 0;
    uint8_t *data = malloc(0xFFFF);
    decomp(data, rom, &i, 0x370000);
    memcpy(&bp_data[0x5140], data, i);
    bp_counter += i;
    free(data);
    memmove(&raw_data[0x1C0], raw_data, raw_counter);
    for (i = 0; i < 0x200; i+=2) {
        raw_data[i]   = rom[0x3625CB+(i ^ 0x20)];
        raw_data[i+1] = rom[0x3625CB+(i ^ 0x20)+1] + 2;
    }
    raw_counter += 0x200;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bleak BG2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x3B0000);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Generic)");
    // The cabins of Blunder, Bazooka and Baffle have transparent spots where the next layer can be seen.
    // It's definitely not a bug here, that's just how the game does it.
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x3B0000);
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x0F, 0xFAC7, 0x350000);
    bear_layout(raw_data, rom, 0x10, 0xFACA, 0x350000);
    bear_layout(raw_data, rom, 0x07, 0xFACD, 0x350000);
    bear_layout(raw_data, rom, 0x01, 0xFAD0, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFAD3, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Bazaar)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    flip_bear_layout(raw_data);
    bear_layout(raw_data, rom, 0x91, 0xFB35, 0x350000);
    bear_layout(raw_data, rom, 0x92, 0xFB38, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFB3B, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6F01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Barnacle)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x13, 0xFB4F, 0x350000);
    bear_layout(raw_data, rom, 0x14, 0xFB52, 0x350000);
    bear_layout(raw_data, rom, 0x15, 0xFB55, 0x350000);
    bear_layout(raw_data, rom, 0x02, 0xFB58, 0x350000);
    bear_layout(raw_data, rom, 0x25, 0xFB5B, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7001, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Brash)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x16, 0xFAEA, 0x350000);
    bear_layout(raw_data, rom, 0x17, 0xFAED, 0x350000);
    bear_layout(raw_data, rom, 0x02, 0xFAF0, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6D01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Blunder)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x18, 0xFB6C, 0x350000);
    bear_layout(raw_data, rom, 0x19, 0xFB6F, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFB72, 0x350000);
    bear_layout(raw_data, rom, 0x00, 0xFB75, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7101, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Blue)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x82, 0xFB80, 0x350000);
    bear_layout(raw_data, rom, 0x1D, 0xFB83, 0x350000);
    bear_layout(raw_data, rom, 0x1E, 0xFB86, 0x350000);
    bear_layout(raw_data, rom, 0x00, 0xFB89, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Bazooka)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    flip_bear_layout(raw_data);
    bear_layout(raw_data, rom, 0x80, 0xFB04, 0x350000);
    bear_layout(raw_data, rom, 0x23, 0xFB07, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFB0A, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6E01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Bramble)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x1A, 0xFB9D, 0x350000);
    bear_layout(raw_data, rom, 0x1B, 0xFBA0, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFBA3, 0x350000);
    bear_layout(raw_data, rom, 0x07, 0xFBA6, 0x350000);
    bear_layout(raw_data, rom, 0x01, 0xFBA9, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6C01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Blizzard)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x1C, 0xFB1B, 0x350000);
    bear_layout(raw_data, rom, 0x00, 0xFB1E, 0x350000);
    bear_layout(raw_data, rom, 0x03, 0xFB21, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Barter)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    flip_bear_layout(raw_data);
    bear_layout(raw_data, rom, 0xA2, 0xFBC3, 0x350000);
    bear_layout(raw_data, rom, 0x03, 0xFBC6, 0x350000);
    bear_layout(raw_data, rom, 0x03, 0xFBC9, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7201, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Benny)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x22, 0xFBDD, 0x350000);
    bear_layout(raw_data, rom, 0x02, 0xFBE0, 0x350000);
    bear_layout(raw_data, rom, 0x04, 0xFBE3, 0x350000);
    bear_layout(raw_data, rom, 0x00, 0xFBE6, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7201, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Bjorn)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    bear_layout(raw_data, rom, 0x1F, 0xFBFA, 0x350000);
    bear_layout(raw_data, rom, 0x06, 0xFBFD, 0x350000);
    bear_layout(raw_data, rom, 0x24, 0xFC00, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Baffle)");
    
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x6D5, 0x290000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x722, 0x290000);
    }
    flip_bear_layout(raw_data);
    bear_layout(raw_data, rom, 0x02, 0xFC14, 0x350000);
    bear_layout(raw_data, rom, 0x20, 0xFC17, 0x350000);
    bear_layout(raw_data, rom, 0x21, 0xFC1A, 0x350000);
    bear_layout(raw_data, rom, 0x26, 0xFC1D, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6E01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Boomer)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7301, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin (Boomer) (AK)");
    
    bp_counter = 0;
    raw_counter = 0x800;
    decomp(bp_data, rom, &bp_counter, 0x3B6897);
    memcpy(&bp_data[0x1000], bp_data, bp_counter);
    bp_counter += 0x1000;
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x49, 0xFAD6, 0x350000);
    bear_layout(raw_data, rom, 0x4D, 0xFAD9, 0x350000);
    bear_layout(raw_data, rom, 0x4A, 0xFADC, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFADF, 0x350000);
    bear_layout(raw_data, rom, 0x4C, 0xFAE2, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFAE5, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Bazaar)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0xCD, 0xFB3E, 0x350000);
    bear_layout(raw_data, rom, 0xCA, 0xFB41, 0x350000);
    bear_layout(raw_data, rom, 0x48, 0xFB44, 0x350000);
    bear_layout(raw_data, rom, 0x49, 0xFB47, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB4A, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6F01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Barnacle)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x4D, 0xFB5E, 0x350000);
    bear_layout(raw_data, rom, 0xCB, 0xFB61, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFB64, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB67, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7001, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Brash)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x4D, 0xFAF3, 0x350000);
    bear_layout(raw_data, rom, 0xCC, 0xFAF6, 0x350000);
    bear_layout(raw_data, rom, 0xCB, 0xFAF9, 0x350000);
    bear_layout(raw_data, rom, 0x4A, 0xFAFC, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFAFF, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6D01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Blunder)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x4D, 0xFB78, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB7B, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7101, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Blue)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x48, 0xFB8C, 0x350000);
    bear_layout(raw_data, rom, 0x49, 0xFB8F, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFB92, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFB95, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB98, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Bazooka)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x4B, 0xFB0D, 0x350000);
    bear_layout(raw_data, rom, 0x4D, 0xFB10, 0x350000);
    bear_layout(raw_data, rom, 0xC9, 0xFB13, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB16, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6E01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Bramble)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0xCB, 0xFBAC, 0x350000);
    bear_layout(raw_data, rom, 0x4D, 0xFBAF, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFBB2, 0x350000);
    bear_layout(raw_data, rom, 0x4C, 0xFBB5, 0x350000);
    bear_layout(raw_data, rom, 0xCC, 0xFBB8, 0x350000);
    bear_layout(raw_data, rom, 0xCB, 0xFBBB, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFBBE, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6C01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Blizzard)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x49, 0xFB24, 0x350000);
    bear_layout(raw_data, rom, 0x4A, 0xFB27, 0x350000);
    bear_layout(raw_data, rom, 0xCD, 0xFB2A, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFB2D, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFB30, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Barter)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0xC9, 0xFBCC, 0x350000);
    bear_layout(raw_data, rom, 0xC8, 0xFBCF, 0x350000);
    bear_layout(raw_data, rom, 0x4A, 0xFBD2, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFBD5, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFBD8, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7201, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Benny)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0xCB, 0xFBE9, 0x350000);
    bear_layout(raw_data, rom, 0x4A, 0xFBEC, 0x350000);
    bear_layout(raw_data, rom, 0x4D, 0xFBEF, 0x350000);
    bear_layout(raw_data, rom, 0x4B, 0xFBF2, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFBF5, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7201, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Bjorn)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0x4A, 0xFC03, 0x350000);
    bear_layout(raw_data, rom, 0xCD, 0xFC06, 0x350000);
    bear_layout(raw_data, rom, 0xC9, 0xFC09, 0x350000);
    bear_layout(raw_data, rom, 0xC8, 0xFC0C, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFC0F, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6A01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Baffle)");
    
    memset(raw_data, 0, 0x800);
    bear_layout(raw_data, rom, 0xCC, 0xFC20, 0x350000);
    bear_layout(raw_data, rom, 0xCB, 0xFC23, 0x350000);
    bear_layout(raw_data, rom, 0x0E, 0xFC26, 0x350000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6E01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Boomer)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7301, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bear Cabin BG (Boomer) (AK)");
    
    // Overworld uses different slightly different palettes depending on what's on the screen.
    // The normal palette causes the northwestern clouds to appear orange.
    // Alternate colours are used to make the clouds white.
    // However, this causes Funky's Rentals and possibly other areas to appear white.
    
    uint8_t *alt_bitplane = malloc(512*2000*4);
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x350000);
    decomp(raw_data, rom, &raw_counter, 0x320000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7421, raw_counter, bp_counter, 2, 0, 0);
    decode_bitplane_3(region, rom, bp_data, raw_data, alt_bitplane, 0x3D7421, raw_counter, bp_counter, 2, 13, 0); // Overlapping palette
    memcpy(bitplane, alt_bitplane, 90*4*256); // 90 rows * 4 bytes per pixel * 256 pixels wide
    assemble_screen(bitplane, raw_counter, 32, dir, "Overworld");
    free(alt_bitplane);
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x321DA4);
    decomp(raw_data, rom, &raw_counter, 0x3215A9);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7421, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Overworld (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0FFF7E);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x99B, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x9E8, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7D41, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lake Orangatanga");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x10FE01);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x9A9, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x9F6, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7D41, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lake Orangatanga (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x0AFDB5);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x7EC, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x839, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6801, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kremwood Forest");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x11FF7D);
    decomp(raw_data, rom, &raw_counter, 0x28734F);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6801, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kremwood Forest (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x04F05F);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xA47, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xA94, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8BE1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cotton-Top Cove");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x36688A);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xA39, 0x360000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xA86, 0x360000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8BE1, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cotton-Top Cove (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x14FFD7);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xB20, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xB6D, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8CE1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mekanos");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x367029);
    decomp(raw_data, rom, &raw_counter, 0x366EE1);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8CE1, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mekanos (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x06FEFE);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0x704, 0x280000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0x751, 0x280000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6B01, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "K3");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x37266A);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xB79, 0x370000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xBC6, 0x370000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8DE1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Razor Ridge");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x3673D5);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xB6B, 0x360000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xBB8, 0x360000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8DE1, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Razor Ridge (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x01FC3C);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xB9D, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xBEA, 0x2B0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8EE1, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "KAOS Kore");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x367816);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xBAB, 0x360000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xBF8, 0x360000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8EE1, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "KAOS Kore (Water)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x1BFE0E);
    if (region == 0) {
        decode(rom, raw_data, &raw_counter, 0xBEB, 0x2A0000);
    }
    else {
        decode(rom, raw_data, &raw_counter, 0xC38, 0x2A0000);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8FF9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krematoa");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D90F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krematoa (AK)");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp(bp_data, rom, &bp_counter, 0x367B9A);
    decomp(raw_data, rom, &raw_counter, 0x367A94);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8FF9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krematoa (Water)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D90F9, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krematoa (Water)(AK)");
    
    // This one uses HDMA to achieve the rippling effect and deductive transparency.
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2B0B39);
        decomp(raw_data, rom, &raw_counter, 0x2B081C);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2B0B39 - 1);
        decomp(raw_data, rom, &raw_counter, 0x2B081C - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2B0871);
        decomp(raw_data, rom, &raw_counter, 0x2B0554);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8FF9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Water");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom, &bp_counter, 0x28BDEC);
    decode(rom, raw_data, &raw_counter, 0x289, 0x280000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D97F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kastle KAOS (Left)");

    raw_counter = 0;
    decode(rom, raw_data, &raw_counter, 0x290, 0x280000);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D97F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kastle KAOS (Right)(1)");
    
    for (i = 0; i < 10; i++) {
        memcpy(&raw_data[0x098 + (i*0x40)], &rom[0x28B2EC + (i*0x0C)], 0x0C);
    }
    for (i = 0; i < 11; i++) {
        memcpy(&raw_data[0x318 + (i*0x40)], &rom[0x28B364 + (i*0x14)], 0x14);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D97F9, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kastle KAOS (Right)(2)");
    
    bp_counter = 0;
    raw_counter = 0;
    bp_counter = 0x1760 + 0x8A0;
    memset(raw_data, 0, 0x800);
    if (region == 1) {
        memcpy(bp_data, &rom[0x29E146], 0x1200);
        memcpy(&bp_data[0x1760], &rom[0x2C475C], 0x8A0);
        memcpy(&raw_data[0x200], &rom[0x2C451C], 0x240);
    }
    else if (region == 2) {
        memcpy(bp_data, &rom[0x29E146 - 1], 0x1200);
        memcpy(&bp_data[0x1760], &rom[0x2C475C - 1], 0x8A0);
        memcpy(&raw_data[0x200], &rom[0x2C451C - 1], 0x240);
    }
    else {
        memcpy(bp_data, &rom[0x29DE8A], 0x1200);
        memcpy(&bp_data[0x1760], &rom[0x2C4494], 0x8A0);
        memcpy(&raw_data[0x200], &rom[0x2C4254], 0x240);
    }
    // memcpy(raw_data[0x480], rom[0x345CC0], 0x280);
    raw_counter = 0x800;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D97F9, raw_counter, bp_counter, 3, 48, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kastle KAOS BG3 (1)");
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D97F9, raw_counter, bp_counter, 3, 49, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kastle KAOS BG3 (2)");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2BDC2A);
        decomp(raw_data, rom, &raw_counter, 0x2C3EB2);
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2BDC2A - 1);
        decomp(raw_data, rom, &raw_counter, 0x2C3EB2 - 1);
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2BD962);
        decomp(raw_data, rom, &raw_counter, 0x2C3BEA);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9939, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Knautilus (Left)");
    
    raw_counter = 0;
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2C3825);
    }
    else if (region == 2) {
        decomp(raw_data, rom, &raw_counter, 0x2C3825 - 1);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 0x2C355D);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9939, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Knautilus (Right)");
    
    bp_counter = 0x1800;
    raw_counter = 0x800;
    memcpy(bp_data, &rom[0x345F40], 0x1800);
    memset(raw_data, 0, 0x800);
    memcpy(&raw_data[0x2C0], &rom[0x345CC0], 0x1C0);
    memcpy(&raw_data[0x440], &rom[0x345CC0], 0x1C0);
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9939, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Knautilus BG1");
    
    bp_counter = 0;
    raw_counter = 0x800;
    memset(bp_data, 0, 0x1800);
    memset(raw_data, 0, 0x800);
    if (region == 1) {
        decomp(bp_data, rom, &bp_counter, 0x2BD1D2);
        memcpy(&raw_data[0x180], &rom[0x2BCD52], 0x300); // BG3
        memcpy(&raw_data[0x5D0], &rom[0x2BD062], 0xB0);  // Conveyor
    }
    else if (region == 2) {
        decomp(bp_data, rom, &bp_counter, 0x2BD1D2 - 1);
        memcpy(&raw_data[0x180], &rom[0x2BCD52 - 1], 0x300); // BG3
        memcpy(&raw_data[0x5D0], &rom[0x2BD062 - 1], 0xB0);  // Conveyor
    }
    else {
        decomp(bp_data, rom, &bp_counter, 0x2BCF0A);
        memcpy(&raw_data[0x180], &rom[0x2BCA8A], 0x300); // BG3
        memcpy(&raw_data[0x5D0], &rom[0x2BCD9A], 0xB0);  // Conveyor
    }
    memcpy(&bp_data[0x13D0], bp_data, bp_counter);
    memcpy(bp_data, &bp_data[0x130], 0x10);
    bp_counter += 0x13D0;
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9939, raw_counter, bp_counter, 3, 41, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Knautilus BG (Left)");
    
    // HDMA is required to maintain the palette.
    raw_counter = 0x800;
    if (region == 1) {
        memcpy(&raw_data[0x180], &rom[0x2BCA52], 0x300);
        memcpy(&raw_data[0x5C0], &rom[0x2BD112], 0xC0);
    }
    else if (region == 2) {
        memcpy(&raw_data[0x180], &rom[0x2BCA52 - 1], 0x300);
        memcpy(&raw_data[0x5C0], &rom[0x2BD112 - 1], 0xC0);
    }
    else {
        memcpy(&raw_data[0x180], &rom[0x2BC78A], 0x300);
        memcpy(&raw_data[0x5C0], &rom[0x2BCE4A], 0xC0);
    }
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9939, raw_counter, bp_counter, 3, 41, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Knautilus BG (Right)");
    
    free( bp_data);
    free(raw_data);
    free(bitplane);
    
} // Special Screens (DKC3)