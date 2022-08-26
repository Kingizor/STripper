#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "bitplane.h"

struct DKC2_ARCH {
    int tileset; // Bitplane
    int raw_map; // 8x8 Layout
    int tilemap; // 32x32 Layout
    int vert;
    int map_size; // Full layout width (32px units)
    int palette; // Default Palette
    char *name;
};

struct DKC2_LEVEL {
    int arch;
    int position; // Cropping data
    int palette;
    int palette_fix;
    char *name;
};


static const struct DKC2_ARCH archetype[] = {
    {0x1F8116, 0x25C627, 0x23E006, 0, 16, 0x3D0CD0, "Ship Deck"},
    {0x1FD537, 0x25E0C8, 0x23F9B2, 1, 32, 0x3D1610, "Ship Mast"},
    {0x1F0870, 0x2597E1, 0x236F39, 1, 80, 0x3D0910, "Ship Hold"},
    {0x1EA932, 0x2572C5, 0x23438B, 0, 16, 0x3D06F0, "Forest"},
    {0x1F3059, 0x25A71C, 0x23B5D4, 1, 80, 0x3D0A10, "Hive 1"},
    {0x1F3059, 0x25A71C, 0x23A1C4, 1, 16, 0x3D0A10, "Hive 2"},
    {0x202195, 0x25F52D, 0x24186E, 0, 16, 0x3D1710, "Fair 1"},
    {0x202195, 0x25F52D, 0x243D8A, 0, 16, 0x3D2EEE, "Fair 2"},
    {0x207174, 0x260FB1, 0x245ADE, 0, 16, 0x3D0DF0, "Lava"},
    {0x20CF31, 0x26303A, 0x2479EE, 1, 24, 0x3D228E, "Mine"},
    {0x2106AF, 0x264C7F, 0x249E44, 0, 16, 0x3D27EE, "Swamp"},
    {0x215B05, 0x2665F8, 0x24B767, 1, 96, 0x3D28EE, "Brambles 1"},
    {0x215B05, 0x2665F8, 0x24E649, 1, 96, 0x3D28EE, "Brambles 2"},
    {0x29A905, 0x268077, 0x24F714, 1, 16, 0x3D2DEE, "Castle"},
    {0x21EFDE, 0x2694EC, 0x25264B, 1, 16, 0x3D2CEE, "Krocodile Kore"},
    {0x223790, 0x269CE2, 0x252732, 1, 16, 0x3D314E, "K.Rool Duel"},
    {0x229E5F, 0x26A896, 0x252832, 1, 80, 0x3D398E, "Ice 1"},
    {0x229E5F, 0x26A896, 0x254351, 1, 16, 0x3D398E, "Ice 2"},
    {0x22E556, 0x26C9FF, 0x254E8A, 0, 16, 0x3D430E, "Jungle"}
};

