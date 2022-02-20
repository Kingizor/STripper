#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

struct Data {
    uint32_t source; // Where to copy/decompress from
    uint32_t target; // Where to copy to (NULL if compressed)
    uint32_t length; // Length to copy
    uint32_t jp_offset;
    uint32_t fr_offset;
    uint32_t de_offset;
}

struct Screen {
    struct Data bp_data;
    struct Data raw_data;
    uint32_t palette;
    uint32_t palette_fix;
    uint8_t bit_mode; // Determines 2bpp or 4bpp decoding
    uint8_t alpha; // Opaque or transparent
    char *name;
};

void spec2(uint8_t *rom, char dir[255], int region) {
    
    struct Screen screens[] = {
        {{0x2A5978, 0, 0,   35, 35, 35}, {0x8F99C,  0, 0,    0,  0,  0}, 0x3D2170, 0, 2, 0, "K.Rool's Kabin & Rattly Room"},
        {{0x2C83A0, 0, 0,   37, 37, 37}, {0x2C7CF0, 0, 0,   37, 37, 37}, 0x3D3C6E, 0, 2, 1, "File Screen"},
        {{0x2C83A0, 0, 0,   37, 37, 37}, {0x2C7CF0, 0, 0,   37, 37, 37}, 0x000000, 3, 2, 1, "Bonus Screen"},
        // {{0x2D0997, 0, 0,   47, 37, 37}, {0x2D02C6, 0, 0, NULL, 37, 37}, 0x3D26AE, 0, 2, 1, "Title Screen"},
        {{0x2D8584, 0, 0, 6939, 37, 35}, {0x2D9E72, 0, 0, 6939, 37, 35}, 0x000000, 4, 2, 0, "Destroy Them All Overlay"},
        {{0x2D78D3, 0, 0, 6939, 37, 35}, {0x2DA030, 0, 0, 6939, 37, 35}, 0x000000, 5, 2, 0, "Collect the Starts Overlay"},
        {{0x2C4D40, 0, 0,   34, 34, 34}, {0x2C4749, 0, 0,   33, 33, 33}, 0x3D3C6E, 0, 2, 0, "Game Selection"},
        {{0x2C4D40, 0, 0,   34, 34, 34}, {0x2C4AAD, 0, 0,   33, 33, 33}, 0x3D3C6E, 0, 2, 0, "Music Test"},
        {{0x2C4D40, 0, 0,   34, 34, 34}, {0x2C4C1C, 0, 0,   34, 34, 34}, 0x3D3C6E, 0, 2, 0, "Cheat Mode"},
        {{0x2DA1AB, 0, 0, 6939, 37, 35}, {0x05FAA8, 0, 0x700,    0,  0,  0}, 0x3D12F0, 0, 2, 1, "Swanky's Bonus Bonanza"},
        {{0x2BCFD1, 0, 0,   35, 35, 35}, {0x2C1B0F, 0, 0x700,   33, 33, 33}, 0x3D0FF0, 0, 2, 1, "Monkey Museum"},
        {{0x2BA27C, 0, 0,   35, 35, 35}, {0x03FD35, 0, 0x700,    0,  0,  0}, 0x3D10F0, 0, 2, 0, "Funky's Flights"},
        {{0x2A121C, 0, 0,   35, 35, 35}, {0x0AFABE, 0, 0x700,    0,  0,  0}, 0x3D10F0, 0, 2, 1, "Funky's Flights BG"},
        {{0x2B50CE, 0, 0,   35, 35, 35}, {0x330000, 0, 0x700,    0,  0,  0}, 0x3D10F0, 0, 2, 1, "Klubba's Kiosk"},
        
        {{0x12B9FD, 0, 0, 0, 0, 0}, {0x0EFA9F, 0, 0x700, 0, 0, 0}, 0x3D11F0, 0, 2, 1, "Kong Kollege"},
        
        /*
        Running into problems:
        
        Title screen is not compressed in JP version, but is compressed in every other version.
        Some areas may require copying after decompressing.
        Data is clunky to look at.
        */
        
        
    };
    
    return 0;
    
}
