#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>
#include "rom.h"

static const struct ROM_META romlist[] = {
    { 0x195B60FE,  0x400000,  SNES_DKC,   1,0 },
    { 0x32E11AF8,  0x400000,  SNES_DKC2,  2,0 },
    { 0xD73850D8,  0x400000,  SNES_DKC2,  2,0 },
    { 0xA5B0964D,  0x400000,  SNES_DKC2,  2,1 },
    { 0x76612943,  0x400000,  SNES_DKC2,  1,0 },
    { 0x7B4D0D69,  0x400000,  SNES_DKC2,  1,1 },
    { 0xE2A62365,  0x400000,  SNES_DKC2,  0,0 },
    { 0x2AF4ED4E,  0x400000,  SNES_DKC2,  0,1 },
//  { 0xC4599C84,  0x400000,  SNES_DKC3,  2,0 },
    { 0x57592E47,  0x400000,  SNES_DKC3,  1,0 },
//  { 0xB11D0174,  0x400000,  SNES_DKC3,  0,0 },
//  { 0x3E2BD924,  0x400000,  SNES_DKC3,  0,1 },
    { 0xCB6CAE97,  0x400000,   GBC_DKC,   0,0 },
    { 0x48D7B8C4,   0x80000,    GB_DKL,   0,0 },
    { 0x53B97DBF,   0x80000,    GB_DKL2,  0,0 },
    { 0x43673D7C,   0x80000,    GB_DKL3,  0,0 },
//  { 0xC7AB9EA1,  0x100000,   GBC_DKL3,  0,0 },
    { 0xF86CB732,  0x800000,   GBA_DKC,   0,0 },
    { 0x7967734C,  0x800000,   GBA_DKC2,  0,0 },
    { 0xFD5900BF,  0x800000,   GBA_DKKOS, 0,0 },
    { 0x7781F51A, 0x1000000,   GBA_DKC3,  0,0 },
    { 0xAF640C57, 0x4000000,    DS_DKJC,  0,0 }
};
static const int romcount = sizeof(romlist) / sizeof(struct ROM_META);


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
    long unsigned hash = XXH32(romd, romlen, 0x50434441);
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
