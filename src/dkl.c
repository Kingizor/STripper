/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - Donkey Kong Land Series */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "main.h"
#include "misc.h"
#include "bitplane.h"


void decode_palette24 (unsigned char *rgb, unsigned *palette) {
    for (int j = 0; j < 4; j++) {
        rgb[j*3  ] = palette[j] >> 16;
        rgb[j*3+1] = palette[j] >>  8;
        rgb[j*3+2] = palette[j];
    }
}

void dkl2_map_fix (
    unsigned char *rom,
    unsigned char *map_data,
    int map_size,
    int src,
    int base
) {
    for (int i = 0; i < map_size; i++)
        if (map_data[i] & 0x80)
            map_data[i] = rom[base + ((rom[src++] & 0x7F) * 3)];
}

void dkl_layout(unsigned char *rom, unsigned char *map_data, unsigned char *lay_data, unsigned layloc, unsigned char width, unsigned char height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int val = map_data[(i*width)+j];
            for (int k = 0; k < 4; k++) {
                for (int m = 0; m < 4; m++) {
                    int addr = (i*width*16)+(j*4)+(k*width*4)+m;
                    int tile = rom[val+(k*0x400)+(m*0x100)+layloc];
                    lay_data[addr] = tile;
                }
            }
        }
    }
}

int tile_generator(unsigned char **raw_data, size_t *rawlen, int tiles) {
    *raw_data = calloc((tiles+1)*16, 1);
    if (raw_data == NULL) {
        fprintf(stderr, "Failed to allocate memory for tilemap.\n");
        return 1;
    }
    for (int i = 0; i < tiles; i++)
        (*raw_data)[i] = i;
    *rawlen = tiles;
    return 0;
}






struct DKL_ARCH {
    unsigned char set_val; // Bitplane Decompression
    unsigned char tiles;   // Number of tiles
    unsigned      layout;  // 32x32 Location
};

struct DKL_LEVEL {
    unsigned char  arch;    // Archetype
    unsigned char  t_width; // Level Width (32x32)
    unsigned       map;     // Tilemap location
    unsigned short pal;     // SGB Palette Address
    char *name;             // Level Name
};

static const struct DKL_ARCH dkl_arch[] = {
    {0x00, 113, 0x07082}, // Jungle
    {0x1A,  94, 0x07000}, // Cave
    {0x18,  52, 0x1F080}, // Water Temple
    {0x06,  70, 0x1E000}, // Snow
    {0x0E,  69, 0x1D000}, // Mast
    {0x1C, 128, 0x1F000}, // Construction (5)
    {0x17,  74, 0x1F0B4}, // Blimp
    {0x0A,  79, 0x1E046}, // Temple
    {0x0C,  66, 0x1D045}, // Coral
    {0x1D, 113, 0x1D087}, // Cloud
    {0x1B,  36, 0x0705E}, // Mountain (10)
    {0x19, 107, 0x1E095}, // Skyscraper
};

static const struct DKL_LEVEL dkl_areas[] = {
    { 0, 0x7C, 0x3481D, 0xE936, "1-1 Jungle Jaunt"},
    { 1, 0x0F, 0x37F3B, 0xE966, "1-1 Jungle Jaunt B1"},
    { 2, 0x0E, 0x1CD85, 0xE9DE, "1-1 Jungle Jaunt B2"},
    { 3, 0xD4, 0x340DF, 0xE92E, "1-2 Freezing Fun"},
    { 0, 0x05, 0x1BE5E, 0xE92E, "1-2 Freezing Fun B1"},
    { 0, 0x91, 0x34AFC, 0xE976, "1-3 Simian Swing"},
    { 1, 0x09, 0x37ED1, 0xE956, "1-3 Simian Swing B1"},
    { 0, 0x05, 0x1BE5E, 0xE9A6, "1-3 Simian Swing B2"},
    { 4, 0x10, 0x363AB, 0xE986, "1-4 Deck Trek"},
    { 5, 0x08, 0x47FA0, 0xE9AE, "1-4 Deck Trek B1"},
    { 0, 0x0F, 0x3BF79, 0xE93E, "1-4 Deck Trek B2"},
    { 3, 0xB5, 0x344DB, 0xE97E, "1-5 Rope Ravine"},
    { 0, 0x05, 0x1BE5E, 0xE9AE, "1-5 Rope Ravine B1"},
    { 2, 0x0E, 0x1CD3D, 0xE9E6, "1-5 Rope Ravine B2"},
    { 0, 0x8E, 0x34E38, 0xE996, "1-6 Tyre Trail"},
    { 0, 0x05, 0x1BE5E, 0xE9A6, "1-6 Tyre Trail B1"},
    { 1, 0x05, 0x37EB5, 0xE94E, "1-6 Tyre Trail B2"},
    { 1, 0x0F, 0x37EFF, 0xE95E, "1-6 Tyre Trail B3"},
    { 4, 0xFA, 0x35E39, 0xE956, "1-7 Riggin' Rumble"},
    { 6, 0x0A, 0x27FD1, 0xE92E, "1-7 Riggin' Rumble B1"},
    { 0, 0x0D, 0x3BFB3, 0xE946, "1-7 Riggin' Rumble B2"},
    { 0, 0x98, 0x351E3, 0xE93E, "1-8 Congo Carnage"},
    { 4, 0x07, 0x1BE79, 0xE96E, "1-8 Congo Carnage B1"},
    { 0, 0x05, 0x1BE5E, 0xE9AE, "1-8 Congo Carnage B2"},
    { 3, 0xAE, 0x3B6F8, 0xE9EE, "1-9 Arctic Barrel Arsenal"},
    { 0, 0x05, 0x1BE5E, 0xE996, "1-9 Arctic Barrel Arsenal B1"},
    { 5, 0x0B, 0x47FC5, 0xE9A6, "1-9 Arctic Barrel Arsenal B2"},
    { 4, 0x05, 0x37FDC, 0xE97E, "1-B Wild Sting Fling"},

    { 7, 0xB8, 0x35521, 0xE946, "2-1 Tricky Temple"},
    { 5, 0x29, 0x1BEC5, 0xE98E, "2-1 Tricky Temple B1"},
    { 2, 0xB6, 0x37B27, 0xE9AE, "2-2 Kremlantis"},
    { 8, 0x35, 0x365C9, 0xE94E, "2-3 Reef Rampage"},
    { 7, 0xC8, 0x35836, 0xE966, "2-4 Snake Charmer's Challenge"},
    { 9, 0x22, 0x1CDC8, 0xE9CE, "2-4 Snake Charmer's Challenge B1"},
    { 2, 0x8A, 0x1B6C2, 0xEA16, "2-5 Chomp's Coliseum"},
    { 8, 0x3E, 0x36C5E, 0xE95E, "2-6 Nautilus Chase"},
    { 7, 0xB3, 0x35B4B, 0xE96E, "2-7 Swirlwind Storm"},
    { 5, 0x1D, 0x1BF6C, 0xE996, "2-7 Swirlwind Storm B1"},
    { 2, 0x05, 0x3BFED, 0xE9FE, "2-B Seabed Showdown"},

    { 1, 0xAC, 0x396EF, 0xE9BE, "3-1 Pot Hole Panic"},
    { 6, 0x19, 0x2FF26, 0xEA0E, "3-1 Pot Hole Panic B1"},
    { 4, 0x05, 0x37FDC, 0xE94E, "3-1 Pot Hole Panic B2"},
    {10, 0x29, 0x39B6D, 0xE9C6, "3-2 Mountain Mayhem"},
    { 4, 0x20, 0x37F80, 0xE976, "3-2 Mountain Mayhem B1"},
    { 6, 0x05, 0x1CCD7, 0xE966, "3-2 Mountain Mayhem B2"},
    { 9, 0x8B, 0x3A7F6, 0xE9DE, "3-3 Track Attack"},
    { 1, 0x05, 0x37EB5, 0xE946, "3-3 Track Attack B1"},
    { 6, 0x05, 0x1CCD7, 0xEA16, "3-3 Track Attack B2"},
    { 1, 0xAF, 0x1B91E, 0xEA26, "3-4 Spiky Tyre Trail"},
    { 0, 0x05, 0x1BE5E, 0xE93E, "3-4 Spiky Tyre Trail B1"},
    { 0, 0x05, 0x1BE5E, 0xE98E, "3-4 Spiky Tyre Trail B2"},
    { 9, 0x30, 0x3ABE4, 0xE9E6, "3-5 Sky High Caper"},
    { 2, 0x20, 0x27F66, 0xE9F6, "3-5 Sky High Caper B1"},
    {10, 0xC6, 0x27A88, 0xEA0E, "3-6 Landslide Leap"},
    { 1, 0x0F, 0x37EFF, 0xE996, "3-6 Landslide Leap B1"},
    { 6, 0x11, 0x2FF80, 0xEA1E, "3-6 Landslide Leap B2"},
    { 9, 0xBD, 0x3B1BD, 0xE9FE, "3-7 Collapsing Clouds"},
    { 6, 0x09, 0x2FF03, 0xE976, "3-7 Collapsing Clouds B1"},
    { 0, 0x05, 0x1BE5E, 0xE9AE, "3-7 Collapsing Clouds B2"},
    { 1, 0x05, 0x1CE9C, 0xEA2E, "3-B Mad Mole Holes"},

    {11, 0x9B, 0x38001, 0xE9B6, "4-1 Construction Site Fight"},
    { 9, 0x15, 0x1CF86, 0xE9B6, "4-1 Construction Site Fight B1"},
    { 9, 0x24, 0x1CEB6, 0xE9D6, "4-1 Construction Site Fight B2"},
    { 6, 0xBD, 0x376BE, 0xE9A6, "4-2 Kong Krazy"},
    { 5, 0x08, 0x47FA0, 0xE956, "4-2 Kong Krazy B1"},
    { 5, 0x9C, 0x39F8A, 0xE9D6, "4-3 Balloon Barrage"},
    { 6, 0x0A, 0x27FD1, 0xE96E, "4-3 Balloon Barrage B1"},
    { 6, 0x0C, 0x2FFB3, 0xE936, "4-3 Balloon Barrage B2"},
    { 6, 0xB3, 0x3BA45, 0xE9F6, "4-4 Fast Barrel Blast"},
    { 0, 0x05, 0x1BE5E, 0xE9A6, "4-4 Fast Barrel Blast B1"},
    { 0, 0x0A, 0x3BF47, 0xE936, "4-4 Fast Barrel Blast B2"},
    {11, 0xC9, 0x385FB, 0xE9CE, "4-5 Skyscraper Caper"},
    { 2, 0x05, 0x3BFED, 0xE95E, "4-5 Skyscraper Caper B1"},
    { 5, 0x8E, 0x3A417, 0xEA1E, "4-6 Button Barrel Blast"},
    { 1, 0x09, 0x37ED1, 0xE98E, "4-6 Button Barrel Blast B1"},
    { 0, 0x0A, 0x3BF47, 0xE98E, "4-6 Button Barrel Blast B2"},
    {11, 0x29, 0x38F08, 0xEA06, "4-7 Oil Drum Slum"},
    { 2, 0x26, 0x1CCEA, 0xE9EE, "4-7 Oil Drum Slum B1"},
    { 4, 0x07, 0x1BE79, 0xE97E, "4-7 Oil Drum Slum B2"},
    { 6, 0x09, 0x2FF03, 0xEA06, "4-B K. Rool's Kingdom"}
};
static const int dkl_size = sizeof(dkl_areas) / sizeof(struct DKL_LEVEL);

