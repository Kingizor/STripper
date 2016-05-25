#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"
#include "misc.h"

struct Arch {
    int tileset; // Bitplane
    int raw_map; // 8x8 Layout
    int tilemap; // 32x32 Layout
    int vert;
    int map_size; // Full layout width (32px units)
    int palette;
    char *name;
};

struct Level {
    int arch;
    int position; // Cropping data
    int palette;
    int palette_fix;
    char *name;
};

static void jungle_rope_fix(uint8_t *rom, uint8_t *tileset, int region) {
    int jungle_offsets[] = {0x2BBD80, 0x2BBD8A, 0x2BC052, 0x2BC051};
    int jungle_fix = (region == 0) ? rom[0xFFDB] : region+1;
    memcpy(&tileset[0x020], &rom[jungle_offsets[jungle_fix]], 0x140);
    memcpy(&tileset[0x160], &rom[jungle_offsets[jungle_fix]], 0x140);
    memcpy(&tileset[0x2A0], &rom[jungle_offsets[jungle_fix]], 0x80);
}

void level3(uint8_t *rom, char dir[255], int priority, int special, int tilesets, int region) {


    struct Arch archetype[] = {
        {0x208E80, 0x2077C6, 0x205BC3, 1, 96, 0x3DA439, "Caves 1"},
        {0x208E80, 0x2077C6, 0x20F035, 0, 32, 0x3DA439, "Caves 2"},
        {0x215058, 0x212377, 0x210E48, 1, 32, 0x3D8161, "Water 1"},
        {0x215058, 0x212377, 0x219CD8, 1, 80, 0x3D8161, "Water 2"},
        {0x221914, 0x21EB1B, 0x21BEB6, 1, 24, 0x3D94F9, "Cliff 1"},
        {0x221914, 0x21EB1B, 0x2275C9, 1, 80, 0x3D94F9, "Cliff 2"},
        {0x22D9E8, 0x22AEED, 0x22849B, 0, 16, 0x3D9C39, "Jungle"},
        {0x238D56, 0x237402, 0x233DD3, 1, 80, 0x3D9E39, "Factoire"},
        {0x243549, 0x241148, 0x23E3B9, 1, 80, 0x3D61C1, "Mill"},
        {0x24D25F, 0x24AEF3, 0x24890B, 0, 16, 0x3D7921, "Riverside"},
        {0x25F598, 0x25D145, 0x25B2AE, 0, 16, 0x3D5FC1, "Snow"},
        {0x254C41, 0x253A78, 0x252618, 0, 16, 0x3D5601, "Boardwalk"},
        {0x269DCF, 0x267BF9, 0x264B0A, 1, 24, 0x3D5D01, "Tree"},
        {0x27588E, 0x274312, 0x27B4F8, 1, 80, 0x3D7F61, "Pipe 1"},
        {0x27588E, 0x274312, 0x27C2A4, 1, 88, 0x3D7F61, "Pipe 2"},
        {0x27588E, 0x274312, 0x273701, 0, 16, 0x3D7F61, "Pipe 3"},
        {0x27F2F2, 0x27D66A, 0x27CBA4, 1, 28, 0x3D5701, "Waterfall 1"},
        {0x27F2F2, 0x27D66A, 0x284EF1, 1, 80, 0x3D5701, "Waterfall 2"}
    };
    
    struct Level levels[] = {
        {0, 0x37B08D, 0x3DA439, 0, "Buzzer Barrage"},
        {1, 0x37B0CA, 0x3DA439, 0, "Buzzer Barrage Bonus 1"},
        {1, 0x37B0FC, 0x3DA439, 0, "Buzzer Barrage Bonus 2"},
        {1, 0x37B0AC, 0x3DA439, 0, "Bazza's Blockade Cave"},
        {1, 0x37B0B6, 0x3DA439, 0, "Fish Food Frenzy Cave"},
        {1, 0x37B0C0, 0x3DA439, 0, "Floodlit Fish Cave"},
        {0, 0x37B097, 0x3DA639, 0, "Creepy Caverns"},
        {1, 0x37B0FC, 0x3DA639, 0, "Creepy Caverns Bonus 1"},
        {1, 0x37B106, 0x3DA639, 0, "Creepy Caverns Bonus 2"},
        {1, 0x37B0D4, 0x3DA539, 0, "Pot Hole Panic"},
        {1, 0x37B0E8, 0x3DA539, 0, "Pot Hole Panic Bonus 1"},
        {1, 0x37B0F2, 0x3DA539, 0, "Pot Hole Panic Bonus 2"},
        {1, 0x37B0DE, 0x3DA339, 0, "Tyrant Twin Tussle"},
        {1, 0x37B110, 0x3DA339, 0, "Tyrant Twin Tussle Bonus 1"},
        {1, 0x37B11A, 0x3DA339, 0, "Tyrant Twin Tussle Bonus 2"},
        {1, 0x37B124, 0x3DA339, 0, "Tyrant Twin Tussle Bonus 3"},
        {1, 0x37B0DE, 0x3DA739, 0, "Tyrant Twin Tussle (Krematoa)"},
        {1, 0x37B110, 0x3DA739, 0, "Tyrant Twin Tussle Bonus 1 (Krematoa)"},
        {1, 0x37B11A, 0x3DA739, 0, "Tyrant Twin Tussle Bonus 2 (Krematoa)"},
        {1, 0x37B124, 0x3DA739, 0, "Tyrant Twin Tussle Bonus 3 (Krematoa)"},
        {2, 0x37B139, 0x3DB799, 0, "Barbos' Barrier"},
        {2, 0x37B143, 0x3D8261, 0, "Floodlit Fish (Natural)"},
        {3, 0x37B18A, 0x3D8261, 0, "Floodlit Fish Bonus 1 (Natural)"},
        {3, 0x37B194, 0x3D8261, 0, "Floodlit Fish Bonus 2 (Natural)"},
        {2, 0x37B143, 0x3D8261, 9, "Floodlit Fish (Dark)"},
        {3, 0x37B18A, 0x3D8261, 9, "Floodlit Fish Bonus 1 (Dark)"},
        {3, 0x37B194, 0x3D8261, 9, "Floodlit Fish Bonus 2 (Dark)"},
        {2, 0x37B143, 0x3D8261, 10, "Floodlit Fish (Light)"},
        {3, 0x37B18A, 0x3D8261, 10, "Floodlit Fish Bonus 1 (Light)"},
        {3, 0x37B194, 0x3D8261, 10, "Floodlit Fish Bonus 2 (Light)"},
        {3, 0x37B158, 0x3D8161, 0, "Bazza's Blockade (Natural)"},
        {3, 0x37B162, 0x3D8161, 0, "Bazza's Blockade Bonus 1 (Natural)"},
        {3, 0x37B16C, 0x3D8161, 0, "Bazza's Blockade Bonus 2 (Natural)"},
        {3, 0x37B158, 0x3D8161, 52, "Bazza's Blockade (Fixed)"},
        {3, 0x37B162, 0x3D8161, 52, "Bazza's Blockade Bonus 1 (Fixed)"},
        {3, 0x37B16C, 0x3D8161, 52, "Bazza's Blockade Bonus 2 (Fixed)"},
        {3, 0x37B19E, 0x3DB899, 0, "Fish Food Frenzy (Natural)"},
        {3, 0x37B176, 0x3DB899, 0, "Fish Food Frenzy Bonus 1 (Natural)"},
        {3, 0x37B180, 0x3DB899, 0, "Fish Food Frenzy Bonus 2 (Natural)"},
        {3, 0x37B19E, 0x3DB899, 52, "Fish Food Frenzy (Fixed)"},
        {3, 0x37B176, 0x3DB899, 52, "Fish Food Frenzy Bonus 1 (Fixed)"},
        {3, 0x37B180, 0x3DB899, 52, "Fish Food Frenzy Bonus 2 (Fixed)"},
        {4, 0x37B1B3, 0x3D94F9, 0, "Krevice Kreepers"},
        {5, 0x37B1F0, 0x3D94F9, 0, "Krevice Kreepers Bonus 1"},
        {5, 0x37B1FA, 0x3D94F9, 0, "Krevice Kreepers Bonus 2"},
        {4, 0x37B1C7, 0x3D91F9, 0, "Kong Fused Cliffs"},
        {5, 0x37B204, 0x3D91F9, 0, "Kong Fused Cliffs Bonuses 1&2"},
        {5, 0x37B1E6, 0x3D93F9, 0, "Ropey Rumpus"},
        {5, 0x37B20E, 0x3D93F9, 0, "Ropey Rumpus Bonus 1"},
        {5, 0x37B218, 0x3D93F9, 0, "Ropey Rumpus Bonus 2"},
        {4, 0x37B1BD, 0x3D95F9, 0, "Criss Kross Cliffs"},
        {5, 0x37B222, 0x3D95F9, 0, "Criss Kross Cliffs Bonus 1"},
        {5, 0x37B22C, 0x3D95F9, 0, "Criss Kross Cliffs Bonus 2"},
        {4, 0x37B1BD, 0x3D92F9, 0, "Criss Kross Cliffs (Krematoa)"},
        {5, 0x37B222, 0x3D92F9, 0, "Criss Kross Cliffs Bonus 1 (Krematoa)"},
        {5, 0x37B22C, 0x3D92F9, 0, "Criss Kross Cliffs Bonus 2 (Krematoa)"},
        {4, 0x37B1D1, 0x3D96F9, 0, "Rocket Rush"},
        {4, 0x37B1D1, 0x3D92F9, 0, "Rocket Rush (Krematoa)"},
        {6, 0x37B24B, 0x3D9C39, 0, "Konveyor Rope Klash"},
        {6, 0x37B273, 0x3D9C39, 0, "Konveyor Rope Klash Bonus 1"},
        {6, 0x37B27D, 0x3D9C39, 0, "Konveyor Rope Klash Bonus 2"},
        {6, 0x37B241, 0x3D9B39, 0, "Koindozer Klamber"},
        {6, 0x37B25F, 0x3D9B39, 0, "Koindozer Klamber Bonus 1"},
        {6, 0x37B269, 0x3D9B39, 0, "Koindozer Klamber Bonus 2"},
        {6, 0x37B255, 0x3D9A39, 0, "Stampede Sprint"},
        {6, 0x37B287, 0x3D9A39, 0, "Stampede Sprint Bonuses 1&3"},
        {6, 0x37B291, 0x3D9A39, 0, "Stampede Sprint Bonus 2"},
        {6, 0x37B255, 0x3D9D39, 0, "Stampede Sprint (Krematoa)"},
        {6, 0x37B287, 0x3D9D39, 0, "Stampede Sprint Bonuses 1&3 (Krematoa)"},
        {6, 0x37B291, 0x3D9D39, 0, "Stampede Sprint Bonus 2 (Krematoa)"},
        {7, 0x37B2A6, 0x3D9E39, 0, "Fire-Ball Frenzy"},
        {7, 0x37B2CE, 0x3D9E39, 0, "Fire-Ball Frenzy Bonus 1"},
        {7, 0x37B2D8, 0x3D9E39, 0, "Fire-Ball Frenzy Bonus 2"},
        {7, 0x37B2B0, 0x3DA039, 0, "Blazing Bazukas"},
        {7, 0x37B2C4, 0x3DA039, 0, "Blazing Bazukas Bonus 1"},
        {7, 0x37B2EC, 0x3DA039, 0, "Blazing Bazukas Bonus 2"},
        {7, 0x37B2BA, 0x3D9F39, 0, "Krack Shot Kroc"},
        {7, 0x37B2D8, 0x3D9F39, 0, "Krack Shot Kroc Bonus 1"},
        {7, 0x37B2E2, 0x3D9F39, 0, "Krack Shot Kroc Bonus 2"},
        {7, 0x37B2F6, 0x3DA139, 0, "KAOS Karnage"},
        {8, 0x37B30B, 0x3D61C1, 0, "Doorstop Dash"},
        {8, 0x37B333, 0x3D61C1, 0, "Doorstop Dash Bonus 1"},
        {8, 0x37B33D, 0x3D61C1, 0, "Doorstop Dash Bonus 2"},
        {8, 0x37B315, 0x3D60C1, 0, "Squeals On Wheels"},
        {8, 0x37B347, 0x3D60C1, 0, "Squeals On Wheels Bonuses 1&2"},
        {8, 0x37B31F, 0x3D6901, 0, "Murky Mill"},
        {8, 0x37B351, 0x3D6901, 0, "Murky Mill Bonus 1"},
        {8, 0x37B35B, 0x3D6901, 0, "Murky Mill Bonus 2"},
        {8, 0x37B329, 0x3D6301, 0, "Belcha's Barn"},
        {9, 0x37B370, 0x3D7921, 0, "Riverside Race"},
        {9, 0x37B37A, 0x3D7921, 0, "Riverside Race Bonus 1"},
        {9, 0x37B384, 0x3D7921, 0, "Riverside Race Bonus 2"},
        {9, 0x37B3C0, 0x3D7B21, 0, "Bobbing Barrel Brawl"},
        {9, 0x37B3A2, 0x3D7B21, 0, "Bobbing Barrel Brawl Bonuses 1&2"},
        {9, 0x37B3B6, 0x3D7A21, 0, "Lightning Look-Out"},
        {9, 0x37B38E, 0x3D7A21, 0, "Lightning Look-Out Bonus 1"},
        {9, 0x37B398, 0x3D7A21, 0, "Lightning Look-Out Bonus 2"},
        {9, 0x37B3B6, 0x3D7C21, 0, "Lightning Look-Out (Bright)"},
        {9, 0x37B38E, 0x3D7C21, 0, "Lightning Look-Out Bonus 1 (Bright)"},
        {9, 0x37B398, 0x3D7C21, 0, "Lightning Look-Out Bonus 2 (Bright)"},
        {10, 0x37B3D5, 0x3D5FC1, 0, "Skidda's Row"},
        {10, 0x37B3F3, 0x3D5FC1, 0, "Skidda's Row Bonus 1"},
        {10, 0x37B3FD, 0x3D5FC1, 0, "Skidda's Row Bonus 2"},
        {10, 0x37B3DF, 0x3DBB99, 0, "Lemguin Lunge"},
        {10, 0x37B407, 0x3DBB99, 0, "Lemguin Lunge Bonus 1"},
        {10, 0x37B411, 0x3DBB99, 0, "Lemguin Lunge Bonus 2"},
        {10, 0x37B3E9, 0x3DBC99, 0, "Tearaway Toboggan"},
        {10, 0x37B41B, 0x3DBC99, 0, "Tearaway Toboggan Bonus 1"},
        {10, 0x37B425, 0x3DBC99, 0, "Tearaway Toboggan Bonus 2"},
        {11, 0x37B43A, 0x3D5601, 0, "Lakeside Limbo"},
        {11, 0x37B458, 0x3D5601, 0, "Lakeside Limbo Bonus 1"},
        {11, 0x37B462, 0x3D5601, 0, "Lakeside Limbo Bonus 2"},
        {11, 0x37B44E, 0x3DBA99, 0, "Tidal Trouble"},
        {11, 0x37B476, 0x3DBA99, 0, "Tidal Trouble Bonus 1"},
        {11, 0x37B480, 0x3DBA99, 0, "Tidal Trouble Bonus 2"},
        {11, 0x37B444, 0x3DB999, 0, "Kreeping Klasps"},
        {11, 0x37B480, 0x3DB999, 0, "Kreeping Klasps Bonus 1"},
        {11, 0x37B48A, 0x3DB999, 0, "Kreeping Klasps Bonus 2"},
        {12, 0x37B49F, 0x3D5D01, 0, "Barrel Shield Bust-Up"},
        {12, 0x37B4C7, 0x3D5D01, 0, "Barrel Shield Bust-Up Bonus 1"},
        {12, 0x37B4D1, 0x3D5D01, 0, "Barrel Shield Bust-Up Bonus 2"},
        {12, 0x37B4A9, 0x3DBE79, 0, "Ripsaw Rage"},
        {12, 0x37B4EF, 0x3DBE79, 0, "Ripsaw Rage Bonus 1"},
        {12, 0x37B4F9, 0x3DBE79, 0, "Ripsaw Rage Bonus 2"},
        {12, 0x37B4BD, 0x3DBD99, 0, "Springin' Spiders"},
        {12, 0x37B4DB, 0x3DBD99, 0, "Springin' Spiders Bonus 1"},
        {12, 0x37B4E5, 0x3DBD99, 0, "Springin' Spiders Bonus 2"},
        {12, 0x37B521, 0x3DBF79, 0, "Swoopy Salvo"},
        {12, 0x37B503, 0x3DBF79, 0, "Swoopy Salvo Bonus 1"},
        {12, 0x37B50D, 0x3DBF79, 0, "Swoopy Salvo Bonus 2"},
        {12, 0x37B517, 0x3DBF79, 0, "Swoopy Salvo Bonus 3"},
        {12, 0x37B521, 0x3DBF79, 0, "Swoopy Salvo (Krematoa)"},
        {12, 0x37B503, 0x3DBF79, 0, "Swoopy Salvo Bonus 1 (Krematoa)"},
        {12, 0x37B50D, 0x3DBF79, 0, "Swoopy Salvo Bonus 2 (Krematoa)"},
        {12, 0x37B517, 0x3DBF79, 0, "Swoopy Salvo Bonus 3 (Krematoa)"},
        {12, 0x37B4B3, 0x3D5EC1, 0, "Arich's Ambush"},
        {13, 0x37B536, 0x3D7F61, 0, "Low-G Labyrinth"},
        {13, 0x37B540, 0x3D7F61, 0, "Low-G Labyrinth Bonus 1"},
        {13, 0x37B54A, 0x3D7F61, 0, "Low-G Labyrinth Bonus 2"},
        {14, 0x37B59C, 0x3D8061, 0, "Poisonous Pipeline"},
        {13, 0x37B554, 0x3D8061, 0, "Poisonous Pipeline Bonus 1"},
        {13, 0x37B55E, 0x3D8061, 0, "Poisonous Pipeline Bonus 2"},
        {15, 0x37B573, 0x3D7E61, 0, "Demolition Drain-Pipe"},
        {15, 0x37B57D, 0x3D7E61, 0, "Demolition Drain-Pipe Bonus 1"},
        {15, 0x37B587, 0x3D7E61, 0, "Demolition Drain-Pipe Bonus 2"},
        {16, 0x37B5B1, 0x3D5701, 0, "Rocket Barrel Ride"},
        {17, 0x37B5D0, 0x3D5701, 0, "Rocket Barrel Ride Bonus 1"},
        {17, 0x37B5DA, 0x3D5701, 0, "Rocket Barrel Ride Bonus 2"},
        {17, 0x37B5C6, 0x3D5901, 0, "Tracker Barrel Trek"},
        {17, 0x37B5DA, 0x3D5901, 0, "Tracker Barrel Trek Bonus 1"},
        {17, 0x37B5E4, 0x3D5901, 0, "Tracker Barrel Trek Bonus 2"},
        {17, 0x37B60C, 0x3D5801, 0, "Barrel Drop Bounce"},
        {17, 0x37B5EE, 0x3D5801, 0, "Barrel Drop Bounce Bonus 1"},
        {17, 0x37B5F8, 0x3D5801, 0, "Barrel Drop Bounce Bonus 2"},
        {17, 0x37B602, 0x3D5A01, 0, "Squirt's Showdown"}

    };
    
    int arch_list[] = {
         0,  2,  4,  6,
         7,  8,  9, 10,
        11, 12, 13, 16
    };
    int length;
    
    if (tilesets) {
        length = sizeof(arch_list) / sizeof(int);
    }
    else if (special) {
        length = sizeof(archetype) / sizeof(struct Arch);
    }
    else {
        length = sizeof(levels) / sizeof(struct Level);
    }
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < length; i++) {
        
        int arch, palette, pfix, river_fix, position;
        char *name;
        
        if (tilesets) {
            arch = arch_list[i];
        }
        else if (special) {
            arch = i;
        }
        else {
            arch = levels[i].arch;
        }
        
        uint8_t *tileset = calloc(0xFFFF, 1);
        uint8_t *raw_map = calloc(0xFFFF, 1);
        uint8_t *tilemap = calloc(0xFFFF, 1);
        int set_counter = 0;
        int raw_counter = 0;
        int map_counter = 0;
        
        if ((tileset == NULL) || (raw_map == NULL) || (tilemap == NULL)) {
            printf("Failed to allocate memory for decompression.\n");
            continue;
        }
        
        // Decompression
        if (decomp(tileset, rom, &set_counter, archetype[arch].tileset)) {
            printf("Error: Tileset decompression failed.\n");
            continue;
        }
        if (decomp(raw_map, rom, &raw_counter, archetype[arch].raw_map)) {
            printf("Error: Raw map decompression failed.\n");
            continue;
        }
        if (decomp(tilemap, rom, &map_counter, archetype[arch].tilemap)) {
            printf("Error: Tilemap decompression failed.\n");
            continue;
        }
        
        uint8_t *bitplane = malloc((map_counter / 2) * 1024 * 4);
        
        if (bitplane == NULL) {
            printf("Failed to allocate memory for image data.\n");
            continue;
        }
        
        if (tilesets || special) {
            palette = archetype[arch].palette;
            pfix = 0;
        }
        else {
            palette = levels[i].palette;
            pfix = levels[i].palette_fix;
        }
        
        if (region == 0) palette -= 77; // JP Palette Offsetting
        
        decode_bitplane(rom, tileset, raw_map, bitplane, palette, raw_counter, set_counter, 1, pfix, priority);
        
        if (tilesets) {
            assemble_bitplane(bitplane, 512, raw_counter, dir, archetype[arch].name);
        }
        else {
            if (special) {
                position = 0;
                river_fix = 0;
                name = archetype[i].name;
            }
            else {
                position = levels[i].position;
                river_fix = (arch == 9) ? 1 : 0; // Riverside Garbage
                if (arch == 6) jungle_rope_fix(rom, tileset, region); // Jungle Rope
                name = levels[i].name;
            }
            
            assemble_level(bitplane, rom, tilemap, map_counter, position, archetype[arch].vert, archetype[arch].map_size, river_fix, dir, name);
            
        }
        
        free(tileset);
        free(raw_map);
        free(tilemap);
        free(bitplane);
    }
    
    return;
}
