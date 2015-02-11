/*

This is a tidier version of "level3.c".

Currently unable to handle:
Per level fixes (cropping in Riverside levels)
Placeholder tiles (Konveyor Rope Klash)

*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

struct Level {
    int bp_loc; // Tile data.
    int raw_loc; // 8x8 layout data.
    int lay_loc; // 32x32 layout.
    int pal_ofs; // Palette data.
    int pal_fix; // Palette Fixes
    int position; // Cropping data
    int vert; // Vertical
    int layout_size; // Width (or height) of layout.
    char *name; // Output Filename
};

void level3(uint8_t *rom, char dir[255], int priority, int special, int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    uint8_t *lay_data = malloc(65535);
    int bp_counter;
    int raw_counter;
    int lay_counter;
    
    int bp_width = 512;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    const struct Level levels[] = {
    
        {0x208E80, 0x2077C6, 0x205BC3, 0x3DA439, 0, 0x37B08D, 1, 96, "Buzzer Barrage.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA439, 0, 0x37B0CA, 0, 32, "Buzzer Barrage Bonus 1.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA439, 0, 0x37B0FC, 0, 32, "Buzzer Barrage Bonus 2.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA439, 0, 0x37B0AC, 0, 32, "Bazza's Blockade Cave.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA439, 0, 0x37B0B6, 0, 32, "Fish Food Frenzy Cave.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA439, 0, 0x37B0C0, 0, 32, "Floodlit Fish Cave.png"},
        {0x208E80, 0x2077C6, 0x205BC3, 0x3DA639, 0, 0x37B097, 1, 96, "Creepy Caverns.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA639, 0, 0x37B0FC, 0, 32, "Creepy Caverns Bonus 1.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA639, 0, 0x37B106, 0, 32, "Creepy Caverns Bonus 2.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA539, 0, 0x37B0D4, 0, 32, "Pot Hole Panic.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA539, 0, 0x37B0E8, 0, 32, "Pot Hole Panic Bonus 1.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA539, 0, 0x37B0F2, 0, 32, "Pot Hole Panic Bonus 2.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA339, 0, 0x37B0DE, 0, 32, "Tyrant Twin Tussle.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA339, 0, 0x37B110, 0, 32, "Tyrant Twin Tussle Bonus 1.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA339, 0, 0x37B11A, 0, 32, "Tyrant Twin Tussle Bonus 2.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA339, 0, 0x37B124, 0, 32, "Tyrant Twin Tussle Bonus 3.png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA739, 0, 0x37B0DE, 0, 32, "Tyrant Twin Tussle (Krematoa).png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA739, 0, 0x37B110, 0, 32, "Tyrant Twin Tussle Bonus 1 (Krematoa).png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA739, 0, 0x37B11A, 0, 32, "Tyrant Twin Tussle Bonus 2 (Krematoa).png"},
        {0x208E80, 0x2077C6, 0x20F035, 0x3DA739, 0, 0x37B124, 0, 32, "Tyrant Twin Tussle Bonus 3 (Krematoa).png"}, // 19
        
        {0x215058, 0x212377, 0x210E48, 0x3DB799, 0, 0x37B139, 1, 32, "Barbos' Barrier.png"},
        {0x215058, 0x212377, 0x210E48, 0x3D8261, 9, 0x37B143, 1, 32, "Floodlit Fish (Dark).png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8261, 9, 0x37B18A, 1, 80, "Floodlit Fish Bonus 1 (Dark).png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8261, 9, 0x37B194, 1, 80, "Floodlit Fish Bonus 2 (Dark).png"},
        {0x215058, 0x212377, 0x210E48, 0x3D8261, 10, 0x37B143, 1, 32, "Floodlit Fish (Light).png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8261, 10, 0x37B18A, 1, 80, "Floodlit Fish Bonus 1 (Light).png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8261, 10, 0x37B194, 1, 80, "Floodlit Fish Bonus 2 (Light).png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8161, 0, 0x37B158, 1, 80, "Bazza's Blockade.png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8161, 0, 0x37B162, 1, 80, "Bazza's Blockade Bonus 1.png"},
        {0x215058, 0x212377, 0x219CD8, 0x3D8161, 0, 0x37B16C, 1, 80, "Bazza's Blockade Bonus 2.png"},
        {0x215058, 0x212377, 0x219CD8, 0x3DB899, 0, 0x37B19E, 1, 80, "Fish Food Frenzy.png"},
        {0x215058, 0x212377, 0x219CD8, 0x3DB899, 0, 0x37B176, 1, 80, "Fish Food Frenzy Bonus 1.png"},
        {0x215058, 0x212377, 0x219CD8, 0x3DB899, 0, 0x37B180, 1, 80, "Fish Food Frenzy Bonus 2.png"}, // 32
        
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D94F9, 0, 0x37B1B3, 1, 24, "Krevice Kreepers.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D94F9, 0, 0x37B1F0, 1, 80, "Krevice Kreepers Bonus 1.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D94F9, 0, 0x37B1FA, 1, 80, "Krevice Kreepers Bonus 2.png"},
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D91F9, 0, 0x37B1C7, 1, 24, "Kong Fused Cliffs.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D91F9, 0, 0x37B204, 1, 80, "Kong Fused Cliffs Bonuses 1&2.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D93F9, 0, 0x37B1E6, 1, 80, "Ropey Rumpus.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D93F9, 0, 0x37B20E, 1, 80, "Ropey Rumpus Bonus 1.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D93F9, 0, 0x37B218, 1, 80, "Ropey Rumpus Bonus 2.png"},
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D95F9, 0, 0x37B1BD, 1, 24, "Criss Kross Cliffs.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D95F9, 0, 0x37B222, 1, 80, "Criss Kross Cliffs Bonus 1.png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D95F9, 0, 0x37B22C, 1, 80, "Criss Kross Cliffs Bonus 2.png"},
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D92F9, 0, 0x37B1BD, 1, 24, "Criss Kross Cliffs (Krematoa).png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D92F9, 0, 0x37B222, 1, 80, "Criss Kross Cliffs Bonus 1 (Krematoa).png"},
        {0x221914, 0x21EB1B, 0x2275C9, 0x3D92F9, 0, 0x37B22C, 1, 80, "Criss Kross Cliffs Bonus 2 (Krematoa).png"},
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D96F9, 0, 0x37B1D1, 1, 24, "Rocket Rush.png"},
        {0x221914, 0x21EB1B, 0x21BEB6, 0x3D92F9, 0, 0x37B1D1, 1, 24, "Rocket Rush (Krematoa).png"}, // 48
        
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9C39, 0, 0x37B24B, 0, 16, "Konveyor Rope Klash.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9C39, 0, 0x37B273, 0, 16, "Konveyor Rope Klash Bonus 1.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9C39, 0, 0x37B27D, 0, 16, "Konveyor Rope Klash Bonus 2.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9B39, 0, 0x37B241, 0, 16, "Koindozer Klamber.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9B39, 0, 0x37B25F, 0, 16, "Koindozer Klamber Bonus 1.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9B39, 0, 0x37B269, 0, 16, "Koindozer Klamber Bonus 2.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9A39, 0, 0x37B255, 0, 16, "Stampede Sprint.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9A39, 0, 0x37B287, 0, 16, "Stampede Sprint Bonus 1.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9A39, 0, 0x37B291, 0, 16, "Stampede Sprint Bonus 2.png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9D39, 0, 0x37B255, 0, 16, "Stampede Sprint (Krematoa).png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9D39, 0, 0x37B287, 0, 16, "Stampede Sprint Bonus 1 (Krematoa).png"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0x3D9D39, 0, 0x37B291, 0, 16, "Stampede Sprint Bonus 2 (Krematoa).png"}, // 60
        
        {0x238D56, 0x237402, 0x233DD3, 0x3D9E39, 0, 0x37B2A6, 1, 80, "Fire-Ball Frenzy.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3D9E39, 0, 0x37B2CE, 1, 80, "Fire-Ball Frenzy Bonus 1.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3D9E39, 0, 0x37B2D8, 1, 80, "Fire-Ball Frenzy Bonus 2.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3DA039, 0, 0x37B2B0, 1, 80, "Blazing Bazukas.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3DA039, 0, 0x37B2C4, 1, 80, "Blazing Bazukas Bonus 1.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3DA039, 0, 0x37B2EC, 1, 80, "Blazing Bazukas Bonus 2.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3DA039, 0, 0x37B2EC, 1, 80, "Blazing Bazukas Bonus 2.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3D9F39, 0, 0x37B2BA, 1, 80, "Krack Shot Kroc.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3D9F39, 0, 0x37B2D8, 1, 80, "Krack Shot Kroc Bonus 1.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3D9F39, 0, 0x37B2E2, 1, 80, "Krack Shot Kroc Bonus 2.png"},
        {0x238D56, 0x237402, 0x233DD3, 0x3DA139, 0, 0x37B2F6, 1, 80, "KAOS Karnage.png"}, // 71
        
        {0x243549, 0x241148, 0x23E3B9, 0x3D61C1, 0, 0x37B30B, 1, 80, "Doorstop Dash.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D61C1, 0, 0x37B333, 1, 80, "Doorstop Dash Bonus 1.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D61C1, 0, 0x37B33D, 1, 80, "Doorstop Dash Bonus 2.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D60C1, 0, 0x37B315, 1, 80, "Squeals On Wheels.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D60C1, 0, 0x37B347, 1, 80, "Squeals On Wheels Bonuses 1&2.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D6901, 0, 0x37B31F, 1, 80, "Murky Mill.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D6901, 0, 0x37B351, 1, 80, "Murky Mill Bonus 1.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D6901, 0, 0x37B35B, 1, 80, "Murky Mill Bonus 2.png"},
        {0x243549, 0x241148, 0x23E3B9, 0x3D6301, 0, 0x37B329, 1, 80, "Belcha's Barn.png"}, // 80
        
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7921, 0, 0x37B370, 0, 16, "Riverside Race.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7921, 0, 0x37B37A, 0, 16, "Riverside Race Bonus 1.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7921, 0, 0x37B384, 0, 16, "Riverside Race Bonus 2.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7B21, 0, 0x37B3C0, 0, 16, "Bobbing Barrel Brawl.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7B21, 0, 0x37B3A2, 0, 16, "Bobbing Barrel Brawl Bonuses 1&2.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7A21, 0, 0x37B3B6, 0, 16, "Lightning Look-Out.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7A21, 0, 0x37B38E, 0, 16, "Lightning Look-Out Bonus 1.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7A21, 0, 0x37B398, 0, 16, "Lightning Look-Out Bonus 2.png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7C21, 0, 0x37B3B6, 0, 16, "Lightning Look-Out (Bright).png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7C21, 0, 0x37B38E, 0, 16, "Lightning Look-Out Bonus 1 (Bright).png"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0x3D7C21, 0, 0x37B398, 0, 16, "Lightning Look-Out Bonus 2 (Bright).png"}, // 91
        
        {0x25F598, 0x25D145, 0x25B2AE, 0x3D5FC1, 0, 0x37B3D5, 0, 16, "Skidda's Row.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3D5FC1, 0, 0x37B3F3, 0, 16, "Skidda's Row Bonus 1.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3D5FC1, 0, 0x37B3FD, 0, 16, "Skidda's Row Bonus 2.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBB99, 0, 0x37B3DF, 0, 16, "Lemguin Lunge.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBB99, 0, 0x37B407, 0, 16, "Lemguin Lunge Bonus 1.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBB99, 0, 0x37B411, 0, 16, "Lemguin Lunge Bonus 2.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBC99, 0, 0x37B3E9, 0, 16, "Tearaway Toboggan.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBC99, 0, 0x37B41B, 0, 16, "Tearaway Toboggan Bonus 1.png"},
        {0x25F598, 0x25D145, 0x25B2AE, 0x3DBC99, 0, 0x37B425, 0, 16, "Tearaway Toboggan Bonus 2.png"}, // 100
        
        {0x254C41, 0x253A78, 0x252618, 0x3D5601, 0, 0x37B43A, 0, 16, "Lakeside Limbo.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3D5601, 0, 0x37B458, 0, 16, "Lakeside Limbo Bonus 1.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3D5601, 0, 0x37B462, 0, 16, "Lakeside Limbo Bonus 2.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DBA99, 0, 0x37B44E, 0, 16, "Tidal Trouble.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DBA99, 0, 0x37B476, 0, 16, "Tidal Trouble Bonus 1.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DBA99, 0, 0x37B480, 0, 16, "Tidal Trouble Bonus 2.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DB999, 0, 0x37B444, 0, 16, "Kreeping Klasps.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DB999, 0, 0x37B480, 0, 16, "Kreeping Klasps Bonus 1.png"},
        {0x254C41, 0x253A78, 0x252618, 0x3DB999, 0, 0x37B48A, 0, 16, "Kreeping Klasps Bonus 2.png"}, // 109
        
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3D5D01, 0, 0x37B49F, 1, 24, "Barrel Shield Bust-Up.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3D5D01, 0, 0x37B4C7, 1, 24, "Barrel Shield Bust-Up Bonus 1.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3D5D01, 0, 0x37B4D1, 1, 24, "Barrel Shield Bust-Up Bonus 2.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBE79, 0, 0x37B4A9, 1, 24, "Ripsaw Rage.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBE79, 0, 0x37B4EF, 1, 24, "Ripsaw Rage Bonus 1.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBE79, 0, 0x37B4F9, 1, 24, "Ripsaw Rage Bonus 2.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBD99, 0, 0x37B4BD, 1, 24, "Springin' Spiders.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBD99, 0, 0x37B4DB, 1, 24, "Springin' Spiders Bonus 1.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBD99, 0, 0x37B4E5, 1, 24, "Springin' Spiders Bonus 2.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B521, 1, 24, "Swoopy Salvo.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B503, 1, 24, "Swoopy Salvo Bonus 1.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B50D, 1, 24, "Swoopy Salvo Bonus 2.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B517, 1, 24, "Swoopy Salvo Bonus 3.png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B521, 1, 24, "Swoopy Salvo (Krematoa).png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B503, 1, 24, "Swoopy Salvo Bonus 1 (Krematoa).png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B50D, 1, 24, "Swoopy Salvo Bonus 2 (Krematoa).png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3DBF79, 0, 0x37B517, 1, 24, "Swoopy Salvo Bonus 3 (Krematoa).png"},
        {0x269DCF, 0x267BF9, 0x264B0A, 0x3D5EC1, 0, 0x37B4B3, 1, 24, "Arich's Ambush.png"}, // 127
        
        {0x27588E, 0x274312, 0x27B4F8, 0x3D7F61, 0, 0x37B536, 1, 80, "Low-G Labyrinth.png"},
        {0x27588E, 0x274312, 0x27B4F8, 0x3D7F61, 0, 0x37B540, 1, 80, "Low-G Labyrinth Bonus 1.png"},
        {0x27588E, 0x274312, 0x27B4F8, 0x3D7F61, 0, 0x37B54A, 1, 80, "Low-G Labyrinth Bonus 2.png"},
        {0x27588E, 0x274312, 0x27C2A4, 0x3D8061, 0, 0x37B59C, 1, 88, "Poisonous Pipeline.png"},
        {0x27588E, 0x274312, 0x27B4F8, 0x3D8061, 0, 0x37B554, 1, 80, "Poisonous Pipeline Bonus 1.png"},
        {0x27588E, 0x274312, 0x27B4F8, 0x3D8061, 0, 0x37B55E, 1, 80, "Poisonous Pipeline Bonus 2.png"},
        {0x27588E, 0x274312, 0x273701, 0x3D7E61, 0, 0x37B573, 0, 16, "Demolition Drain-Pipe.png"},
        {0x27588E, 0x274312, 0x273701, 0x3D7E61, 0, 0x37B57D, 0, 16, "Demolition Drain-Pipe Bonus 1.png"},
        {0x27588E, 0x274312, 0x273701, 0x3D7E61, 0, 0x37B587, 0, 16, "Demolition Drain-Pipe Bonus 2.png"}, // 136
        
        {0x27F2F2, 0x27D66A, 0x27CBA4, 0x3D5701, 0, 0x37B5B1, 1, 28, "Rocket Barrel Ride.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5701, 0, 0x37B5D0, 1, 80, "Rocket Barrel Ride Bonus 1.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5701, 0, 0x37B5DA, 1, 80, "Rocket Barrel Ride Bonus 2.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5901, 0, 0x37B5C6, 1, 80, "Tracker Barrel Trek.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5901, 0, 0x37B5DA, 1, 80, "Tracker Barrel Trek Bonus 1.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5901, 0, 0x37B5E4, 1, 80, "Tracker Barrel Trek Bonus 2.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5801, 0, 0x37B60C, 1, 80, "Barrel Drop Bounce.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5801, 0, 0x37B5EE, 1, 80, "Barrel Drop Bounce Bonus 1.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5801, 0, 0x37B5F8, 1, 80, "Barrel Drop Bounce Bonus 2.png"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 0x3D5A01, 0, 0x37B602, 1, 80, "Squirt's Showdown.png"} // 146
        
    };
    
    int i;
    
    for (i = 0; i < 147; i++) {
    
         bp_counter = 0;
        raw_counter = 0;
        lay_counter = 0;
        decomp( bp_data, rom,  &bp_counter, levels[i].bp_loc);
        decomp(raw_data, rom, &raw_counter, levels[i].raw_loc);
        decomp(lay_data, rom, &lay_counter, levels[i].lay_loc);
        
        decode_bitplane_3(region, rom, bp_data, raw_data, bitplane, levels[i].pal_ofs, raw_counter, bp_counter, 1, levels[i].pal_fix, priority);
        assemble_level(bitplane, rom, lay_data, lay_counter, levels[i].position, levels[i].vert, levels[i].layout_size, 0, dir, levels[i].name);
    }
    
    free( bp_data);
    free(raw_data);
    free(lay_data);
    free(bitplane);
    
} // Levels (DKC3)