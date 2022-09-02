/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - SNES DKC2 screens */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "bitplane.h"


enum COMPTYPE {
    COMPTYPE_RAW,
    COMPTYPE_BD,
    COMPTYPE_SD,
    COMPTYPE_BD_SPEC /* JP version isn't compressed */
};

struct DATA {
    unsigned addr;
      signed ofs_jp;
      signed ofs_eu;
      signed ofs_de;
    enum COMPTYPE type;
};

struct DKC2_SCREEN {
    struct DATA set;
    struct DATA map;
    unsigned palette;
    unsigned char spec;
    unsigned char mode;
    unsigned char fix;
    unsigned char bg;
    char *name;
};

static unsigned get_region (const struct DATA *d, int region) {
    switch (region) {
        case 0:  { return d->addr + d->ofs_jp; }
        default: { return d->addr; }
        case 2:  { return d->addr + d->ofs_eu; }
        case 3:  { return d->addr + d->ofs_de; }
    }
}

static const struct DKC2_SCREEN dkc2[] = {
    { { 0x2A5978,   35,35,35, 1 }, { 0x8F99C,      0, 0, 0,1 }, 0x3D2170, 0,2, 0,0, "K.Rool's Kabin & Rattly Room" },
    { { 0x2C83A0,   37,37,37, 1 }, { 0x2C7CF0,    37,37,37,1 }, 0x3D3C6E, 0,2, 0,1, "File Screen" },
    { { 0x2C83A0,   37,37,37, 1 }, { 0x2C7CF0,    37,37,37,1 }, 0x000000, 0,2, 3,1, "Bonus Screen" },
    { { 0x2D0997,   47,37,37, 1 }, { 0x2D02A1, -1745,37,37,3 }, 0x3D26AE, 1,2, 0,1, "Title Screen" },
    { { 0x2D8584, 6939,37,35, 1 }, { 0x2D9C9A,  6939,37,35,1 }, 0x000000, 0,2, 3,0, "Find the Token Overlay" },
    { { 0x2D9179, 6939,37,35, 1 }, { 0x2D9E72,  6939,37,35,1 }, 0x000000, 0,2, 4,0, "Destroy Them All Overlay" },
    { { 0x2D78D3, 6939,37,35, 1 }, { 0x2DA030,  6939,37,35,1 }, 0x000000, 0,2, 5,0, "Collect the Stars Overlay" },
    { { 0x2C4D40,   34,34,34, 1 }, { 0x2C4749,    33,33,33,1 }, 0x3D3C6E, 0,2, 0,0, "Game Selection" },
    { { 0x2C4D40,   34,34,34, 1 }, { 0x2C4AAD,    33,33,33,1 }, 0x3D3C6E, 0,2, 0,0, "Music Test" },
    { { 0x2C4D40,   34,34,34, 1 }, { 0x2C4C1C,    34,34,34,1 }, 0x3D3C6E, 0,2, 0,0, "Cheat Mode" },
    { { 0x12B9FD,    0, 0, 0, 1 }, { 0x0EFA9F,     0, 0, 0,0 }, 0x3D11F0, 0,2, 0,1, "Kong Kollege" },
    { { 0x2DA1AB, 6939,37,35, 1 }, { 0x05FAA8,     0, 0, 0,1 }, 0x3D12F0, 0,2, 0,1, "Swanky's Bonus Bonanza" },
    { { 0x2BCFD1,   35,35,35, 1 }, { 0x2C1B0F,    33,33,33,1 }, 0x3D0FF0, 0,2, 0,1, "Monkey Museum" },
    { { 0x2BA27C,   35,35,35, 1 }, { 0x03FD35,     0, 0, 0,1 }, 0x3D10F0, 0,2, 0,0, "Funky's Flights" },
    { { 0x2A121C,   35,35,35, 1 }, { 0x0AFABE,     0, 0, 0,1 }, 0x3D10F0, 0,2, 0,1, "Funky's Flights BG" },
    { { 0x2B50CE,   35,35,35, 1 }, { 0x330000,     0, 0, 0,0 }, 0x3D13F0, 0,2, 0,1, "Klubba's Kiosk" },
    { { 0x2CD032,   37,37,37, 1 }, { 0x2CF8CE,    37,37,37,1 }, 0x3D402E, 0,2, 0,1, "Video Game Heroes Screen (Unused Version)" },
    { { 0x361D3E,    0, 0, 0, 1 }, { 0x2CFDEE,    37,37,37,1 }, 0x3D402E, 0,2, 0,1, "Video Game Heroes Screen" },
    { { 0x399400,    0, 0, 0, 1 }, { 0x39C775,     0, 0, 0,1 }, 0x000000, 0,2, 6,1, "Ending Screen BG" },
    { { 0x3661C1,    0, 0, 0, 1 }, { 0x367D1B,     0, 0, 0,1 }, 0x000000, 0,2, 6,0, "Ending Screen FG" },
    { { 0x3444D2,    0, 0, 0, 1 }, { 0x3A56C1,     0, 0, 0,0 }, 0x3D258E, 0,2, 0,1, "Game Over" },
    { { 0x2ADA86,   35,35,35, 1 }, { 0x1D0AFC,     0, 0, 0,1 }, 0x3D0B10, 0,2, 0,0, "Overworld Map" },
    { { 0x26FCF3,   35,35,35, 1 }, { 0x19F97B,     0, 0, 0,0 }, 0x3D1870, 0,2, 0,1, "Gangplank Galleon" },
    { { 0x275471,   35,35,35, 1 }, { 0x274D71,    35,35,35,0 }, 0x3D1970, 0,2, 0,1, "Crocodile Cauldron" },
    { { 0x279E78,   35,35,35, 1 }, { 0x17F9A7,     0, 0, 0,1 }, 0x3D1A70, 0,2, 0,1, "Krem Quay Left" },
    { { 0x279E78,   35,35,35, 1 }, { 0x04F9C0,     0, 0, 0,1 }, 0x3D1A70, 0,2, 0,1, "Krem Quay Right" },
    { { 0x280535,   35,35,35, 1 }, { 0x1BFB2C,     0, 0, 0,1 }, 0x3D1B70, 0,2, 0,1, "Krazy Kremland Top" },
    { { 0x2909EC,   35,35,35, 1 }, { 0x1A4C4F,     0, 0, 0,1 }, 0x3D2070, 0,2, 0,1, "Krazy Kremland Bottom" },
    { { 0x2854F8,   35,35,35, 1 }, { 0x284E0D,     0, 0, 0,1 }, 0x3D1C70, 0,2, 0,1, "Gloomy Gulch" },
    { { 0x382106,    0, 0, 0, 1 }, { 0x28A6D7,    35,35,35,1 }, 0x3D1D70, 0,2, 0,1, "K.Rool's Keep" },
    { { 0x28B924,   35,35,35, 1 }, { 0x28B224,    35,35,35,0 }, 0x3D1E70, 0,2, 0,1, "The Flying Krock" },
    { { 0x0BAC49,    0, 0, 0, 1 }, { 0x298407,    35,35,35,0 }, 0x3D1F70, 0,2, 0,1, "The Lost World" },
    { { 0x29FDB3,   35,35,35, 1 }, { 0x07FCB8,     0, 0, 0,1 }, 0x3D0CD0, 0,2, 0,1, "Pirate Panic BG" },
    { { 0x29FDB3,   35,35,35, 1 }, { 0x07FCB8,     0, 0, 0,1 }, 0x3D0CD0, 0,2,65,1, "Gangplank Galleon BG" },
    { { 0x29FDB3,   35,35,35, 1 }, { 0x07FCB8,     0, 0, 0,1 }, 0x3D324E, 0,2, 0,1, "Rattle Battle BG" },
    /* Pirate Panic foreground needs extra work! */
    { { 0x2A121C,   35,35,35, 1 }, { 0x0AFABE,     0, 0, 0,1 }, 0x3D1610, 0,2, 0,1, "Mainbrace Mayhem BG" },
    { { 0x2A121C,   35,35,35, 1 }, { 0x0AFABE,     0, 0, 0,1 }, 0x3D14F0, 0,2, 0,1, "Topsail Trouble BG" },
    { { 0x2A121C,   35,35,35, 1 }, { 0x0AFABE,     0, 0, 0,1 }, 0x3D304E, 0,2, 0,1, "Slime Climb BG" },

    /* The numbers in these BGs probably don't correspond to their BGn number */
    { { 0x29A905,   35,35,35, 1 }, { 0x02FAC9,     0, 0, 0,1 }, 0x3D0910, 0,2, 0,0, "Lockjaw's Locker BG-1" },
    { { 0x29A905,   35,35,35, 1 }, { 0x02FAC9,     0, 0, 0,1 }, 0x3D2BEE, 0,2, 1,0, "Glimmer's Galleon BG-1" },
    { { 0x29A905,   35,35,35, 1 }, { 0x02FAC9,     0, 0, 0,1 }, 0x3D07F0, 0,2, 0,0, "Lava Lagoon BG-1" },
    { { 0x29A905,   35,35,35, 1 }, { 0x02FAC9,     0, 0, 0,1 }, 0x3D2DEE, 0,2,44,0, "Toxic Tower BG-1" },
    { { 0x29A905,   35,35,35, 1 }, { 0x06FC11,     0, 0, 0,1 }, 0x3D0910, 0,2, 0,0, "Lockjaw's Locker BG-2" },
    { { 0x29A905,   35,35,35, 1 }, { 0x06FC11,     0, 0, 0,1 }, 0x3D2BEE, 0,2, 1,0, "Glimmer's Galleon BG-2" },
    { { 0x29A905,   35,35,35, 1 }, { 0x06FC11,     0, 0, 0,1 }, 0x3D07F0, 0,2, 0,0, "Lava Lagoon BG-2" },
    { { 0x29A905,   35,35,35, 1 }, { 0x0CFD95,     0, 0, 0,1 }, 0x3D0910, 0,2, 0,0, "Lockjaw's Locker BG-3" },
    { { 0x29A905,   35,35,35, 1 }, { 0x0CFD95,     0, 0, 0,1 }, 0x3D2BEE, 0,2, 1,0, "Glimmer's Galleon BG-3" },
    { { 0x29A905,   35,35,35, 1 }, { 0x0CFD95,     0, 0, 0,1 }, 0x3D07F0, 0,2, 0,0, "Lava Lagoon BG-3" },
    { { 0x29A905,   35,35,35, 1 }, { 0x09FF37,     0, 0, 0,1 }, 0x3D0910, 0,2, 0,0, "Lockjaw's Locker BG-4" },
    { { 0x29A905,   35,35,35, 1 }, { 0x09FF37,     0, 0, 0,1 }, 0x3D2BEE, 0,2, 1,0, "Glimmer's Galleon BG-4" },
    { { 0x29A905,   35,35,35, 1 }, { 0x09FF37,     0, 0, 0,1 }, 0x3D07F0, 0,2, 0,0, "Lava Lagoon BG-4" },
    { { 0x2A4E35,   35,35,35, 1 }, { 0x09F99A,     0, 0, 0,1 }, 0x3D27EE, 2,3, 0,1, "Barrel Bayou BG" },
    { { 0x2A4E35,   35,35,35, 1 }, { 0x09F99A,     0, 0, 0,1 }, 0x3D450E, 2,3, 0,1, "Krockhead Klamber BG" },
    { { 0x2A4E35,   35,35,35, 1 }, { 0x09F99A,     0, 0, 0,1 }, 0x3D460E, 2,3, 0,1, "Mudhole Marsh BG" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x0DFEE1,     0, 0, 0,1 }, 0x3D27EE, 0,2, 0,0, "Barrel Bayou FG-1" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x0DFEE1,     0, 0, 0,1 }, 0x3D450E, 0,2, 0,0, "Krockhead Klamber FG-1" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x0DFEE1,     0, 0, 0,1 }, 0x3D460E, 0,2, 0,0, "Mudhole Marsh FG-1" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x1AFBB1,     0, 0, 0,1 }, 0x3D27EE, 0,2, 0,0, "Barrel Bayou FG-2" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x1AFBB1,     0, 0, 0,1 }, 0x3D450E, 0,2, 0,0, "Krockhead Klamber FG-2" },
    { { 0x2A3BEA,   35,35,35, 1 }, { 0x1AFBB1,     0, 0, 0,1 }, 0x3D460E, 0,2, 0,0, "Mudhole Marsh FG-2" },
    { { 0x360000,    0, 0, 0, 1 }, { 0x13FAF0,     0, 0, 0,1 }, 0x3D228E, 0,2, 0,1, "Kannon's Klaim BG" },
    { { 0x360000,    0, 0, 0, 1 }, { 0x13FAF0,     0, 0, 0,1 }, 0x3D238E, 0,2, 0,1, "Squawks's Shaft BG" },
    { { 0x360000,    0, 0, 0, 1 }, { 0x13FAF0,     0, 0, 0,1 }, 0x3D248E, 0,2, 0,1, "Windy Well BG" },
    { { 0x16F791,    0, 0, 0, 1 }, { 0x14FC2D,     0, 0, 0,1 }, 0x3D0A10, 0,3, 0,1, "Hive BG" },
    { { 0x2B2EA0,   35,35,35, 1 }, { 0x18FB53,     0, 0, 0,1 }, 0x3D0A10, 5,2, 0,1, "Hive FG" },
    { { 0x2AB6F0,   35,35,35, 1 }, { 0x380000,     0, 0, 0,1 }, 0x3D28EE, 0,2, 0,1, "Bramble Blast BG-1" },
    { { 0x2AB6F0,   35,35,35, 1 }, { 0x380000,     0, 0, 0,1 }, 0x3D2AEE, 0,2, 0,1, "Bramble Scramble BG-1" },
    { { 0x2AB6F0,   35,35,35, 1 }, { 0x380000,     0, 0, 0,1 }, 0x3D29EE, 0,2, 0,1, "Screech's Sprint BG-1" },
    { { 0x2AA94D,   35,35,35, 1 }, { 0x1DFDBA,     0, 0, 0,1 }, 0x3D28EE, 6,3, 0,1, "Bramble Blast BG-2" },
    { { 0x2AA94D,   35,35,35, 1 }, { 0x1DFDBA,     0, 0, 0,1 }, 0x3D2AEE, 6,3, 0,1, "Bramble Scramble BG-2" },
    { { 0x2AA94D,   35,35,35, 1 }, { 0x1DFDBA,     0, 0, 0,1 }, 0x3D29EE, 6,3, 0,1, "Screech's Sprint BG-2" },
    { { 0x2A23D4,   35,35,35, 1 }, { 0x0FFFD9,     0, 0, 0,1 }, 0x3D1710, 0,2, 0,1, "Fair BG" },
    { { 0x29943A,   35,35,35, 1 }, { 0x298C68,    35,35,35,1 }, 0x3D2EEE, 3,3, 0,1, "Hall BG" },
    { { 0x295221,   35,35,35, 1 }, { 0x0CFEDF,     0, 0, 0,1 }, 0x3D2EEE, 7,2, 0,0, "Kackle Body" },
    { { 0x2A972E,   35,35,35, 1 }, { 0x10FE0D,     0, 0, 0,1 }, 0x3D06F0, 0,2,43,0, "Gloomy Gulch FG" },
    { { 0x3B6FC0,    0, 0, 0, 3 }, { 0x15FEB3,     0, 0, 0,1 }, 0x3D06F0, 8,3, 0,1, "Ghostly Grove & Gusty Glade BG" },
    { { 0x3B6FC0,    0, 0, 0, 3 }, { 0x15FEB3,     0, 0, 0,1 }, 0x3D3A4E, 8,3, 0,1, "Web Woods BG" },
    { { 0x295623,   35,35,35, 1 }, { 0x1CFA5E,    35,35,35,1 }, 0x3D398E, 0,2, 0,1, "Ice BG" },
    { { 0x2A10BB,   35,35,35, 1 }, { 0x01FEC0,     0, 0, 0,1 }, 0x3D07F0, 9,3, 0,1, "Lava" },
    { { 0x2A10BB,   35,35,35, 1 }, { 0x01FEC0,     0, 0, 0,1 }, 0x3D0910, 9,3, 0,1, "Water" },
    { { 0x2A10BB,   35,35,35, 1 }, { 0x01FEC0,     0, 0, 0,1 }, 0x3D398E, 9,3, 0,1, "Ice Water" },
    { { 0x2A10BB,   35,35,35, 1 }, { 0x01FEC0,     0, 0, 0,1 }, 0x3D2DEE, 9,3,44,1, "Toxic Tower Water" },
    { { 0x2B4916,   35,35,35, 1 }, { 0x29A745,    35,35,35,0 }, 0x3D2DEE, 4,3, 0,0, "Castle BG-1" },
    { { 0x297549,   35,35,35, 1 }, { 0x330802,     0, 0, 0,1 }, 0x3D430E, 0,3, 0,1, "Klobber Karnage BG" },
    { { 0x297549,   35,35,35, 1 }, { 0x330802,     0, 0, 0,1 }, 0x3D440E, 0,3, 0,1, "Jungle Jinx BG" },
    { { 0x297549,   35,35,35, 1 }, { 0x330802,     0, 0, 0,1 }, 0x3D3B6E, 0,3, 0,1, "Animal Antics BG" }
};
static const int screen_count = sizeof(dkc2) / sizeof(struct DKC2_SCREEN);

