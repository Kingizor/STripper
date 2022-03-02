#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitplane.h"
#include "gba_misc.h"
#include "dkc2_decomp.h"

struct dkc2_archetype {
    unsigned char raw_split; // Some tile data is grouped together in pairs while some isn't.
    unsigned char lay_split;
    struct comp bp;
    struct comp raw;
};

struct dkc2_gba_levels {
    unsigned char arch;
    struct comp lay;
    struct comp pal;
    char *name;
};

static void lay_double(unsigned char *lay_data, int laylen) {

    unsigned char *lay_double = malloc(laylen*2);
    int i;

    for (i = 0; i < laylen-4; i++) {
        lay_double[4 + (i*2)] = lay_data[4 + i];
        lay_double[4 + (i*2) + 1] = 0;
    }
    memcpy(&lay_data[4], &lay_double[4], (laylen-4)*2);
    free(lay_double);
}

void dkc2_gba_levels(unsigned char *rom, char *dir, int priority, int tileset) {

    struct dkc2_archetype arch[] = {
        {0, 0, {0x31A5F4, 0, 1}, {0x4A290C, 0, 1}}, // Ship Deck
        {0, 0, {0x32A498, 0, 2}, {0x4A54DC, 0, 2}}, // Ship Mast
        {1, 0, {0x325534, 0, 1}, {0x4A4898, 0, 1}}, // Ship Hold
        {0, 1, {0x301BE0, 0, 1}, {0x49D664, 0, 2}}, // La-Va
        {0, 0, {0x3071D4, 0, 1}, {0x49E484, 0, 1}}, // Mine
        {0, 0, {0x3315C0, 0, 1}, {0x4A68B0, 0, 1}}, // Swamp (5)
        {0, 0, {0x2A8664, 0, 2}, {0x48F278, 0, 1}}, // Brambles
        {0, 0, {0x2C674C, 0, 2}, {0x4944A8, 0, 1}}, // Hive
        {0, 0, {0x310748, 0, 1}, {0x4A0538, 0, 1}}, // Fair
        {0, 0, {0x2B7764, 0, 2}, {0x491240, 0, 1}}, // Forest
        {0, 0, {0x2D2308, 0, 2}, {0x4977B0, 0, 1}}, // Ice (10)
        {1, 1, {0x2B3000, 0, 2}, {0x496FA0, 0, 1}}, // Castle
        {0, 0, {0x2E1F90, 0, 2}, {0x499C88, 0, 1}}, // Jungle
        {0, 0, {0x2FADC4, 0, 3}, {0x4A8F38, 0, 2}}, // K.Rool Duel
        {1, 1, {0x2F6AB0, 0, 1}, {0x4C2AAC, 0, 2}}, // Krocodile Kore
        {1, 1, {0x2DED9C, 0, 1}, {0x4A89D4, 0, 1}}, // K.Rool's Kabin (15)
        {0, 0, {0x3F0440, 0, 3}, {0x4BECFC, 0, 3}}, // Overworld
        {0, 0, {0x33C528, 0, 3}, {0x4AA918, 0, 3}}, // Gangplank Galleon
        {0, 0, {0x353178, 0, 3}, {0x4AD45C, 0, 3}}, // Crocodile Cauldron
        {0, 0, {0x36B488, 0, 3}, {0x4B0030, 0, 2}}, // Krem Quay
        {0, 0, {0x3859DC, 0, 3}, {0x4B2D7C, 0, 3}}, // Krazy Kremland (20)
        {0, 0, {0x3A0448, 0, 3}, {0x4B5BD0, 0, 2}}, // Gloomy Gulch
        {0, 0, {0x3BA698, 0, 3}, {0x4B8D8C, 0, 3}}, // K.Rool's Keep
        {0, 1, {0x3D1B34, 0, 3}, {0x4BB578, 0, 3}}, // The Flying Krock
        {0, 0, {0x3D9584, 0, 3}, {0x4BC3FC, 0, 3}}, // The Lost World
    };

    struct dkc2_gba_levels dkc[] = {
        { 0, {0x4F724C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic"},
        { 0, {0x4F8C30, 0, 2}, {0xEA2FC, 0x0200, 1}, "Pirate Panic Warp"},
        { 0, {0x4F8CB4, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic B1"},
        { 0, {0x4F8D5C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic B2"},
        { 1, {0x4FDE1C, 0, 1}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem"},
        { 1, {0x4FF6D0, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem Warp"},
        { 1, {0x4FF454, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B1"},
        { 1, {0x4FF2F0, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B2"},
        { 1, {0x4FF37C, 0, 1}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B3"},
        { 0, {0x4F7A0C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Gangplank Galley (Day)"},
        { 0, {0x4F7A0C, 0, 1}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley (Dusk)"},
        { 0, {0x4F90B0, 0, 2}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley Warp"},
        // { 0, {0x4C????, 0, 2}, {0x??????, 0x0200, 1}, "Gangplank Galley B1"},
        { 0, {0x4F8EBC, 0, 2}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley B2"},
        { 2, {0x4FB4F4, 0, 1}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker"},
        { 2, {0x4FC94C, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker Warp"},
        { 2, {0x4FC1FC, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker B1"},
        { 1, {0x4FE27C, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble"},
        { 1, {0x4FF724, 0, 2}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble Warp"},
        { 1, {0x4FF764, 0, 2}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble B1"},
        { 1, {0x4FF7EC, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble B2"},
        { 1, {0x4FF280, 0, 2}, {0xEA2FC, 0x0400, 1}, "Krow's Nest"}, // 22 - 1

        { 3, {0x4ED05C, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop"},
        { 3, {0x4EDDCC, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop Warp"},
        { 3, {0x4EDF7C, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B1"},
        { 3, {0x4EE190, 0, 2}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B2"},
        { 3, {0x4EDEDC, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B3"},
        { 4, {0x4EEF24, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim"},
        { 4, {0x4F0DF4, 0, 2}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim Warp"},
        { 4, {0x4F0A14, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B1"},
        { 4, {0x4F103C, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B2"},
        { 4, {0x4F08E4, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B3"},
        { 2, {0x4FA760, 0, 1}, {0xEA2FC, 0x1200, 1}, "Lava Lagoon"},
        { 2, {0x4FC9BC, 0, 2}, {0xEA2FC, 0x1200, 1}, "Lava Lagoon Warp"},
        { 2, {0x4FCA2C, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lava Lagoon B1"},
        { 3, {0x4ED4B4, 0, 1}, {0xEA2FC, 0x0E00, 1}, "Red-Hot Ride"},
        { 3, {0x4EE1E4, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride Warp"},
        { 3, {0x4EDFF4, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride B1"},
        { 3, {0x4EE12C, 0, 2}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride B2"},
        { 4, {0x4EE234, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft"},
        { 4, {0x4F0E64, 0, 2}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft Warp"},
        { 4, {0x4F0D48, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B1"},
        { 4, {0x4F0558, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B2"},
        { 4, {0x4F0750, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B3"},
        { 3, {0x4EDE14, 0, 1}, {0xEA2FC, 0x1800, 1}, "Kleever's Kiln"},

        { 5, {0x4FF934, 0, 1}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou"},
        { 5, {0x501534, 0, 2}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou B1"},
        { 5, {0x5013D8, 0, 1}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou B2"},
        { 2, {0x4F96AC, 0, 1}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon"},
        { 2, {0x4FC7A8, 0, 2}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon B1"},
        { 2, {0x4FC538, 0, 2}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon B2"},
        { 0, {0x4F8BD8, 0, 2}, {0xEB5F0, 0x0200, 1}, "Glimmer's Galleon Exit"},
        { 5, {0x50008C, 0, 1}, {0xEB5F0, 0x0000, 1}, "Krochead Klamber"},
        { 5, {0x501570, 0, 1}, {0xEB5F0, 0x0000, 1}, "Krochead Klamber B1"},
        { 0, {0x4F819C, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle"},
        { 0, {0x4F8F38, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle B1"},
        { 1, {0x4FF8C0, 0, 1}, {0xEB5F0, 0x0400, 1}, "Rattle Battle B2"},
        { 0, {0x4F9034, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle B3"},
        { 1, {0x4FE708, 0, 1}, {0xEB5F0, 0x0400, 1}, "Slime Climb"},
        { 1, {0x4FF594, 0, 2}, {0xEB5F0, 0x0400, 1}, "Slime Climb B1"},
        { 1, {0x4FF5F0, 0, 2}, {0xEB5F0, 0x0400, 1}, "Slime Climb B2"},
        { 6, {0x4D8654, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast"},
        { 6, {0x4DB534, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast B1"},
        { 6, {0x4DB6D4, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast B2"},
        { 5, {0x501360, 0, 2}, {0xEB5F0, 0x0800, 1}, "Kudgel's Kontest"},

        { 7, {0x4E29EC, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole"},
        { 7, {0x4E6994, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B1"},
        { 7, {0x4E67A4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B2"},
        { 7, {0x4E6530, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B3"},
        { 8, {0x4F1220, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Target Terror"},
        { 8, {0x4F5354, 0, 2}, {0xEB5F0, 0x0C00, 1}, "Target Terror Exit"},
        { 6, {0x4DB9C4, 0, 1}, {0xEB5F0, 0x0600, 1}, "Target Terror B1"},
        { 8, {0x4F47BC, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Target Terror B2"},
        { 6, {0x4D9220, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Bramble Scramble"},
        { 6, {0x4DBB78, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Bramble Scramble B1"},
        { 8, {0x4F1F90, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Rickety Race"},
        { 8, {0x4F53EC, 0, 2}, {0xEB5F0, 0x0C00, 1}, "Rickety Race Exit"},
        { 8, {0x4F4EF0, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Rickety Race B1"},
        { 5, {0x5008DC, 0, 1}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh"},
        { 5, {0x5015C0, 0, 2}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh B1"},
        { 5, {0x50169C, 0, 2}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh B2"},
        { 7, {0x4E403C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble"},
        { 7, {0x4E63D0, 0, 2}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble Room"},
        { 7, {0x4E643C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble B1"},
        { 7, {0x4E66C4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble B2"},
        { 7, {0x4E6218, 0, 2}, {0xEB5F0, 0x0A00, 1}, "King Zing Sting"},

        { 9, {0x4DEEB0, 0, 1}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove"},
        { 9, {0x4E0044, 0, 2}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove B1"},
        { 9, {0x4DFDD8, 0, 2}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove B2"},
        { 8, {0x4F3294, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall"},
        { 8, {0x4F547C, 0, 2}, {0xEB5F0, 0x1400, 1}, "Haunted Hall Exit"},
        { 8, {0x4F4AB4, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B1"},
        { 8, {0x4F510C, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B2"},
        { 8, {0x4F4374, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B3"},
        { 9, {0x4DDF80, 0, 1}, {0xEB5F0, 0x1600, 1}, "Gusty Glade"},
        { 9, {0x4E0190, 0, 1}, {0xEB5F0, 0x1600, 1}, "Gusty Glade B1"},
        { 9, {0x4DFC18, 0, 2}, {0xEB5F0, 0x1600, 1}, "Gusty Glade B2"},
        { 7, {0x4E5168, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic"},
        { 7, {0x4E65D4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic B1"},
        { 7, {0x4E6A74, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic B2"},
        { 7, {0x4E6368, 0, 2}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic Rare Room"},
        { 9, {0x4DCEF4, 0, 1}, {0xEB5F0, 0x1800, 1}, "Web Woods"},
        { 9, {0x4E00EC, 0, 2}, {0xEB5F0, 0x1800, 1}, "Web Woods Room"},
        { 6, {0x4DBD3C, 0, 1}, {0xEB5F0, 0x0600, 1}, "Web Woods B1"},
        { 9, {0x4DFF84, 0, 2}, {0xEB5F0, 0x1800, 1}, "Web Woods B2"},
        { 1, {0x4FF014, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Kreepy Krow"},

        {10, {0x4E79D8, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss"},
        {10, {0x4E9404, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss B1"},
        {10, {0x4E96B4, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss B2"},
        {11, {0x4E02F4, 0, 1}, {0xECDB0, 0x0000, 1}, "Castle Crush"},
        {11, {0x4E27F4, 0, 2}, {0xECDB0, 0x0000, 1}, "Castle Crush B1"},
        {11, {0x4E2864, 0, 1}, {0xECDB0, 0x0000, 1}, "Castle Crush B2"},
        {10, {0x4E81F4, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern"},
        {10, {0x4E901C, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern B1"},
        {10, {0x4E91D4, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern B2"},
        { 4, {0x4EF83C, 0, 1}, {0xEB5F0, 0x1E00, 1}, "Windy Well"},
        { 4, {0x4F0EF0, 0, 1}, {0xEB5F0, 0x1E00, 1}, "Windy Well B1"},
        { 6, {0x4DC0A0, 0, 2}, {0xEB5F0, 0x0E00, 1}, "Windy Well B2"},
        {11, {0x4E0F44, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber"},
        {11, {0x4E22B8, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber B1"},
        {11, {0x4E2760, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber B2"},
        {11, {0x4E18AC, 0, 1}, {0xECDB0, 0x0800, 1}, "Toxic Tower"},
        { 6, {0x4DC140, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Toxic Tower B1"},
        {11, {0x4E2010, 0, 2}, {0xECDB0, 0x0A00, 1}, "Stronghold Showdown"},

        { 6, {0x4DA104, 0, 1}, {0xECDB0, 0x0C00, 1}, "Screech's Sprint"},
        { 6, {0x4DC450, 0, 1}, {0xECDB0, 0x0C00, 1}, "Screech's Sprint B1"},
        // {13, {0x5019BC, 0, 1}, {0xEE554, 0x1E00, 3}, "K.Rool Duel"},

        {12, {0x4EAEC8, 0, 1}, {0xECDB0, 0x0E00, 1}, "Jungle Jinx"},
        {12, {0x4ECF30, 0, 2}, {0xECDB0, 0x0E00, 1}, "Jungle Jinx B1"},
        { 3, {0x4ED860, 0, 1}, {0xECDB0, 0x1400, 1}, "Fiery Furnace"},
        { 3, {0x4ED860, 0, 1}, {0xECDB0, 0x1600, 1}, "Fiery Furnace (Bright)"},
        { 6, {0x4DBE38, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Fiery Furnace B1"},
        {10, {0x4E6F88, 0, 1}, {0xECDB0, 0x1000, 1}, "Black Ice Battle"},
        {10, {0x4E951C, 0, 1}, {0xECDB0, 0x1000, 1}, "Black Ice Battle B1"},
        {12, {0x4E9EC0, 0, 1}, {0xECDB0, 0x1200, 1}, "Klobber Karnage"},
        {12, {0x4ECDB4, 0, 1}, {0xECDB0, 0x1200, 1}, "Klobber Karnage B1"},
        {12, {0x4EBF4C, 0, 1}, {0xECDB0, 0x0E00, 1}, "Animal Antics - Rambi"},
        {10, {0x4E8A90, 0, 1}, {0xECDB0, 0x1800, 1}, "Animal Antics - Enguarde"},
        {12, {0x4EC3B4, 0, 1}, {0xECDB0, 0x1A00, 1}, "Animal Antics - Squitter"},
        { 6, {0x4DAF28, 0, 1}, {0xECDB0, 0x1C00, 1}, "Animal Antics - Squawks"},
        {12, {0x4EC924, 0, 1}, {0xECDB0, 0x1E00, 1}, "Animal Antics - Rattly"},
        { 6, {0x4DC29C, 0, 1}, {0xECDB0, 0x1C00, 1}, "Animal Antics B1"},
        // {14, {0x501B60, 0x90, 0}, {0xEE554, 0x0600, 3}, "Krocodile Kore"},

        {15, {0x501870, 0x4A, 0}, {0xEE554, 0x0200, 3}, "K.Rool's Kabin"},

        {16, {0x503B58, 0, 2}, {0xEE554, 0x1A00, 3}, "Overworld"},
        {17, {0x501B54, 0, 2}, {0xEE554, 0x0800, 3}, "Gangplank Galleon"},
        {18, {0x501FAC, 0, 2}, {0xEE554, 0x0A00, 3}, "Crocodile Cauldron"},
        {19, {0x5023DC, 0, 2}, {0xEE554, 0x0E00, 3}, "Krem Quay"},
        {20, {0x502844, 0, 2}, {0xEE554, 0x1000, 3}, "Krazy Kremland"},
        {21, {0x502CD0, 0, 2}, {0xEE554, 0x1200, 3}, "Gloomy Gulch"},
        {22, {0x5031C4, 0, 2}, {0xEE554, 0x1400, 3}, "K.Rool's Keep"},
        {23, {0x503624, 0, 1}, {0xEE554, 0x1600, 3}, "The Flying Krock"},
        {23, {0x50370C, 0, 1}, {0xEE554, 0x1600, 3}, "The Flying Krock Overlay"},
        {24, {0x503768, 0, 2}, {0xEE554, 0x1800, 3}, "The Lost World"},

        { 6, {0x4DCAEC, 0, 1}, {0xEB5F0, 0x0600, 1}, "Funky (1)"},
        { 2, {0x4FCC58, 0, 1}, {0xEA2FC, 0x0800, 1}, "Funky (2)"},
        {11, {0x4E2044, 0, 1}, {0xECDB0, 0x0600, 1}, "Funky (3)"},
        { 7, {0x4E6B9C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Funky (4)"},
        {10, {0x4E9868, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Funky (5)"},
        { 2, {0x4FD894, 0, 1}, {0xEA2FC, 0x0800, 1}, "Funky (6)"},
        {11, {0x4E231C, 0, 1}, {0xECDB0, 0x0800, 1}, "Funky (7)"},
    };

    int size = (sizeof(dkc) / sizeof(struct dkc2_gba_levels));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {

        if (tileset) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (dkc[j].arch == dkc[i].arch && dkc[j].pal.loc == dkc[i].pal.loc && dkc[j].pal.ofs == dkc[i].pal.ofs) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate) {
                continue;
            }
        }

        unsigned char *bp_data  = malloc(0x21000);
        unsigned char *raw_data = malloc(0x10000);
        unsigned char *lay_data = malloc(0x100000);
        unsigned char *att_data = malloc(0x80000);
        unsigned char *pal_data = malloc(0x50000);
        unsigned char *rgb = malloc(768);
        int width = 0;
        int height = 0;
        int bp_len = 0;
        int raw_len = 0;
        int lay_len = 0;
        int pal_len = 0;
        int a = dkc[i].arch;
        int mode = 0;
        char name[255];

        if (bp_data == NULL || raw_data == NULL || lay_data == NULL || att_data == NULL || pal_data == NULL || rgb == NULL) {
            printf("Error allocating memory for decompression.\n");
            continue;
        }

        gba_data(rom, bp_data, &bp_len, arch[a].bp.loc, arch[a].bp.ofs, arch[a].bp.type);
        gba_data(rom, raw_data, &raw_len, arch[a].raw.loc, arch[a].raw.ofs, arch[a].raw.type);
        gba_data(rom, lay_data, &lay_len, dkc[i].lay.loc, dkc[i].lay.ofs, dkc[i].lay.type);
        gba_data(rom, pal_data, &pal_len, dkc[i].pal.loc, dkc[i].pal.ofs, dkc[i].pal.type);

        if (arch[a].lay_split) lay_double(lay_data, lay_len);

        if (tileset) {
            gba_tileset(lay_data, raw_data);
            lay_len = (raw_data[0] + (raw_data[1] * 256) + 24) * 2;
            sprintf(name, "%s Tiles", dkc[i].name);
        }
        else {
            strcpy(name, dkc[i].name);
        }

        gba_layout(lay_data, raw_data, att_data, &width, &height, mode);

        if (arch[a].raw_split) gba_split(lay_data, att_data, width*height*9);
        decode_palette(rgb, pal_data, 256);

        unsigned char *bitplane = malloc((lay_len / 2) * 576 * 4);

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
        free(pal_data);
        free(rgb);
        free(bitplane);
    }

}

void dkc3_gba_levels(unsigned char *rom, char *dir, int priority, int tileset) {


    struct dkc2_archetype arch[] = {
        {0, 0, {0x335C08, 0, 1}, {0x5C57E8, 0, 1}}, // Boardwalk
        {0, 0, {0x398098, 0, 1}, {0x5D47EC, 0, 1}}, // Mill
        {0, 0, {0x3B5908, 0, 1}, {0x5D9878, 0, 1}}, // Snow
        {0, 0, {0x3C3FA8, 0, 2}, {0x5DBFA0, 0, 1}}, // Tree
        {0, 0, {0x3A2A94, 0, 2}, {0x5D7008, 0, 1}}, // Riverside
        {0, 0, {0x357FB0, 0x21F60, 0}, {0x5CB104, 0, 3}}, // Underwater (5)
        {0, 0, {0x33AB68, 0, 3}, {0x5C694C, 0, 1}}, // Cave
        {0, 0, {0x3DCD6C, 0, 3}, {0x5E0530, 0, 3}}, // Waterfall
        {0, 0, {0x379F30, 0, 1}, {0x5CF718, 0, 1}}, // Le Factoire
        {0, 0, {0x3D2320, 0, 1}, {0x5DE5A4, 0, 1}}, // Pipe
        {0, 0, {0x34421C, 0, 2}, {0x5C7E60, 0, 1}}, // Cliff (10)
        {1, 1, {0x3EE6C4, 0, 1}, {0x5E3424, 0, 1}}, // Bleak
        {0, 0, {0x383694, 0, 2}, {0x5D1598, 0, 1}}, // Jungle
        {1, 1, {0x4001F4, 0, 1}, {0x5E5840, 0, 3}}, // Kastle Kaos
        {1, 1, {0x4001F4, 0, 1}, {0x5E1E60, 0, 1}}, // Knautilus (Need BP)
        {0, 0, {0x49576C, 0x40000, 0}, {0x5FA36C, 0, 1}}, // Northern Kremisphere (15)
        {0, 0, {0x4C6558, 0x20000, 0}, {0x5FF804, 0, 2}}, // Lake Orangatanga
        {0, 0, {0x4E0ADC, 0x40000, 0}, {0x601EA0, 0, 3}}, // Kremwood Forest
        {0, 0, {0x4FD480, 0x40000, 0}, {0x604724, 0, 3}}, // Cotton-Top Cove
        {0, 0, {0x5180A0, 0x40000, 0}, {0x606DA0, 0, 3}}, // Mekanos
        {0, 0, {0x539F94, 0x40000, 0}, {0x609CBC, 0, 3}}, // K3 (20)
        {0, 0, {0x555840, 0x40000, 0}, {0x60C2DC, 0, 3}}, // Pacifica
        {0, 0, {0x56DEE8, 0x40000, 0}, {0x60E630, 0, 3}}, // Razor Ridge
        {0, 0, {0x58AA70, 0x40000, 0}, {0x610D7C, 0, 3}}, // Kaos Kore
        {0, 0, {0x5A56C4, 0x40000, 0}, {0x613338, 0, 3}}, // Krematoa
        {0, 1, {0x486314, 0, 2}, {0x5F7AB4, 0, 2}}, // All-Time Greats (25)
    };

    struct dkc2_gba_levels dkc[] = {
        { 0, {0x615858, 0, 1}, {0xE6758, 0x0200, 1}, "Lakeside Limbo"},
        { 0, {0x617854, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B1"},
        { 0, {0x617924, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B2"},
        { 1, {0x631E2C, 0, 1}, {0xE6758, 0x0400, 1}, "Doorstop Dash"},
        { 1, {0x637628, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B1"},
        { 1, {0x637740, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B2"},
        { 0, {0x61630C, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble"},
        { 0, {0x617A00, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B1"},
        { 0, {0x617AA0, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B2"},
        { 2, {0x63AF5C, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row"},
        { 2, {0x63CED8, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row B1"},
        { 2, {0x63CFF8, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row B2"},
        { 1, {0x633860, 0, 1}, {0xE6758, 0x0C00, 1}, "Murky Mill"},
        { 1, {0x637894, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B1"},
        { 1, {0x6379C0, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B2"},
        { 1, {0x6372B4, 0, 1}, {0xE6758, 0x0E00, 1}, "Belcha's Barn"},

        { 3, {0x63E7D4, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders"},
        { 3, {0x641F38, 0, 2}, {0xE6758, 0x1600, 1}, "Springin' Spiders B1"},
        { 3, {0x641A5C, 0, 2}, {0xE6758, 0x1600, 1}, "Springin' Spiders B2"},
        { 4, {0x637D64, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race"},
        { 4, {0x63A7BC, 0, 2}, {0xE6758, 0x1200, 1}, "Riverside Race B1"},
        { 4, {0x63A8F0, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B2"},
        { 1, {0x635280, 0, 1}, {0xE6758, 0x1400, 1}, "Squeals On Wheels"},
        { 1, {0x637B08, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B1"},
        { 1, {0x637C38, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B2"},
        { 3, {0x63D654, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up"},
        { 3, {0x641BF4, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B1"},
        { 3, {0x641DE0, 0, 2}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B2"},
        { 4, {0x638B80, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl"},
        { 4, {0x63AA5C, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B1"},
        { 4, {0x63AE50, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B2"},
        { 3, {0x6419D8, 0, 2}, {0xE6758, 0x1A00, 1}, "Arich's Ambush"},

        { 5, {0x6226A4, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade"},
        { 5, {0x626028, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B1"},
        { 5, {0x6264F8, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B2"},
        { 6, {0x61CDEC, 0, 1}, {0xE93BC, 0x0800, 1}, "Bazza's Blockade Exit"},
        { 7, {0x64B3D0, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride"},
        { 7, {0x64BD60, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride FG"},
        { 7, {0x64DB64, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B1"},
        { 7, {0x64DC9C, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B2"},
        { 0, {0x616B9C, 0, 1}, {0xE6758, 0x0200, 1}, "Kreeping Klasps"},
        { 0, {0x617BA0, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B1"},
        { 0, {0x617C50, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B2"},
        { 7, {0x64C12C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek"},
        { 7, {0x64C97C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek FG"},
        { 7, {0x64DD94, 0, 2}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B1"},
        { 7, {0x64DE30, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B2"},
        { 5, {0x6237FC, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy"},
        { 5, {0x6261B0, 0, 2}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B1"},
        { 5, {0x626250, 0, 2}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B2"},
        { 6, {0x61CF64, 0, 2}, {0xE93BC, 0x0800, 1}, "Fish Food Frenzy Exit"},
        { 7, {0x64DA88, 0, 2}, {0xE7CF4, 0x0400, 1}, "Squirt's Showdown"},

        { 8, {0x627DEC, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy"},
        { 8, {0x62CF44, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B1"},
        { 8, {0x62D648, 0, 2}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B2"},
        { 9, {0x643650, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe"},
        { 9, {0x6479DC, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B1"},
        { 9, {0x647D74, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B2"},
        { 3, {0x63FB34, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage"},
        { 3, {0x642264, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B1"},
        { 3, {0x642084, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B2"},
        { 8, {0x629880, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas"},
        { 8, {0x62CDC4, 0, 2}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B1"},
        { 8, {0x62D0A8, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B2"},
        { 9, {0x644AD4, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth"},
        { 9, {0x647104, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B1"},
        { 9, {0x647268, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B2"},
        { 8, {0x62D57C, 0, 2}, {0xE7CF4, 0x1000, 1}, "Kaos Karnage"},

        {10, {0x61D308, 0, 1}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers"},
        {10, {0x620FD0, 0, 2}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers B1"},
        {10, {0x621100, 0, 1}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers B2"},
        { 2, {0x63B74C, 0, 1}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan"},
        { 2, {0x63D34C, 0, 2}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan B1"},
        { 2, {0x63D3E0, 0, 1}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan B2"},
        { 7, {0x64CC50, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce"},
        { 7, {0x64D618, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce FG"},
        { 7, {0x64DF0C, 0, 2}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B1"},
        { 7, {0x64DF84, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B2"},
        { 8, {0x62B5E8, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc"},
        { 8, {0x62CFD0, 0, 2}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B1"},
        { 8, {0x62D20C, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B2"},
        { 2, {0x63C4F4, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge"},
        { 2, {0x63D11C, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge B1"},
        { 2, {0x63D254, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge B2"},
        {11, {0x64E9C0, 0, 1}, {0xE93BC, 0x0000, 1}, "Bleak's House"},

        { 9, {0x648154, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe"},
        { 9, {0x64A86C, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B1"},
        { 9, {0x64AA2C, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B2"},
        { 0, {0x617D9C, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas"},
        { 0, {0x618C78, 0, 2}, {0xEAD70, 0x0600, 1}, "Stormy Seas B1"},
        { 0, {0x618DB8, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas B2"},
        { 3, {0x6427B8, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce"},
        { 3, {0x6432D0, 0, 2}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B1"},
        { 3, {0x643470, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B2"},
        {10, {0x621544, 0, 1}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast"},
        {10, {0x622418, 0, 1}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast B1"},
        {10, {0x62252C, 0, 2}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast B2"},
        { 5, {0x626BB0, 0, 1}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef"},
        { 5, {0x627930, 0, 2}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef B1"},
        { 5, {0x627B84, 0, 2}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef B2"},
        { 6, {0x61CBB8, 0, 1}, {0xE93BC, 0x0800, 1}, "Ripcurl Reef Exit"},
        { 9, {0x649508, 0, 1}, {0xEAD70, 0x1200, 1}, "Surf's Up"},
        { 9, {0x64ACF8, 0, 1}, {0xEAD70, 0x0C00, 1}, "Surf's Up B1"},
        { 9, {0x64B038, 0, 1}, {0xE7CF4, 0x0800, 1}, "Surf's Up B2"},
        { 5, {0x625C94, 0, 1}, {0xE93BC, 0x0C00, 1}, "Barbos' Barrier"},

        { 6, {0x618E90, 0, 1}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage"},
        { 6, {0x61C15C, 0, 1}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage B1"},
        { 6, {0x61C5E4, 0, 2}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage B2"},
        {10, {0x61DE84, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs"},
        {10, {0x621464, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs B1"},
        {10, {0x6225C4, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs B2"},
        { 5, {0x624774, 0, 1}, {0xE93BC, 0x0600, 1}, "Floodlit Fish"},
        { 5, {0x626670, 0, 2}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B1"},
        { 5, {0x626928, 0, 2}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B2"},
        { 6, {0x61D11C, 0, 1}, {0xE93BC, 0x0800, 1}, "Floodlit Fish Exit"},
        { 6, {0x619CF4, 0, 1}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic"},
        { 6, {0x61C458, 0, 2}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic B1"},
        { 6, {0x61C518, 0, 2}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic B2"},
        {10, {0x61EA6C, 0, 1}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus"},
        {10, {0x621068, 0, 2}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus B1"},
        {10, {0x62120C, 0, 2}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus B2"},
        { 7, {0x64E078, 0, 2}, {0xEAD70, 0x0E00, 1}, "Kroctopus Krush"},

        {12, {0x62D714, 0, 1}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash"},
        {12, {0x631864, 0, 2}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B1"},
        {12, {0x6319DC, 0, 2}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B2"},
        { 6, {0x61AAD4, 0, 1}, {0xE93BC, 0x1000, 1}, "Creepy Caverns"},
        { 6, {0x61CD30, 0, 2}, {0xE93BC, 0x1000, 1}, "Creepy Caverns B1"},
        { 6, {0x61C6A0, 0, 1}, {0xE93BC, 0x1000, 1}, "Creepy Caverns B2"},
        { 4, {0x639A54, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout"},
        { 4, {0x63ACB4, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B1"},
        { 4, {0x63ABD0, 0, 2}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B2"},
        {12, {0x62EAD4, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber"},
        {12, {0x631684, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B1"},
        {12, {0x631780, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B2"},
        { 9, {0x645F98, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline"},
        { 9, {0x6477C4, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B1"},
        { 9, {0x6475A4, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B2"},
        {13, {0x64ED70, 0x84, 0}, {0xE93BC, 0x1800, 1}, "Kastle Kaos"},

        {12, {0x62FD18, 0, 1}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint"},
        {12, {0x631B08, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B1"},
        {12, {0x631468, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B2"},
        {12, {0x631578, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3"},
        // {12, {0x631638, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3 FG"},
        {10, {0x61F77C, 0, 1}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs"},
        {10, {0x621294, 0, 2}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs B1"},
        {10, {0x621340, 0, 1}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs B2"},
        { 6, {0x61B610, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle"},
        { 6, {0x61C7D4, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B1"},
        { 6, {0x61C91C, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B2"},
        { 6, {0x61CAA4, 0, 2}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B3"},
        { 3, {0x640A50, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo"},
        { 3, {0x6424E0, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B1"},
        { 3, {0x642604, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B2"},
        { 3, {0x6426E4, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B3"},
        {10, {0x6203A0, 0, 1}, {0xEAD70, 0x0200, 1}, "Rocket Rush"},
        // {14, {0x64E114, 0x84, 0}, {0xEAD70, 0x0400, 1}, "Knautilus"},

        {15, {0x6579B4, 0, 2}, {0xEAD70, 0x1400, 1}, "Northern Kremisphere"},
        {16, {0x658484, 0, 2}, {0xEAD70, 0x1600, 1}, "Lake Orangatanga"},
        {17, {0x6588C0, 0, 2}, {0xEAD70, 0x1800, 1}, "Kremwood Forest"},
        {18, {0x658CB0, 0, 2}, {0xEAD70, 0x1A00, 1}, "Cotton-Top Cove"},
        {19, {0x659120, 0, 2}, {0xEAD70, 0x1C00, 1}, "Mekanos"},
        {20, {0x6595BC, 0, 2}, {0xEAD70, 0x1E00, 1}, "K3"},
        {21, {0x6599A8, 0, 2}, {0xEC8DC, 0x0000, 3}, "Pacifica"},
        {22, {0x659DAC, 0, 2}, {0xEC8DC, 0x0200, 3}, "Razor Ridge"},
        {23, {0x65A14C, 0, 2}, {0xEC8DC, 0x0400, 3}, "Kaos Kore"},
        {24, {0x65A534, 0, 2}, {0xEC8DC, 0x0600, 3}, "Krematoa"},

        {25, {0x650764, 0x50, 0}, {0xF0584, 0x0E00, 1}, "ATG - Castle"},
        {25, {0x6507B4, 0x50, 0}, {0xF0584, 0x1000, 1}, "ATG - Mill"},
        {25, {0x650804, 0x50, 0}, {0xF0584, 0x1200, 1}, "ATG - Arich"},

        { 0, {0x615E94, 0, 1}, {0xE6758, 0x0200, 1}, "Lakeside Limbo FG"},
        { 0, {0x6178E0, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B1 FG"},
        { 0, {0x6179AC, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B2 FG"},
        { 1, {0x632D14, 0, 1}, {0xE6758, 0x0400, 1}, "Doorstop Dash FG"},
        { 1, {0x6376D8, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B1 FG"},
        { 1, {0x637814, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B2 FG"},
        { 0, {0x6167E8, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble FG"},
        { 0, {0x617A54, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B1 FG"},
        { 0, {0x617B38, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble B2 FG"},
        { 1, {0x634718, 0, 1}, {0xE6758, 0x0C00, 1}, "Murky Mill FG"},
        { 1, {0x637954, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B1 FG"},
        { 1, {0x637A90, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B2 FG"},
        { 1, {0x6374D0, 0, 1}, {0xE6758, 0x0E00, 1}, "Belcha's Barn FG"},

        { 3, {0x63F4E4, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders FG"},
        { 3, {0x642024, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders B1 FG"},
        { 3, {0x641B78, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders B2 FG"},
        { 4, {0x638774, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race FG"},
        { 4, {0x63A8B4, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B1 FG"},
        { 4, {0x63AA18, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B2 FG"},
        { 1, {0x6364B0, 0, 1}, {0xE6758, 0x1400, 1}, "Squeals On Wheels FG"},
        { 1, {0x637BCC, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B1 FG"},
        { 1, {0x637CFC, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B2 FG"},
        { 3, {0x63E18C, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up FG"},
        { 3, {0x641D40, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B1 FG"},
        { 3, {0x641ED8, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B2 FG"},
        { 4, {0x6395D8, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl FG"},
        { 4, {0x63AB84, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B1 FG"},
        { 4, {0x63AF1C, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B2 FG"},
        { 3, {0x641A5C, 0, 2}, {0xE6758, 0x1A00, 1}, "Arich's Ambush FG"},
        { 5, {0x6235A0, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade FG"},
        { 5, {0x626180, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B1 FG"},
        { 5, {0x626600, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B2 FG"},
        { 7, {0x64BD60, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride FG"},
        { 7, {0x64DC44, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B1 FG"},
        { 7, {0x64DD68, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B2 FG"},
        { 0, {0x617314, 0, 1}, {0xE6758, 0x0200, 1}, "Kreeping Klasps FG"},
        { 0, {0x617C14, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B1 FG"},
        { 0, {0x617D24, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B2 FG"},
        { 7, {0x64C97C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek FG"},
        { 7, {0x64DE0C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B1 FG"},
        { 7, {0x64DEE0, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B2 FG"},
        { 5, {0x6246C0, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy FG"},
        { 5, {0x626230, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B1 FG"},
        { 5, {0x6264D0, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B2 FG"},

        { 8, {0x628E74, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy FG"},
        { 8, {0x62D6D8, 0, 2}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B2 FG"},
        { 9, {0x644184, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe FG"},
        { 9, {0x647C28, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B1 FG"},
        { 9, {0x647FE4, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B2 FG"},
        { 3, {0x640568, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage FG"},
        { 3, {0x64242C, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B1 FG"},
        { 3, {0x6421B0, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B2 FG"},
        { 8, {0x62AB2C, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas FG"},
        { 8, {0x62CEE0, 0, 2}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B1 FG"},
        { 8, {0x62D1B4, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B2 FG"},
        { 9, {0x645684, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth FG"},
        { 9, {0x6471E4, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B1 FG"},
        { 9, {0x647464, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B2 FG"},

        { 7, {0x64D618, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce FG"},
        { 7, {0x64DF6C, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B1 FG"},
        { 7, {0x64E04C, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B2 FG"},
        { 8, {0x62C4F8, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc FG"},
        { 8, {0x62D060, 0, 2}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B1 FG"},
        { 8, {0x62D3EC, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B2 FG"},
        // {11, {0x64EA0C, 0, 1}, {0xE93BC, 0x0000, 1}, "Bleak's House FG"},

        { 9, {0x648C24, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe FG"},
        { 9, {0x64A998, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B1 FG"},
        { 9, {0x64ABE4, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B2 FG"},
        { 0, {0x618628, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas FG"},
        { 0, {0x618D50, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas B1 FG"},
        { 0, {0x618E40, 0, 2}, {0xEAD70, 0x0600, 1}, "Stormy Seas B2 FG"},
        { 3, {0x642EF8, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce FG"},
        { 3, {0x6433F4, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B1 FG"},
        { 3, {0x6435BC, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B2 FG"},
        {10, {0x62230C, 0, 1}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast FG"},
        { 5, {0x627874, 0, 1}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef FG"},
        { 9, {0x649FD0, 0, 1}, {0xEAD70, 0x1200, 1}, "Surf's Up FG"},
        { 9, {0x64AEF4, 0, 1}, {0xEAD70, 0x0C00, 1}, "Surf's Up B1 FG"},
        { 9, {0x64B284, 0, 1}, {0xE7CF4, 0x0800, 1}, "Surf's Up B2 FG"},

        {10, {0x61E990, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs FG"},
        { 5, {0x625B1C, 0, 1}, {0xE93BC, 0x0600, 1}, "Floodlit Fish FG"},
        { 5, {0x6268F0, 0, 1}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B1 FG"},
        { 5, {0x626B7C, 0, 1}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B2 FG"},
        {10, {0x61F5A0, 0, 1}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus FG"},

        {12, {0x62E3C4, 0, 1}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash FG"},
        {12, {0x63197C, 0, 1}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B1 FG"},
        {12, {0x631AC0, 0, 1}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B2 FG"},
        { 4, {0x63A3E8, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout FG"},
        { 4, {0x63AE08, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B1 FG"},
        { 4, {0x63AC90, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B2 FG"},
        {12, {0x62F740, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber FG"},
        {12, {0x631738, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B1 FG"},
        {12, {0x631820, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B2 FG"},
        { 9, {0x646968, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline FG"},
        { 9, {0x647908, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B1 FG"},
        { 9, {0x647700, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B2 FG"},

        {12, {0x630BE4, 0, 1}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint FG"},
        {12, {0x631528, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B2 FG"},
        {12, {0x631638, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3 FG"},
        { 3, {0x6413F8, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo FG"},
        { 3, {0x6425AC, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B1 FG"},
        { 3, {0x64269C, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B2 FG"},
        { 3, {0x642778, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B3 FG"},
        {10, {0x620EF8, 0, 1}, {0xEAD70, 0x0200, 1}, "Rocket Rush FG"},

        {15, {0x658004, 0, 1}, {0xEAD70, 0x1400, 1}, "Northern Kremisphere FG"},
        {16, {0x658790, 0, 1}, {0xEAD70, 0x1600, 1}, "Lake Orangatanga FG"},
        {17, {0x658BE8, 0, 1}, {0xEAD70, 0x1800, 1}, "Kremwood Forest FG"},
        {18, {0x658FC8, 0, 1}, {0xEAD70, 0x1A00, 1}, "Cotton-Top Cove FG"},
        {19, {0x6594C8, 0, 1}, {0xEAD70, 0x1C00, 1}, "Mekanos FG"},
        {20, {0x6598E0, 0, 1}, {0xEAD70, 0x1E00, 1}, "K3 FG"},
        {21, {0x659C90, 0, 1}, {0xEC8DC, 0x0000, 3}, "Pacifica FG"},
        {22, {0x65A0BC, 0, 1}, {0xEC8DC, 0x0200, 3}, "Razor Ridge FG"},
        {23, {0x65A448, 0, 1}, {0xEC8DC, 0x0400, 3}, "Kaos Kore FG"},
        {24, {0x65A840, 0, 1}, {0xEC8DC, 0x0600, 3}, "Krematoa FG"}
    };

    int size = (sizeof(dkc) / sizeof(struct dkc2_gba_levels));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {

        if (tileset) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (dkc[j].arch == dkc[i].arch && dkc[j].pal.loc == dkc[i].pal.loc && dkc[j].pal.ofs == dkc[i].pal.ofs) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate) {
                continue;
            }
        }

        unsigned char *bp_data = malloc(0x40000);
        unsigned char *raw_data = malloc(0x10000);
        unsigned char *lay_data = malloc(0x100000);
        unsigned char *att_data = malloc(0x50000);
        unsigned char *pal_data = malloc(0x50000);
        unsigned char *rgb = malloc(768);
        int width = 0;
        int height = 0;
        int bp_len = 0;
        int raw_len = 0;
        int lay_len = 0;
        int pal_len = 0;
        int mode = 0;
        int a = dkc[i].arch;
        char name[255];

        if (bp_data == NULL || raw_data == NULL || lay_data == NULL || att_data == NULL || pal_data == NULL || rgb == NULL) {
            printf("Error allocating memory for decompression.\n");
            continue;
        }

        gba_data(rom, bp_data, &bp_len, arch[a].bp.loc, arch[a].bp.ofs, arch[a].bp.type);
        gba_data(rom, raw_data, &raw_len, arch[a].raw.loc, arch[a].raw.ofs, arch[a].raw.type);
        gba_data(rom, lay_data, &lay_len, dkc[i].lay.loc, dkc[i].lay.ofs, dkc[i].lay.type);
        gba_data(rom, pal_data, &pal_len, dkc[i].pal.loc, dkc[i].pal.ofs, dkc[i].pal.type);

        if (arch[a].lay_split) lay_double(lay_data, lay_len);

        if (tileset) {
            gba_tileset(lay_data, raw_data);
            lay_len = (raw_data[0] + (raw_data[1] * 256) + 24) * 2;
            sprintf(name, "%s Tiles", dkc[i].name);
        }
        else {
            strcpy(name, dkc[i].name);
        }

        gba_layout(lay_data, raw_data, att_data, &width, &height, mode);
        if (arch[a].raw_split) gba_split(lay_data, att_data, width*height*9);
        decode_palette(rgb, pal_data, 256);

        unsigned char *bitplane = malloc(width * height * 576 * 4);

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
        free(pal_data);
        free(rgb);
        free(bitplane);

    }

}

