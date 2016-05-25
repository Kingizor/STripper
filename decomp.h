#ifndef decomp_h
#define decomp_h

int decomp(uint8_t *output,uint8_t *rom,int *length,int offset);
void decode(uint8_t *rom, uint8_t *output, int *outlen, int x, int bank);

#endif
