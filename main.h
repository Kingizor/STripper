#ifndef main_h
#define main_h

void level1(unsigned char *rom, char *dir, int priority, int mode);
void level2(uint8_t *rom, char dir[255], int priority, int special, int region);
void spec1(uint8_t *rom, char dir[255]);
void spec2(uint8_t *rom, char dir[255], int region);
void spec3(uint8_t *rom, char dir[255], int region);
void level3(uint8_t *rom, char dir[255], int priority, int special, int region);
void anim2(uint8_t *rom, char dir[255], int priority, int region);
void anim3(uint8_t *rom, char dir[255], int priority, int region);
void raw_bitplane2(uint8_t *rom, char dir[255]);
void raw_bitplane3(uint8_t *rom, char dir[255]);
void dkl_levels(uint8_t *rom, char *dir, uint8_t sgb);
void dkl2_levels(uint8_t *rom, char *dir, uint8_t sgb);
void dkl3_levels(uint8_t *rom, char *dir, uint8_t sgb);
void gbc_levels(uint8_t *rom, char dir[255]);
void dkc_gba_levels(uint8_t *rom, char *dir, int priority, int tileset);
void dkc2_gba_levels(uint8_t *rom, char *dir, int priority, int tileset);
void dkc3_gba_levels(uint8_t *rom, char *dir, int priority, int tileset);

#endif