void dkl_levels (unsigned char *rom, size_t rom_size, char *dir, int sgb, unsigned *palette, int tileset) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < dkl_size; i++) {

        const struct DKL_LEVEL *d = &dkl_areas[i];
        const struct DKL_ARCH  *a = &dkl_arch[d->arch];
        unsigned char *set_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *map_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char rgb[12];
        int t_width, t_height;
        size_t set_size = 0;
        size_t map_size = 0;
        char *name = d->name;
        char nbuf[64];
        int e;

        // filter duplicate tilesets
        if (tileset) {
            int j;
            for (j = 0; j < i; j++)
                if (d->arch == dkl_areas[j].arch
                && (sgb || (!sgb && (d->pal == dkl_areas[j].pal))))
                    break;
            if (j < i)
                continue;
        }

        lay_data = malloc(0x20000);
        if (lay_data == NULL) {
            fprintf(stderr, "Failed to allocate memory for set/lay data.\n");
            goto cleanup;
        }

        if (!tileset) {
            if ((e = dk_decompress_mem_to_mem(DKL_COMP, &map_data, &map_size, rom+d->map, rom_size-d->map))) {
                fprintf(stderr, "tilemap:%d: %s\n", i, dk_get_error(e));
                goto cleanup;
            }
            t_width  = d->t_width;
            t_height = map_size / d->t_width;
        }
        else {
            if (tile_generator(&map_data, &map_size, a->tiles))
                goto cleanup;
            t_width = 16;
            t_height = (map_size / t_width);
            if (map_size % t_width) t_height++;
            snprintf(nbuf, 64, "%s Tileset", name);
            name = nbuf;
        }

        // Decompress bitplane data
        unsigned char *bp = &rom[0x18000 | (a->set_val*5+0x4001)];
        unsigned      set_addr = ((bp[2] - 0x40) << 8) | bp[1];
        set_addr |= 0x4000 * (((bp[3] & 15) ? (bp[3] & 15) : 18));
        if ((e = dkl_huffman_decode(rom+set_addr, rom_size-set_addr, &set_data, &set_size, &rom[0x3D00], bp[0]))) {
            fprintf(stderr, "tileset:%d: %s\n", i, dk_get_error(e));
            goto cleanup;
        }

        // Skyscraper Rope Fix
        if (d->arch == 11) {
            int j;
            for (j = 0; j < 16; j++) {
                unsigned char t   = set_data[0x1C0+j];
                set_data[0x1C0+j] = set_data[0x1D0+j];
                set_data[0x1D0+j] = t;
            }
        }

        // Assemble complete layout from lay and raw
        dkl_layout(rom, map_data, lay_data, a->layout, t_width, t_height);

        if (!sgb)
            decode_palette(rgb, &rom[d->pal], 4);
        else
            decode_palette24(rgb, palette);

        bitplane = malloc(t_width * t_height * 1024 * 4);
        if (bitplane == NULL) {
            fprintf(stderr, "Failed to allocate memory for bitplane. (%d)\n", i);
            goto cleanup;
        }

        gbc_assemble(bitplane, set_data, lay_data, lay_data, rgb, t_width, t_height, 0);
        arrange_gbc(bitplane, (t_width*32), (t_height*32), dir, name);
cleanup:
        free(set_data);
        free(lay_data);
        free(map_data);
        free(bitplane);
    }
}



















struct DKL2_LEVEL {
    unsigned char t_width;  // Level Width (32x32)
    unsigned char arch;     // Archetype
    unsigned      map;      // Tilemap data
    unsigned      mf1;      // Map Fixes Part 1
    unsigned      mf2;      // Map Fixes Part 2
    char *name;             // Level Name
};

struct DKL2_ARCH {
    unsigned      set_addr; // Bitplane Decompression
    unsigned      layout;   // 32x32 Location
    unsigned char tiles;    // Number of
    unsigned char pal[8];   // SGB Palette (Archetype specific)
};

static const struct DKL2_ARCH dkl2_arch[] = {
    //  3A35 ROM,  HL/SP, A      2FB8 DBD0, ROM & H
    { 0x48AE8, 0x4703D, 114, {0xFF, 0x3A, 0xB8, 0x11, 0xF0, 0x08, 0x00, 0x00} }, // Ship Deck
    { 0x48002, 0x460AE,  72, {0x53, 0x7F, 0x10, 0x42, 0xCE, 0x10, 0x00, 0x00} }, // Ship Mast
    { 0x51512, 0x440A2,  86, {0x4F, 0x7F, 0x0D, 0x42, 0xCC, 0x00, 0x00, 0x00} }, // Ship Hold
    { 0x4E0B8, 0x45070, 114, {0x5F, 0x33, 0x7E, 0x02, 0x92, 0x10, 0x00, 0x00} }, // Lava
    { 0x4944A, 0x44067,  59, {0x76, 0x22, 0x71, 0x01, 0xAC, 0x00, 0x00, 0x00} }, // Mine
    { 0x4EA2C, 0x470AF,  69, {0x9A, 0x03, 0x50, 0x02, 0x0B, 0x01, 0x00, 0x00} }, // Swamp
    { 0x50ABD, 0x45000, 112, {0x5F, 0x03, 0x19, 0x02, 0xF3, 0x00, 0x00, 0x00} }, // Hive
    { 0x4D5A1, 0x46057,  87, {0xBF, 0x5B, 0x58, 0x32, 0xAD, 0x00, 0x00, 0x00} }, // Fair
    { 0x4C002, 0x52000,  51, {0x76, 0x22, 0x48, 0x02, 0x64, 0x01, 0x00, 0x00} }, // Brambles
    { 0x49E6B, 0x44000, 102, {0x76, 0x22, 0x90, 0x01, 0xAE, 0x00, 0x00, 0x00} }, // Forest
    { 0x4CB68, 0x47000,  61, {0x7B, 0x77, 0xF1, 0x72, 0x80, 0x65, 0x00, 0x00} }, // Ice
    { 0x50002, 0x52032,  51, {0x78, 0x7F, 0x31, 0x4A, 0x10, 0x01, 0x00, 0x00} }, // Castle
    { 0x4A8AE, 0x46000,  85, {0x58, 0x03, 0x91, 0x02, 0x8C, 0x01, 0x00, 0x00} }, // Jungle
};

