#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"
#include "misc.h"

void level3(uint8_t *rom, char dir[255], int priority, int special, int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    uint8_t *lay_data_a = malloc(65535);
    uint8_t *lay_data_b = malloc(65535);
    int bp_counter;
    int raw_counter;
    int lay_counter_a;
    int lay_counter_b;
    
    int bp_width = 512;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    // Cave
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x208E80);
    decomp(raw_data, rom, &raw_counter, 0x2077C6);
    decomp(lay_data_a, rom, &lay_counter_a, 0x205BC3);
    decomp(lay_data_b, rom, &lay_counter_b, 0x20F035);
    
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA439, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 96, 0, dir, "Caves-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 0, 32, 0, dir, "Caves-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Buzzer Barrage Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B08D, 1, 96, 0, dir, "Buzzer Barrage.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0AC, 0, 32, 0, dir, "Bazza's Blockade Cave.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0B6, 0, 32, 0, dir, "Fish Food Frenzy Cave.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0C0, 0, 32, 0, dir, "Floodlit Fish Cave.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0CA, 0, 32, 0, dir, "Buzzer Barrage Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0FC, 0, 32, 0, dir, "Buzzer Barrage Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA639, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Creepy Caverns Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B097, 1, 96, 0, dir, "Creepy Caverns.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0FC, 0, 32, 0, dir, "Creepy Caverns Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B106, 0, 32, 0, dir, "Creepy Caverns Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA539, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Pot Hole Panic Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0D4, 0, 32, 0, dir, "Pot Hole Panic.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0E8, 0, 32, 0, dir, "Pot Hole Panic Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0F2, 0, 32, 0, dir, "Pot Hole Panic Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA339, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Tyrant Twin Tussle Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0DE, 0, 32, 0, dir, "Tyrant Twin Tussle.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B110, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B11A, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B124, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 3.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA739, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Tyrant Twin Tussle (Krematoa) Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B0DE, 0, 32, 0, dir, "Tyrant Twin Tussle (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B110, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 1 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B11A, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 2 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B124, 0, 32, 0, dir, "Tyrant Twin Tussle Bonus 3 (Krematoa).png");
    }
    
    // Water
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x215058);
    decomp(raw_data, rom, &raw_counter, 0x212377);
    decomp(lay_data_a, rom, &lay_counter_a, 0x210E48);
    decomp(lay_data_b, rom, &lay_counter_b, 0x219CD8);
    
    if (special == 2) {
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8161, raw_counter, bp_counter, 1, 0, priority);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 32, 0, dir, "Water-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 80, 0, dir, "Water-2.png");
    }
    else {
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB799, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Barbos' Barrier Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B139, 1, 32, 0, dir, "Barbos' Barrier.png");

        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8161, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Bazza's Blockade Tiles (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B158, 1, 80, 0, dir, "Bazza's Blockade (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B162, 1, 80, 0, dir, "Bazza's Blockade Bonus 1 (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B16C, 1, 80, 0, dir, "Bazza's Blockade Bonus 2 (Natural).png");

        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8161, raw_counter, bp_counter, 1, 52, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Bazza's Blockade Tiles (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B158, 1, 80, 0, dir, "Bazza's Blockade (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B162, 1, 80, 0, dir, "Bazza's Blockade Bonus 1 (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B16C, 1, 80, 0, dir, "Bazza's Blockade Bonus 2 (Fixed).png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Floodlit Fish Tiles (Natural).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B143, 1, 32, 0, dir, "Floodlit Fish (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B18A, 1, 80, 0, dir, "Floodlit Fish Bonus 1 (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B194, 1, 80, 0, dir, "Floodlit Fish Bonus 2 (Natural).png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 1, 9, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Floodlit Fish Tiles (Dark).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B143, 1, 32, 0, dir, "Floodlit Fish (Dark).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B18A, 1, 80, 0, dir, "Floodlit Fish Bonus 1 (Dark).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B194, 1, 80, 0, dir, "Floodlit Fish Bonus 2 (Dark).png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8261, raw_counter, bp_counter, 1, 10, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Floodlit Fish Tiles (Light).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B143, 1, 32, 0, dir, "Floodlit Fish (Light).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B18A, 1, 80, 0, dir, "Floodlit Fish Bonus 1 (Light).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B194, 1, 80, 0, dir, "Floodlit Fish Bonus 2 (Light).png");

        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB899, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Fish Food Frenzy Tiles (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B19E, 1, 80, 0, dir, "Fish Food Frenzy (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B176, 1, 80, 0, dir, "Fish Food Frenzy Bonus 1 (Natural).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B180, 1, 80, 0, dir, "Fish Food Frenzy Bonus 2 (Natural).png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB899, raw_counter, bp_counter, 1, 52, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Fish Food Frenzy Tiles (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B19E, 1, 80, 0, dir, "Fish Food Frenzy (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B176, 1, 80, 0, dir, "Fish Food Frenzy Bonus 1 (Fixed).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B180, 1, 80, 0, dir, "Fish Food Frenzy Bonus 2 (Fixed).png");
    }
    
    // Cliff
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp(     bp_data, rom,  &bp_counter, 0x221914);
    decomp(    raw_data, rom, &raw_counter, 0x21EB1B);
    decomp(lay_data_a, rom, &lay_counter_a, 0x21BEB6);
    decomp(lay_data_b, rom, &lay_counter_b, 0x2275C9);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D94F9, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 24, 0, dir, "Cliffs-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 80, 0, dir, "Cliffs-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Krevice Kreepers Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1B3, 1, 24, 0, dir, "Krevice Kreepers.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B1F0, 1, 80, 0, dir, "Krevice Kreepers Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B1FA, 1, 80, 0, dir, "Krevice Kreepers Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D91F9, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Kong-Fused Cliffs Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1C7, 1, 24, 0, dir, "Kong-Fused Cliffs.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B204, 1, 80, 0, dir, "Kong-Fused Cliffs Bonuses 1&2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D93F9, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Ropey Rumpus Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B1E6, 1, 80, 0, dir, "Ropey Rumpus.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B20E, 1, 80, 0, dir, "Ropey Rumpus Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B218, 1, 80, 0, dir, "Ropey Rumpus Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D95F9, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Criss Kross Cliffs Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1BD, 1, 24, 0, dir, "Criss Kross Cliffs.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B222, 1, 80, 0, dir, "Criss Kross Cliffs Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B22C, 1, 80, 0, dir, "Criss Kross Cliffs Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D96F9, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Rocket Rush Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1D1, 1, 24, 0, dir, "Rocket Rush.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D92F9, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Rocket Rush & Criss Kross Cliffs Tiles (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1D1, 1, 24, 0, dir, "Rocket Rush (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B1BD, 1, 24, 0, dir, "Criss Kross Cliffs (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B222, 1, 80, 0, dir, "Criss Kross Cliffs Bonus 1 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B22C, 1, 80, 0, dir, "Criss Kross Cliffs Bonus 2 (Krematoa).png");
    }
    
    // Jungle
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x22D9E8);
    if (region == 0) {
        if (rom[0xFFDB] == 0) {
            memcpy(&bp_data[0x020], &rom[0x2BBD80], 0x140);
            memcpy(&bp_data[0x160], &rom[0x2BBD80], 0x140);
            memcpy(&bp_data[0x2A0], &rom[0x2BBD80], 0x80);
        }
        else {
            memcpy(&bp_data[0x020], &rom[0x2BBD8A], 0x140);
            memcpy(&bp_data[0x160], &rom[0x2BBD8A], 0x140);
            memcpy(&bp_data[0x2A0], &rom[0x2BBD8A], 0x80);
        }
    }
    else if (region == 1) {
        memcpy(&bp_data[0x020], &rom[0x2BC052], 0x140);
        memcpy(&bp_data[0x160], &rom[0x2BC052], 0x140);
        memcpy(&bp_data[0x2A0], &rom[0x2BC052], 0x80);
    }
    else {
        memcpy(&bp_data[0x020], &rom[0x2BC051], 0x140);
        memcpy(&bp_data[0x160], &rom[0x2BC051], 0x140);
        memcpy(&bp_data[0x2A0], &rom[0x2BC051], 0x80);
    }
    
    decomp(raw_data, rom, &raw_counter, 0x22AEED);
    decomp(lay_data_a, rom, &lay_counter_a, 0x22849B);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9C39, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Jungle-DKC3.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Konveyor Rope Klash Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B24B, 0, 16, 0, dir, "Konveyor Rope Klash.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B273, 0, 16, 0, dir, "Konveyor Rope Klash Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B27D, 0, 16, 0, dir, "Konveyor Rope Klash Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9B39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Koindozer Klamber Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B241, 0, 16, 0, dir, "Koindozer Klamber.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B25F, 0, 16, 0, dir, "Koindozer Klamber Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B269, 0, 16, 0, dir, "Koindozer Klamber Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9A39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Stampede Sprint Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B255, 0, 16, 0, dir, "Stampede Sprint.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B287, 0, 16, 0, dir, "Stampede Sprint Bonuses 1&2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B291, 0, 16, 0, dir, "Stampede Sprint Bonus 3.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9D39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Stampede Sprint Tiles (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B255, 0, 16, 0, dir, "Stampede Sprint (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B287, 0, 16, 0, dir, "Stampede Sprint Bonuses 1&2 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B291, 0, 16, 0, dir, "Stampede Sprint Bonus 3 (Krematoa).png");
    }
    
    // Le Factoire
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x238D56);
    decomp(raw_data, rom, &raw_counter, 0x237402);
    decomp(lay_data_a, rom, &lay_counter_a, 0x233DD3);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9E39, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Factory.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Fire-Ball Frenzy Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2A6, 1, 80, 0, dir, "Fire-Ball Frenzy.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2CE, 1, 80, 0, dir, "Fire-Ball Frenzy Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2D8, 1, 80, 0, dir, "Fire-Ball Frenzy Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA039, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Blazing Bazukas Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2B0, 1, 80, 0, dir, "Blazing Bazukas.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2C4, 1, 80, 0, dir, "Blazing Bazukas Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2EC, 1, 80, 0, dir, "Blazing Bazukas Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D9F39, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Krack Shot Kroc Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2BA, 1, 80, 0, dir, "Krack Shot Kroc.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2D8, 1, 80, 0, dir, "Krack Shot Kroc Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2E2, 1, 80, 0, dir, "Krack Shot Kroc Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DA139, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "KAOS Karnage Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B2F6, 1, 80, 0, dir, "KAOS Karnage.png");
    }
    
    // Mill
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x243549);
    decomp(raw_data, rom, &raw_counter, 0x241148);
    decomp(lay_data_a, rom, &lay_counter_a, 0x23E3B9);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D61C1, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Mill.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Doorstop Dash Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B30B, 1, 80, 0, dir, "Doorstop Dash.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B333, 1, 80, 0, dir, "Doorstop Dash Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B33D, 1, 80, 0, dir, "Doorstop Dash Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D60C1, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Squeals On Wheels Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B315, 1, 80, 0, dir, "Squeals On Wheels.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B347, 1, 80, 0, dir, "Squeals On Wheels Bonuses 1&2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6901, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Murky Mill Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B31F, 1, 80, 0, dir, "Murky Mill.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B351, 1, 80, 0, dir, "Murky Mill Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B35B, 1, 80, 0, dir, "Murky Mill Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D6301, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Belcha's Barn Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B329, 1, 80, 0, dir, "Belcha's Barn.png");
    }
    
    // Riverside
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x24D25F);
    decomp(raw_data, rom, &raw_counter, 0x24AEF3);
    decomp(lay_data_a, rom, &lay_counter_a, 0x24890B);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7921, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 1, dir, "Riverside.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Riverside Race Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B370, 0, 16, 1, dir, "Riverside Race.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B37A, 0, 16, 1, dir, "Riverside Race Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B384, 0, 16, 1, dir, "Riverside Race Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7B21, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Bobbing Barrel Brawl Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3C0, 0, 16, 1, dir, "Bobbing Barrel Brawl.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3A2, 0, 16, 1, dir, "Bobbing Barrel Brawl Bonuses 1&2.png"); // Different address, but same "Bobbing Barrel Brawl Bonus 1&2.png"t.
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7A21, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lightning Look-Out Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3B6, 0, 16, 1, dir, "Lightning Look-Out.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B38E, 0, 16, 1, dir, "Lightning Look-Out Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B398, 0, 16, 1, dir, "Lightning Look-Out Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7C21, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lightning Look-Out Tiles (Bright).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3B6, 0, 16, 1, dir, "Lightning Look-Out (Bright).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B38E, 0, 16, 1, dir, "Lightning Look-Out Bonus 1 (Bright).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B398, 0, 16, 1, dir, "Lightning Look-Out Bonus 2 (Bright).png");
    }
    
    // Snow
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x25F598);
    decomp(raw_data, rom, &raw_counter, 0x25D145);
    decomp(lay_data_a, rom, &lay_counter_a, 0x25B2AE);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5FC1, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Snow.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Skidda's Row Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3D5, 0, 16, 0, dir, "Skidda's Row.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3F3, 0, 16, 0, dir, "Skidda's Row Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3FD, 0, 16, 0, dir, "Skidda's Row Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBB99, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lemguin Lunge Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3DF, 0, 16, 0, dir, "Lemguin Lunge.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B407, 0, 16, 0, dir, "Lemguin Lunge Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B411, 0, 16, 0, dir, "Lemguin Lunge Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBC99, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Tearaway Toboggan Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B3E9, 0, 16, 0, dir, "Tearaway Toboggan.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B41B, 0, 16, 0, dir, "Tearaway Toboggan Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B425, 0, 16, 0, dir, "Tearaway Toboggan Bonus 2.png");
    }
    
    // Boardwalk
    
    bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x254C41);
    decomp(raw_data, rom, &raw_counter, 0x253A78);
    decomp(lay_data_a, rom, &lay_counter_a, 0x252618);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5601, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Boardwalk.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lakeside Limbo Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B43A, 0, 16, 0, dir, "Lakeside Limbo.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B458, 0, 16, 0, dir, "Lakeside Limbo Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B462, 0, 16, 0, dir, "Lakeside Limbo Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBA99, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Tidal Trouble Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B44E, 0, 16, 0, dir, "Tidal Trouble.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B476, 0, 16, 0, dir, "Tidal Trouble Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B480, 0, 16, 0, dir, "Tidal Trouble Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DB999, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Kreeping Klasps Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B444, 0, 16, 0, dir, "Kreeping Klasps.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B480, 0, 16, 0, dir, "Kreeping Klasps Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B48A, 0, 16, 0, dir, "Kreeping Klasps Bonus 2.png");
    }
    
    // Tree
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x269DCF);
    decomp(raw_data, rom, &raw_counter, 0x267BF9);
    decomp(lay_data_a, rom, &lay_counter_a, 0x264B0A);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5D01, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 24, 0, dir, "Tree.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Barrel Shield Bust-Up Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B49F, 1, 24, 0, dir, "Barrel Shield Bust-Up.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4C7, 1, 24, 0, dir, "Barrel Shield Bust-Up Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4D1, 1, 24, 0, dir, "Barrel Shield Bust-Up Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBE79, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Ripsaw Rage Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4A9, 1, 24, 0, dir, "Ripsaw Rage.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4EF, 1, 24, 0, dir, "Ripsaw Rage Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4F9, 1, 24, 0, dir, "Ripsaw Rage Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBD99, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Springin' Spiders Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4BD, 1, 24, 0, dir, "Springin' Spiders.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4DB, 1, 24, 0, dir, "Springin' Spiders Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4E5, 1, 24, 0, dir, "Springin' Spiders Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3DBF79, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Swoopy Salvo Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B521, 1, 24, 0, dir, "Swoopy Salvo.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B503, 1, 24, 0, dir, "Swoopy Salvo Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B50D, 1, 24, 0, dir, "Swoopy Salvo Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B517, 1, 24, 0, dir, "Swoopy Salvo Bonus 3.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5DE1, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Swoopy Salvo Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B521, 1, 24, 0, dir, "Swoopy Salvo (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B503, 1, 24, 0, dir, "Swoopy Salvo Bonus 1 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B50D, 1, 24, 0, dir, "Swoopy Salvo Bonus 2 (Krematoa).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B517, 1, 24, 0, dir, "Swoopy Salvo Bonus 3 (Krematoa).png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5EC1, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Arich's Ambush Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B4B3, 1, 24, 0, dir, "Arich's Ambush.png");
    }
    
    // Pipe (LGL & PP)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x27588E);
    decomp(raw_data, rom, &raw_counter, 0x274312);
    decomp(lay_data_a, rom, &lay_counter_a, 0x27B4F8); // LGL
    decomp(lay_data_b, rom, &lay_counter_b, 0x27C2A4); // PP
    
    if (special == 2) {
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7E61, raw_counter, bp_counter, 1, 0, priority);
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Pipe-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 88, 0, dir, "Pipe-2.png");
    }
    else {
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7F61, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Low-G Labyrinth Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B536, 1, 80, 0, dir, "Low-G Labyrinth.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B540, 1, 80, 0, dir, "Low-G Labyrinth Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B54A, 1, 80, 0, dir, "Low-G Labyrinth Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D8061, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Poisonous Pipeline Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B59C, 1, 88, 0, dir, "Poisonous Pipeline.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B554, 1, 80, 0, dir, "Poisonous Pipeline Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B55E, 1, 80, 0, dir, "Poisonous Pipeline Bonus 2.png");
    }
    
    // Pipe2 (DD)
    
    lay_counter_a = 0;
    decomp(lay_data_a, rom, &lay_counter_a, 0x273701);
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D7E61, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Pipe-3.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Demolition Drain-Pipe Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B573, 0, 16, 0, dir, "Demolition Drain-Pipe.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B57D, 0, 16, 0, dir, "Demolition Drain-Pipe Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B587, 0, 16, 0, dir, "Demolition Drain-Pipe Bonus 2.png");
    }
    
    // Waterfall
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x27F2F2);
    decomp(raw_data, rom, &raw_counter, 0x27D66A);
    decomp(lay_data_a, rom, &lay_counter_a, 0x284EF1);
    decomp(lay_data_b, rom, &lay_counter_b, 0x27CBA4); // RBR
    
    decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5701, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 28, 0, dir, "Waterfall-1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Waterfall-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Rocket Barrel Ride Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x37B5B1, 1, 28, 0, dir, "Rocket Barrel Ride.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5D0, 1, 80, 0, dir, "Rocket Barrel Ride Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5DA, 1, 80, 0, dir, "Rocket Barrel Ride Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5901, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Tracker Barrel Trek Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5C6, 1, 80, 0, dir, "Tracker Barrel Trek.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5DA, 1, 80, 0, dir, "Tracker Barrel Trek Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5E4, 1, 80, 0, dir, "Tracker Barrel Trek Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5801, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Barrel Drop Bounce Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B60C, 1, 80, 0, dir, "Barrel Drop Bounce.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5EE, 1, 80, 0, dir, "Barrel Drop Bounce Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B5F8, 1, 80, 0, dir, "Barrel Drop Bounce Bonus 2.png");
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, 0x3D5A01, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Squirt's Showdown Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x37B602, 1, 80, 0, dir, "Squirt's Showdown.png");
    }
    
    free( bp_data);
    free(raw_data);
    free(lay_data_a);
    free(lay_data_b);
    free(bitplane);
    
} // Levels (DKC3)