static const struct DKC2_LEVEL levels[] = {
    {0, 0x35BE29, 0x3D0CD0,  0, "Pirate Panic"},
    {0, 0x35BE5B, 0x3D0CD0,  0, "Pirate Panic Bonus 1"},
    {0, 0x35BE65, 0x3D0CD0,  0, "Pirate Panic Bonus 2"},
    {0, 0x35BE51, 0x3D0CD0,  0, "Pirate Panic Warp Room"},
    {1, 0x35BEB1, 0x3D1610,  0, "Mainbrace Mayhem"},
    {1, 0x35BECF, 0x3D1610,  0, "Mainbrace Mayhem Bonus 1"},
    {1, 0x35BED9, 0x3D1610,  0, "Mainbrace Mayhem Bonus 2"},
    {1, 0x35BEF7, 0x3D1610,  0, "Mainbrace Mayhem Bonus 3"},
    {1, 0x35BEC5, 0x3D1610,  0, "Mainbrace Mayhem Warp Room"},
    {0, 0x35BE33, 0x3D0CD0,  0, "Gangplank Galley"},
    {0, 0x35BE6F, 0x3D0CD0,  0, "Gangplank Galley Bonus 2"},
    {0, 0x35BE33, 0x3D0CD0, 65, "Gangplank Galley (Sunset)"},
    {2, 0x35BD91, 0x3D0910,  0, "Lockjaw's Locker"},
    {2, 0x35BD9B, 0x3D0910,  0, "Lockjaw's Locker Bonus 1"},
    {2, 0x35BDAF, 0x3D0910,  0, "Lockjaw's Locker Warp Room"},
    {2, 0x35BD91, 0x3D0910, 53, "Lockjaw's Locker (Underwater)"},
    {2, 0x35BD9B, 0x3D0910, 53, "Lockjaw's Locker Bonus 1 (Underwater)"},
    {2, 0x35BDAF, 0x3D0910, 53, "Lockjaw's Locker Warp Room (Underwater)"},
    {1, 0x35BEA7, 0x3D14F0,  0, "Topsail Trouble"},
    {1, 0x35BEED, 0x3D14F0,  0, "Topsail Trouble Bonus 1"},
    {1, 0x35BEF7, 0x3D14F0,  0, "Topsail Trouble Bonus 2"},
    {1, 0x35BEC5, 0x3D14F0,  0, "Topsail Trouble Warp Room"},
    {1, 0x35BE93, 0x3D1610,  0, "Krow's Nest"},

    {8, 0x35BF49, 0x3D0DF0,  0, "Hot Head Hop"},
    {8, 0x35BF8F, 0x3D0DF0,  0, "Hot Head Hop Bonus 1"},
    {8, 0x35BF67, 0x3D0DF0,  0, "Hot Head Hop Bonus 2 & Warp Room"},
    {8, 0x35BF85, 0x3D0DF0,  0, "Hot Head Hop Bonus 3"},
    {9, 0x35BFEB, 0x3D228E,  0, "Kannon's Klaim"},
    {9, 0x35C01D, 0x3D228E,  0, "Kannon's Klaim Bonus 1"},
    {9, 0x35C031, 0x3D228E,  0, "Kannon's Klaim Bonus 2"},
    {9, 0x35C013, 0x3D228E,  0, "Kannon's Klaim Bonus 3"},
    {9, 0x35BFF5, 0x3D228E,  0, "Kannon's Klaim Warp Room"},
    {2, 0x35BD87, 0x3D07F0, 53, "Lava Lagoon (Underwater)"},
    {2, 0x35BDAF, 0x3D07F0, 53, "Lava Lagoon Bonus 1 & Warp Room (Underwater)"},
    {2, 0x35BD87, 0x3D07F0, 54, "Lava Lagoon (Lava)"},
    {2, 0x35BDAF, 0x3D07F0, 54, "Lava Lagoon Bonus 1 & Warp Room (Lava)"},
    {2, 0x35BD87, 0x3D07F0,  0, "Lava Lagoon"},
    {2, 0x35BDAF, 0x3D07F0,  0, "Lava Lagoon Bonus 1 & Warp Room"},
    {8, 0x35BF53, 0x3D0DF0,  0, "Red-Hot Ride"},
    {8, 0x35BF99, 0x3D0DF0,  0, "Red-Hot Ride Bonus 1"},
    {8, 0x35BFA3, 0x3D0DF0,  0, "Red-Hot Ride Bonus 2"},
    {9, 0x35BFE1, 0x3D238E,  0, "Squawks's Shaft (Natural)"},
    {9, 0x35C027, 0x3D238E,  0, "Squawks's Shaft Bonus 1 (Natural)"},
    {9, 0x35C031, 0x3D238E,  0, "Squawks's Shaft Bonus 2 (Natural)"},
    {9, 0x35C009, 0x3D238E,  0, "Squawks's Shaft Bonus 3 (Natural)"},
    {9, 0x35BFE1, 0x3D238E,  0, "Squawks's Shaft (Fixed)"},
    {9, 0x35C027, 0x3D238E, 97, "Squawks's Shaft Bonus 1 (Fixed)"},
    {9, 0x35C031, 0x3D238E, 97, "Squawks's Shaft Bonus 2 (Fixed)"},
    {9, 0x35C009, 0x3D238E, 97, "Squawks's Shaft Bonus 3 (Fixed)"},
    {9, 0x35BFF5, 0x3D238E, 97, "Squawks's Shaft Warp Room (Fixed)"},
    {8, 0x35BF7B, 0x3D0DF0,  0, "Kleever's Kiln"},

    {10, 0x35C041, 0x3D27EE,  0, "Barrel Bayou"},
    {10, 0x35C073, 0x3D27EE,  0, "Barrel Bayou Bonus 1"},
    {10, 0x35C07D, 0x3D27EE,  0, "Barrel Bayou Bonus 2"},
    {10, 0x35C055, 0x3D27EE,  0, "Barrel Bayou Warp Room"},
    { 2, 0x35BD7D, 0x3D2BEE,  0, "Glimmer's Galleon (Natural)"},
    { 2, 0x35BDB9, 0x3D2BEE,  0, "Glimmer's Galleon Bonus 1 (Natural)"},
    { 2, 0x35BDA5, 0x3D2BEE,  0, "Glimmer's Galleon Bonus 2 (Natural)"},
    { 2, 0x35BD7D, 0x3D2BEE, 12, "Glimmer's Galleon (Dark)"},
    { 2, 0x35BDB9, 0x3D2BEE, 12, "Glimmer's Galleon Bonus 1 (Dark)"},
    { 2, 0x35BDA5, 0x3D2BEE, 12, "Glimmer's Galleon Bonus 2 (Dark)"},
    { 2, 0x35BD7D, 0x3D2BEE, 11, "Glimmer's Galleon (Light)"},
    { 2, 0x35BDB9, 0x3D2BEE, 11, "Glimmer's Galleon Bonus 1 (Light)"},
    { 2, 0x35BDA5, 0x3D2BEE, 11, "Glimmer's Galleon Bonus 2 (Light)"},
    { 0, 0x35BE47, 0x3D324E,  0, "Glimmer's Galleon Exit Room"},
    {10, 0x35C04B, 0x3D450E,  0, "Krockhead Klamber"},
    {10, 0x35C087, 0x3D450E,  0, "Krockhead Klamber Bonus"},
    { 0, 0x35BE3D, 0x3D324E,  0, "Rattle Battle"},
    { 0, 0x35BE79, 0x3D324E,  0, "Rattle Battle Bonus 1"},
    { 1, 0x35BF15, 0x3D304E,  0, "Rattle Battle Bonus 2"},
    { 0, 0x35BE83, 0x3D324E,  0, "Rattle Battle Bonus 3"},
    { 1, 0x35BE9D, 0x3D304E,  0, "Slime Climb"},
    { 1, 0x35BE9D, 0x3D304E, 55, "Slime Climb (Underwater)"},
    { 1, 0x35BEE3, 0x3D304E,  0, "Slime Climb Bonus 1"},
    { 1, 0x35BF0B, 0x3D304E,  0, "Slime Climb Bonus 2"},
    {11, 0x35C0BF, 0x3D28EE,  0, "Bramble Blast"},
    {12, 0x35C279, 0x3D28EE,  0, "Bramble Blast Bonus 1"},
    {12, 0x35C23D, 0x3D28EE,  0, "Bramble Blast Bonus 2"},
    {10, 0x35C0AF, 0x3D27EE,  0, "Kudgel's Kontest"},

    { 4, 0x35BDD3, 0x3D0A10, 0, "Hornet Hole"},
    { 4, 0x35BDF1, 0x3D0A10, 0, "Hornet Hole Bonus 1"},
    { 5, 0x35BFC7, 0x3D0A10, 0, "Hornet Hole Bonus 2"},
    { 4, 0x35BE05, 0x3D0A10, 0, "Hornet Hole Bonus 3"},
    { 6, 0x35BF25, 0x3D1710, 2, "Target Terror (Red)"},
    { 6, 0x35C29D, 0x3D1710, 0, "Target Terror & Rickety Race Exit Room"},
    { 6, 0x35BF25, 0x3D1710, 7, "Target Terror (Green)"},
    { 6, 0x35BF25, 0x3D1710, 8, "Target Terror (Grey)"},
    {12, 0x35C247, 0x3D2AEE, 0, "Target Terror Bonus 1"},
    { 6, 0x35C2BB, 0x3D1710, 0, "Target Terror Bonus 2"},
    {11, 0x35C0C9, 0x3D2AEE, 0, "Bramble Scramble"},
    {12, 0x35C251, 0x3D2AEE, 0, "Bramble Scramble Bonus"},
    { 6, 0x35BF2F, 0x3D1710, 0, "Rickety Race"},
    { 6, 0x35C2CF, 0x3D1710, 0, "Rickety Race Bonus 1"},
    {10, 0x35C05F, 0x3D460E, 0, "Mudhole Marsh"},
    {10, 0x35C091, 0x3D460E, 0, "Mudhole Marsh Bonus 1"},
    {10, 0x35C09B, 0x3D460E, 0, "Mudhole Marsh Bonus 2"},
    { 4, 0x35BDC9, 0x3D0A10, 0, "Rambi Rumble"},
    { 5, 0x35BFD1, 0x3D0A10, 0, "Rambi Rumble Bonus 1"},
    { 4, 0x35BE19, 0x3D0A10, 0, "Rambi Rumble Bonus 2"},
    { 4, 0x35BDDD, 0x3D0A10, 0, "Rambi Rumble Rambi Room"},
    { 5, 0x35BFBD, 0x3D0A10, 0, "King Zing Sting"},

    { 3, 0x35BD1D, 0x3D06F0, 0, "Ghostly Grove"},
    { 3, 0x35BD3B, 0x3D06F0, 0, "Ghostly Grove Bonus 1"},
    { 3, 0x35BD59, 0x3D06F0, 0, "Ghostly Grove Bonus 2"},
    { 7, 0x35C293, 0x3D2EEE, 0, "Haunted Hall"},
    { 7, 0x35BF39, 0x3D2EEE, 0, "Haunted Hall Bonus 1"},
    { 7, 0x35C2C5, 0x3D2EEE, 0, "Haunted Hall Bonus 2"},
    { 7, 0x35C2B1, 0x3D2EEE, 0, "Haunted Hall Bonus 3"},
    { 7, 0x35C2A7, 0x3D2EEE, 0, "Haunted Hall Exit Room"},
    { 3, 0x35BD13, 0x3D0610, 0, "Gusty Glade"},
    { 3, 0x35BD59, 0x3D0610, 0, "Gusty Glade Bonus 1"},
    { 3, 0x35BD4F, 0x3D0610, 0, "Gusty Glade Bonus 2"},
    { 5, 0x35BFB3, 0x3D0A10, 0, "Parrot Chute Panic"},
    { 4, 0x35BE0F, 0x3D0A10, 0, "Parrot Chute Panic Bonus 1"},
    { 4, 0x35BDFB, 0x3D0A10, 0, "Parrot Chute Panic Bonus 2"},
    { 4, 0x35BDE7, 0x3D0A10, 0, "Parrot Chute Panic Rare Room"},
    { 3, 0x35BD09, 0x3D3A4E, 0, "Web Woods"},
    {12, 0x35C265, 0x3D2AEE, 0, "Web Woods Bonus 1"},
    { 3, 0x35BD63, 0x3D3A4E, 0, "Web Woods Bonus 2"},
    { 3, 0x35BD31, 0x3D3A4E, 0, "Web Woods - Squitter Room"},
    { 1, 0x35BEBB, 0x3D14F0, 0, "Kreepy Krow"},

    {16, 0x35C177, 0x3D398E,  0, "Arctic Abyss"},
    {16, 0x35C1A9, 0x3D398E,  0, "Arctic Abyss Bonus 1"},
    {16, 0x35C1BD, 0x3D398E,  0, "Arctic Abyss Bonus 2"},
    { 9, 0x35BFFF, 0x3D248E,  0, "Windy Well"},
    { 9, 0x35C009, 0x3D248E,  0, "Windy Well Bonus 1"},
    {12, 0x35C25B, 0x3D2AEE,  0, "Windy Well Bonus 2"},
    {13, 0x35C0F7, 0x3D2DEE,  0, "Castle Crush"},
    {13, 0x35C133, 0x3D2DEE,  0, "Castle Crush Bonus 1"},
    {13, 0x35C13D, 0x3D2DEE,  0, "Castle Crush Bonus 2"},
    {16, 0x35C181, 0x3D398E,  0, "Clapper's Cavern"},
    {16, 0x35C195, 0x3D398E,  0, "Clapper's Cavern Bonus 1"},
    {16, 0x35C19F, 0x3D398E,  0, "Clapper's Cavern Bonus 2"},
    {13, 0x35C10B, 0x3D2DEE,  0, "Chain Link Chamber"},
    {13, 0x35C11F, 0x3D2DEE,  0, "Chain Link Chamber Bonus 1"},
    {13, 0x35C129, 0x3D2DEE,  0, "Chain Link Chamber Bonus 2"},
    {13, 0x35C115, 0x3D2DEE,  0, "Toxic Tower"},
    {13, 0x35C115, 0x3D2DEE, 56, "Toxic Tower (Underwater)"},
    {12, 0x35C26F, 0x3D28EE,  0, "Toxic Tower Bonus 1"},
    {13, 0x35C147, 0x3D2DEE,  0, "Stronghold Showdown"},

    {11, 0x35C0D3, 0x3D29EE, 0, "Screech's Sprint"},
    {12, 0x35C283, 0x3D29EE, 0, "Screech's Sprint Bonus"},
    {15, 0x35C167, 0x3D314E, 0, "K.Rool Duel"},

    {18, 0x35C1FF, 0x3D440E, 0, "Jungle Jinx"},
    {18, 0x35C1F5, 0x3D430E, 0, "Jungle Jinx Bonus"},
    { 8, 0x35BF5D, 0x3D0EF0, 0, "Fiery Furnace"},
    {11, 0x35C0E7, 0x3D29EE, 0, "Fiery Furnace Bonus"},
    {17, 0x35C223, 0x3D398E, 0, "Black Ice Battle"},
    {16, 0x35C1B3, 0x3D398E, 0, "Black Ice Battle Bonus"},
    {18, 0x35C1CD, 0x3D430E, 0, "Klobber Karnage"},
    {18, 0x35C1EB, 0x3D440E, 0, "Klobber Karnage Bonus"},
    {18, 0x35C1E1, 0x3D3B6E, 0, "Animal Antics - Rambi"},
    {16, 0x35C18B, 0x3D398E, 0, "Animal Antics - Enguarde"},
    {18, 0x35C209, 0x3D3B6E, 0, "Animal Antics - Squitter"},
    {12, 0x35C233, 0x3D28EE, 0, "Animal Antics - Squawks"},
    {18, 0x35C213, 0x3D3B6E, 0, "Animal Antics - Rattly"},
    {11, 0x35C0DD, 0x3D28EE, 0, "Animal Antics Bonus"},
    {14, 0x35C157, 0x3D2CEE, 0, "Krocodile Kore"},

    { 3, 0x35BD6D, 0x3D3A4E, 0, "Forest Unused Area"},
    {10, 0x35C069, 0x3D460E, 0, "Swamp Unused Area 1"},
    {10, 0x35C0A5, 0x3D460E, 0, "Swamp Unused Area 2"},
    { 8, 0x35BF71, 0x3D0DF0, 0, "Lava BG"},
    {13, 0x35C101, 0x3D2DEE, 0, "Castle BG"},
    {18, 0x35C1D7, 0x3D3B6E, 0, "Jungle BG"}
};

