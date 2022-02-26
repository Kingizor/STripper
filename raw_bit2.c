#include <stdlib.h>
#include <stdint.h>
#include <dkcomp.h>
#include "bitplane.h"

struct Bitplane {
    int offset;
    char *name;
};

void raw_bitplane2(unsigned char *rom, size_t rom_size, char *dir) {

    static const struct Bitplane bp4[] = {
        {0x1F8116, "Ship Deck.png"},
        {0x1FD537, "Ship Mast.png"},
        {0x1F0870, "Ship Hold.png"},
        {0x1EA932, "Forest.png"},
        {0x1F3059, "Hive.png"},
        {0x202195, "Fair.png"},
        {0x207174, "Lava.png"},
        {0x20CF31, "Mine.png"},
        {0x2106AF, "Swamp.png"},
        {0x215B05, "Brambles.png"},
        {0x29A905, "Castle (RAW).png"},
        {0x229E5F, "Ice.png"},
        {0x22E556, "Jungle (2).png"} // 13
    };
    int i;

    for (i = 0; i < 13; i++) {
        unsigned char *set_data = NULL;
        size_t set_size = 0;
        dk_decompress_mem_to_mem(BD_DECOMP, &set_data, &set_size, rom+bp4[i].offset, rom_size);
        dump_bitplane(set_data, set_size, 4, 16, dir, bp4[i].name);
        free(set_data);
    }


} // dump_bitplanes3();
