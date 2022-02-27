#ifndef main_h
#define main_h

void level1(unsigned char *rom, size_t rom_size, char *dir, int priority, int mode, int tileset);
void level2(unsigned char *rom, size_t rom_size, char *dir, int priority, int special, int tileset, int region);
void level3(unsigned char *rom, size_t rom_size, char *dir, int priority, int special, int tileset, int region);
void spec1 (unsigned char *rom, char *dir);
void spec2 (unsigned char *rom, size_t rom_size, char *dir, int region);
void spec3 (unsigned char *rom, size_t rom_size, char *dir, int region);
void anim2 (unsigned char *rom, char *dir, int priority, int region);
void anim3 (unsigned char *rom, char *dir, int priority, int region);
void raw_bitplane2(unsigned char *rom, char *dir);
void raw_bitplane3(unsigned char *rom, char *dir);
void dkl_levels (unsigned char *rom, char *dir, unsigned char sgb, int tileset);
void dkl2_levels(unsigned char *rom, char *dir, unsigned char sgb, int tileset);
void dkl3_levels(unsigned char *rom, char *dir, unsigned char sgb, int tileset);
void gbc_levels (unsigned char *rom, size_t rom_size, char *dir);

void dkc_gba_levels (unsigned char *rom, char *dir, int priority, int tileset);
void dkc2_gba_levels(unsigned char *rom, char *dir, int priority, int tileset);
void dkc3_gba_levels(unsigned char *rom, char *dir, int priority, int tileset);

void kos_levels(unsigned char *rom, char *dir);
void jc_levels (unsigned char *rom, char *dir);

#endif
