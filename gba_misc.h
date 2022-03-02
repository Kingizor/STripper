#ifndef gba_misc
#define gba_misc

struct comp {
    unsigned loc; // Data Location
    unsigned ofs; // (Compressed) ? Data Offset : Data Size
    unsigned char type; // Compression Type

    /*
    00 - Not Compressed
    01 - BIOS
    02 - DKC2-Type (Large)
    03 - DKC3-Type (Small)
    04 - Reserved
    */
};

void huff_decomp(unsigned char *rom, unsigned char *output, int len, int rpos);

void rle_decomp(unsigned char *rom, unsigned char *output, int len, int rpos);

void lz77_decomp(unsigned char *rom, unsigned char *output, int len, int rpos);

void gba_decomp(unsigned char *rom, unsigned char *output, int *len, int address);

void gba_data(unsigned char *rom, unsigned char *output, int *length, unsigned location, unsigned offset, unsigned char type);

void gba_layout(unsigned char *lay_data, unsigned char *raw_data, unsigned char *att_data, int *width, int *height, int mode);

void gba_split(unsigned char *lay_data, unsigned char *att_data, int size);

void gba_tiles(unsigned char *bitplane, unsigned char *bp_data, unsigned char *lay_data, unsigned char *att_data, unsigned char *rgb, int lay_len, int priority, int mode);

void gba_tileset(unsigned char *lay_data, unsigned char *raw_data);

#endif
