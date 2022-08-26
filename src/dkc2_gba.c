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

static int lay_double (unsigned char **lay_data, int laylen) {

    unsigned char *lay_dd = malloc(laylen*2);
    int i;
    if (lay_dd == NULL) {
        fprintf(stderr, "Failed to allocate memory for layout.\n");
        return 1;
    }

    memcpy(lay_dd, *lay_data, 4);
    for (i = 0; i < laylen-4; i++) {
        lay_dd[4 + (i*2)] = (*lay_data)[4 + i];
        lay_dd[4 + (i*2) + 1] = 0;
    }
    free(*lay_data);
    *lay_data = lay_dd;
    return 0;
}

static const struct dkc2_archetype arch2[] = {
    {0, 0, {0x31A5F4, 0, 1}, {0x4A290C, 0, 1}}, // Ship Deck
    {0, 0, {0x32A494, 0, 2}, {0x4A54D8, 0, 2}}, // Ship Mast
    {1, 0, {0x325534, 0, 1}, {0x4A4898, 0, 1}}, // Ship Hold
    {0, 1, {0x301BE0, 0, 1}, {0x49D660, 0, 2}}, // La-Va
    {0, 0, {0x3071D4, 0, 1}, {0x49E484, 0, 1}}, // Mine
    {0, 0, {0x3315C0, 0, 1}, {0x4A68B0, 0, 1}}, // Swamp (5)
    {0, 0, {0x2A8660, 0, 2}, {0x48F278, 0, 1}}, // Brambles
    {0, 0, {0x2C6748, 0, 2}, {0x4944A8, 0, 1}}, // Hive
    {0, 0, {0x310748, 0, 1}, {0x4A0538, 0, 1}}, // Fair
    {0, 0, {0x2B7760, 0, 2}, {0x491240, 0, 1}}, // Forest
    {0, 0, {0x2D2304, 0, 2}, {0x4977B0, 0, 1}}, // Ice (10)
    {1, 1, {0x2B2FFC, 0, 2}, {0x496FA0, 0, 1}}, // Castle
    {0, 0, {0x2E1F8C, 0, 2}, {0x499C88, 0, 1}}, // Jungle
    {0, 0, {0x2FADC0, 0, 3}, {0x4A8F34, 0, 2}}, // K.Rool Duel
    {1, 1, {0x2F6AB0, 0, 1}, {0x4C2AA8, 0, 2}}, // Krocodile Kore
    {1, 1, {0x2DED9C, 0, 1}, {0x4A89D4, 0, 1}}, // K.Rool's Kabin (15)
    {0, 0, {0x3F043C, 0, 3}, {0x4BECF8, 0, 3}}, // Overworld
    {0, 0, {0x33C524, 0, 3}, {0x4AA914, 0, 3}}, // Gangplank Galleon
    {0, 0, {0x353174, 0, 3}, {0x4AD458, 0, 3}}, // Crocodile Cauldron
    {0, 0, {0x36B484, 0, 3}, {0x4B002C, 0, 2}}, // Krem Quay
    {0, 0, {0x3859D8, 0, 3}, {0x4B2D78, 0, 3}}, // Krazy Kremland (20)
    {0, 0, {0x3A0444, 0, 3}, {0x4B5BCC, 0, 2}}, // Gloomy Gulch
    {0, 0, {0x3BA694, 0, 3}, {0x4B8D88, 0, 3}}, // K.Rool's Keep
    {0, 1, {0x3D1B30, 0, 3}, {0x4BB574, 0, 3}}, // The Flying Krock
    {0, 0, {0x3D9580, 0, 3}, {0x4BC3F8, 0, 3}}, // The Lost World
};

