/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - ROM Handling and Metadata */

#ifndef ROM_H
#define ROM_H

enum GAME_LIST {
    UNKNOWN,
    SNES_DKC,
    SNES_DKC2,
    SNES_DKC3,
      GB_DKL,
      GB_DKL2,
      GB_DKL3,
     GBC_DKL3,
     GBC_DKC,
     GBA_DKC,
     GBA_DKC2,
     GBA_DKC3,
     GBA_DKKOS,
      DS_DKJC
};

struct ROM_META {
    long unsigned hash;
    size_t size;
    enum GAME_LIST game;
    int region;
    int version;
};

struct ROM_DATA {
    unsigned char *buf;
    const struct ROM_META *meta;
};

char *verify_rom(struct ROM_DATA*, char*);

#endif
