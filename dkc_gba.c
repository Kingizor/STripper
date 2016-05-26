#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bitplane.h"
#include "gba_misc.h"

struct dkc_archetype {
    int bp;
    int raw;
};

struct dkc_gba_levels {
    uint8_t arch;
    struct comp lay;
    int pal;
    char *name;
};

void dkc_gba_levels(uint8_t *rom, char *dir, int priority, int tileset) {

    struct dkc_archetype arch[] = {
        {0x2D7CDC, 0x4A57FC}, // Jungle
        {0x2E4930, 0x4A9878}, // Caves
        {0x305270, 0x4AD5A8}, // Underwater
        {0x368658, 0x4C0F44}, // Walkway
        {0x350334, 0x4BD4C0}, // Mine Cart Carnage
        {0x341110, 0x4BA1FC}, // Mine
        {0x36E6E8, 0x4C27FC}, // Temple
        {0x32D464, 0x4B7AB8}, // Forest
        {0x321030, 0x4B5B70}, // Tree Top
        {0x35C5C0, 0x4BF6D8}, // Snow
        {0x31308C, 0x4B40D4}, // Slipslide Ride
        {0x326BB0, 0x4B66FC}, // Le Factoire
        {0x2FF724, 0x4ACE0C}, // Boss 
        {0x33C7B4, 0x4B9964}, // Gang-Plank Galleon
        {0x3787C8, 0x4C4028}, // Warp Room
        {0x3C67A0, 0x4D1394}, // DK's Treehouse (Huff)
        {0x37F3CC, 0x4C6050}, // Most Normal Screens
        {0x38534C, 0x4C6A1C}, // Warp Screen
        {0x48F8D0, 0x4E81EC}, // DK Island
        {0x40C1B4, 0x4D9198}, // Kongo Jungle
        {0x425754, 0x4DBE40}, // Monkey Mines
        {0x43A388, 0x4DE4A4}, // Vine Valley
        {0x450328, 0x4E0C00}, // Gorilla Glacier
        {0x463C68, 0x4E31A0}, // Kremkroc Industries
        {0x4786C4, 0x4E5998}, // Chimp Caverns
    };
    
    struct dkc_gba_levels dkc[] = {
    
        { 0, {0x4EAF90, 0, 1}, 0x9DB4C, "Jungle Hijinxs"},
        { 1, {0x4F1B80, 0, 1}, 0xA1D4C, "Jungle Hijinxs B1"},
        { 0, {0x4ED5AC, 0, 1}, 0x9DB4C, "Jungle Hijinxs B2"},
        { 0, {0x4EB984, 0, 1}, 0x9E34C, "Ropey Rampage"},
        { 0, {0x4ED778, 0, 1}, 0x9E34C, "Ropey Rampage B1"},
        { 0, {0x4EF02C, 0, 1}, 0x9E34C, "Ropey Rampage B2"},
        { 1, {0x4EF3AC, 0, 1}, 0x9E54C, "Reptile Rumble"},
        { 1, {0x4F1A40, 0, 1}, 0xA1D4C, "Reptile Rumble B1"},
        { 1, {0x4F26E8, 0, 1}, 0xA1D4C, "Reptile Rumble B2"},
        { 1, {0x4F1CE4, 0, 1}, 0xA1D4C, "Reptile Rumble B3"},
        { 2, {0x4F71A0, 0, 1}, 0x9E74C, "Coral Capers"},
        { 0, {0x4ED90C, 0, 1}, 0x9E94C, "Barrel Cannon Canyon"},
        { 1, {0x4F3630, 0, 1}, 0xA1D4C, "Barrel Cannon Canyon B1"},
        { 0, {0x4EE984, 0, 1}, 0x9DB4C, "Barrel Cannon Canyon B2"},
        
        { 3, {0x504574, 0, 1}, 0x9EB4C, "Winky's Walkway"},
        { 3, {0x504470, 0, 1}, 0x9EB4C, "Winky's Walkway B1"},
        { 4, {0x4FFA24, 0, 1}, 0x9ED4C, "Mine Cart Carnage"},
        { 1, {0x4EFDA8, 0, 1}, 0x9EF4C, "Bouncy Bonanza"},
        { 1, {0x4F1E24, 0, 1}, 0xA1D4C, "Bouncy Bonanza B1"},
        { 1, {0x4F41A0, 0, 1}, 0xA1D4C, "Bouncy Bonanza B2"},
        { 1, {0x4F0B78, 0, 1}, 0xA1D4C, "Bouncy Bonanza B3"},
        { 5, {0x4FDB74, 0, 1}, 0x9F14C, "Stop & Go Station (Off)"},
        { 5, {0x4FDB74, 0, 1}, 0x9F34C, "Stop & Go Station (On)"},
        { 5, {0x4FCCC4, 0, 1}, 0xA134C, "Stop & Go Station B1"},
        { 5, {0x4FCC78, 0, 1}, 0xA134C, "Stop & Go Station B2"},
        { 6, {0x504D28, 0, 1}, 0x9F54C, "Millstone Mayhem"},
        { 6, {0x5064F8, 0, 1}, 0xA1F4C, "Millstone Mayhem B1"},
        { 6, {0x506684, 0, 1}, 0xA1F4C, "Millstone Mayhem B2"},
        { 6, {0x5065F8, 0, 1}, 0xA1F4C, "Millstone Mayhem B3"},
        
        { 7, {0x4FB554, 0, 1}, 0x9F74C, "Vulture Culture"},
        { 7, {0x4FC914, 0, 1}, 0x9F74C, "Vulture Culture B1"},
        { 7, {0x4FC628, 0, 1}, 0x9F74C, "Vulture Culture B2"},
        { 7, {0x4FC6C4, 0, 1}, 0x9F74C, "Vulture Culture B3"},
        { 8, {0x4F91D4, 0, 1}, 0x9F94C, "Tree Top Town"},
        { 8, {0x4F98D8, 0, 1}, 0x9F94C, "Tree Top Town B1"},
        { 8, {0x4F9894, 0, 1}, 0x9F94C, "Tree Top Town B2"},
        { 7, {0x4FBE28, 0, 1}, 0x9F74C, "Forest Frenzy"},
        { 7, {0x4FC5A0, 0, 1}, 0x9F74C, "Forest Frenzy B1"},
        { 7, {0x4FC804, 0, 1}, 0x9F74C, "Forest Frenzy B2"},
        { 0, {0x4EC824, 0, 1}, 0x9FB4C, "Orang-Utan Gang"},
        { 0, {0x4EF1F4, 0, 1}, 0x9FB4C, "Orang-Utan Gang B1"},
        { 0, {0x4ED6E8, 0, 1}, 0x9FB4C, "Orang-Utan Gang B2"},
        { 0, {0x4EF288, 0, 1}, 0x9FB4C, "Orang-Utan Gang B3"},
        { 0, {0x4ED654, 0, 1}, 0x9FB4C, "Orang-Utan Gang B4"},
        { 0, {0x4EF060, 0, 1}, 0x9FB4C, "Orang-Utan Gang B5"},
        { 2, {0x4F7BD8, 0, 1}, 0x9FD4C, "Clam City"},
        { 6, {0x505938, 0, 1}, 0x9F54C, "Temple Tempest"},
        { 6, {0x50654C, 0, 1}, 0xA1F4C, "Temple Tempest B1"},
        { 6, {0x506710, 0, 1}, 0xA1F4C, "Temple Tempest B2"},
        
        { 9, {0x500A08, 0, 1}, 0x9FF4C, "Snow Barrel Blast"},
        { 9, {0x5014B4, 0, 1}, 0x9FF4C, "Snow Barrel Blast B1"},
        { 9, {0x5013B8, 0, 1}, 0x9FF4C, "Snow Barrel Blast B2"},
        { 9, {0x501424, 0, 1}, 0x9FF4C, "Snow Barrel Blast B3"},
        {10, {0x4F8614, 0, 1}, 0xA014C, "Slipslide Ride"},
        {10, {0x4F84D4, 0, 1}, 0xA014C, "Slipslide Ride B1"},
        {10, {0x4F915C, 0, 1}, 0xA014C, "Slipslide Ride B2"},
        {10, {0x4F854C, 0, 1}, 0xA014C, "Slipslide Ride B3"},
        { 2, {0x4F55F4, 0, 1}, 0xA074C, "Croctopus Chase"},
        { 9, {0x501658, 0, 1}, 0xA034C, "Ice Age Alley"},
        { 9, {0x5015A4, 0, 1}, 0x9FF4C, "Ice Age Alley B1"},
        { 9, {0x5021A4, 0, 1}, 0x9FF4C, "Ice Age Alley B2"},
        { 8, {0x4F9930, 0, 1}, 0xA094C, "Rope Bridge Rumble"},
        { 8, {0x4F9F30, 0, 1}, 0xA094C, "Rope Bridge Rumble B1"},
        { 8, {0x4F9FA8, 0, 1}, 0xA094C, "Rope Bridge Rumble B2"},
        { 1, {0x4F1E9C, 0, 1}, 0x9E54C, "Torchlight Trouble"},
        { 1, {0x4F3CA8, 0, 1}, 0x9E54C, "Torchlight Trouble B1"},
        { 1, {0x4F3D20, 0, 1}, 0x9E54C, "Torchlight Trouble B2"},
        
        {11, {0x4FA000, 0, 1}, 0xA0B4C, "Oil Drum Alley"},
        {11, {0x4FB390, 0, 1}, 0xA0B4C, "Oil Drum Alley B1"},
        {11, {0x4FB300, 0, 1}, 0xA0B4C, "Oil Drum Alley B2"},
        {11, {0x4FB220, 0, 1}, 0xA0B4C, "Oil Drum Alley B3"},
        {11, {0x4FB168, 0, 1}, 0xA0B4C, "Oil Drum Alley B4"},
        { 3, {0x503460, 0, 1}, 0x9DD4C, "Trick Track Trek"},
        { 3, {0x5044E4, 0, 1}, 0x9EB4C, "Trick Track Trek B1"},
        { 3, {0x504CB4, 0, 1}, 0x9EB4C, "Trick Track Trek B2"},
        { 3, {0x5049FC, 0, 1}, 0x9EB4C, "Trick Track Trek B3"},
        { 2, {0x4F4728, 0, 1}, 0xA0D4C, "Poison Pond"},
        { 1, {0x4F27F0, 0, 1}, 0x9E54C, "Elevator Antics"},
        { 1, {0x4F3D98, 0, 1}, 0x9E54C, "Elevator Antics B1"},
        { 1, {0x4F3E10, 0, 1}, 0x9E54C, "Elevator Antics B2"},
        { 1, {0x4F3F38, 0, 1}, 0x9E54C, "Elevator Antics B3"},
        {11, {0x4FAA1C, 0, 1}, 0xA0F4C, "Blackout Basement"},
        {11, {0x4FB3FC, 0, 1}, 0xA0F4C, "Blackout Basement B1"},
        {11, {0x4FB4C4, 0, 1}, 0xA0F4C, "Blackout Basement B2"},
        { 3, {0x502228, 0, 1}, 0x9DF4C, "Mine Cart Madness"},
        { 1, {0x4F3F88, 0, 1}, 0xA1D4C, "Mine Cart Madness B1"},
        { 3, {0x504A70, 0, 1}, 0x9EB4C, "Mine Cart Madness B2"},
        { 3, {0x504AE4, 0, 1}, 0x9EB4C, "Mine Cart Madness B3"},
        
        { 3, {0x503C3C, 0, 1}, 0x9E14C, "Tanked-Up Trouble"},
        { 3, {0x504B60, 0, 1}, 0x9EB4C, "Tanked-Up Trouble B1"},
        { 1, {0x4F0BC8, 0, 1}, 0xA114C, "Manic Mincers"},
        { 1, {0x4F4128, 0, 1}, 0xA1D4C, "Manic Mincers B1"},
        { 1, {0x4F3554, 0, 1}, 0xA1D4C, "Manic Mincers B2"},
        { 1, {0x4F40B0, 0, 1}, 0xA1D4C, "Manic Mincers B3"},
        { 5, {0x4FCD68, 0, 1}, 0xA134C, "Misty Mine"},
        { 5, {0x4FE9E0, 0, 1}, 0xA134C, "Misty Mine B1"},
        { 5, {0x4FE8B8, 0, 1}, 0xA134C, "Misty Mine B2"},
        { 5, {0x4FEA54, 0, 1}, 0xA154C, "Loopy Lights"},
        { 5, {0x4FE960, 0, 1}, 0xA154C, "Loopy Lights B1"},
        { 5, {0x4FF760, 0, 1}, 0xA154C, "Loopy Lights B2"},
        { 3, {0x502BE4, 0, 1}, 0x9EB4C, "Platform Perils"},
        { 3, {0x504BD4, 0, 1}, 0x9EB4C, "Platform Perils B1"},
        { 3, {0x504C48, 0, 1}, 0x9EB4C, "Platform Perils B2"},
        
        {15, {0x50C554, 0x90, 0}, 0xA314C, "DK's Treehouse"},
        {15, {0x50C5E8, 0, 1}, 0xA314C, "DK's Treehouse Overlay"}, // Huff
        { 1, {0x4F4218, 0, 1}, 0xA6B4C, "Banana Hoard"},
        { 0, {0x4EEA40, 0, 1}, 0x9DB4C, "Expresso Room"},
        { 1, {0x4F38E4, 0, 1}, 0x9EF4C, "Winky Room"},
        { 9, {0x501EB0, 0, 1}, 0x9FF4C, "Rambi Room"},
        { 2, {0x4F6CD4, 0, 1}, 0xAB34C, "Enguarde Room"},
        {14, {0x50679C, 0, 1}, 0xAB94C, "Warp Room"},
        {12, {0x4F45FC, 0, 1}, 0xA194C, "Boss Room (Small)"},
        {12, {0x4F4504, 0, 1}, 0xA194C, "Boss Room (Large)"},
        {13, {0x4FCA38, 0, 1}, 0xA174C, "Gang-Plank Galleon"},
        {18, {0x5102B8, 0, 1}, 0xA2F4C, "Donkey Kong Island"},
        {19, {0x50E738, 0x34C, 0}, 0xA214C, "Kongo Jungle"}, // LAYOUT NOT COMPRESSED
        {20, {0x50ED10, 0x34C, 0}, 0xA234C, "Monkey Mines"}, //
        {21, {0x50F128, 0x34C, 0}, 0xA254C, "Vine Valley"}, //
        {22, {0x50F65C, 0x34C, 0}, 0xA274C, "Gorilla Glacier"}, //
        {23, {0x50FA58, 0x34C, 0}, 0xA294C, "Kremkroc Industries Inc"}, //
        {24, {0x50ED10, 0x34C, 0}, 0xA2D4C, "Chimp Caverns"}, //
        
        {19, {0x50EA88, 0, 1}, 0xA214C, "Kongo Jungle Overlay"},
        {20, {0x50EC60, 0, 1}, 0xA234C, "Monkey Mines Overlay"},
        {21, {0x50F478, 0, 1}, 0xA254C, "Vine Valley Overlay"},
        {22, {0x50F9AC, 0, 1}, 0xA274C, "Gorilla Glacier Overlay"},
        {23, {0x50FDA8, 0, 1}, 0xA294C, "Kremkroc Industries Inc Overlay"},
        
        {16, {0x50A28C, 0, 1}, 0xA5F4C, "Find the Exit"},
        {17, {0x50A4E4, 0, 1}, 0xA694C, "Warp"},
        {16, {0x50A304, 0, 1}, 0xA614C, "Stop the Barrel(s)"},
        {16, {0x50A214, 0, 1}, 0xA5D4C, "Collect the Bananas"},
        {17, {0x50A37C, 0, 1}, 0xA654C, "Collect the Prizes"},
        {17, {0x50A3FC, 0, 1}, 0xA674C, "Spell It Out"},
        {17, {0x50A474, 0, 1}, 0xA634C, "Bash the Baddies"},
        {16, {0x50A110, 0, 1}, 0xA574C, "Expresso"},
        {16, {0x50A1B4, 0, 1}, 0xA5B4C, "Winky"},
        {16, {0x50A158, 0, 1}, 0xA594C, "Rambi"},
        {16, {0x50A0C8, 0, 1}, 0xA554C, "Enguarde"},
    
    };
    
    int size = (sizeof(dkc) / sizeof(struct dkc_gba_levels));
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {
        
        if (tileset) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (dkc[j].arch == dkc[i].arch && dkc[j].pal == dkc[i].pal) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate) {
                continue;
            }
        }
        
        uint8_t *bp_data = malloc(0x20000);
        uint8_t *raw_data = malloc(0x10000);
        uint8_t *lay_data = malloc(0x100000);
        uint8_t *att_data = malloc(0x50000);
        uint8_t *rgb = malloc(768);
        int bp_len = 0;
        int raw_len = 0;
        int lay_len = 0;
        int width = 0;
        int height = 0;
        int a = dkc[i].arch;
        char name[255];
        
        if (bp_data == NULL || raw_data == NULL || lay_data == NULL || att_data == NULL || rgb == NULL) {
            printf("Error allocating memory for decompression.\n");
            continue;
        }
        
        gba_decomp(rom,  bp_data,  &bp_len, arch[a].bp);
        gba_decomp(rom, raw_data, &raw_len, arch[a].raw);
        gba_data(rom, lay_data, &lay_len, dkc[i].lay.loc, dkc[i].lay.ofs, dkc[i].lay.type);
        
        int mode = (a == 8 || (a > 11 && a < 18)) ? 1 : 0; // Levels/Areas with fewer sets of tiles.
        
        if (tileset) {
            gba_tileset(lay_data, raw_data);
            // Number of tiles + extra row
            lay_len = (raw_data[0] + (raw_data[1]*256) + 24) * 2;
            sprintf(name, "%s Tiles", dkc[i].name);
        }
        else {
            strcpy(name, dkc[i].name);
        }
        
        gba_layout(lay_data, raw_data, att_data, &width, &height, mode);
        
        if (mode) gba_split(lay_data, att_data, width*height*9);
        
        decode_palette(rgb, &rom[dkc[i].pal], 256);
        
        // (Number of 24x24 tiles) * (24*24 dots) * (RGBA)
        uint8_t *bitplane = malloc((lay_len / 2) * 576 * 4);
        
        if (bitplane == NULL) {
            printf("Error allocating memory for output.\n");
            continue;
        }
        
        gba_tiles(bitplane, bp_data, lay_data, att_data, rgb, width*height*9, priority, 0);
        arrange_gbc(bitplane, width*24, height*24, dir, name);
        
        free(bp_data);
        free(raw_data);
        free(lay_data);
        free(att_data);
        free(rgb);
        free(bitplane);
        
    }
    
}