static const struct DKL2_LEVEL dkl2_areas[] = {
    {0xBD, 0x00, 0x5934F, 0x59690, 0x40AF9, "1-1 Pirate Panic"},
    {0x09, 0x00, 0x5C1C0, 0x5C1F2, 0x4176E, "1-1 Pirate Panic B1"},
    {0x14, 0x00, 0x5C1FB, 0x5C263, 0x4176E, "1-1 Pirate Panic B2"},
    {0x1E, 0x01, 0x588DA, 0x58CCD, 0x40A18, "1-2 Mainbrace Mayhem"},
    {0x08, 0x01, 0x5C002, 0x5C02A, 0x41732, "1-2 Mainbrace Mayhem B1"},
    {0x10, 0x01, 0x5C030, 0x5C0A0, 0x41732, "1-2 Mainbrace Mayhem B2"},
    {0xE3, 0x00, 0x596EE, 0x59A69, 0x40AF9, "1-3 Gangplank Galley"},
    {0x09, 0x00, 0x5C1C0, 0x5C1F2, 0x4176E, "1-3 Gangplank Galley B1"},
    {0x21, 0x00, 0x5C266, 0x5C2B9, 0x4176E, "1-3 Gangplank Galley B2"},
    {0x4C, 0x02, 0x59AED, 0x5A344, 0x4164E, "1-4 Lockjaw's Locker"},
    {0x08, 0x02, 0x5D7C9, 0x5D7F4, 0x419B7, "1-4 Lockjaw's Locker B1"},
    {0x1E, 0x01, 0x58D73, 0x591FE, 0x40A18, "1-5 Topsail Trouble"},
    {0x08, 0x01, 0x5C002, 0x5C02A, 0x41732, "1-5 Topsail Trouble B1"},
    {0x0F, 0x01, 0x5C0B5, 0x5C137, 0x41732, "1-5 Topsail Trouble B2"},
    {0x06, 0x01, 0x5929C, 0x592B7, 0x40A18, "1-B Krow's Nest"}, // 15

    {0xFB, 0x03, 0x5B413, 0x5B7F3, 0x41156, "2-1 Hothead Hop"},
    {0x12, 0x03, 0x5C82F, 0x5C891, 0x41855, "2-1 Hothead Hop B1"},
    {0x1A, 0x03, 0x5C896, 0x5C910, 0x41855, "2-1 Hothead Hop B2"},
    {0x0F, 0x04, 0x5AC38, 0x5AF3D, 0x40CEE, "2-2 Kannon's Klaim"},
    {0x0C, 0x04, 0x5C611, 0x5C69F, 0x41822, "2-2 Kannon's Klaim B1"},
    {0x93, 0x02, 0x5A3F8, 0x5AB99, 0x4164E, "2-3 Lava Lagoon"},
    {0x15, 0x02, 0x5D7F9, 0x5D902, 0x419B7, "2-3 Lava Lagoon B1"},
    {0x1A, 0x02, 0x5D917, 0x5D9C4, 0x419B7, "2-3 Lava Lagoon B2"},
    {0xE2, 0x03, 0x5B854, 0x5BB90, 0x41156, "2-4 Redhot Ride"},
    {0x13, 0x03, 0x5C917, 0x5C971, 0x41855, "2-4 Redhot Ride B1"},
    {0x13, 0x04, 0x5AFA5, 0x5B380, 0x40CEE, "2-5 Squawks's Shaft"},
    {0x0C, 0x04, 0x5C6AB, 0x5C734, 0x41822, "2-5 Squawks's Shaft B1"},
    {0x09, 0x04, 0x5C741, 0x5C7BF, 0x41822, "2-5 Squawks's Shaft B2"},
    {0xF5, 0x05, 0x5DFC8, 0x5E36A, 0x412A0, "2-6 Barrel Bayou"},
    {0x0A, 0x05, 0x5CD04, 0x5CD3C, 0x418AC, "2-6 Barrel Bayou B1"},
    {0x10, 0x05, 0x5CD43, 0x5CD9B, 0x418AC, "2-6 Barrel Bayou B2"},
    {0x55, 0x02, 0x62158, 0x62C3F, 0x4164E, "2-7 Glimmer's Galleon"},
    {0x0C, 0x02, 0x5D9CC, 0x5DA5E, 0x419B7, "2-7 Glimmer's Galleon B1"},
    {0x15, 0x02, 0x5DA89, 0x5DB98, 0x419B7, "2-7 Glimmer's Galleon B2"},
    {0xDE, 0x05, 0x617DB, 0x61BF0, 0x412A0, "2-8 Krochead Klamber"},
    {0x1C, 0x05, 0x5CDA5, 0x5CE3B, 0x418AC, "2-8 Krochead Klamber B1"},
    {0xE4, 0x00, 0x6066E, 0x60A64, 0x40AF9, "2-9 Rattle Battle"},
    {0x10, 0x00, 0x5C2C2, 0x5C334, 0x4176E, "2-9 Rattle Battle B1"},
    {0x1E, 0x01, 0x60002, 0x60586, 0x40A18, "2-A Slime Climb"},
    {0x15, 0x01, 0x5C143, 0x5C1B9, 0x41732, "2-A Slime Climb B1"},
    {0x1B, 0x03, 0x592BA, 0x59346, 0x41156, "2-B Kleaver's Kiln"}, // 26

    {0x4C, 0x06, 0x60ADD, 0x60FFA, 0x414D4, "3-1 Hornet Hole"},
    {0x11, 0x06, 0x5D193, 0x5D1FC, 0x41936, "3-1 Hornet Hole B1"},
    {0xFF, 0x07, 0x58003, 0x58414, 0x4107B, "3-2 Target Terror"},
    {0x36, 0x07, 0x5D3E8, 0x5D4ED, 0x4196C, "3-2 Target Terror B1"},
    {0x2B, 0x07, 0x5D5E9, 0x5D6A6, 0x4196C, "3-2 Target Terror B2"},
    {0x4F, 0x08, 0x62D35, 0x63373, 0x40EEC, "3-3 Bramble Blast"},
    {0x11, 0x08, 0x5CA20, 0x5CAE7, 0x41885, "3-3 Bramble Blast B1"},
    {0x15, 0x08, 0x5CAF5, 0x5CB85, 0x41885, "3-3 Bramble Blast B2"},
    {0xFF, 0x07, 0x58486, 0x5887F, 0x4107B, "3-4 Rickety Race"},
    {0x46, 0x08, 0x63460, 0x63B91, 0x40EEC, "3-5 Bramble Scramble"},
    {0x15, 0x08, 0x5CB90, 0x5CC27, 0x41885, "3-5 Bramble Scramble B1"},
    {0xFE, 0x05, 0x61C6A, 0x620D8, 0x412A0, "3-6 Mudhole Marsh"},
    {0x19, 0x05, 0x5CE4A, 0x5CEC9, 0x418AC, "3-6 Mudhole Marsh B1"},
    {0x25, 0x05, 0x5CED1, 0x5CF93, 0x418AC, "3-6 Mudhole Marsh B2"},
    {0x46, 0x06, 0x610AF, 0x616E5, 0x414D4, "3-7 Rambi Rumble"},
    {0x13, 0x06, 0x5D201, 0x5D262, 0x41936, "3-7 Rambi Rumble B1"},
    {0x29, 0x06, 0x5D26D, 0x5D2E8, 0x41936, "3-7 Rambi Rumble B2"},
    {0x0C, 0x06, 0x5DF82, 0x5DFC4, 0x414D4, "3-B King Zing Sting"}, // 18

    {0xFF, 0x09, 0x64002, 0x64478, 0x40E02, "4-1 Ghostly Grove"},
    {0x1D, 0x09, 0x5CFB3, 0x5D04E, 0x41906, "4-1 Ghostly Grove B1"},
    {0xFF, 0x07, 0x64EC9, 0x65383, 0x4107B, "4-2 Krazy Koaster"},
    {0x38, 0x07, 0x5D6B3, 0x5D7A4, 0x4196C, "4-2 Krazy Koaster B1"},
    {0xFF, 0x09, 0x6452C, 0x64993, 0x40E02, "4-3 Gusty Glade"},
    {0x15, 0x09, 0x5D068, 0x5D0BE, 0x41906, "4-3 Gusty Glade B1"},
    {0x18, 0x09, 0x5D0CD, 0x5D132, 0x41906, "4-3 Gusty Glade B2"},
    {0x13, 0x06, 0x653D9, 0x65911, 0x414D4, "4-4 Parrot Chute Panic"},
    {0x0F, 0x06, 0x5D2F5, 0x5D357, 0x41936, "4-4 Parrot Chute Panic B1"},
    {0x12, 0x06, 0x5D361, 0x5D3DA, 0x41936, "4-4 Parrot Chute Panic B2"},
    {0xFF, 0x09, 0x649EF, 0x64E51, 0x40E02, "4-5 Web Woods"},
    {0x14, 0x09, 0x5D13A, 0x5D18D, 0x41906, "4-5 Web Woods B1"},
    {0x06, 0x01, 0x5E3E2, 0x5E422, 0x40A18, "4-B Kreepy Krow"}, // 13

    {0x47, 0x0A, 0x659D6, 0x6600F, 0x40BE0, "5-1 Arctic Abyss"},
    {0x0B, 0x0A, 0x5C34C, 0x5C3CF, 0x417AA, "5-1 Arctic Abyss B1"},
    {0x11, 0x0A, 0x5C3D8, 0x5C472, 0x417AA, "5-1 Arctic Abyss B2"},
    {0x0B, 0x04, 0x69FD6, 0x6A366, 0x40CEE, "5-2 Windy Well"},
    {0x14, 0x04, 0x5C7C7, 0x5C80E, 0x41822, "5-2 Windy Well B1"},
    {0x20, 0x0B, 0x665F4, 0x66C10, 0x413F0, "5-3 Dungeon Danger"},
    {0x12, 0x0B, 0x5DCDD, 0x5DD45, 0x417FE, "5-3 Dungeon Danger B1"},
    {0x12, 0x0B, 0x5DD4B, 0x5DDA5, 0x417FE, "5-3 Dungeon Danger B2"},
    {0xFC, 0x0A, 0x660A9, 0x6656D, 0x40BE0, "5-4 Clapper's Cavern"},
    {0x1D, 0x0A, 0x5C48D, 0x5C52B, 0x417AA, "5-4 Clapper's Cavern B1"},
    {0x10, 0x0B, 0x66C6A, 0x672A2, 0x413F0, "5-5 Chain Link Chamber"},
    {0x14, 0x0B, 0x5DDBA, 0x5DE35, 0x417FE, "5-5 Chain Link Chamber B1"},
    {0x19, 0x0B, 0x5DE48, 0x5DEBA, 0x417FE, "5-5 Chain Link Chamber B2"},
    {0x13, 0x0B, 0x6734F, 0x67AE2, 0x413F0, "5-6 Toxic Tower"},
    {0x15, 0x0B, 0x5DEC1, 0x5DF48, 0x417FE, "5-6 Toxic Tower B1"},
    {0x09, 0x0B, 0x67BAA, 0x67CCE, 0x413F0, "5-B Stronghold Showdown"}, // 16

    {0x5B, 0x08, 0x69205, 0x69845, 0x40EEC, "6-1 Screech's Sprint"},
    {0x1A, 0x08, 0x5CC49, 0x5CCFB, 0x41885, "6-1 Screech's Sprint B1"},
    {0x08, 0x0B, 0x5DF58, 0x5DF7F, 0x413F0, "6-B K.Rool Duel"}, // 3

    {0xFF, 0x0C, 0x684A1, 0x6889A, 0x40FBE, "7-1 Jungle Jinx"},
    {0x19, 0x0C, 0x5DBB2, 0x5DC00, 0x417D7, "7-1 Jungle Jinx B1"},
    {0x11, 0x0A, 0x6990F, 0x69EFD, 0x40BE0, "7-2 Black Ice Battle"},
    {0x0D, 0x0A, 0x5C533, 0x5C604, 0x417AA, "7-2 Black Ice Battle B1"},
    {0xFF, 0x03, 0x68003, 0x6840F, 0x41156, "7-3 Fiery Furnace"},
    {0x1D, 0x03, 0x5C989, 0x5CA11, 0x41855, "7-3 Fiery Furnace B1"},
    {0xFF, 0x0C, 0x68917, 0x68D0D, 0x40FBE, "7-4 Klobber Karnage"},
    {0x14, 0x0C, 0x5DC0A, 0x5DC61, 0x417D7, "7-4 Klobber Karnage B1"},
    {0xE9, 0x0C, 0x68DBB, 0x69187, 0x40FBE, "7-5 Animal Antics"},
    {0x16, 0x0C, 0x5DC74, 0x5DCC2, 0x417D7, "7-5 Animal Antics B1"},
    {0x08, 0x03, 0x69FAB, 0x69FD3, 0x41156, "7-B Krocodile Kore"}, // 11
};
static const int dkl2_size = sizeof(dkl2_areas) / sizeof(struct DKL2_LEVEL);