/*
static int extend (unsigned char **data, size_t *size, size_t add) {
    unsigned char *z = realloc(*data, *size + add);
    if (z == NULL)
        return 1;
    *data = z;
    *size += add;
    return 0;
}
*/
static int resize (unsigned char **data, size_t *size, size_t new_size) {
    unsigned char *z = realloc(*data, new_size);
    if (z == NULL)
        return 1;
    *data = z;
    *size = new_size;
    return 0;
}
static int offset (unsigned char **data, size_t *size, size_t ofs) {
    if (resize(data, size, *size + ofs))
        return 1;
    memmove(*data + ofs, *data, *size - ofs);
    memset(*data, 0, ofs);
    return 0;
}


static int decomp (unsigned char **data, size_t *size, unsigned char *src, size_t src_size, const struct DATA *d) {
    switch (d->type) {
        case COMPTYPE_RAW: { /* memcpy needs a size */
            unsigned cpsize = 0x700;
            unsigned char *z = calloc(cpsize, 1);
            if (z == NULL)
                return 1;
            *data = z;
            memcpy(*data, src, cpsize);
            *size = cpsize;
            break;
        }
        case COMPTYPE_BD: { /* big data */
            if (dk_decompress_mem_to_mem(BD_COMP, data, size, src, src_size))
                return 1;
            break;
        }
        case COMPTYPE_BD_SPEC: { return 0; }
        default:           { return 1; }
    }
    return 0;
}

