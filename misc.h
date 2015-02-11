#ifndef misc_h
#define misc_h

void flip_bear_layout(uint8_t *raw_data);
void bear_layout(uint8_t *raw_data, uint8_t *rom, uint8_t a, int x, int bank);
void bear_photo_layout(uint8_t *raw_data, uint8_t *temp_data, int offset);
void boss_photo_layout(uint8_t *raw_data, uint8_t *temp_data, int offset);
void waterfall_layout(uint8_t *rom, uint8_t *raw_data, int type);
void lights_layout(uint8_t *raw_data, unsigned int raw_counter);
void surface_layout(uint8_t *raw_data, unsigned int raw_counter);
void pipe_layout(uint8_t *raw_data, unsigned int raw_counter);
void decode_bitplane_3(int region, uint8_t *rom, uint8_t *bp_data, uint8_t *raw_data, uint8_t *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg);

#endif