static const struct dkc2_gba_levels dkc2[] = {
    { 0, {0x4F724C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic"},
    { 0, {0x4F8C2C, 0, 2}, {0xEA2FC, 0x0200, 1}, "Pirate Panic Warp"},
    { 0, {0x4F8CB4, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic B1"},
    { 0, {0x4F8D5C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Pirate Panic B2"},
    { 1, {0x4FDE1C, 0, 1}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem"},
    { 1, {0x4FF6CC, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem Warp"},
    { 1, {0x4FF450, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B1"},
    { 1, {0x4FF2EC, 0, 2}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B2"},
    { 1, {0x4FF37C, 0, 1}, {0xEA2FC, 0x0400, 1}, "Mainbrace Mayhem B3"},
    { 0, {0x4F7A0C, 0, 1}, {0xEA2FC, 0x0200, 1}, "Gangplank Galley (Day)"},
    { 0, {0x4F7A0C, 0, 1}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley (Dusk)"},
    { 0, {0x4F90AC, 0, 2}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley Warp"},
    // { 0, {0x4C????, 0, 2}, {0x??????, 0x0200, 1}, "Gangplank Galley B1"},
    { 0, {0x4F8EB8, 0, 2}, {0xEA2FC, 0x0600, 1}, "Gangplank Galley B2"},
    { 2, {0x4FB4F4, 0, 1}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker"},
    { 2, {0x4FC948, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker Warp"},
    { 2, {0x4FC1F8, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lockjaw's Locker B1"},
    { 1, {0x4FE27C, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble"},
    { 1, {0x4FF720, 0, 2}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble Warp"},
    { 1, {0x4FF760, 0, 2}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble B1"},
    { 1, {0x4FF7EC, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Topsail Trouble B2"},
    { 1, {0x4FF27C, 0, 2}, {0xEA2FC, 0x0400, 1}, "Krow's Nest"}, // 22 - 1

    { 3, {0x4ED05C, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop"},
    { 3, {0x4EDDCC, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop Warp"},
    { 3, {0x4EDF7C, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B1"},
    { 3, {0x4EE18C, 0, 2}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B2"},
    { 3, {0x4EDEDC, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Hot-Head Hop B3"},
    { 4, {0x4EEF24, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim"},
    { 4, {0x4F0DF0, 0, 2}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim Warp"},
    { 4, {0x4F0A14, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B1"},
    { 4, {0x4F103C, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B2"},
    { 4, {0x4F08E4, 0, 1}, {0xEA2FC, 0x1000, 1}, "Kannon's Klaim B3"},
    { 2, {0x4FA760, 0, 1}, {0xEA2FC, 0x1200, 1}, "Lava Lagoon"},
    { 2, {0x4FC9B8, 0, 2}, {0xEA2FC, 0x1200, 1}, "Lava Lagoon Warp"},
    { 2, {0x4FCA28, 0, 2}, {0xEA2FC, 0x0800, 1}, "Lava Lagoon B1"},
    { 3, {0x4ED4B4, 0, 1}, {0xEA2FC, 0x0E00, 1}, "Red-Hot Ride"},
    { 3, {0x4EE1E4, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride Warp"},
    { 3, {0x4EDFF4, 0, 1}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride B1"},
    { 3, {0x4EE128, 0, 2}, {0xEA2FC, 0x0C00, 1}, "Red-Hot Ride B2"},
    { 4, {0x4EE234, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft"},
    { 4, {0x4F0E60, 0, 2}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft Warp"},
    { 4, {0x4F0D48, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B1"},
    { 4, {0x4F0558, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B2"},
    { 4, {0x4F0750, 0, 1}, {0xEA2FC, 0x1600, 1}, "Squawks's Shaft B3"},
    { 3, {0x4EDE14, 0, 1}, {0xEA2FC, 0x1800, 1}, "Kleever's Kiln"},

    { 5, {0x4FF934, 0, 1}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou"},
    { 5, {0x501530, 0, 2}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou B1"},
    { 5, {0x5013D8, 0, 1}, {0xEA2FC, 0x1C00, 1}, "Barrel Bayou B2"},
    { 2, {0x4F96AC, 0, 1}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon"},
    { 2, {0x4FC7A4, 0, 2}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon B1"},
    { 2, {0x4FC534, 0, 2}, {0xEA2FC, 0x1E00, 1}, "Glimmer's Galleon B2"},
    { 0, {0x4F8BD4, 0, 2}, {0xEB5F0, 0x0200, 1}, "Glimmer's Galleon Exit"},
    { 5, {0x50008C, 0, 1}, {0xEB5F0, 0x0000, 1}, "Krochead Klamber"},
    { 5, {0x501570, 0, 1}, {0xEB5F0, 0x0000, 1}, "Krochead Klamber B1"},
    { 0, {0x4F819C, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle"},
    { 0, {0x4F8F38, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle B1"},
    { 1, {0x4FF8C0, 0, 1}, {0xEB5F0, 0x0400, 1}, "Rattle Battle B2"},
    { 0, {0x4F9034, 0, 1}, {0xEB5F0, 0x0200, 1}, "Rattle Battle B3"},
    { 1, {0x4FE708, 0, 1}, {0xEB5F0, 0x0400, 1}, "Slime Climb"},
    { 1, {0x4FF590, 0, 2}, {0xEB5F0, 0x0400, 1}, "Slime Climb B1"},
    { 1, {0x4FF5EC, 0, 2}, {0xEB5F0, 0x0400, 1}, "Slime Climb B2"},
    { 6, {0x4D8654, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast"},
    { 6, {0x4DB534, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast B1"},
    { 6, {0x4DB6D4, 0, 1}, {0xEB5F0, 0x0600, 1}, "Bramble Blast B2"},
    { 5, {0x50135C, 0, 2}, {0xEB5F0, 0x0800, 1}, "Kudgel's Kontest"},

    { 7, {0x4E29EC, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole"},
    { 7, {0x4E6994, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B1"},
    { 7, {0x4E67A4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B2"},
    { 7, {0x4E6530, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Hornet Hole B3"},
    { 8, {0x4F1220, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Target Terror"},
    { 8, {0x4F5350, 0, 2}, {0xEB5F0, 0x0C00, 1}, "Target Terror Exit"},
    { 6, {0x4DB9C4, 0, 1}, {0xEB5F0, 0x0600, 1}, "Target Terror B1"},
    { 8, {0x4F47BC, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Target Terror B2"},
    { 6, {0x4D9220, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Bramble Scramble"},
    { 6, {0x4DBB78, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Bramble Scramble B1"},
    { 8, {0x4F1F90, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Rickety Race"},
    { 8, {0x4F53E8, 0, 2}, {0xEB5F0, 0x0C00, 1}, "Rickety Race Exit"},
    { 8, {0x4F4EF0, 0, 1}, {0xEB5F0, 0x0C00, 1}, "Rickety Race B1"},
    { 5, {0x5008DC, 0, 1}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh"},
    { 5, {0x5015BC, 0, 2}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh B1"},
    { 5, {0x501698, 0, 2}, {0xEB5F0, 0x1000, 1}, "Mudhole Marsh B2"},
    { 7, {0x4E403C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble"},
    { 7, {0x4E63CC, 0, 2}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble Room"},
    { 7, {0x4E643C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble B1"},
    { 7, {0x4E66C4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Rambi Rumble B2"},
    { 7, {0x4E6214, 0, 2}, {0xEB5F0, 0x0A00, 1}, "King Zing Sting"},

    { 9, {0x4DEEB0, 0, 1}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove"},
    { 9, {0x4E0040, 0, 2}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove B1"},
    { 9, {0x4DFDD4, 0, 2}, {0xEB5F0, 0x1200, 1}, "Ghostly Grove B2"},
    { 8, {0x4F3294, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall"},
    { 8, {0x4F5478, 0, 2}, {0xEB5F0, 0x1400, 1}, "Haunted Hall Exit"},
    { 8, {0x4F4AB4, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B1"},
    { 8, {0x4F510C, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B2"},
    { 8, {0x4F4374, 0, 1}, {0xEB5F0, 0x1400, 1}, "Haunted Hall B3"},
    { 9, {0x4DDF80, 0, 1}, {0xEB5F0, 0x1600, 1}, "Gusty Glade"},
    { 9, {0x4E0190, 0, 1}, {0xEB5F0, 0x1600, 1}, "Gusty Glade B1"},
    { 9, {0x4DFC14, 0, 2}, {0xEB5F0, 0x1600, 1}, "Gusty Glade B2"},
    { 7, {0x4E5168, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic"},
    { 7, {0x4E65D4, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic B1"},
    { 7, {0x4E6A74, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic B2"},
    { 7, {0x4E6364, 0, 2}, {0xEB5F0, 0x0A00, 1}, "Parrot Chute Panic Rare Room"},
    { 9, {0x4DCEF4, 0, 1}, {0xEB5F0, 0x1800, 1}, "Web Woods"},
    { 9, {0x4E00E8, 0, 2}, {0xEB5F0, 0x1800, 1}, "Web Woods Room"},
    { 6, {0x4DBD3C, 0, 1}, {0xEB5F0, 0x0600, 1}, "Web Woods B1"},
    { 9, {0x4DFF80, 0, 2}, {0xEB5F0, 0x1800, 1}, "Web Woods B2"},
    { 1, {0x4FF014, 0, 1}, {0xEA2FC, 0x0A00, 1}, "Kreepy Krow"},

    {10, {0x4E79D8, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss"},
    {10, {0x4E9404, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss B1"},
    {10, {0x4E96B4, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Arctic Abyss B2"},
    {11, {0x4E02F4, 0, 1}, {0xECDB0, 0x0000, 1}, "Castle Crush"},
    {11, {0x4E27F0, 0, 2}, {0xECDB0, 0x0000, 1}, "Castle Crush B1"},
    {11, {0x4E2864, 0, 1}, {0xECDB0, 0x0000, 1}, "Castle Crush B2"},
    {10, {0x4E81F4, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern"},
    {10, {0x4E901C, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern B1"},
    {10, {0x4E91D4, 0, 1}, {0xECDB0, 0x0400, 1}, "Clapper's Cavern B2"},
    { 4, {0x4EF83C, 0, 1}, {0xEB5F0, 0x1E00, 1}, "Windy Well"},
    { 4, {0x4F0EF0, 0, 1}, {0xEB5F0, 0x1E00, 1}, "Windy Well B1"},
    { 6, {0x4DC09C, 0, 2}, {0xEB5F0, 0x0E00, 1}, "Windy Well B2"},
    {11, {0x4E0F44, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber"},
    {11, {0x4E22B8, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber B1"},
    {11, {0x4E2760, 0, 1}, {0xECDB0, 0x0600, 1}, "Chain Link Chamber B2"},
    {11, {0x4E18AC, 0, 1}, {0xECDB0, 0x0800, 1}, "Toxic Tower"},
    { 6, {0x4DC140, 0, 1}, {0xEB5F0, 0x0E00, 1}, "Toxic Tower B1"},
    {11, {0x4E200C, 0, 2}, {0xECDB0, 0x0A00, 1}, "Stronghold Showdown"},

    { 6, {0x4DA104, 0, 1}, {0xECDB0, 0x0C00, 1}, "Screech's Sprint"},
    { 6, {0x4DC450, 0, 1}, {0xECDB0, 0x0C00, 1}, "Screech's Sprint B1"},
    // {13, {0x5019BC, 0, 1}, {0xEE550, 0x1E00, 3}, "K.Rool Duel"},

    {12, {0x4EAEC8, 0, 1}, {0xECDB0, 0x0E00, 1}, "Jungle Jinx"},
    {12, {0x4ECF2C, 0, 2}, {0xECDB0, 0x0E00, 1}, "Jungle Jinx B1"},
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
    // {14, {0x501B60, 0x90, 0}, {0xEE550, 0x0600, 3}, "Krocodile Kore"},

    {15, {0x501870, 0x4A, 0}, {0xEE550, 0x0200, 3}, "K.Rool's Kabin"},

    {16, {0x503B54, 0, 2}, {0xEE550, 0x1A00, 3}, "Overworld"},
    {17, {0x501B50, 0, 2}, {0xEE550, 0x0800, 3}, "Gangplank Galleon"},
    {18, {0x501FA8, 0, 2}, {0xEE550, 0x0A00, 3}, "Crocodile Cauldron"},
    {19, {0x5023D8, 0, 2}, {0xEE550, 0x0E00, 3}, "Krem Quay"},
    {20, {0x502840, 0, 2}, {0xEE550, 0x1000, 3}, "Krazy Kremland"},
    {21, {0x502CCC, 0, 2}, {0xEE550, 0x1200, 3}, "Gloomy Gulch"},
    {22, {0x5031C0, 0, 2}, {0xEE550, 0x1400, 3}, "K.Rool's Keep"},
    {23, {0x503624, 0, 1}, {0xEE550, 0x1600, 3}, "The Flying Krock"},
    {23, {0x50370C, 0, 1}, {0xEE550, 0x1600, 3}, "The Flying Krock Overlay"},
    {24, {0x503764, 0, 2}, {0xEE550, 0x1800, 3}, "The Lost World"},

    { 6, {0x4DCAEC, 0, 1}, {0xEB5F0, 0x0600, 1}, "Funky (1)"},
    { 2, {0x4FCC58, 0, 1}, {0xEA2FC, 0x0800, 1}, "Funky (2)"},
    {11, {0x4E2044, 0, 1}, {0xECDB0, 0x0600, 1}, "Funky (3)"},
    { 7, {0x4E6B9C, 0, 1}, {0xEB5F0, 0x0A00, 1}, "Funky (4)"},
    {10, {0x4E9868, 0, 1}, {0xEB5F0, 0x1C00, 1}, "Funky (5)"},
    { 2, {0x4FD894, 0, 1}, {0xEA2FC, 0x0800, 1}, "Funky (6)"},
    {11, {0x4E231C, 0, 1}, {0xECDB0, 0x0800, 1}, "Funky (7)"},
};


static const struct dkc2_archetype arch3[] = {
    {0, 0, {0x335C08, 0, 1}, {0x5C57E8, 0, 1}}, // Boardwalk
    {0, 0, {0x398098, 0, 1}, {0x5D47EC, 0, 1}}, // Mill
    {0, 0, {0x3B5908, 0, 1}, {0x5D9878, 0, 1}}, // Snow
    {0, 0, {0x3C3FA4, 0, 2}, {0x5DBFA0, 0, 1}}, // Tree
    {0, 0, {0x3A2A90, 0, 2}, {0x5D7008, 0, 1}}, // Riverside
    {0, 0, {0x357FB0, 0x21F60, 0}, {0x5CB100, 0, 3}}, // Underwater (5)
    {0, 0, {0x33AB64, 0, 3}, {0x5C694C, 0, 1}}, // Cave
    {0, 0, {0x3DCD68, 0, 3}, {0x5E052C, 0, 3}}, // Waterfall
    {0, 0, {0x379F30, 0, 1}, {0x5CF718, 0, 1}}, // Le Factoire
    {0, 0, {0x3D2320, 0, 1}, {0x5DE5A4, 0, 1}}, // Pipe
    {0, 0, {0x344218, 0, 2}, {0x5C7E60, 0, 1}}, // Cliff (10)
    {1, 1, {0x3EE6C4, 0, 1}, {0x5E3424, 0, 1}}, // Bleak
    {0, 0, {0x383690, 0, 2}, {0x5D1598, 0, 1}}, // Jungle
    {1, 1, {0x4001F4, 0, 1}, {0x5E583C, 0, 3}}, // Kastle Kaos
    {1, 1, {0x4001F4, 0, 1}, {0x5E1E60, 0, 1}}, // Knautilus (Need BP)
    {0, 0, {0x49576C, 0x40000, 0}, {0x5FA36C, 0, 1}}, // Northern Kremisphere (15)
    {0, 0, {0x4C6558, 0x20000, 0}, {0x5FF800, 0, 2}}, // Lake Orangatanga
    {0, 0, {0x4E0ADC, 0x40000, 0}, {0x601E9C, 0, 3}}, // Kremwood Forest
    {0, 0, {0x4FD480, 0x40000, 0}, {0x604720, 0, 3}}, // Cotton-Top Cove
    {0, 0, {0x5180A0, 0x40000, 0}, {0x606D9C, 0, 3}}, // Mekanos
    {0, 0, {0x539F94, 0x40000, 0}, {0x609CB8, 0, 3}}, // K3 (20)
    {0, 0, {0x555840, 0x40000, 0}, {0x60C2D8, 0, 3}}, // Pacifica
    {0, 0, {0x56DEE8, 0x40000, 0}, {0x60E62C, 0, 3}}, // Razor Ridge
    {0, 0, {0x58AA70, 0x40000, 0}, {0x610D78, 0, 3}}, // Kaos Kore
    {0, 0, {0x5A56C4, 0x40000, 0}, {0x613334, 0, 3}}, // Krematoa
    {0, 1, {0x486310, 0, 2}, {0x5F7AB0, 0, 2}}, // All-Time Greats (25)
};

static const struct dkc2_gba_levels dkc3[] = {
    { 0, {0x615858, 0, 1}, {0xE6758, 0x0200, 1}, "Lakeside Limbo"},
    { 0, {0x617850, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B1"},
    { 0, {0x617920, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B2"},
    { 1, {0x631E2C, 0, 1}, {0xE6758, 0x0400, 1}, "Doorstop Dash"},
    { 1, {0x637624, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B1"},
    { 1, {0x63773C, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B2"},
    { 0, {0x61630C, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble"},
    { 0, {0x6179FC, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B1"},
    { 0, {0x617A9C, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B2"},
    { 2, {0x63AF5C, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row"},
    { 2, {0x63CED8, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row B1"},
    { 2, {0x63CFF8, 0, 1}, {0xE6758, 0x0800, 1}, "Skidda's Row B2"},
    { 1, {0x633860, 0, 1}, {0xE6758, 0x0C00, 1}, "Murky Mill"},
    { 1, {0x637890, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B1"},
    { 1, {0x6379BC, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B2"},
    { 1, {0x6372B4, 0, 1}, {0xE6758, 0x0E00, 1}, "Belcha's Barn"},

    { 3, {0x63E7D4, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders"},
    { 3, {0x641F34, 0, 2}, {0xE6758, 0x1600, 1}, "Springin' Spiders B1"},
    { 3, {0x641A58, 0, 2}, {0xE6758, 0x1600, 1}, "Springin' Spiders B2"},
    { 4, {0x637D64, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race"},
    { 4, {0x63A7B8, 0, 2}, {0xE6758, 0x1200, 1}, "Riverside Race B1"},
    { 4, {0x63A8F0, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B2"},
    { 1, {0x635280, 0, 1}, {0xE6758, 0x1400, 1}, "Squeals On Wheels"},
    { 1, {0x637B04, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B1"},
    { 1, {0x637C34, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B2"},
    { 3, {0x63D654, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up"},
    { 3, {0x641BF4, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B1"},
    { 3, {0x641DDC, 0, 2}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B2"},
    { 4, {0x638B80, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl"},
    { 4, {0x63AA5C, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B1"},
    { 4, {0x63AE50, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B2"},
    { 3, {0x6419D4, 0, 2}, {0xE6758, 0x1A00, 1}, "Arich's Ambush"},

    { 5, {0x6226A4, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade"},
    { 5, {0x626028, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B1"},
    { 5, {0x6264F8, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B2"},
    { 6, {0x61CDEC, 0, 1}, {0xE93BC, 0x0800, 1}, "Bazza's Blockade Exit"},
    { 7, {0x64B3D0, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride"},
    { 7, {0x64BD60, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride FG"},
    { 7, {0x64DB64, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B1"},
    { 7, {0x64DC9C, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B2"},
    { 0, {0x616B9C, 0, 1}, {0xE6758, 0x0200, 1}, "Kreeping Klasps"},
    { 0, {0x617B9C, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B1"},
    { 0, {0x617C4C, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B2"},
    { 7, {0x64C12C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek"},
    { 7, {0x64C97C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek FG"},
    { 7, {0x64DD90, 0, 2}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B1"},
    { 7, {0x64DE30, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B2"},
    { 5, {0x6237FC, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy"},
    { 5, {0x6261AC, 0, 2}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B1"},
    { 5, {0x62624C, 0, 2}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B2"},
    { 6, {0x61CF60, 0, 2}, {0xE93BC, 0x0800, 1}, "Fish Food Frenzy Exit"},
    { 7, {0x64DA84, 0, 2}, {0xE7CF4, 0x0400, 1}, "Squirt's Showdown"},

    { 8, {0x627DEC, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy"},
    { 8, {0x62CF44, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B1"},
    { 8, {0x62D644, 0, 2}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B2"},
    { 9, {0x643650, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe"},
    { 9, {0x6479DC, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B1"},
    { 9, {0x647D74, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B2"},
    { 3, {0x63FB34, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage"},
    { 3, {0x642264, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B1"},
    { 3, {0x642084, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B2"},
    { 8, {0x629880, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas"},
    { 8, {0x62CDC0, 0, 2}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B1"},
    { 8, {0x62D0A8, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B2"},
    { 9, {0x644AD4, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth"},
    { 9, {0x647104, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B1"},
    { 9, {0x647268, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B2"},
    { 8, {0x62D578, 0, 2}, {0xE7CF4, 0x1000, 1}, "Kaos Karnage"},

    {10, {0x61D308, 0, 1}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers"},
    {10, {0x620FCC, 0, 2}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers B1"},
    {10, {0x621100, 0, 1}, {0xE7CF4, 0x1200, 1}, "Krevice Kreepers B2"},
    { 2, {0x63B74C, 0, 1}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan"},
    { 2, {0x63D348, 0, 2}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan B1"},
    { 2, {0x63D3E0, 0, 1}, {0xE7CF4, 0x1400, 1}, "Tearaway Toboggan B2"},
    { 7, {0x64CC50, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce"},
    { 7, {0x64D618, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce FG"},
    { 7, {0x64DF08, 0, 2}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B1"},
    { 7, {0x64DF84, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B2"},
    { 8, {0x62B5E8, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc"},
    { 8, {0x62CFCC, 0, 2}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B1"},
    { 8, {0x62D20C, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B2"},
    { 2, {0x63C4F4, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge"},
    { 2, {0x63D11C, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge B1"},
    { 2, {0x63D254, 0, 1}, {0xE7CF4, 0x1C00, 1}, "Lemguin Lunge B2"},
    {11, {0x64E9C0, 0, 1}, {0xE93BC, 0x0000, 1}, "Bleak's House"},

    { 9, {0x648154, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe"},
    { 9, {0x64A86C, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B1"},
    { 9, {0x64AA2C, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B2"},
    { 0, {0x617D9C, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas"},
    { 0, {0x618C74, 0, 2}, {0xEAD70, 0x0600, 1}, "Stormy Seas B1"},
    { 0, {0x618DB8, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas B2"},
    { 3, {0x6427B8, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce"},
    { 3, {0x6432CC, 0, 2}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B1"},
    { 3, {0x643470, 0, 1}, {0xEAD70, 0x0800, 1}, "Sunken Spruce B2"},
    {10, {0x621544, 0, 1}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast"},
    {10, {0x622418, 0, 1}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast B1"},
    {10, {0x622528, 0, 2}, {0xE7CF4, 0x1200, 1}, "Cliffside Blast B2"},
    { 5, {0x626BB0, 0, 1}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef"},
    { 5, {0x62792C, 0, 2}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef B1"},
    { 5, {0x627B80, 0, 2}, {0xEAD70, 0x0A00, 1}, "Ripcurl Reef B2"},
    { 6, {0x61CBB8, 0, 1}, {0xE93BC, 0x0800, 1}, "Ripcurl Reef Exit"},
    { 9, {0x649508, 0, 1}, {0xEAD70, 0x1200, 1}, "Surf's Up"},
    { 9, {0x64ACF8, 0, 1}, {0xEAD70, 0x0C00, 1}, "Surf's Up B1"},
    { 9, {0x64B038, 0, 1}, {0xE7CF4, 0x0800, 1}, "Surf's Up B2"},
    { 5, {0x625C94, 0, 1}, {0xE93BC, 0x0C00, 1}, "Barbos' Barrier"},

    { 6, {0x618E90, 0, 1}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage"},
    { 6, {0x61C15C, 0, 1}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage B1"},
    { 6, {0x61C5E0, 0, 2}, {0xE93BC, 0x0200, 1}, "Buzzer Barrage B2"},
    {10, {0x61DE84, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs"},
    {10, {0x621464, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs B1"},
    {10, {0x6225C4, 0, 1}, {0xE93BC, 0x0400, 1}, "Kong-Fused Cliffs B2"},
    { 5, {0x624774, 0, 1}, {0xE93BC, 0x0600, 1}, "Floodlit Fish"},
    { 5, {0x62666C, 0, 2}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B1"},
    { 5, {0x626924, 0, 2}, {0xE93BC, 0x0600, 1}, "Floodlit Fish B2"},
    { 6, {0x61D11C, 0, 1}, {0xE93BC, 0x0800, 1}, "Floodlit Fish Exit"},
    { 6, {0x619CF4, 0, 1}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic"},
    { 6, {0x61C454, 0, 2}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic B1"},
    { 6, {0x61C514, 0, 2}, {0xE93BC, 0x0800, 1}, "Pot Hole Panic B2"},
    {10, {0x61EA6C, 0, 1}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus"},
    {10, {0x621064, 0, 2}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus B1"},
    {10, {0x621208, 0, 2}, {0xE93BC, 0x0A00, 1}, "Ropey Rumpus B2"},
    { 7, {0x64E074, 0, 2}, {0xEAD70, 0x0E00, 1}, "Kroctopus Krush"},

    {12, {0x62D714, 0, 1}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash"},
    {12, {0x631860, 0, 2}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B1"},
    {12, {0x6319D8, 0, 2}, {0xE93BC, 0x0E00, 1}, "Konveyor Rope Klash B2"},
    { 6, {0x61AAD4, 0, 1}, {0xE93BC, 0x1000, 1}, "Creepy Caverns"},
    { 6, {0x61CD2C, 0, 2}, {0xE93BC, 0x1000, 1}, "Creepy Caverns B1"},
    { 6, {0x61C6A0, 0, 1}, {0xE93BC, 0x1000, 1}, "Creepy Caverns B2"},
    { 4, {0x639A54, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout"},
    { 4, {0x63ACB4, 0, 1}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B1"},
    { 4, {0x63ABCC, 0, 2}, {0xE93BC, 0x1200, 1}, "Lightning Lookout B2"},
    {12, {0x62EAD4, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber"},
    {12, {0x631684, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B1"},
    {12, {0x631780, 0, 1}, {0xE93BC, 0x1400, 1}, "Koindozer Klamber B2"},
    { 9, {0x645F98, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline"},
    { 9, {0x6477C4, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B1"},
    { 9, {0x6475A4, 0, 1}, {0xE93BC, 0x1600, 1}, "Poisonous Pipeline B2"},
    {13, {0x64ED70, 0x84, 0}, {0xE93BC, 0x1800, 1}, "Kastle Kaos"},

    {12, {0x62FD18, 0, 1}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint"},
    {12, {0x631B04, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B1"},
    {12, {0x631464, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B2"},
    {12, {0x631574, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3"},
    // {12, {0x631634, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3 FG"},
    {10, {0x61F77C, 0, 1}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs"},
    {10, {0x621290, 0, 2}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs B1"},
    {10, {0x621340, 0, 1}, {0xE93BC, 0x1C00, 1}, "Criss Kross Cliffs B2"},
    { 6, {0x61B610, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle"},
    { 6, {0x61C7D4, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B1"},
    { 6, {0x61C91C, 0, 1}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B2"},
    { 6, {0x61CAA0, 0, 2}, {0xE93BC, 0x1E00, 1}, "Tyrant Twin Tussle B3"},
    { 3, {0x640A50, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo"},
    { 3, {0x6424DC, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B1"},
    { 3, {0x642600, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B2"},
    { 3, {0x6426E0, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B3"},
    {10, {0x6203A0, 0, 1}, {0xEAD70, 0x0200, 1}, "Rocket Rush"},
    // {14, {0x64E114, 0x84, 0}, {0xEAD70, 0x0400, 1}, "Knautilus"},

    {15, {0x6579B0, 0, 2}, {0xEAD70, 0x1400, 1}, "Northern Kremisphere"},
    {16, {0x658480, 0, 2}, {0xEAD70, 0x1600, 1}, "Lake Orangatanga"},
    {17, {0x6588BC, 0, 2}, {0xEAD70, 0x1800, 1}, "Kremwood Forest"},
    {18, {0x658CAC, 0, 2}, {0xEAD70, 0x1A00, 1}, "Cotton-Top Cove"},
    {19, {0x65911C, 0, 2}, {0xEAD70, 0x1C00, 1}, "Mekanos"},
    {20, {0x6595B8, 0, 2}, {0xEAD70, 0x1E00, 1}, "K3"},
    {21, {0x6599A4, 0, 2}, {0xEC8D8, 0x0000, 3}, "Pacifica"},
    {22, {0x659DA8, 0, 2}, {0xEC8D8, 0x0200, 3}, "Razor Ridge"},
    {23, {0x65A148, 0, 2}, {0xEC8D8, 0x0400, 3}, "Kaos Kore"},
    {24, {0x65A530, 0, 2}, {0xEC8D8, 0x0600, 3}, "Krematoa"},

    {25, {0x650764, 0x50, 0}, {0xF0584, 0x0E00, 1}, "ATG - Castle"},
    {25, {0x6507B4, 0x50, 0}, {0xF0584, 0x1000, 1}, "ATG - Mill"},
    {25, {0x650804, 0x50, 0}, {0xF0584, 0x1200, 1}, "ATG - Arich"},

    { 0, {0x615E94, 0, 1}, {0xE6758, 0x0200, 1}, "Lakeside Limbo FG"},
    { 0, {0x6178DC, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B1 FG"},
    { 0, {0x6179A8, 0, 2}, {0xE6758, 0x0200, 1}, "Lakeside Limbo B2 FG"},
    { 1, {0x632D14, 0, 1}, {0xE6758, 0x0400, 1}, "Doorstop Dash FG"},
    { 1, {0x6376D4, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B1 FG"},
    { 1, {0x637810, 0, 2}, {0xE6758, 0x0400, 1}, "Doorstop Dash B2 FG"},
    { 0, {0x6167E8, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble FG"},
    { 0, {0x617A50, 0, 2}, {0xE6758, 0x0600, 1}, "Tidal Trouble B1 FG"},
    { 0, {0x617B38, 0, 1}, {0xE6758, 0x0600, 1}, "Tidal Trouble B2 FG"},
    { 1, {0x634718, 0, 1}, {0xE6758, 0x0C00, 1}, "Murky Mill FG"},
    { 1, {0x637950, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B1 FG"},
    { 1, {0x637A8C, 0, 2}, {0xE6758, 0x0C00, 1}, "Murky Mill B2 FG"},
    { 1, {0x6374D0, 0, 1}, {0xE6758, 0x0E00, 1}, "Belcha's Barn FG"},

    { 3, {0x63F4E4, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders FG"},
    { 3, {0x642024, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders B1 FG"},
    { 3, {0x641B78, 0, 1}, {0xE6758, 0x1600, 1}, "Springin' Spiders B2 FG"},
    { 4, {0x638774, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race FG"},
    { 4, {0x63A8B4, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B1 FG"},
    { 4, {0x63AA18, 0, 1}, {0xE6758, 0x1200, 1}, "Riverside Race B2 FG"},
    { 1, {0x6364B0, 0, 1}, {0xE6758, 0x1400, 1}, "Squeals On Wheels FG"},
    { 1, {0x637BC8, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B1 FG"},
    { 1, {0x637CF8, 0, 2}, {0xE6758, 0x1400, 1}, "Squeals On Wheels B2 FG"},
    { 3, {0x63E18C, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up FG"},
    { 3, {0x641D40, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B1 FG"},
    { 3, {0x641ED8, 0, 1}, {0xE6758, 0x1000, 1}, "Barrel Shield Bust-Up B2 FG"},
    { 4, {0x6395D8, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl FG"},
    { 4, {0x63AB84, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B1 FG"},
    { 4, {0x63AF1C, 0, 1}, {0xE6758, 0x1800, 1}, "Bobbing Barrel Brawl B2 FG"},
    { 3, {0x641A58, 0, 2}, {0xE6758, 0x1A00, 1}, "Arich's Ambush FG"},
    { 5, {0x6235A0, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade FG"},
    { 5, {0x626180, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B1 FG"},
    { 5, {0x626600, 0, 1}, {0xE6758, 0x1C00, 1}, "Bazza's Blockade B2 FG"},
    { 7, {0x64BD60, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride FG"},
    { 7, {0x64DC44, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B1 FG"},
    { 7, {0x64DD68, 0, 1}, {0xE6758, 0x1E00, 1}, "Rocket Barrel Ride B2 FG"},
    { 0, {0x617314, 0, 1}, {0xE6758, 0x0200, 1}, "Kreeping Klasps FG"},
    { 0, {0x617C10, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B1 FG"},
    { 0, {0x617D20, 0, 2}, {0xE6758, 0x0200, 1}, "Kreeping Klasps B2 FG"},
    { 7, {0x64C97C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek FG"},
    { 7, {0x64DE0C, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B1 FG"},
    { 7, {0x64DEE0, 0, 1}, {0xE7CF4, 0x0000, 1}, "Tracker Barrel Trek B2 FG"},
    { 5, {0x6246C0, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy FG"},
    { 5, {0x626230, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B1 FG"},
    { 5, {0x6264D0, 0, 1}, {0xE7CF4, 0x0200, 1}, "Fish Food Frenzy B2 FG"},

    { 8, {0x628E74, 0, 1}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy FG"},
    { 8, {0x62D6D4, 0, 2}, {0xE7CF4, 0x0600, 1}, "Fire-Ball Frenzy B2 FG"},
    { 9, {0x644184, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe FG"},
    { 9, {0x647C28, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B1 FG"},
    { 9, {0x647FE4, 0, 1}, {0xE7CF4, 0x0800, 1}, "Demolition Drain-Pipe B2 FG"},
    { 3, {0x640568, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage FG"},
    { 3, {0x64242C, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B1 FG"},
    { 3, {0x6421B0, 0, 1}, {0xE7CF4, 0x0A00, 1}, "Ripsaw Rage B2 FG"},
    { 8, {0x62AB2C, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas FG"},
    { 8, {0x62CEDC, 0, 2}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B1 FG"},
    { 8, {0x62D1B4, 0, 1}, {0xE7CF4, 0x0C00, 1}, "Blazing Bazukas B2 FG"},
    { 9, {0x645684, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth FG"},
    { 9, {0x6471E4, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B1 FG"},
    { 9, {0x647464, 0, 1}, {0xE7CF4, 0x0E00, 1}, "Low-G Labyrinth B2 FG"},

    { 7, {0x64D618, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce FG"},
    { 7, {0x64DF6C, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B1 FG"},
    { 7, {0x64E04C, 0, 1}, {0xE7CF4, 0x1800, 1}, "Barrel Drop Bounce B2 FG"},
    { 8, {0x62C4F8, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc FG"},
    { 8, {0x62D05C, 0, 2}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B1 FG"},
    { 8, {0x62D3EC, 0, 1}, {0xE7CF4, 0x1A00, 1}, "Krack Shot Kroc B2 FG"},
    // {11, {0x64EA0C, 0, 1}, {0xE93BC, 0x0000, 1}, "Bleak's House FG"},

    { 9, {0x648C24, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe FG"},
    { 9, {0x64A998, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B1 FG"},
    { 9, {0x64ABE4, 0, 1}, {0xEAD70, 0x0C00, 1}, "Dingy Drain-Pipe B2 FG"},
    { 0, {0x618628, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas FG"},
    { 0, {0x618D50, 0, 1}, {0xEAD70, 0x0600, 1}, "Stormy Seas B1 FG"},
    { 0, {0x618E3C, 0, 2}, {0xEAD70, 0x0600, 1}, "Stormy Seas B2 FG"},
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
    {12, {0x631524, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B2 FG"},
    {12, {0x631634, 0, 2}, {0xE93BC, 0x1A00, 1}, "Stampede Sprint B3 FG"},
    { 3, {0x6413F8, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo FG"},
    { 3, {0x6425A8, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B1 FG"},
    { 3, {0x64269C, 0, 1}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B2 FG"},
    { 3, {0x642774, 0, 2}, {0xEAD70, 0x0000, 1}, "Swoopy Salvo B3 FG"},
    {10, {0x620EF8, 0, 1}, {0xEAD70, 0x0200, 1}, "Rocket Rush FG"},

    {15, {0x658004, 0, 1}, {0xEAD70, 0x1400, 1}, "Northern Kremisphere FG"},
    {16, {0x658790, 0, 1}, {0xEAD70, 0x1600, 1}, "Lake Orangatanga FG"},
    {17, {0x658BE8, 0, 1}, {0xEAD70, 0x1800, 1}, "Kremwood Forest FG"},
    {18, {0x658FC8, 0, 1}, {0xEAD70, 0x1A00, 1}, "Cotton-Top Cove FG"},
    {19, {0x6594C8, 0, 1}, {0xEAD70, 0x1C00, 1}, "Mekanos FG"},
    {20, {0x6598E0, 0, 1}, {0xEAD70, 0x1E00, 1}, "K3 FG"},
    {21, {0x659C90, 0, 1}, {0xEC8D8, 0x0000, 3}, "Pacifica FG"},
    {22, {0x65A0BC, 0, 1}, {0xEC8D8, 0x0200, 3}, "Razor Ridge FG"},
    {23, {0x65A448, 0, 1}, {0xEC8D8, 0x0400, 3}, "Kaos Kore FG"},
    {24, {0x65A840, 0, 1}, {0xEC8D8, 0x0600, 3}, "Krematoa FG"}
};




void dkc2_gba_levels(unsigned char *rom, size_t romsize, char *dir, int priority, int tileset) {

    int size = (sizeof(dkc2) / sizeof(struct dkc2_gba_levels));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {

        if (tileset) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (dkc2[j].arch == dkc2[i].arch && dkc2[j].pal.loc == dkc2[i].pal.loc && dkc2[j].pal.ofs == dkc2[i].pal.ofs) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate) {
                continue;
            }
        }

        unsigned char  *bp_data = NULL;
        unsigned char *raw_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char *pal_data = NULL;
        unsigned char *att_data = malloc(0x80000);
        unsigned char rgb[768];
        int width = 0;
        int height = 0;
        size_t  bp_len = 0;
        size_t raw_len = 0;
        size_t lay_len = 0;
        size_t pal_len = 0;
        int a = dkc2[i].arch;
        int mode = 0;
        char name[255];

        if (att_data == NULL) {
            printf("Error allocating memory for layout attribute data.\n");
            goto next;
        }

        if (gba_data(rom, romsize, &bp_data,   &bp_len, arch2[a]. bp.loc, arch2[a]. bp.ofs, arch2[a]. bp.type)
        ||  gba_data(rom, romsize, &raw_data, &raw_len, arch2[a].raw.loc, arch2[a].raw.ofs, arch2[a].raw.type)
        ||  gba_data(rom, romsize, &lay_data, &lay_len,  dkc2[i].lay.loc,  dkc2[i].lay.ofs,  dkc2[i].lay.type)
        ||  gba_data(rom, romsize, &pal_data, &pal_len,  dkc2[i].pal.loc,  dkc2[i].pal.ofs,  dkc2[i].pal.type))
            goto next;

        if (arch2[a].lay_split)
            if (lay_double(&lay_data, lay_len))
                goto next;

        if (tileset) {
            if (gba_tileset(&lay_data, &lay_len, raw_data))
                goto next;
            sprintf(name, "%s Tiles", dkc2[i].name);
        }
        else {
            strcpy(name, dkc2[i].name);
        }

        gba_layout(&lay_data, raw_data, att_data, &width, &height, mode);

        if (arch2[a].raw_split)
            gba_split(lay_data, att_data, width*height*9);
        decode_palette(rgb, pal_data, 256);

        bitplane = malloc(width * height * 576 * 4);
        if (bitplane == NULL) {
            printf("Error allocating memory for output.\n");
            goto next;
        }

        gba_tiles(bitplane, bp_data, lay_data, att_data, rgb, width*height*9, priority, 0);

        arrange_gbc(bitplane, width*24, height*24, dir, name);

next:
        free(bp_data);
        free(raw_data);
        free(lay_data);
        free(att_data);
        free(pal_data);
        free(bitplane);
    }

}


void dkc3_gba_levels(unsigned char *rom, size_t romsize, char *dir, int priority, int tileset) {

    int size = (sizeof(dkc3) / sizeof(struct dkc2_gba_levels));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {

        if (tileset) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (dkc3[j].arch == dkc3[i].arch && dkc3[j].pal.loc == dkc3[i].pal.loc && dkc3[j].pal.ofs == dkc3[i].pal.ofs) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate) {
                continue;
            }
        }

        unsigned char  *bp_data = NULL;
        unsigned char *raw_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char *pal_data = NULL;
        unsigned char *att_data = malloc(0x50000);
        unsigned char rgb[768];
        int width = 0;
        int height = 0;
        size_t  bp_len = 0;
        size_t raw_len = 0;
        size_t lay_len = 0;
        size_t pal_len = 0;
        int mode = 0;
        int a = dkc3[i].arch;
        char name[255];

        if (att_data == NULL) {
            printf("Error allocating memory for decompression.\n");
            goto next;
        }

        if (gba_data(rom, romsize,  &bp_data,  &bp_len, arch3[a]. bp.loc, arch3[a]. bp.ofs, arch3[a]. bp.type)
        ||  gba_data(rom, romsize, &raw_data, &raw_len, arch3[a].raw.loc, arch3[a].raw.ofs, arch3[a].raw.type)
        ||  gba_data(rom, romsize, &lay_data, &lay_len,  dkc3[i].lay.loc,  dkc3[i].lay.ofs,  dkc3[i].lay.type)
        ||  gba_data(rom, romsize, &pal_data, &pal_len,  dkc3[i].pal.loc,  dkc3[i].pal.ofs,  dkc3[i].pal.type))
            goto next;

        if (arch3[a].lay_split)
            if (lay_double(&lay_data, lay_len))
                goto next;

        if (tileset) {
            if (gba_tileset(&lay_data, &lay_len, raw_data))
                goto next;
            sprintf(name, "%s Tiles", dkc3[i].name);
        }
        else {
            strcpy(name, dkc3[i].name);
        }

        gba_layout(&lay_data, raw_data, att_data, &width, &height, mode);
        if (arch3[a].raw_split)
            gba_split(lay_data, att_data, width*height*9);
        decode_palette(rgb, pal_data, 256);

        bitplane = malloc(width * height * 576 * 4);
        if (bitplane == NULL) {
            printf("Error allocating memory for output.\n");
            goto next;
        }

        gba_tiles(bitplane, bp_data, lay_data, att_data, rgb, width*height*9, priority, 0);
        arrange_gbc(bitplane, width*24, height*24, dir, name);

next:
        free(bp_data);
        free(raw_data);
        free(lay_data);
        free(att_data);
        free(pal_data);
        free(bitplane);
    }

}