void spec2 (unsigned char *rom, size_t rom_size, char *dir, int region) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < screen_count; i++) {
        unsigned char *set_data = NULL;
        unsigned char *map_data = NULL;
        unsigned char *bp       = NULL;
        size_t set_size = 0;
        size_t map_size = 0;
        const struct DKC2_SCREEN *d = &dkc2[i];
        unsigned char *set = rom + get_region(&d->set, region);
        unsigned char *map = rom + get_region(&d->map, region);

        if (decomp(&set_data, &set_size, set, rom_size-(set-rom), &d->set))
            goto error;
        if (decomp(&map_data, &map_size, map, rom_size-(map-rom), &d->map))
            goto error;

        switch (d->spec) {
            case 0: { break; }
            case 1: { /* Title screen */
                struct DATA mapd = d->map;
                mapd.type = (region == 0) ? COMPTYPE_RAW : COMPTYPE_BD;
                if (decomp(&map_data, &map_size, map, rom_size-(map-rom), &mapd))
                    goto error;
                break;
            }
            case 2: { /* Mudhole Marsh */
                if (offset(&set_data, &set_size, 0x1230)
                ||  resize(&map_data, &map_size, 0x800))
                    goto error;
                memcpy(&map_data[0x700], &map_data[0x6C0], 0x40);
                memcpy(&map_data[0x740], &map_data[0x6C0], 0x40);
                memcpy(&map_data[0x780], &map_data[0x6C0], 0x40);
                memcpy(&map_data[0x7C0], &map_data[0x6C0], 0x40);
                break;
            }
            case 4: { /* Castle BG */
                if (resize(&map_data, &map_size, 0x800))
                    goto error;
                memset(&map_data[0x1C0], 0, 0x640);
                break;
            }
            case 8: { /* Woods BG */
                set_size = 0x1000;
                set_data = malloc(set_size);
                if (set_data == NULL)
                    goto error;
                memcpy(set_data, set, set_size);
                break;
            }
            case 3: { if (resize(&map_data, &map_size, 0x1000)) goto error; break; }
            case 5: { if (offset(&set_data, &set_size,  0x600)) goto error; break; }
            case 6: { if (offset(&set_data, &set_size, 0x1110)) goto error; break; }
            case 7: { if (offset(&set_data, &set_size,  0x6A0)) goto error; break; }
            case 9: { if (offset(&set_data, &set_size, 0x14C0)) goto error; break; }
        };

        size_t bp_size = map_size*8*8*4;
        bp = malloc(bp_size);
        if (bp == NULL)
            goto error;

        decode_bitplane(rom, set_data, map_data, bp, d->palette, map_size, set_size, d->mode, d->fix, d->bg);
        assemble_screen(bp, map_size, 32, dir, d->name);
        free(set_data);
        free(map_data);
        free(bp);
        continue;
error:
        fprintf(stderr, "Encountered an error with entry %d, (%s).\n", i, d->name);
        free(set_data);
        free(map_data);
        free(bp);
    }

} // Special Screens (DKC2)