void dkl2_levels (unsigned char *rom, size_t rom_size, char *dir, int sgb, unsigned *palette, int tileset) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < dkl2_size; i++) {

        const struct DKL2_LEVEL *d = &dkl2_areas[i];
        const struct DKL2_ARCH  *a = &dkl2_arch[d->arch];
        unsigned char *set_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *col_data = NULL;
        unsigned char *map_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char rgb[12];
        int t_width, t_height;
        size_t set_size = 0;
        size_t map_size = 0;
        char *name = d->name;
        char nbuf[64];
        int e;

        // filter duplicate tilesets
        if (tileset) {
            int j;
            for (j = 0; j < i; j++)
                if (d->arch == dkl2_areas[j].arch)
                    break;
            if (j < i)
                continue;
        }

        lay_data = malloc(0x20000);
        col_data = malloc(0x20000);
        if (lay_data == NULL
        ||  col_data == NULL) {
            fprintf(stderr, "Failed to allocate memory for set/lay/col data. (%d)\n", i);
            goto cleanup;
        }

        if (!tileset) {
            if ((e = dk_decompress_mem_to_mem(DKL_COMP, &map_data, &map_size, rom+d->map, rom_size-d->map))) {
                fprintf(stderr, "tilemap:%d: %s\n", i, dk_get_error(e));
                goto cleanup;
            }
            dkl2_map_fix(rom, map_data, map_size, d->mf1, d->mf2);
            t_width  = d->t_width;
            t_height = map_size / d->t_width;
        }
        else {
            if (tile_generator(&map_data, &map_size, a->tiles))
                goto cleanup;
            t_width = 16;
            t_height = (map_size / t_width);
            if (map_size % t_width) t_height++;
            snprintf(nbuf, 64, "%s Tileset", name);
            name = nbuf;
        }

        // Decompress Bitplane data
        if ((e = dkl_huffman_decode(rom+a->set_addr, rom_size-a->set_addr, &set_data, &set_size, &rom[0x3D00], rom[a->set_addr-1]))) {
            fprintf(stderr, "tileset:%d: %s\n", i, dk_get_error(e));
            goto cleanup;
        }

        // Decode layout (raw is stored uncompressed in ROM)
        dkl_layout(rom, map_data, lay_data, a->layout, t_width, t_height);

        if (!sgb)
            decode_palette(rgb, a->pal, 4);
        else
            decode_palette24(rgb, palette);

        bitplane = malloc(t_width * t_height * 1024 * 4);
        if (bitplane == NULL) {
            fprintf(stderr, "Failed to allocate memory for bitplane. (%d)\n", i);
            goto cleanup;
        }

        gbc_assemble(bitplane, set_data, lay_data, col_data, rgb, t_width, t_height, 0);
        arrange_gbc(bitplane, (t_width*32), (t_height*32), dir, name);

cleanup:
        free(set_data);
        free(lay_data);
        free(map_data);
        free(col_data);
        free(bitplane);
    }
}










struct DKL3_LEVEL {
    unsigned char t_width; // Level Width (32x32)
    unsigned char arch;    // Archetype
    unsigned      map;     // Tilemap data
    unsigned      mf1;     // Map Fixes Part 1
    unsigned      mf2;     // Map Fixes Part 2
    unsigned char pal;     // SGB Palette (Level specific)
    char *name;            // Level Name
};

struct DKL3_ARCH {
    unsigned      set_addr; // Bitplane Decompression
    unsigned      layout;   // 32x32 Location
    unsigned char tiles;    // Number of tiles in tileset
};

static const struct DKL3_ARCH dkl3_arch[] = {
    { 0x49CB1, 0x44000, 128}, // Boardwalk
    { 0x4A673, 0x5405F,  50}, // Underwater
    { 0x48002, 0x47000, 109}, // Mill
    { 0x4E1AA, 0x45000, 106}, // Riverside
    { 0x4EC8C, 0x4407F,  99}, // Tree
    { 0x48796, 0x4706D,  95}, // Snow
    { 0x4C002, 0x46000,  76}, // Waterfall
    { 0x4CBDF, 0x4604C,  79}, // Cliff
    { 0x4B251, 0x4506A,  68}, // Jungle
    { 0x4D788, 0x54000,  95}, // Le Factoire
    { 0x4F746, 0x54091, 106}, // Pipe
    { 0x4915A, 0x4609B,  71}, // Cave
};

static const unsigned char dkl3_pal[][8] = { // SGB Palettes
    { 0x16, 0x7F, 0xD0, 0x45, 0x8A, 0x08, 0x00, 0x00 },
    { 0xFD, 0x7E, 0xF6, 0x59, 0x0F, 0x35, 0x00, 0x00 },
    { 0x1B, 0x57, 0xD2, 0x2D, 0x8A, 0x08, 0x00, 0x00 },
    { 0x3F, 0x03, 0x17, 0x02, 0x0E, 0x01, 0x00, 0x00 },
    { 0x79, 0x26, 0xAC, 0x15, 0x00, 0x01, 0x00, 0x00 },
    { 0x36, 0x16, 0x91, 0x0D, 0xEB, 0x00, 0x00, 0x00 },
    { 0xDA, 0x7F, 0xD1, 0x5A, 0xC9, 0x35, 0x00, 0x00 },
    { 0x9F, 0x7E, 0x58, 0x51, 0x11, 0x20, 0x00, 0x00 },
    { 0xD9, 0x2E, 0xEF, 0x15, 0x24, 0x01, 0x00, 0x00 },
    { 0xDF, 0x36, 0xB4, 0x21, 0x8B, 0x08, 0x00, 0x00 },
    { 0xF7, 0x6F, 0x8C, 0x46, 0x00, 0x21, 0x00, 0x00 },
    { 0xD3, 0x2A, 0xE6, 0x19, 0x00, 0x0D, 0x00, 0x00 },
    { 0x36, 0x36, 0x91, 0x21, 0xEB, 0x10, 0x00, 0x00 },
    { 0xF8, 0x7F, 0xA6, 0x56, 0x60, 0x29, 0x00, 0x00 },
    { 0x3C, 0x67, 0x4C, 0x41, 0x08, 0x21, 0x00, 0x00 },
    { 0x57, 0x6F, 0xEC, 0x49, 0xA0, 0x24, 0x00, 0x00 },
    { 0x0E, 0x33, 0x2C, 0x1A, 0x29, 0x01, 0x00, 0x00 },
    { 0xD7, 0x6E, 0xAF, 0x49, 0xA6, 0x24, 0x00, 0x00 },
    { 0x79, 0x02, 0x52, 0x01, 0x2B, 0x00, 0x00, 0x00 },
    { 0xBA, 0x7F, 0x2F, 0x4E, 0xA3, 0x1C, 0x00, 0x00 },
    { 0xF5, 0x02, 0x0E, 0x02, 0xC7, 0x00, 0x00, 0x00 },
    { 0xF8, 0x6F, 0x6C, 0x4E, 0xC0, 0x2C, 0x00, 0x00 },
    { 0x9A, 0x7E, 0xCD, 0x55, 0x00, 0x29, 0x00, 0x00 },
    { 0xFF, 0x27, 0x97, 0x1A, 0x0F, 0x11, 0x00, 0x00 },
    { 0x72, 0x02, 0x8C, 0x01, 0xA7, 0x00, 0x00, 0x00 },
    { 0x3C, 0x67, 0x14, 0x42, 0x0C, 0x21, 0x00, 0x00 },
    { 0xFF, 0x3A, 0xB8, 0x11, 0xF0, 0x08, 0x00, 0x00 },
    { 0xF4, 0x02, 0x10, 0x02, 0x2B, 0x01, 0x00, 0x00 },
    { 0x36, 0x6F, 0x11, 0x4E, 0x0C, 0x31, 0x00, 0x00 },
    { 0xF8, 0x7F, 0x70, 0x42, 0xC9, 0x00, 0x00, 0x00 },
    { 0x1F, 0x7F, 0x17, 0x5A, 0x0E, 0x35, 0x00, 0x00 },
    { 0xF7, 0x43, 0x91, 0x22, 0x0C, 0x01, 0x00, 0x00 },
    { 0x16, 0x7F, 0x4C, 0x5A, 0x81, 0x35, 0x00, 0x00 },
    { 0x7F, 0x63, 0xF5, 0x31, 0x8A, 0x00, 0x00, 0x00 },
    { 0xFC, 0x56, 0x94, 0x45, 0x4C, 0x38, 0x00, 0x00 },
    { 0xF7, 0x02, 0xF1, 0x01, 0xEC, 0x00, 0x00, 0x00 },
    { 0x13, 0x6B, 0xF1, 0x35, 0xCD, 0x00, 0x00, 0x00 },
    { 0x1F, 0x1B, 0xF6, 0x0D, 0xCD, 0x00, 0x00, 0x00 }
};

