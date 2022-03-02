#include <stdlib.h>
#include <dkcomp.h>
#include "bitplane.h"

struct Bitplane {
    int offset;
    char *name;
};

void raw_bitplane3(unsigned char *rom, size_t rom_size, char *dir) {

    const struct Bitplane bp4[] = {
        {0x208E80, "4bpp_Caves.png"},
        {0x215058, "4bpp_Underwater.png"},
        {0x221914, "4bpp_Cliffs.png"},
        {0x22D9E8, "4bpp_Jungle (3).png"},
        {0x238D56, "4bpp_Factory.png"},
        {0x243549, "4bpp_Mill.png"},
        {0x24D25F, "4bpp_Riverside.png"},
        {0x25F598, "4bpp_Snow.png"},
        {0x254C41, "4bpp_Boardwalk.png"},
        {0x269DCF, "4bpp_Tree.png"},
        {0x27588E, "4bpp_Pipe.png"},
        {0x27F2F2, "4bpp_Waterfall.png"}, // 12
        {0x330200, "4bpp_Nintendo Presents 1.png"},
        {0x360B11, "4bpp_Nintendo Presents 2.png"},
        {0x0CFE3D, "4bpp_Title Screen.png"},
        {0x29342E, "4bpp_Title Screen Text.png"},
        {0x13FE2A, "4bpp_File Screen.png"},
        {0x2A077B, "4bpp_File Screen VHS.png"},
        {0x2AC294, "4bpp_Bonus Screen.png"},
        {0x2AA8DC, "4bpp_Grab 15 Bananans.png"},
        {0x2A9DCC, "4bpp_Collect the Stars.png"},
        {0x2AB4DE, "4bpp_Find the Coin.png"},
        {0x2A8675, "4bpp_Bash the Baddies.png"},
        {0x0BFE6D, "4bpp_Game Over Screen.png"}, // 24
        {0x0EFD21, "4bpp_Swanky's Sideshow.png"},
        {0x08FE75, "4bpp_Cranky's Sideshow.png"},
        {0x0A0000, "4bpp_Cranky's Sideshow Border.png"},
        {0x0DFED2, "4bpp_Funky's Rentals.png"},
        {0x07FD8A, "4bpp_Wrinkly's Save Cave.png"},
        {0x382E96, "4bpp_Banana Bird Caves.png"},
        {0x2B04B0, "4bpp_Banana Bird Caves Ice.png"},
        {0x331DED, "4bpp_Banana Queen BG.png"},
        {0x2B8538, "4bpp_Banana Queen Block & Sunlight.png"},
        {0x334FAD, "4bpp_Dixie's Photo Album BG.png"},
        {0x29342E, "4bpp_Dixie's Photo Album Text.png"},
        {0x2B1BC4, "4bpp_Boss Photographs.png"}, // 36
        {0x380000, "4bpp_Bear Photographs.png"},
        {0x25968A, "4bpp_Boardwalk BG1.png"},
        {0x2BAB57, "4bpp_K.Rool Hovercraft.png"},
        {0x1DFE8F, "4bpp_Mill BG1.png"},
        {0x370000, "4bpp_Snow BG.png"},
        {0x299BFB, "4bpp_Belcha.png"},
        {0x2717A6, "4bpp_Tree BG1.png"},
        {0x28861B, "4bpp_Riverside BG1.png"},
        {0x2A21AB, "4bpp_Waterfalls.png"},
        {0x2B14E2, "4bpp_Gleamin' Bream Underlay.png"},
        {0x29BFBC, "4bpp_Barbos.png"},
        {0x2A68F2, "4bpp_Pipe BG2.png"}, // 48
        {0x2A426A, "4bpp_Cliff BG1.png"},
        {0x32587C, "4bpp_Cave BG1.png"},
        {0x32275C, "4bpp_Jungle BG1.png"},
        {0x29481D, "4bpp_Bleak BG.png"},
        {0x3B0000, "4bpp_Bear Cabins.png"},
        {0x350000, "4bpp_Overworld.png"},
        {0x321DA4, "4bpp_Overworld (Water).png"},
        {0x0FFF7E, "4bpp_Lake Orangatanga.png"},
        {0x0AFDB5, "4bpp_Kremwood Forest.png"},
        {0x04F05F, "4bpp_Cotton-Top Cove.png"},
        {0x14FFD7, "4bpp_Mekanos.png"},
        {0x06FEFE, "4bpp_K3.png"}, // 60
        {0x37266A, "4bpp_Razor Ridge.png"},
        {0x01FC3C, "4bpp_KAOS Kore.png"},
        {0x1BFE0E, "4bpp_Krematoa.png"},
        {0x2B0B39, "4bpp_Water.png"},
        {0x28BDEC, "4bpp_Kastle KAOS.png"},
        {0x2BDC2A, "4bpp_Knautilus.png"} // 66
    };
    int bp4_size = sizeof(bp4) / sizeof(struct Bitplane);

    const struct Bitplane bp2[] = {
        {0x1CFF76, "2bpp_Boardwalk BG2.png"},
        {0x29B6C0, "2bpp_Factory BG2.png"},
        {0x2703F1, "2bpp_Tree BG2.png"},
        {0x28A874, "2bpp_Riverside BG2 & Lightning.png"},
        {0x2A1349, "2bpp_Squirt's Showdown BG.png"},
        {0x2A6712, "2bpp_Pipe BG3.png"},
        {0x2A3387, "2bpp_Cliff BG2.png"},
        {0x32470A, "2bpp_Jungle BG2.png"},
        {0x10FE01, "2bpp_Lake Orangatanga (Water).png"},
        {0x11FF7D, "2bpp_Kremwood Forest (Water).png"},
        {0x36688A, "2bpp_Cotton-Top Cove (Water).png"},
        {0x367029, "2bpp_Mekanos (Water).png"},
        {0x3673D5, "2bpp_Razor Ridge (Water).png"},
        {0x367816, "2bpp_KAOS Kore (Water).png"},
        {0x367B9A, "2bpp_Krematoa (Water).png"},
        {0x2BD1D2, "2bpp_Knautilus.png"} // 16
    };
    int bp2_size = sizeof(bp2) / sizeof(struct Bitplane);
    int i;

    for (i = 0; i < bp4_size; i++) {
        unsigned char *set_data = NULL;
        size_t set_size;
        dk_decompress_mem_to_mem(BD_DECOMP, &set_data, &set_size, rom+bp4[i].offset, rom_size);
        dump_bitplane(set_data, set_size, 4, 16, dir, bp4[i].name);
    }

    for (i = 0; i < bp2_size; i++) {
        unsigned char *set_data = NULL;
        size_t set_size;
        dk_decompress_mem_to_mem(BD_DECOMP, &set_data, &set_size, rom+bp2[i].offset, rom_size);
        dump_bitplane(set_data, set_size, 2, 16, dir, bp2[i].name);
    }


} // dump_bitplanes3();
