#ifndef bitplane_h
#define bitplane_h

void assemble_level(uint8_t *bitplane, uint8_t *rom, uint8_t *layout, unsigned int layout_len, int position_addr, int vert, int layout_size, int fix, char dir[255], char name[255]);
void assemble_screen(uint8_t *bitplane, unsigned int raw_len, unsigned int width, char dir[255], char name[255]);
void assemble_bitplane(uint8_t *bitplane, unsigned int width, unsigned int raw_len, char dir[255], char name[255]);
void dump_bitplane(uint8_t *bp_data, int bp_len, int bpp, int width, char dir[255], char name[255]);
void decode_bitplane(uint8_t *rom, uint8_t *bp_data, uint8_t *raw_data, uint8_t *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg);
void decode_palette(uint8_t *rgb, uint8_t *rom, int count);
void bbc_pal(uint8_t *rom, uint8_t *palette, int seed);
void gbc_assemble(uint8_t *bitplane, uint8_t *bp_data, uint8_t *lay_data, uint8_t *col_data, uint8_t *rgb, int t_width, int t_height, int mode);
void arrange_gbc(uint8_t *source, int width, int height, char dir[255], char *name);
void tile_generator(uint8_t *raw_data, int *rawlen, int tiles, int mode);
void write_png(char *name, uint8_t *image, int w, int h);

#endif
