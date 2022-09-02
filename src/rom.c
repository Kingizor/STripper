/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - ROM Loading and Identification */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rom.h"

static const struct ROM_META romlist[] = {
    { 0xC946DCA0,  0x400000,  SNES_DKC,   1,0 },
    { 0x2EA6B7E8,  0x400000,  SNES_DKC2,  3,0 },
    { 0x4C794A4D,  0x400000,  SNES_DKC2,  2,0 },
    { 0x2EB33785,  0x400000,  SNES_DKC2,  2,1 },
    { 0x006364DB,  0x400000,  SNES_DKC2,  1,0 },
    { 0x4E2D90F4,  0x400000,  SNES_DKC2,  1,1 },
    { 0xA0BE8E71,  0x400000,  SNES_DKC2,  0,0 },
    { 0x46079C0F,  0x400000,  SNES_DKC2,  0,1 },
//  { 0x0551FE84,  0x400000,  SNES_DKC3,  2,0 },
    { 0x448EEC19,  0x400000,  SNES_DKC3,  1,0 },
//  { 0x0F712928,  0x400000,  SNES_DKC3,  0,0 },
//  { 0x5B337FB6,  0x400000,  SNES_DKC3,  0,1 },
    { 0xB1743477,  0x400000,   GBC_DKC,   0,0 },
    { 0x49DC0D37,   0x80000,    GB_DKL,   0,0 },
    { 0x2827E5D4,   0x80000,    GB_DKL2,  0,0 },
    { 0xB40C159C,   0x80000,    GB_DKL3,  0,0 },
    { 0x28D7E8D3,  0x100000,   GBC_DKL3,  0,0 },
    { 0x12F7A968,  0x800000,   GBA_DKC,   0,0 },
    { 0x11417FC1,  0x800000,   GBA_DKC2,  0,0 },
    { 0xD610B239,  0x800000,   GBA_DKKOS, 0,0 },
    { 0xFE03E5AF, 0x1000000,   GBA_DKC3,  0,0 },
    { 0x038EED18, 0x4000000,    DS_DKJC,  0,0 }
};
static const int romcount = sizeof(romlist) / sizeof(struct ROM_META);

static unsigned crc32 (unsigned char *rom, size_t len) {
    unsigned crc = 0xFFFFFFFF;
    size_t i;
    for (i = 0; i < len; i++) {
        crc ^= rom[i];
        for (int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ ((-(crc & 1)) & 0xEDB88320);
    }
    return ~crc;
}

char *verify_rom (struct ROM_DATA *rom, char *name) {
    FILE *romfile = fopen(name, "rb");

    if (romfile == NULL)
        return "Error opening input file.";

    size_t romlen;
    if ((fseek(romfile, 0, SEEK_END) == -1)
    || ((romlen = ftell(romfile))    == -1u)
    ||  (fseek(romfile, 0, SEEK_SET) == -1)) {
        fclose(romfile);
        return "Error seeking input file.";
    }
    if (romlen < 0x50000) {
        fclose(romfile);
        return "Unsupported ROM version. (file size too small?)";
    }
    else if (romlen > 0x4000000) {
        fclose(romfile);
        return "Unsupported ROM version. (file size too big?)";
    }

    unsigned char *romd = malloc(romlen);
    if (romd == NULL) {
        fclose(romfile);
        return "Unable to allocate memory for input file.";
    }
    if (fread(romd, 1, romlen, romfile) != romlen) {
        fclose(romfile);
        free(romd);
        return "Error reading from input file.";
    }
    fclose(romfile);

    /* SMC header */
    if (romlen == 0x400200) {
        memmove(rom, rom+0x200, 0x400000);
        romlen -= 0x200;
    }

    /* verify */
    unsigned hash = crc32(romd, romlen);
    int i;
    for (i = 0; i < romcount; i++)
        if (romlist[i].hash == hash)
            break;
    if (i == romcount) {
        free(romd);
        return "Unsupported ROM version.";
    }

    rom->buf  = romd;
    rom->meta = &romlist[i];
    return NULL;
}

