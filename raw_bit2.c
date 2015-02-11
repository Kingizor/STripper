#include <stdlib.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

struct Bitplane {
    int offset;
    char *name;
};

void raw_bitplane2(uint8_t *rom, char dir[255]) {

    const struct Bitplane bp4[] = {
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

    uint8_t *bp_data = malloc(65535);
    int bp_counter;
    int i;
    
    for (i = 0; i < 13; i++) {
        bp_counter = 0;
        decomp(bp_data, rom, &bp_counter, bp4[i].offset);
        dump_bitplane(bp_data, bp_counter, 4, 16, dir, bp4[i].name);
    }
    

} // dump_bitplanes3();