static const struct DKL3_LEVEL dkl3_areas[] = {
    {0x97, 0x00, 0x5C003, 0x5C3A0, 0x40F5A,  0, "1-1 Red Wharf"},
    {0x19, 0x00, 0x61447, 0x614E5, 0x41B33,  0, "1-1 Red Wharf B1"},
    {0x0F, 0x00, 0x614EF, 0x6156B, 0x41B33,  0, "1-1 Red Wharf B2"},
    {0x0E, 0x00, 0x621CE, 0x62206, 0x41B33,  0, "1-1 Red Wharf Warp"},
    {0x47, 0x01, 0x58003, 0x58B60, 0x40BF1,  1, "1-2 Seabed Shanty"},
    {0x15, 0x01, 0x6046F, 0x60546, 0x41A22,  1, "1-2 Seabed Shanty B1"},
    {0x18, 0x01, 0x6054F, 0x60647, 0x41A22,  1, "1-2 Seabed Shanty B2"},
    {0x15, 0x01, 0x620B7, 0x62118, 0x41A22,  1, "1-2 Seabed Shanty Warp"},
    {0xBF, 0x00, 0x5C3E2, 0x5C912, 0x40F5A,  2, "1-3 Ford Knocks"},
    {0x19, 0x00, 0x61575, 0x6160D, 0x41B33,  2, "1-3 Ford Knocks B1"},
    {0x14, 0x00, 0x61615, 0x61655, 0x41B33,  2, "1-3 Ford Knocks B2"},
    {0x0B, 0x00, 0x6220C, 0x6225B, 0x41B33,  2, "1-3 Ford Knocks Warp"},
    {0x38, 0x02, 0x5A459, 0x5A824, 0x40CF9,  3, "1-4 Total Rekoil"},
    {0x17, 0x02, 0x60917, 0x60967, 0x41A82,  3, "1-4 Total Rekoil B1"},
    {0x16, 0x02, 0x6096F, 0x609D7, 0x41A82,  3, "1-4 Total Rekoil B2"},
    {0x10, 0x02, 0x62123, 0x6215C, 0x41A82,  3, "1-4 Total Rekoil Warp"},
    {0xA2, 0x03, 0x6A261, 0x6A5A5, 0x40E37,  4, "1-5 Koco Channel"},
    {0x29, 0x03, 0x60C45, 0x60CF6, 0x41ADC,  4, "1-5 Koco Channel B1"},
    {0x1E, 0x03, 0x60D06, 0x60DD3, 0x41ADC,  4, "1-5 Koco Channel B2"},
    {0x0C, 0x03, 0x621AF, 0x621CA, 0x41ADC,  4, "1-5 Koco Channel Warp"},
    {0x4A, 0x02, 0x5A890, 0x5AE0E, 0x40CF9,  5, "1-6 Liftshaft Lottery"},
    {0x12, 0x02, 0x609E3, 0x60A5D, 0x41A82,  5, "1-6 Liftshaft Lottery B1"},
    {0x18, 0x02, 0x60A70, 0x60B21, 0x41A82,  5, "1-6 Liftshaft Lottery B2"},
    {0x15, 0x02, 0x62161, 0x621A7, 0x41A82,  5, "1-6 Liftshaft Lottery Warp"},
    {0x09, 0x01, 0x5A425, 0x5A455, 0x40BF1,  6, "1-B Barbos Bastion"},
    {0x44, 0x01, 0x58C04, 0x59704, 0x40BF1,  7, "2-1 Coral Quarrel"},
    {0x21, 0x01, 0x60658, 0x606DF, 0x41A22,  7, "2-1 Coral Quarrel B1"},
    {0x16, 0x01, 0x606EE, 0x6079A, 0x41A22,  7, "2-1 Coral Quarrel B2"},
    {0x4E, 0x04, 0x5DE58, 0x5E410, 0x411DF,  8, "2-2 Minky Mischief"},
    {0x0D, 0x04, 0x61824, 0x618CB, 0x41BE4,  8, "2-2 Minky Mischief B1"},
    {0x19, 0x04, 0x618D8, 0x619AC, 0x41BE4,  8, "2-2 Minky Mischief B2"},
    {0xD0, 0x00, 0x5C977, 0x5CE12, 0x40F5A,  9, "2-3 Jetty Jitters"},
    {0x21, 0x00, 0x61731, 0x61813, 0x41B33,  9, "2-3 Jetty Jitters B1"},
    {0x22, 0x00, 0x6165C, 0x61721, 0x41B33,  9, "2-3 Jetty Jitters B2"},
    {0xEE, 0x05, 0x5CE72, 0x5D408, 0x410B6, 10, "2-4 Black Ice Blitz"},
    {0x2B, 0x05, 0x612E0, 0x613A6, 0x41B96, 10, "2-4 Black Ice Blitz B1"},
    {0x16, 0x05, 0x613B4, 0x6143C, 0x41B96, 10, "2-4 Black Ice Blitz B2"},
    {0xCC, 0x03, 0x5BB0F, 0x5BF61, 0x40E37, 11, "2-5 Riverbank Riot"},
    {0x20, 0x03, 0x60DE0, 0x60E66, 0x41ADC, 11, "2-5 Riverbank Riot B1"},
    {0x13, 0x03, 0x60E72, 0x60EF0, 0x41ADC, 11, "2-5 Riverbank Riot B2"},
    {0x1E, 0x02, 0x5AE95, 0x5B2BD, 0x40CF9, 12, "2-6 Miller Instinct"},
    {0x1B, 0x02, 0x60B31, 0x60BEA, 0x41A82, 12, "2-6 Miller Instinct B1"},
    {0x0F, 0x02, 0x60BFB, 0x60C37, 0x41A82, 12, "2-6 Miller Instinct B2"},
    {0x07, 0x05, 0x6BD9F, 0x6BDCE, 0x410B6,  6, "2-B Bleak Magic"},
    {0x1A, 0x06, 0x60003, 0x603CB, 0x4142B, 13, "3-1 Rocketeer Rally"},
    {0x0E, 0x06, 0x62CF5, 0x62D66, 0x41C95, 13, "3-1 Rocketeer Rally B1"},
    {0x0C, 0x06, 0x62D79, 0x62DD0, 0x41C95, 13, "3-1 Rocketeer Rally B2"},
    {0x26, 0x07, 0x68D3A, 0x691EA, 0x4135C, 14, "3-2 Vertigo Verge"},
    {0x0E, 0x07, 0x61D3E, 0x61D81, 0x41C5C, 14, "3-2 Vertigo Verge B1"},
    {0x14, 0x07, 0x61D8E, 0x61E36, 0x41C5C, 14, "3-2 Vertigo Verge B2"},
    {0xD5, 0x05, 0x5D459, 0x5D9B0, 0x410B6, 15, "3-3 Polar Pitfalls"},
    {0x1D, 0x05, 0x61012, 0x610B0, 0x41B96, 15, "3-3 Polar Pitfalls B1"},
    {0x1E, 0x05, 0x610BA, 0x6117E, 0x41B96, 15, "3-3 Polar Pitfalls B2"},
    {0xC9, 0x03, 0x69EDD, 0x6A1FA, 0x40E37, 16, "3-4 Surface Tension"},
    {0x17, 0x03, 0x60EFE, 0x60F58, 0x41ADC, 16, "3-4 Surface Tension B1"},
    {0x1C, 0x03, 0x60F65, 0x61007, 0x41ADC, 16, "3-4 Surface Tension B2"},
    {0xD7, 0x05, 0x5DA01, 0x5DE17, 0x410B6, 17, "3-5 Tundra Blunda"},
    {0x19, 0x05, 0x61185, 0x61210, 0x41B96, 17, "3-5 Tundra Blunda B1"},
    {0x24, 0x05, 0x61219, 0x612DA, 0x41B96, 17, "3-5 Tundra Blunda B2"},
    {0x37, 0x04, 0x5E489, 0x5E8DE, 0x411DF, 18, "3-6 Redwood Rampage"},
    {0x0E, 0x04, 0x619BE, 0x61A79, 0x41BE4, 18, "3-6 Redwood Rampage B1"},
    {0x0E, 0x04, 0x61A86, 0x61B4B, 0x41BE4, 18, "3-6 Redwood Rampage B2"},
    {0x08, 0x04, 0x5E93B, 0x5E963, 0x411DF, 19, "3-B Arich Attack"},
    {0xE9, 0x08, 0x5E966, 0x5EDA5, 0x416CB, 20, "4-1 Jungle Jeopardy"},
    {0x1F, 0x08, 0x5F6A6, 0x5F73D, 0x41D55, 20, "4-1 Jungle Jeopardy B1"},
    {0x23, 0x08, 0x5F749, 0x5F7C8, 0x41D55, 20, "4-1 Jungle Jeopardy B2"},
    {0x32, 0x06, 0x62260, 0x626D9, 0x4142B, 21, "4-2 Footloose Falls"},
    {0x12, 0x06, 0x62DDF, 0x62E58, 0x41C95, 21, "4-2 Footloose Falls B1"},
    {0x0A, 0x06, 0x62E64, 0x62EAD, 0x41C95, 21, "4-2 Footloose Falls B2"},
    {0x45, 0x01, 0x5979E, 0x5A377, 0x40BF1, 22, "4-3 Deep Reef Grief"},
    {0x13, 0x01, 0x607A4, 0x60854, 0x41A22, 22, "4-3 Deep Reef Grief B1"},
    {0x13, 0x01, 0x60863, 0x60908, 0x41A22, 22, "4-3 Deep Reef Grief B2"},
    {0x46, 0x09, 0x6A5F9, 0x6AAF4, 0x417D0, 23, "4-4 Karbine KAOS"},
    {0x19, 0x09, 0x6B85D, 0x6B8C9, 0x41DA9, 23, "4-4 Karbine KAOS B1"},
    {0x15, 0x09, 0x6B8D3, 0x6B933, 0x41DA9, 23, "4-4 Karbine KAOS B2"},
    {0x41, 0x04, 0x62FA4, 0x63347, 0x411DF, 24, "4-5 Simian Shimmy"},
    {0x10, 0x04, 0x61B5D, 0x61C0E, 0x41BE4, 24, "4-5 Simian Shimmy B1"},
    {0x17, 0x04, 0x61C1B, 0x61D26, 0x41BE4, 24, "4-5 Simian Shimmy B2"},
    {0x43, 0x07, 0x69267, 0x697D4, 0x4135C, 25, "4-6 Rockface Chase"},
    {0x2C, 0x07, 0x61E40, 0x61F07, 0x41C5C, 25, "4-6 Rockface Chase B1"},
    {0x14, 0x07, 0x61F24, 0x61FB8, 0x41C5C, 25, "4-6 Rockface Chase B2"},
    {0x09, 0x09, 0x6BD60, 0x6BD9C, 0x417D0, 26, "4-B Krazy KAOS"},
    {0xEB, 0x08, 0x5EE06, 0x5F22D, 0x416CB, 27, "5-1 Tropical Tightropes"},
    {0x1C, 0x08, 0x5F7D2, 0x5F854, 0x41D55, 27, "5-1 Tropical Tightropes B1"},
    {0x25, 0x08, 0x5F862, 0x5F8D4, 0x41D55, 27, "5-1 Tropical Tightropes B2"},
    {0x21, 0x07, 0x69875, 0x69E40, 0x4135C, 28, "5-2 Clifftop Critters"},
    {0x0E, 0x07, 0x61FCA, 0x6201B, 0x41C5C, 28, "5-2 Clifftop Critters B1"},
    {0x0D, 0x07, 0x6202A, 0x620AD, 0x41C5C, 28, "5-2 Clifftop Critters B2"},
    {0x4C, 0x06, 0x62777, 0x62C68, 0x4142B, 29, "5-3 Rickety Rapids"},
    {0x10, 0x06, 0x62EB5, 0x62F25, 0x41C95, 29, "5-3 Rickety Rapids B1"},
    {0x0C, 0x06, 0x62F32, 0x62F94, 0x41C95, 29, "5-3 Rickety Rapids B2"},
    {0x47, 0x09, 0x6B184, 0x6B7E9, 0x417D0, 19, "5-4 Bazuka Bombard"},
    {0x0D, 0x09, 0x6B945, 0x6B9D8, 0x41DA9, 19, "5-4 Bazuka Bombard B1"},
    {0x1A, 0x09, 0x6B9F0, 0x6BAC5, 0x41DA9, 19, "5-4 Bazuka Bombard B2"},
    {0x32, 0x0A, 0x439BA, 0x43DBE, 0x4190E, 30, "5-5 Ugly Ducting"},
    {0x10, 0x0A, 0x5FA44, 0x5FAE0, 0x41E06, 30, "5-5 Ugly Ducting B1"},
    {0x10, 0x0A, 0x5FAF0, 0x5FB54, 0x41E06, 30, "5-5 Ugly Ducting B2"},
    {0x63, 0x0B, 0x63865, 0x63E55, 0x415A8, 31, "5-6 Stalagmite Frights"},
    {0x1E, 0x0B, 0x633A7, 0x63456, 0x41CEF, 31, "5-6 Stalagmite Frights B1"},
    {0x16, 0x0B, 0x6345F, 0x6354A, 0x41CEF, 31, "5-6 Stalagmite Frights B2"},
    {0x0B, 0x0A, 0x6BDD1, 0x6BE0B, 0x4190E, 26, "5-B K.Rool Duel"},
    {0xC4, 0x0A, 0x5FC0D, 0x5FF11, 0x4190E, 32, "6-1 Whiplash Dash"},
    {0x16, 0x0A, 0x5FB5D, 0x5FB89, 0x41E06, 32, "6-1 Whiplash Dash B1"},
    {0x1E, 0x0A, 0x5FB92, 0x5FC01, 0x41E06, 32, "6-1 Whiplash Dash B2"},
    {0x54, 0x09, 0x6AB69, 0x6B112, 0x417D0, 33, "6-2 Kuchuka Karnage"},
    {0x20, 0x09, 0x6BAD8, 0x6BB64, 0x41DA9, 33, "6-2 Kuchuka Karnage B1"},
    {0x0E, 0x09, 0x6BB71, 0x6BC2D, 0x41DA9, 33, "6-2 Kuchuka Karnage B2"},
    {0x3B, 0x0B, 0x68003, 0x68521, 0x415A8, 34, "6-3 Haunted Hollows"},
    {0x1D, 0x0B, 0x6355C, 0x635F4, 0x41CEF, 34, "6-3 Haunted Hollows B1"},
    {0x2D, 0x0B, 0x635FF, 0x6370E, 0x41CEF, 34, "6-3 Haunted Hollows B2"},
    {0xDF, 0x08, 0x5F2A3, 0x5F664, 0x416CB, 35, "6-4 Rainforest Rumble"},
    {0x14, 0x08, 0x5F98C, 0x5FA36, 0x41D55, 35, "6-4 Rainforest Rumble B1"},
    {0x17, 0x08, 0x5F8E1, 0x5F97C, 0x41D55, 35, "6-4 Rainforest Rumble B2"},
    {0x3B, 0x09, 0x5B32A, 0x5BA66, 0x417D0, 36, "6-5 Barrel Boulevard"},
    {0x15, 0x09, 0x6BC3C, 0x6BCB7, 0x41DA9, 36, "6-5 Barrel Boulevard B1"},
    {0x15, 0x09, 0x6BCC6, 0x6BD47, 0x41DA9, 36, "6-5 Barrel Boulevard B2"},
    {0x45, 0x0B, 0x685AC, 0x68C9E, 0x415A8, 37, "6-6 Ghoulish Grotto"},
    {0x0F, 0x0B, 0x63725, 0x637C3, 0x41CEF, 37, "6-6 Ghoulish Grotto B1"},
    {0x22, 0x0B, 0x637E7, 0x6385D, 0x41CEF, 37, "6-6 Ghoulish Grotto B2"},
    {0x0B, 0x0B, 0x6BE0E, 0x6BE5E, 0x415A8, 26, "6-B K.Rool's Last Stand"},
};
static const int dkl3_size = sizeof(dkl3_areas) / sizeof(struct DKL3_LEVEL);


