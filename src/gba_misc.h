/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - GBA Definitions */

#ifndef gba_misc
#define gba_misc

enum GBA_COMP_TYPE {
    GBA_COMP_NONE,
    GBA_COMP_BIOS,
    GBA_COMP_DKC2,
    GBA_COMP_DKC3
};

struct comp {
    unsigned loc; // Data Location
    unsigned ofs; // (Compressed) ? Data Offset : Data Size
    enum GBA_COMP_TYPE type; // Compression Type
};

int  gba_data   (unsigned char *rom, size_t romsize, unsigned char **output, size_t *outsize, unsigned loc, unsigned offset, int data_type);
int  gba_layout (unsigned char**lay_data, unsigned char *raw_data, unsigned char *att_data, int *width, int *height, int mode);
void gba_split  (unsigned char *lay_data, unsigned char *att_data, int size);
void gba_tiles  (unsigned char *bitplane, unsigned char *bp_data, unsigned char *lay_data, unsigned char *att_data, unsigned char *rgb, int lay_len, int priority, int mode);
int  gba_tileset(unsigned char **lay_data, size_t *lay_len, unsigned char *raw_data);

#endif