void level2 (
    unsigned char *rom,
    size_t rom_size,
    char *dir,
    int priority,
    int special,
    int tilesets,
    int region
) {
    int length;

    if (special & 2)
        length = sizeof(archetype) / sizeof(struct DKC2_ARCH);
    else
        length = sizeof(levels) / sizeof(struct DKC2_LEVEL);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < length; i++) {

        int arch, palette, pfix, position;
        char *name;

        if (special & 2) {
            arch = i;
            palette = archetype[arch].palette;
            pfix = 0;
        }
        else {
            arch = levels[i].arch;
            palette = levels[i].palette;
            pfix = levels[i].palette_fix;
        }

        if (tilesets) {
            int duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (levels[j].palette == palette && levels[j].palette_fix == pfix) {
                    duplicate = 1;
                    j = i;
                }
            }
            if (duplicate)
                continue;
        }

        unsigned char *tileset  = NULL;
        unsigned char *raw_map  = NULL;
        unsigned char *tilemap  = NULL;
        unsigned char *bitplane = NULL;
        size_t set_counter = 0;
        size_t raw_counter = 0;
        size_t map_counter = 0;
        struct DKC2_ARCH aa = archetype[arch];

        // Data offsets for different regions
        if (region != 1) {
            aa.raw_map += 25;
            aa.tilemap += 25;
            if (arch > 15) {
                aa.raw_map += 10;
                aa.tileset += 25;
            }
        }

        // Decompression
        if (dk_decompress_mem_to_mem(BD_COMP, &tileset, &set_counter, &rom[aa.tileset], rom_size)) {
            fprintf(stderr, "Tileset decompression failed. (%d)\n", i);
            continue;
        }
        if (dk_decompress_mem_to_mem(BD_COMP, &raw_map, &raw_counter, &rom[aa.raw_map], rom_size)) {
            fprintf(stderr, "Raw map decompression failed. (%d)\n", i);
            goto cleanup;
        }


        if (arch == 15) { // K.Rool Duel
            map_counter = 0x100;
            tilemap = malloc(map_counter);
            if (tilemap == NULL) {
                fprintf(stderr, "Tilemap allocation failed. (%d)\n", i);
                goto cleanup;
            }
            memcpy(tilemap, &rom[aa.tilemap], map_counter);
        }
        else if (dk_decompress_mem_to_mem(BD_COMP, &tilemap, &map_counter, &rom[aa.tilemap], rom_size)) {
            fprintf(stderr, "Tilemap decompression failed. (%d)\n", i);
            goto cleanup;
        }

        if (arch == 8) { // Lava Numbers
            memcpy(&tileset[0x340], &rom[0x3B4FC0], 0x1A0);
            memcpy(&tileset[0x020], &rom[0x351047], 0x180);
            memcpy(&tileset[0x4E0], &rom[0x3B0AE0], 0x2C0);
            memcpy(&tileset[0x1A0], &rom[0x3B3AA0], 0x1A0);
        }
        if (arch == 13) { // Castle Tiles
            memcpy(tileset, &rom[0x2199BE], 0x257F);
            memcpy(&tileset[0x4840], &rom[0x21BF3E], 0x37C0);
            set_counter = 0x8000;
        }

        bitplane = malloc((map_counter / 2) * 1024 * 4);

        if (bitplane == NULL) {
            printf("Failed to allocate memory for image data.\n");
            goto cleanup;
        }

        decode_bitplane(rom, tileset, raw_map, bitplane, palette, raw_counter, set_counter, 1, pfix, priority);

        if (tilesets) {
            assemble_bitplane(bitplane, 512, raw_counter, dir, levels[i].name);
        }
        else {
            if (special & 2) {
                position = 0;
                name = archetype[i].name;
            }
            else {
                position = levels[i].position;
                name = levels[i].name;
            }
            assemble_level(bitplane, rom, tilemap, map_counter, position, aa.vert, aa.map_size, 0, dir, name);
        }

cleanup:
        free(tileset);
        free(raw_map);
        free(tilemap);
        free(bitplane);
    }

    return;
}