void dkl3_levels(unsigned char *rom, size_t rom_size, char *dir, int sgb, unsigned *palette, int tileset) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < dkl3_size; i++) {

        const struct DKL3_LEVEL *d = &dkl3_areas[i];
        const struct DKL3_ARCH  *a = &dkl3_arch[d->arch];
        unsigned char *set_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *col_data = NULL;
        unsigned char *map_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char rgb[12];
        int t_width, t_height;
        size_t set_size = 0;
        size_t map_size = 0;
        char *name = d->name;
        char nbuf[64];
        int e;

        // filter duplicate tilesets
        if (tileset) {
            int j;
            for (j = 0; j < i; j++)
                if (d->arch == dkl3_areas[j].arch
                &&  (sgb || (!sgb && d->pal == dkl3_areas[j].pal)))
                    break;
            if (j < i)
                continue;
        }

        lay_data = malloc(0x20000);
        col_data = malloc(0x20000);
        if (lay_data == NULL
        ||  col_data == NULL) {
            fprintf(stderr, "Failed to allocate memory for set/lay/col data. (%d)\n", i);
            goto cleanup;
        }

        if (!tileset) {
            if ((e = dk_decompress_mem_to_mem(DKL_COMP, &map_data, &map_size, rom+d->map, rom_size-d->map))) {
                fprintf(stderr, "tilemap:%d: %s\n", i, dk_get_error(e));
                goto cleanup;
            }
            dkl2_map_fix(rom, map_data, map_size, d->mf1, d->mf2);
            t_width  = d->t_width;
            t_height = map_size / d->t_width;
        }
        else {
            if (tile_generator(&map_data, &map_size, a->tiles))
                goto cleanup;
            t_width = 16;
            t_height = (map_size / t_width);
            if (map_size % t_width) t_height++;
            snprintf(nbuf, 64, "%s Tileset", name);
            name = nbuf;
        }

        if ((e = dkl_huffman_decode(rom+a->set_addr, rom_size-a->set_addr, &set_data, &set_size, &rom[0x3D00], rom[a->set_addr-1]))) {
            fprintf(stderr, "tileset:%d: %s\n", i, dk_get_error(e));
            goto cleanup;
        }
        dkl_layout(rom, map_data, lay_data, a->layout, t_width, t_height);

        if (!sgb)
            decode_palette(rgb, dkl3_pal[d->pal], 4);
        else
            decode_palette24(rgb, palette);

        bitplane = malloc(t_width * t_height * 1024 * 4);
        if (bitplane == NULL) {
            fprintf(stderr, "Failed to allocate memory for bitplane. (%d)\n", i);
            goto cleanup;
        }

        gbc_assemble(bitplane, set_data, lay_data, col_data, rgb, t_width, t_height, 0);
        arrange_gbc(bitplane, (t_width*32), (t_height*32), dir, name);

cleanup:
        free(set_data);
        free(lay_data);
        free(map_data);
        free(col_data);
        free(bitplane);
    }

}











struct DKL3C_ARCH {
    unsigned set_addr; // Bitplane Decompression
    unsigned layout;   // 32x32 Location
    unsigned tiles;    // Number of tiles in tileset
};

struct DKL3C_LEVEL {
    unsigned char t_width; // Level Width (32x32)
    unsigned char arch;    // Archetype
    unsigned      attr;
    unsigned      map;     // Tilemap data
    unsigned      mf1;     // Map Fixes Part 1
    unsigned      mf2;     // Map Fixes Part 2
    unsigned      pal;     // GBC Palette (Level specific)
    char *name;            // Level Name
};

static const struct DKL3C_ARCH dkl3c_arch[] = {
    {0x49CB2, 0x44000, 128}, // Boardwalk
    {0x4A674, 0x5405F,  50}, // Underwater
    {0x48002, 0x47000, 109}, // Mill
    {0x4E1A8, 0x45000, 106}, // Riverside
    {0x4EC8A, 0x4407F,  99}, // Tree
    {0x48796, 0x4706D,  95}, // Snow
    {0x4C002, 0x46000,  76}, // Waterfall
    {0x4CBDE, 0x4604C,  79}, // Cliff
    {0x4B253, 0x4506A,  68}, // Jungle
    {0x4D786, 0x54000,  95}, // Le Factoire
    {0x4F745, 0x54091, 106}, // Pipe
    {0x4915B, 0x4609B,  71}, // Cave
};

