#ifndef gba_misc
#define gba_misc

struct comp {
    uint32_t loc; // Data Location
    uint32_t ofs; // (Compressed) ? Data Offset : Data Size
    uint8_t type; // Compression Type
    
    /*
    00 - Not Compressed
    01 - BIOS
    02 - DKC2-Type (Large)
    03 - DKC3-Type (Small)
    04 - Reserved
    */
};

void huff_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void rle_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void lz77_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void gba_decomp(uint8_t *rom, uint8_t *output, int *len, int address);

void gba_data(uint8_t *rom, uint8_t *output, int *length, uint32_t location, uint32_t offset, uint8_t type);

void gba_layout(uint8_t *lay_data, uint8_t *raw_data, uint8_t *att_data, int *width, int *height, int mode);

void gba_split(uint8_t *lay_data, uint8_t *att_data, int size);

void gba_tiles(uint8_t *bitplane, uint8_t *bp_data, uint8_t *lay_data, uint8_t *att_data, uint8_t *rgb, int lay_len, int priority, int mode);

void gba_tileset(uint8_t *lay_data, uint8_t *raw_data);

#endif