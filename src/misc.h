#ifndef misc_h
#define misc_h

void flip_bear_layout(unsigned char *raw_data);
void bear_layout(unsigned char *raw_data, unsigned char *rom, unsigned char a, int x, int bank);
void bear_photo_layout(unsigned char *raw_data, unsigned char *temp_data, int offset);
void boss_photo_layout(unsigned char *raw_data, unsigned char *temp_data, int offset);
void waterfall_layout(unsigned char *rom, unsigned char *raw_data, int type);
void lights_layout(unsigned char *raw_data, unsigned int raw_counter);
void surface_layout(unsigned char *raw_data, unsigned int raw_counter);
void pipe_layout(unsigned char *raw_data, unsigned int raw_counter);
void decode_bitplane_3(int region, unsigned char *rom, unsigned char *bp_data, unsigned char *raw_data, unsigned char *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg);

#endif