static const struct DKL3C_LEVEL dkl3c_areas[] = {
    {0x97,  0, 0x80000, 0x5C003, 0x5C3A0, 0x40FA4, 0x850FB, "1-1 Red Wharf"},
    {0x19,  0, 0x80000, 0x61447, 0x614E5, 0x41B7D, 0x850FB, "1-1 Red Wharf B1"},
    {0x0F,  0, 0x80000, 0x614EF, 0x6156B, 0x41B7D, 0x850FB, "1-1 Red Wharf B2"},
    {0x0E,  0, 0x80000, 0x621CE, 0x62206, 0x41B7D, 0x850FB, "1-1 Red Wharf Warp"},
    {0x47,  1, 0x8405F, 0x58003, 0x58B60, 0x40C3B, 0x84FFB, "1-2 Seabed Shanty"},
    {0x15,  1, 0x8405F, 0x6046F, 0x60546, 0x41A6C, 0x84FFB, "1-2 Seabed Shanty B1"},
    {0x18,  1, 0x8405F, 0x6054F, 0x60647, 0x41A6C, 0x84FFB, "1-2 Seabed Shanty B2"},
    {0x15,  1, 0x8405F, 0x620B7, 0x62118, 0x41A6C, 0x84FFB, "1-2 Seabed Shanty Warp"},
    {0xBF,  0, 0x80000, 0x5C3E2, 0x5C912, 0x40FA4, 0x850FB, "1-3 Ford Knocks"},
    {0x19,  0, 0x80000, 0x61575, 0x6160D, 0x41B7D, 0x850FB, "1-3 Ford Knocks B1"},
    {0x14,  0, 0x80000, 0x61615, 0x61655, 0x41B7D, 0x850FB, "1-3 Ford Knocks B2"},
    {0x0B,  0, 0x80000, 0x6220C, 0x6225B, 0x41B7D, 0x850FB, "1-3 Ford Knocks Warp"},
    {0x38,  2, 0x83000, 0x5A459, 0x5A824, 0x40D43, 0x8503B, "1-4 Total Rekoil"},
    {0x17,  2, 0x83000, 0x60917, 0x60967, 0x41ACC, 0x8503B, "1-4 Total Rekoil B1"},
    {0x16,  2, 0x83000, 0x6096F, 0x609D7, 0x41ACC, 0x8503B, "1-4 Total Rekoil B2"},
    {0x10,  2, 0x83000, 0x62123, 0x6215C, 0x41ACC, 0x8503B, "1-4 Total Rekoil Warp"},
    {0xA2,  3, 0x81000, 0x6A261, 0x6A5A5, 0x40E81, 0x8507B, "1-5 Koco Channel"},
    {0x29,  3, 0x81000, 0x60C45, 0x60CF6, 0x41B26, 0x8507B, "1-5 Koco Channel B1"},
    {0x1E,  3, 0x81000, 0x60D06, 0x60DD3, 0x41B26, 0x8507B, "1-5 Koco Channel B2"},
    {0x0C,  3, 0x81000, 0x621AF, 0x621CA, 0x41B26, 0x8507B, "1-5 Koco Channel Warp"},
    {0x4A,  2, 0x83000, 0x5A890, 0x5AE0E, 0x40D43, 0x8503B, "1-6 Liftshaft Lottery"},
    {0x12,  2, 0x83000, 0x609E3, 0x60A5D, 0x41ACC, 0x8503B, "1-6 Liftshaft Lottery B1"},
    {0x18,  2, 0x83000, 0x60A70, 0x60B21, 0x41ACC, 0x8503B, "1-6 Liftshaft Lottery B2"},
    {0x15,  2, 0x83000, 0x62161, 0x621A7, 0x41ACC, 0x8503B, "1-6 Liftshaft Lottery B3"},
    {0x09,  1, 0x8405F, 0x5A425, 0x5A455, 0x40C3B, 0x84FFB, "1-B Barbos Bastion"},
    {0x44,  1, 0x8405F, 0x58C04, 0x59704, 0x40C3B, 0x84FFB, "2-1 Coral Quarrel"},
    {0x21,  1, 0x8405F, 0x60658, 0x606DF, 0x41A6C, 0x84FFB, "2-1 Coral Quarrel B1"},
    {0x16,  1, 0x8405F, 0x606EE, 0x6079A, 0x41A6C, 0x84FFB, "2-1 Coral Quarrel B2"},
    {0x4E,  4, 0x8007F, 0x5DE71, 0x5E429, 0x41229, 0x8513B, "2-2 Minky Mischief"},
    {0x0D,  4, 0x8007F, 0x61824, 0x618CB, 0x41C2E, 0x8513B, "2-2 Minky Mischief B1"},
    {0x19,  4, 0x8007F, 0x618D8, 0x619AC, 0x41C2E, 0x8513B, "2-2 Minky Mischief B2"},
    {0xD0,  0, 0x80000, 0x5C977, 0x5CE12, 0x40FA4, 0x850FB, "2-3 Jetty Jitters"},
    {0x21,  0, 0x80000, 0x61731, 0x61813, 0x41B7D, 0x850FB, "2-3 Jetty Jitters B1"},
    {0x22,  0, 0x80000, 0x6165C, 0x61721, 0x41B7D, 0x850FB, "2-3 Jetty Jitters B2"},
    {0xEE,  5, 0x8306D, 0x5CE72, 0x5D408, 0x41100, 0x850BB, "2-4 Black Ice Blitz"},
    {0x2B,  5, 0x8306D, 0x612E0, 0x613A6, 0x41BE0, 0x850BB, "2-4 Black Ice Blitz B1"},
    {0x16,  5, 0x8306D, 0x613B4, 0x6143C, 0x41BE0, 0x850BB, "2-4 Black Ice Blitz B2"},
    {0xCC,  3, 0x81000, 0x5BB0F, 0x5BF61, 0x40E81, 0x8507B, "2-5 Riverbank Riot"},
    {0x20,  3, 0x81000, 0x60DE0, 0x60E66, 0x41B26, 0x8507B, "2-5 Riverbank Riot B1"},
    {0x13,  3, 0x81000, 0x60E72, 0x60EF0, 0x41B26, 0x8507B, "2-5 Riverbank Riot B2"},
    {0x1E,  2, 0x83000, 0x5AE95, 0x5B2BD, 0x40D43, 0x8503B, "2-6 Miller Instinct"},
    {0x1B,  2, 0x83000, 0x60B31, 0x60BEA, 0x41ACC, 0x8503B, "2-6 Miller Instinct B1"},
    {0x0F,  2, 0x83000, 0x60BFB, 0x60C37, 0x41ACC, 0x8503B, "2-6 Miller Instinct B2"},
    {0x07,  5, 0x8306D, 0x6BD9F, 0x6BDCE, 0x41100, 0x850BB, "2-B Bleak Magic"},
    {0x1A,  6, 0x82000, 0x60003, 0x603CB, 0x41475, 0x851BB, "3-1 Rocketeer Rally"},
    {0x0E,  6, 0x82000, 0x62CF5, 0x62D66, 0x41CDF, 0x851BB, "3-1 Rocketeer Rally B1"},
    {0x0C,  6, 0x82000, 0x62D79, 0x62DD0, 0x41CDF, 0x851BB, "3-1 Rocketeer Rally B2"},
    {0x26,  7, 0x8204C, 0x68D3A, 0x691EA, 0x413A6, 0x8517B, "3-2 Vertigo Verge"},
    {0x0E,  7, 0x8204C, 0x61D3E, 0x61D81, 0x41CA6, 0x8517B, "3-2 Vertigo Verge B1"},
    {0x14,  7, 0x8204C, 0x61D8E, 0x61E36, 0x41CA6, 0x8517B, "3-2 Vertigo Verge B2"},
    {0xD5,  5, 0x8306D, 0x5D459, 0x5D9B0, 0x41100, 0x850BB, "3-3 Polar Pitfalls"},
    {0x1D,  5, 0x8306D, 0x61012, 0x610B0, 0x41BE0, 0x850BB, "3-3 Polar Pitfalls B1"},
    {0x1E,  5, 0x8306D, 0x610BA, 0x6117E, 0x41BE0, 0x850BB, "3-3 Polar Pitfalls B2"},
    {0xC9,  3, 0x81000, 0x69EDD, 0x6A1FA, 0x40E81, 0x8507B, "3-4 Surface Tension"},
    {0x17,  3, 0x81000, 0x60EFE, 0x60F58, 0x41B26, 0x8507B, "3-4 Surface Tension B1"},
    {0x1C,  3, 0x81000, 0x60F65, 0x61007, 0x41B26, 0x8507B, "3-4 Surface Tension B2"},
    {0xDA,  5, 0x8306D, 0x5DA01, 0x5DE30, 0x41100, 0x850BB, "3-5 Tundra Blunda"},
    {0x19,  5, 0x8306D, 0x61185, 0x61210, 0x41BE0, 0x850BB, "3-5 Tundra Blunda B1"},
    {0x24,  5, 0x8306D, 0x61219, 0x612DA, 0x41BE0, 0x850BB, "3-5 Tundra Blunda B2"},
    {0x37,  4, 0x8007F, 0x5E4A2, 0x5E8F7, 0x41229, 0x8513B, "3-6 Redwood Rampage"},
    {0x0E,  4, 0x8007F, 0x619BE, 0x61A79, 0x41C2E, 0x8513B, "3-6 Redwood Rampage B1"},
    {0x0E,  4, 0x8007F, 0x61A86, 0x61B4B, 0x41C2E, 0x8513B, "3-6 Redwood Rampage B2"},
    {0x08,  4, 0x8007F, 0x5E954, 0x5E97C, 0x41229, 0x8513B, "3-B Arich Attack"},
    {0xE9,  8, 0x8106A, 0x5E97F, 0x5EDBE, 0x41715, 0x8523B, "4-1 Jungle Jeopardy"},
    {0x1F,  8, 0x8106A, 0x5F6BF, 0x5F756, 0x41D9F, 0x8523B, "4-1 Jungle Jeopardy B1"},
    {0x23,  8, 0x8106A, 0x5F762, 0x5F7E1, 0x41D9F, 0x8523B, "4-1 Jungle Jeopardy B2"},
    {0x32,  6, 0x82000, 0x62260, 0x626D9, 0x41475, 0x851BB, "4-2 Footloose Falls"},
    {0x12,  6, 0x82000, 0x62DDF, 0x62E58, 0x41CDF, 0x851BB, "4-2 Footloose Falls B1"},
    {0x0A,  6, 0x82000, 0x62E64, 0x62EAD, 0x41CDF, 0x851BB, "4-2 Footloose Falls B2"},
    {0x45,  1, 0x8405F, 0x5979E, 0x5A377, 0x40C3B, 0x84FFB, "4-3 Deep Reef Grief"},
    {0x13,  1, 0x8405F, 0x607A4, 0x60854, 0x41A6C, 0x84FFB, "4-3 Deep Reef Grief B1"},
    {0x13,  1, 0x8405F, 0x60863, 0x60908, 0x41A6C, 0x84FFB, "4-3 Deep Reef Grief B2"},
    {0x46,  9, 0x84000, 0x6A5F9, 0x6AAF4, 0x4181A, 0x8527B, "4-4 Karbine KAOS"},
    {0x19,  9, 0x84000, 0x6B85D, 0x6B8C9, 0x41DF3, 0x8527B, "4-4 Karbine KAOS B1"},
    {0x15,  9, 0x84000, 0x6B8D3, 0x6B933, 0x41DF3, 0x8527B, "4-4 Karbine KAOS B2"},
    {0x41,  4, 0x8007F, 0x62FA4, 0x63347, 0x41229, 0x8513B, "4-5 Simian Shimmy"},
    {0x10,  4, 0x8007F, 0x61B5D, 0x61C0E, 0x41C2E, 0x8513B, "4-5 Simian Shimmy B1"},
    {0x17,  4, 0x8007F, 0x61C1B, 0x61D26, 0x41C2E, 0x8513B, "4-5 Simian Shimmy B2"},
    {0x43,  7, 0x8204C, 0x69267, 0x697D4, 0x413A6, 0x8517B, "4-6 Rockface Chase"},
    {0x2C,  7, 0x8204C, 0x61E40, 0x61F07, 0x41CA6, 0x8517B, "4-6 Rockface Chase B1"},
    {0x14,  7, 0x8204C, 0x61F24, 0x61FB8, 0x41CA6, 0x8517B, "4-6 Rockface Chase B2"},
    {0x09,  9, 0x84000, 0x6BD60, 0x6BD9C, 0x4181A, 0x8527B, "4-B Krazy KAOS"},
    {0xEB,  8, 0x8106A, 0x5EE1F, 0x5F246, 0x41715, 0x8523B, "5-1 Tropical Tightropes"},
    {0x1C,  8, 0x8106A, 0x5F7EB, 0x5F86D, 0x41D9F, 0x8523B, "5-1 Tropical Tightropes B1"},
    {0x25,  8, 0x8106A, 0x5F87B, 0x5F8ED, 0x41D9F, 0x8523B, "5-1 Tropical Tightropes B2"},
    {0x21,  7, 0x8204C, 0x69875, 0x69E40, 0x413A6, 0x8517B, "5-2 Clifftop Critters"},
    {0x0E,  7, 0x8204C, 0x61FCA, 0x6201B, 0x41CA6, 0x8517B, "5-2 Clifftop Critters B1"},
    {0x0D,  7, 0x8204C, 0x6202A, 0x620AD, 0x41CA6, 0x8517B, "5-2 Clifftop Critters B2"},
    {0x4C,  6, 0x82000, 0x62777, 0x62C68, 0x41475, 0x851BB, "5-3 Rickety Rapids"},
    {0x10,  6, 0x82000, 0x62EB5, 0x62F25, 0x41CDF, 0x851BB, "5-3 Rickety Rapids B1"},
    {0x0C,  6, 0x82000, 0x62F32, 0x62F94, 0x41CDF, 0x851BB, "5-3 Rickety Rapids B2"},
    {0x47,  9, 0x84000, 0x6B184, 0x6B7E9, 0x4181A, 0x8527B, "5-4 Bazuka Bombard"},
    {0x0D,  9, 0x84000, 0x6B945, 0x6B9D8, 0x41DF3, 0x8527B, "5-4 Bazuka Bombard B1"},
    {0x1A,  9, 0x84000, 0x6B9F0, 0x6BAC5, 0x41DF3, 0x8527B, "5-4 Bazuka Bombard B2"},
    {0x32, 10, 0x84091, 0x43A0A, 0x43E0E, 0x41958, 0x852BB, "5-5 Ugly Ducting"},
    {0x10, 10, 0x84091, 0x5FA5D, 0x5FAF9, 0x41E50, 0x852BB, "5-5 Ugly Ducting B1"},
    {0x10, 10, 0x84091, 0x5FB09, 0x5FB6D, 0x41E50, 0x852BB, "5-5 Ugly Ducting B2"},
    {0x63, 11, 0x8209B, 0x63865, 0x63E55, 0x415F2, 0x851FB, "5-6 Stalagmite Frights"},
    {0x1E, 11, 0x8209B, 0x633A7, 0x63456, 0x41D39, 0x851FB, "5-6 Stalagmite Frights B1"},
    {0x16, 11, 0x8209B, 0x6345F, 0x6354A, 0x41D39, 0x851FB, "5-6 Stalagmite Frights B2"},
    {0x0B, 10, 0x84091, 0x6BDD1, 0x6BE0B, 0x41958, 0x852BB, "5-B K.Rool Duel"},
    {0xC4, 10, 0x84091, 0x5FC26, 0x5FF2A, 0x41958, 0x852BB, "6-1 Whiplash Dash"},
    {0x16, 10, 0x84091, 0x5FB76, 0x5FBA2, 0x41E50, 0x852BB, "6-1 Whiplash Dash B1"},
    {0x1E, 10, 0x84091, 0x5FBAB, 0x5FC1A, 0x41E50, 0x852BB, "6-1 Whiplash Dash B2"},
    {0x54,  9, 0x84000, 0x6AB69, 0x6B112, 0x4181A, 0x8527B, "6-2 Kuchuka Karnage"},
    {0x20,  9, 0x84000, 0x6BAD8, 0x6BB64, 0x41DF3, 0x8527B, "6-2 Kuchuka Karnage B1"},
    {0x0E,  9, 0x84000, 0x6BB71, 0x6BC2D, 0x41DF3, 0x8527B, "6-2 Kuchuka Karnage B2"},
    {0x3B, 11, 0x8209B, 0x68003, 0x68521, 0x415F2, 0x851FB, "6-3 Haunted Hollows"},
    {0x1D, 11, 0x8209B, 0x6355C, 0x635F4, 0x41D39, 0x851FB, "6-3 Haunted Hollows B1"},
    {0x2D, 11, 0x8209B, 0x635FF, 0x6370E, 0x41D39, 0x851FB, "6-3 Haunted Hollows B2"},
    {0xDF,  8, 0x8106A, 0x5F2BC, 0x5F67D, 0x41715, 0x8523B, "6-4 Rainforest Rumble"},
    {0x14,  8, 0x8106A, 0x5F9A5, 0x5FA4F, 0x41D9F, 0x8523B, "6-4 Rainforest Rumble B1"},
    {0x17,  8, 0x8106A, 0x5F8FA, 0x5F995, 0x41D9F, 0x8523B, "6-4 Rainforest Rumble B2"},
    {0x3B,  9, 0x84000, 0x5B32A, 0x5BA66, 0x4181A, 0x8527B, "6-5 Barrel Boulevard"},
    {0x15,  9, 0x84000, 0x6BC3C, 0x6BCB7, 0x41DF3, 0x8527B, "6-5 Barrel Boulevard B1"},
    {0x15,  9, 0x84000, 0x6BCC6, 0x6BD47, 0x41DF3, 0x8527B, "6-5 Barrel Boulevard B2"},
    {0x45, 11, 0x8209B, 0x685AC, 0x68C9E, 0x415F2, 0x851FB, "6-6 Ghoulish Grotto"},
    {0x0F, 11, 0x8209B, 0x63725, 0x637C3, 0x41D39, 0x851FB, "6-6 Ghoulish Grotto B1"},
    {0x22, 11, 0x8209B, 0x637E7, 0x6385D, 0x41D39, 0x851FB, "6-6 Ghoulish Grotto B2"},
    {0x0B, 11, 0x8209B, 0x6BE0E, 0x6BE5E, 0x415F2, 0x851FB, "6-B K.Rool's Last Stand"}
};

