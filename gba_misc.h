void huff_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void rle_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void lz77_decomp(uint8_t *rom, uint8_t *output, int len, int rpos);

void gba_decomp(uint8_t *rom, uint8_t *output, int *len, int address);

void gba_layout(uint8_t *lay_data, uint8_t *raw_data, uint8_t *att_data, int *width, int *height, int mode);

void gba_split(uint8_t *lay_data, uint8_t *att_data, int size);

void gba_tiles(uint8_t *bitplane, uint8_t *bp_data, uint8_t *lay_data, uint8_t *att_data, uint8_t *rgb, int lay_len, int priority);

void gba_tileset(uint8_t *lay_data, uint8_t *raw_data);