static const int dkl3c_size = sizeof(dkl3c_areas) / sizeof(struct DKL3C_LEVEL);

void dkl3c_levels(unsigned char *rom, size_t rom_size, char *dir, int tileset) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < dkl3c_size; i++) {

        const struct DKL3C_LEVEL *d = &dkl3c_areas[i];
        const struct DKL3C_ARCH  *a = &dkl3c_arch[d->arch];
        unsigned char *set_data = NULL;
        unsigned char *lay_data = NULL;
        unsigned char *col_data = NULL;
        unsigned char *map_data = NULL;
        unsigned char *bitplane = NULL;
        unsigned char rgb[384];
        int t_width, t_height;
        size_t set_size = 0;
        size_t map_size = 0;
        char *name = d->name;
        char nbuf[64];
        int e;

        // filter duplicate tilesets
        if (tileset) {
            int j;
            for (j = 0; j < i; j++)
                if (d->arch == dkl3c_areas[j].arch
                &&  d->pal  == dkl3c_areas[j].pal)
                    break;
            if (j < i)
                continue;
        }

        lay_data = malloc(0x20000);
        col_data = malloc(0x20000);
        if (lay_data == NULL
        ||  col_data == NULL) {
            fprintf(stderr, "Failed to allocate memory for set/lay/col data. (%d)\n", i);
            goto cleanup;
        }

        if (!tileset) {
            if ((e = dk_decompress_mem_to_mem(DKL_COMP, &map_data, &map_size, rom+d->map, rom_size-d->map))) {
                fprintf(stderr, "tilemap:%d: %s\n", i, dk_get_error(e));
                goto cleanup;
            }
            dkl2_map_fix(rom, map_data, map_size, d->mf1, d->mf2);
            t_width  = d->t_width;
            t_height = map_size / d->t_width;
        }
        else {
            if (tile_generator(&map_data, &map_size, a->tiles))
                goto cleanup;
            t_width = 16;
            t_height = (map_size / t_width);
            if (map_size % t_width) t_height++;
            snprintf(nbuf, 64, "%s Tileset", name);
            name = nbuf;
        }

        if ((e = dkl_huffman_decode(rom+a->set_addr, rom_size-a->set_addr, &set_data, &set_size, &rom[0x3D00], rom[a->set_addr-1]))) {
            fprintf(stderr, "tileset:%d: %s\n", i, dk_get_error(e));
            goto cleanup;
        }
        dkl_layout(rom, map_data, lay_data, a->layout,     t_width, t_height);
        dkl_layout(rom, map_data, col_data, d->attr, t_width, t_height);
        decode_palette(rgb, &rom[d->pal], 128);

        bitplane = malloc(t_width * t_height * 1024 * 4);
        if (bitplane == NULL) {
            fprintf(stderr, "Failed to allocate memory for bitplane. (%d)\n", i);
            goto cleanup;
        }

        gbc_assemble(bitplane, set_data, lay_data, col_data, rgb, t_width, t_height, 1);
        arrange_gbc(bitplane, (t_width*32), (t_height*32), dir, name);

cleanup:
        free(set_data);
        free(lay_data);
        free(map_data);
        free(col_data);
        free(bitplane);
    }
}

