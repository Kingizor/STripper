/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - SNES DKC3 screens */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "bitplane.h"
#include "misc.h"

enum COMP_TYPE {
    RAW = 0,
    COMP_BD,
    COMP_SD,
    SPEC
};

struct DATA {
    unsigned addr;
    unsigned size; /* for memcpy */
      signed move; /* for amalgams */
    enum COMP_TYPE type;
};

struct DKC3_SCREEN {
    struct DATA set;
    struct DATA map;
    unsigned palette;
    unsigned char spec; /* special layout */
    unsigned char mode;
    unsigned char fix;
    unsigned char bg;
    char *name;
};

struct BEAR_DATA { /* kill me */
    unsigned char n;
    unsigned char  a1;
    unsigned short a2;
};

static const struct DKC3_SCREEN dkc3[] = {
    { { 0x298030, 0x520,0, 0 }, { 0x297FB0, 0x80,0, 0 }, 0x3D7901, 0,2, 0,1, "Nintendo 1996" },
    { { 0x330200, 0,0, 1 }, { 0x330000, 0,0, 2 }, 0x3D7681,  0,2, 0,0, "Nintendo Presents 1" },
    { { 0x360B11, 0,0, 1 }, { 0x3621E6, 0,0, 2 }, 0x3D7681,  0,2, 0,0, "Nintendo Presents 2" },
    { { 0x3316F0, 0,0, 1 }, { 0x3315C6, 0,0, 2 }, 0x3D7681,  0,3, 0,0, "Nintendo Presents 3" },
    { { 0x0CFE3D, 0,0, 1 }, { 0x287581, 0,0, 2 }, 0x3D7701,  0,2, 0,1, "Title Screen" },
    { { 0x29342E, 0,0, 1 }, { 0x29339E, 0,0, 2 }, 0x3D7701,  0,2, 0,0, "Title Screen Text" },
    { { 0x12FF14, 0,0, 1 }, { 0x28B052, 0,0, 2 }, 0x3D8361,  0,2, 0,0, "TV Screen" },
    { { 0x13FE2A, 0,0, 1 }, { 0x2A0488, 0,0, 2 }, 0x3D8641,  0,2, 0,0, "File Screen" },
    { { 0x2A077B, 0,0, 1 }, { 0x2A0730, 0,0, 2 }, 0x3D8641,  0,2, 0,0, "File Screen VHS" },
    { { 0x2AC294, 0,0, 1 }, { 0x2AC190, 0,0, 2 }, 0x3D8841,  0,2, 0,0, "Bonus Screen" },
    { { 0x2AA8DC, 0,0, 1 }, { 0x2AA818, 0,0, 2 }, 0x3D8A01,  0,2,14,0, "Grab 15 Bananas" },
    { { 0x2AA8DC, 0,0, 1 }, { 0x2AA818, 0,0, 2 }, 0x3D8A01,  0,2,51,0, "Grab 15 Bananas 2P" },
    { { 0x2A9DCC, 0,0, 1 }, { 0x2A9CD8, 0,0, 2 }, 0x3D8841,  0,2, 0,0, "Collect the Stars" },
    { { 0x2A9DCC, 0,0, 1 }, { 0x2A9CD8, 0,0, 2 }, 0x3D8841,  0,2,50,0, "Collect the Stars 2P" },
    { { 0x2AB4DE, 0,0, 1 }, { 0x2AB3E9, 0,0, 2 }, 0x3D8AE1,  0,2,14,0, "Find the Coin" },
    { { 0x2AB4DE, 0,0, 1 }, { 0x2AB3E9, 0,0, 2 }, 0x3D8AE1,  0,2,51,0, "Find the Coin 2P" },
    { { 0x2A8675, 0,0, 1 }, { 0x2A858A, 0,0, 2 }, 0x3D8A01,  0,2,14,0, "Bash the Baddies" },
    { { 0x2A8675, 0,0, 1 }, { 0x2A858A, 0,0, 2 }, 0x3D8A01,  0,2,51,0, "Bash the Baddies 2P" },
    { { 0x0BFE6D, 0,0, 1 }, { 0x1FFE80, 0,0, 2 }, 0x3D7521,  0,2, 0,1, "Game Over" },
    { { 0x0EFD21, 0,0, 1 }, { 0x287D26, 0,0, 2 }, 0x3D8461,  0,2, 0,1, "Swanky's Sideshow (0)" },
    { { 0x0EFD21, 0,0, 1 }, { 0x287D26, 0,0, 2 }, 0x3D8461,  0,2,45,1, "Swanky's Sideshow (1)" },
    { { 0x0EFD21, 0,0, 1 }, { 0x287D26, 0,0, 2 }, 0x3D8461,  0,2,46,1, "Swanky's Sideshow (2)" },
    { { 0x0EFD21, 0,0, 1 }, { 0x287D26, 0,0, 2 }, 0x3D8461,  0,2,47,1, "Swanky's Sideshow (3)" },
    { { 0x08FE75, 0,0, 1 }, { 0x28640C, 0,0, 2 }, 0x3D5B01,  0,2, 0,1, "Cranky's Sideshow" },
    { { 0x0A0000, 0,0, 1 }, { 0x2866E0, 0,0, 2 }, 0x3D5B01,  0,2, 0,0, "Cranky's Sideshow Border" },
    { { 0x0DFED2, 0,0, 1 }, { 0x287AA3, 0,0, 2 }, 0x3D8561,  0,2, 0,0, "Funky's Rentals" },
    { { 0x07FD8A, 0,0, 1 }, { 0x2877CC, 0,0, 2 }, 0x3D6401,  0,2, 0,1, "Wrinkly's Save Cave (1)" },
    { { 0x07FD8A, 0,0, 1 }, { 0x2877CC, 0,0, 2 }, 0x3D6501,  0,2, 0,1, "Wrinkly's Save Cave (2)" },
    { { 0x07FD8A, 0,0, 1 }, { 0x2877CC, 0,0, 2 }, 0x3D6601,  0,2, 0,1, "Wrinkly's Save Cave (3)" },
    { { 0x07FD8A, 0,0, 1 }, { 0x2877CC, 0,0, 2 }, 0x3D6701,  0,2, 0,1, "Wrinkly's Save Cave (4)" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,16,1, "BBC - K's Kache" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,17,1, "BBC - Hill-Top Hoard" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,18,1, "BBC - Undercover Cove" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,19,1, "BBC - Kong Cave" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,20,1, "BBC - Bounty Beach" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,21,1, "BBC - Belcha's Burrow" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,22,1, "BBC - Smuggler's Cove" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,23,1, "BBC - Arich's Hoard" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,24,1, "BBC - Bounty Bay" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,25,1, "BBC - Sky-High Secret" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,26,1, "BBC - Glacial Grotto" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,27,1, "BBC - Clifftop Cache" },
    { { 0x382E96, 0,0, 1 }, { 0x2A9AD9, 0,0, 2 }, 0x3D8741,  0,2,28,1, "BBC - Sewer Stockpile" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,16,1, "BBC - K's Kache Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,17,1, "BBC - Hill-Top Hoard Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,18,1, "BBC - Undercover Cove Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,19,1, "BBC - Kong Cave Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,20,1, "BBC - Bounty Beach Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,21,1, "BBC - Belcha's Burrow Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,22,1, "BBC - Smuggler's Cove Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,23,1, "BBC - Arich's Hoard Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,24,1, "BBC - Bounty Bay Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,25,1, "BBC - Sky-High Secret Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,26,1, "BBC - Glacial Grotto Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,27,1, "BBC - Clifftop Cache Ice" },
    { { 0x2B04B0, 0,0, 1 }, { 0x2B0486, 0,0, 2 }, 0x3D8741,  0,2,28,1, "BBC - Sewer Stockpile Ice" },
    { { 0x331DED, 0,0, 1 }, { 0x331B38, 0,0, 2 }, 0x3DA239,  0,2, 0,0, "Banana Queen BG" },
    { { 0x2B8538, 0,0, 1 }, { 0x2B81FF, 0,0, 2 }, 0x3DA239,  0,2, 0,0, "Banana Queen Block & Sunlight" },
    { { 0x334FAD, 0,0, 1 }, { 0x33480A, 0,0, 2 }, 0x3DA839,  0,2, 0,0, "Dixie's Photo Album BG" },
    { { 0x29342E, 0,0, 1 }, { 0x29339E, 0,0, 2 }, 0x3DA839,  0,2, 0,0, "Dixie's Photo Album Text" },
    { { 0x334FAD, 0,0, 1 }, { 0x334C21, 0,0, 1 }, 0x3DA839,  0,2,40,0, "Photograph Category BG" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB139,  1,2, 0,0, "Photograph - Belcha" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB239,  2,2, 0,0, "Photograph - Arich" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB339,  3,2, 0,0, "Photograph - Squirt" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB439,  4,2, 0,0, "Photograph - KAOS" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB539,  5,2, 0,0, "Photograph - Bleak" },
    { { 0x2B1BC4, 0,0, 1 }, { 0x2B19BB, 0,0, 2 }, 0x3DB639,  6,2, 0,0, "Photograph - Barbos" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DA999,  7,2, 0,0, "Photograph - Bazaar & Barnacle" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DAC99,  8,2, 0,0, "Photograph - Brash & Blunder" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DAB99,  9,2, 0,0, "Photograph - Blue & Bazooka" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DAA99, 10,2, 0,0, "Photograph - Blizzard & Bramble" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DAD99, 11,2, 0,0, "Photograph - Bennry, Bjorn & Barter" },
    { { 0x380000, 0,0, 1 }, { 0x2B12E2, 0,0, 2 }, 0x3DA899, 12,2, 0,0, "Photograph - Baffle & Boomer" },

    { { 0x25968A, 0,0,      1 }, { 0x2591C9, 0,0, 2 }, 0x3D5601, 0,2, 0,0, "Lakeside Limbo BG1" },
    { { 0x25968A, 0,0,      1 }, { 0x2591C9, 0,0, 2 }, 0x3DBA99, 0,2, 0,0, "Tidal Trouble BG1" },
    { { 0x25968A, 0,0,      1 }, { 0x2591C9, 0,0, 2 }, 0x3DB999, 0,2, 0,0, "Kreeping Klasps BG1" },
    { { 0x25968A, 0,0,      1 }, { 0x2594A2, 0,0, 1 }, 0x3D5601, 0,2, 0,0, "Boardwalk BG1-3" },
    { { 0x25968A, 0,0,      1 }, { 0x259533, 0,0, 1 }, 0x3D5601, 0,2, 0,0, "Lakeside Limbo BG1-2" },
    { { 0x25968A, 0,0,      1 }, { 0x259533, 0,0, 1 }, 0x3DBA99, 0,2, 0,0, "Tidal Trouble BG1-2" },
    { { 0x25968A, 0,0,      1 }, { 0x259533, 0,0, 1 }, 0x3DB999, 0,2, 0,0, "Kreeping Klasps BG1-2" },
    { { 0x1CFF76, 0,0x1220, 1 }, { 0x1CFECD, 0,0, 2 }, 0x3D5601, 0,3, 0,0, "Lakeside Limbo BG2" },
    { { 0x1CFF76, 0,0x1220, 1 }, { 0x1CFECD, 0,0, 2 }, 0x3DBA99, 0,3, 0,0, "Tidal Trouble BG2" },
    { { 0x1CFF76, 0,0x1220, 1 }, { 0x1CFECD, 0,0, 2 }, 0x3DB999, 0,3, 0,0, "Kreeping Klasps BG2" },

    { { 0x2BAB57, 0,0,      1 }, { 0x2BAABA, 0,0,     2 }, 0x3D5601, 0,2,42,0, "K.Rool in Hovercraft" },
    { { 0x1DFE8F, 0,0x14A0, 1 }, { 0x1DF873, 0,0,     1 }, 0x3D61C1, 0,2, 0,0, "Doorstop Dash BG1" },
    { { 0x1DFE8F, 0,0x14A0, 1 }, { 0x1DF873, 0,0,     1 }, 0x3D60C1, 0,2, 0,0, "Squeals on Wheels BG1" },
    { { 0x1DFE8F, 0,0x14A0, 1 }, { 0x1DF873, 0,0,     1 }, 0x3D6901, 0,2, 0,0, "Murky Mill BG1" },
    { { 0x1F04D4,0x8D0,0x1720, 0 }, { 0x1EFF14, 0x5C0,0, 0 }, 0x3D6901,61,3,39,1, "Murky Mill Lights" },
    { { 0x370000, 0,0x1140, 1 }, { 0x36254B, 0x700,0, 0 }, 0x3D5FC1, 0,2, 0,1, "Skidda's Row BG" },
    { { 0x370000, 0,0x1140, 1 }, { 0x36254B, 0x700,0, 0 }, 0x3DBC99, 0,2, 0,1, "Tearaway Toboggan BG" },
    { { 0x370000, 0,0x1140, 1 }, { 0x36254B, 0x700,0, 0 }, 0x3DBB99, 0,2,29,1, "Lemguin Lunge BG" },

    { { 0x299BFB, 0,0x1E40, 1 }, { 0x299AB2, 0,0,     2 }, 0x3D6301, 0,2,15,0, "Belcha" },
    { { 0x29B6C0, 0,0x11C0, 1 }, { 0x29B0BD, 0,0,     2 }, 0x3D6301, 0,3,15,0, "Belcha's Barn BG2" },

    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3D5D01,62,2, 0,0, "Barrel Shield Bust-Up BG1" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3DBD99,62,2, 0,0, "Springin' Spiders BG1" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3DBE79,62,2, 0,0, "Ripsaw Rage BG1" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3DBF79,62,2, 0,0, "Swoopy Salvo BG1" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3D5DE1,62,2, 0,0, "Swoopy Salvo BG1 (AK)" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,-0xC0, 2 }, 0x3D5EC1,62,2, 0,0, "Arich's Ambush BG1 (Alt)" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,0,     2 }, 0x3DBE79,63,2, 0,0, "Ripsaw Rage Saw" },
    { { 0x2717A6, 0,0xBE0,  1 }, { 0x2712B8, 0,0,     2 }, 0x3DBE79,63,2,64,0, "Ripsaw Rage Saw (Alt)" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3D5D01, 0,3, 0,1, "Barrel Shield Bust-Up BG2" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3DBD99, 0,3, 0,1, "Springin' Spiders BG2" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3DBE79, 0,3, 0,1, "Ripsaw Rage BG2" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3DBF79, 0,3, 0,1, "Swoopy Salvo BG2" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3D5DE1, 0,3, 0,1, "Swoopy Salvo BG2 (AK)" },
    { { 0x2703F1, 0,0,      1 }, { 0x26FCEA, 0,0,     1 }, 0x3D5EC1, 0,3, 0,1, "Arich's Ambush BG2" },
    { { 0x298D12, 0x1982,0xBE0, 0 }, { 0x298550, 0x800,0, 3 }, 0x3D5EC1,13,2, 0,0, "Arich" },

    { { 0x28861B, 0,0x6080, 1 }, { 0x288388, 0,0, 2 }, 0x3D7921, 0,2, 0,0, "Riverside Race BG1" },
    { { 0x28861B, 0,0x6080, 1 }, { 0x288388, 0,0, 2 }, 0x3D7B21, 0,2, 0,0, "Bobbing Barrel Brawl BG1" },
    { { 0x28861B, 0,0x6080, 1 }, { 0x288388, 0,0, 2 }, 0x3D7A21, 0,2, 0,0, "Lightning Look-Out BG1" },
    { { 0x28861B, 0,0x6080, 1 }, { 0x288388, 0,0, 2 }, 0x3D7C21, 0,2, 0,0, "Lightning Look-Out BG1 (Bright)" },

    { { 0x28A874, 0,0xEA0, 1 }, { 0x28A634, 0x240,0, 0}, 0x3D7921, 14,3, 0,1, "Riverside Race BG2" },
    { { 0x28A874, 0,0xEA0, 1 }, { 0x28A634, 0x240,0, 0}, 0x3D7B21, 14,3, 0,1, "Bobbing Barrel Brawl BG2" },
    { { 0x28A874, 0,0xEA0, 1 }, { 0x28A634, 0x240,0, 0}, 0x3D7A21, 14,3, 0,1, "Lightning Look-Out & Pot Hole Panic BG2" },
    { { 0x28A874, 0,0xEA0, 1 }, { 0x28A634, 0x240,0, 0}, 0x3D7C21, 14,3, 0,1, "Lightning Look-Out BG2 (Bright)" }, /* check */

    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,30,0, "Lightning-1" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,31,0, "Lightning-2" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,32,0, "Lightning-3" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,33,0, "Lightning-4" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,34,0, "Lightning-5" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,35,0, "Lightning-6" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,36,0, "Lightning-7" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,37,0, "Lightning-8" },
    { { 0x28A874, 0,0x1EA0, 1 }, { 0x28A412, 0,0, 1 }, 0x3D7A21, 0,3,38,0, "Lightning-9" },

    { { 0x2A160B, 0x1900,0x1E00, 0 }, { 0x2A0C0C, 0,0, 2 }, 0x3D5701, 0,3, 0,1, "Rocket Barrel Ride BG" },
    { { 0x2A160B, 0x1900,0x1E00, 0 }, { 0x2A0C0C, 0,0, 2 }, 0x3D5901, 0,3, 0,1, "Tracker Barrel Trek BG" },
    { { 0x2A160B, 0x1900,0x1E00, 0 }, { 0x2A0C0C, 0,0, 2 }, 0x3D5801, 0,3, 0,1, "Barrel Drop Bounce BG" },
    { { 0x2A160B, 0x1900,0x1E00, 0 }, { 0x2A0C0C, 0,0, 2 }, 0x3D5A01, 0,3, 0,1, "Squirt's Showdown BG" },
    { { 0x2A1349, 0x1900,0x1E00, 1 }, { 0x2A0C0C, 0,0, 2 }, 0x3D5A01,65,3, 0,1, "Squirt's Showdown BG2" },

    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5A01, 15,2, 0,0, "Waterfall SS (128)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5701, 16,2, 0,0, "Waterfall RBR (192)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5801, 16,2, 0,0, "Waterfall BDB (192)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 17,2, 0,0, "Waterfall TBT (256L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5801, 17,2, 0,0, "Waterfall BDB (256L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 18,2, 0,0, "Waterfall TBT (256R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5801, 18,2, 0,0, "Waterfall BDB (256R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 19,2, 0,0, "Waterfall TBT (320L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 20,2, 0,0, "Waterfall TBT (320R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 21,2, 0,0, "Waterfall TBT (384L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5801, 21,2, 0,0, "Waterfall BDB (384L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 22,2, 0,0, "Waterfall TBT (384R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5801, 22,2, 0,0, "Waterfall BDB (384R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 23,2, 0,0, "Waterfall TBT (448&576L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5701, 23,2, 0,0, "Waterfall RBR (576L)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 24,2, 0,0, "Waterfall TBT (448&576M)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5701, 24,2, 0,0, "Waterfall RBR (576M)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5901, 25,2, 0,0, "Waterfall TBT (448&576R)" },
    { { 0x2A21AB, 0,0x1520, 1 }, { 0, 0,0,3 }, 0x3D5701, 25,2, 0,0, "Waterfall RBR (576R)" },

    { { 0x2B14E2, 0,0,  1 }, { 0x2B13B3, 0,0, 2 }, 0x3D8261, 0,2, 0,1, "Gleamin' Bream Underlay" },
    { { 0x29F818, 0xC70,0, 0 }, { 0x29F346, 0,0, 1 }, 0x3D8161, 0,3, 0,0, "Bazza's Blockade BG" },
    { { 0x29F818, 0xC70,0, 0 }, { 0x29F346, 0,0, 1 }, 0x3DB899, 0,3, 0,0, "Fish Food Frenzy BG" },
    { { 0x29F818, 0xC70,0, 0 }, { 0x29F346, 0,0, 1 }, 0x3D8261, 0,3, 9,0, "Floodlit Fish Dark BG" },
    { { 0x29F818, 0xC70,0, 0 }, { 0x29F346, 0,0, 1 }, 0x3D8261, 0,3,10,0, "Floodlit Fish Light BG" },
    { { 0x29F818, 0xC70,0, 0 }, { 0x29F346, 0,0, 1 }, 0x3DB799, 0,3, 0,0, "Barbos' Barrier BG" },
    { { 0x29BFBC, 0,0, 1 }, { 0x29BEAE, 0,0, 2 }, 0x3DB799, 0,2, 0,0, "Barbos" },

    { { 0x2C576C, 0x1F60,0, 0 }, { 0x2C4F6C, 0x800,0, 0 }, 0x3D9E39, 0,2, 0,0, "Fire-Ball Frenzy BG1" },
    { { 0x2C576C, 0x1F60,0, 0 }, { 0x2C4F6C, 0x800,0, 0 }, 0x3DA039, 0,2, 0,0, "Blazing Bazukas BG1" },
    { { 0x2C576C, 0x1F60,0, 0 }, { 0x2C4F6C, 0x800,0, 0 }, 0x3D9F39, 0,2, 0,0, "Krack-Shot Kroc BG1" },
    { { 0x29B6C0, 0,0x11C0, 1 }, { 0x29B0BD, 0,0,     2 }, 0x3D9E39, 0,3, 0,0, "Factory BG2" },
    { { 0x29E146, 0x1200,0, 0 }, { 0x29DD86, 0x3E0,0, 0 }, 0x3DA139, 0,2, 0,0, "KAOS" },

    { { 0x2A68F2, 0,0,      1 }, { 0x2A7D8A, 0x800,0, 0 }, 0x3D7F61,64,2, 0,0, "Low-G Labyrinth Smoke" }, /* smoke needs pipe_layout(); */
    { { 0x2A68F2, 0,0,      1 }, { 0x2A7D8A, 0x800,0, 0 }, 0x3D8061,64,2, 0,0, "Poisonous Pipeline Smoke" },
    { { 0x2A6712, 0,0x1D80, 1 }, { 0x2A68CB, 0,0,     2 }, 0x3D8061, 0,3, 0,0, "Poisonous Pipeline BG2" },

    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D94F9, 0,2, 0,0, "Krevice Kreepers BG1" },
    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D91F9, 0,2, 0,0, "Kong-Fused Cliffs BG1" },
    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D93F9, 0,2, 0,0, "Ropey Rumpus BG1" },
    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D95F9, 0,2, 0,0, "Criss Kross Cliffs BG1" },
    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D96F9, 0,2, 0,0, "Rocket Rush BG1" },
    { { 0x2A426A, 0,0, 1 }, { 0x2A5F1C, 0,0, 1 }, 0x3D92F9, 0,2, 0,0, "Cliffs BG1 (AK)" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D94F9, 0,3, 0,0, "Krevice Kreepers BG2" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D91F9, 0,3, 0,0, "Kong-Fused Cliffs BG2" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D93F9, 0,3, 0,0, "Ropey Rumpus BG2" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D95F9, 0,3, 0,0, "Criss Kross Cliffs BG2" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D96F9, 0,3, 0,0, "Rocket Rush BG2" },
    { { 0x2A3387, 0,0, 1 }, { 0x2A5B53, 0,0, 2 }, 0x3D92F9, 0,3, 0,0, "Cliffs BG2 (AK)" },

    { { 0x32587C, 0,0, 1 }, { 0x3275D5, 0,0, 2 }, 0x3DA439, 0,2, 0,0, "Buzzer Barrage BG1" },
    { { 0x32587C, 0,0, 1 }, { 0x3275D5, 0,0, 2 }, 0x3DA639, 0,2, 0,0, "Creepy Caverns BG1" },
    { { 0x32587C, 0,0, 1 }, { 0x3275D5, 0,0, 2 }, 0x3DA539, 0,2, 0,0, "Pot Hole Panic BG1" },
    { { 0x32587C, 0,0, 1 }, { 0x3275D5, 0,0, 2 }, 0x3DA339, 0,2, 0,0, "Tyrant Twin Tussle BG1" },
    { { 0x32587C, 0,0, 1 }, { 0x3275D5, 0,0, 2 }, 0x3DA739, 0,2, 0,0, "Tyrant Twin Tussle BG1 (AK)" },

    { { 0x32275C, 0,0, 1 }, { 0x324170, 0,0, 1 }, 0x3D9C39, 0,2, 0,0, "Konveyor Rope Klash BG2" },
    { { 0x32275C, 0,0, 1 }, { 0x324170, 0,0, 1 }, 0x3D9B39, 0,2, 0,0, "Koindozer Klamber BG2" },
    { { 0x32275C, 0,0, 1 }, { 0x324170, 0,0, 1 }, 0x3D9A39, 0,2, 0,0, "Stampede Sprint BG2" },
    { { 0x32275C, 0,0, 1 }, { 0x324170, 0,0, 1 }, 0x3D9D39, 0,2, 0,0, "Stampede Sprint BG2 (AK)" },
    { { 0x32470A, 0,0, 1 }, { 0x325437, 0,0, 2 }, 0x3D9C39, 0,3, 0,1, "Konveyor Rope Klash BG2" },
    { { 0x32470A, 0,0, 1 }, { 0x325437, 0,0, 2 }, 0x3D9B39, 0,3, 0,1, "Koindozer Klamber BG2" },
    { { 0x32470A, 0,0, 1 }, { 0x325437, 0,0, 2 }, 0x3D9A39, 0,3, 0,1, "Stampede Sprint BG2" },
    { { 0x32470A, 0,0, 1 }, { 0x325437, 0,0, 2 }, 0x3D9D39, 0,3, 0,1, "Stampede Sprint BG2 (AK)" },

    { { 0x29481D, 0,0, 1 }, { 0x2945BC, 0,0, 2}, 0x3D7801,  0,2, 0,0, "Bleak BG1" },
    { { 0x29481D, 0,0, 1 }, { 0x294782, 0,0, 2}, 0x3D7801, 26,2, 0,0, "Bleak BG2" },

    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6A01,  0,2, 0,0, "Bear Cabin (Generic)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6A01, 27,2, 0,0, "Bear Cabin (Bazaar)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6F01, 28,2, 0,0, "Bear Cabin (Barnacle)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D7001, 29,2, 0,0, "Bear Cabin (Brash)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6D01, 30,2, 0,0, "Bear Cabin (Blunder)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D7101, 31,2, 0,0, "Bear Cabin (Blue)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6A01, 32,2, 0,0, "Bear Cabin (Bazooka)" }, // <-- Problem
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6E01, 33,2, 0,0, "Bear Cabin (Bramble)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6C01, 34,2, 0,0, "Bear Cabin (Blizzard)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6A01, 35,2, 0,0, "Bear Cabin (Barter)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D7201, 36,2, 0,0, "Bear Cabin (Benny)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D7201, 37,2, 0,0, "Bear Cabin (Bjorn)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6A01, 38,2, 0,0, "Bear Cabin (Baffle)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D6E01, 39,2, 0,0, "Bear Cabin (Boomer)" },
    { { 0x3B0000, 0,0, 1 }, { 0x29305A, 0,0, 2 }, 0x3D7301, 39,2, 0,0, "Bear Cabin (Boomer) (AK)" },

    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6A01, 41,2, 0,0, "Bear Cabin BG (Bazaar)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6F01, 42,2, 0,0, "Bear Cabin BG (Barnacle)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D7001, 43,2, 0,0, "Bear Cabin BG (Brash)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6D01, 44,2, 0,0, "Bear Cabin BG (Blunder)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D7101, 45,2, 0,0, "Bear Cabin BG (Blue)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6A01, 46,2, 0,0, "Bear Cabin BG (Bazooka)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6E01, 47,2, 0,0, "Bear Cabin BG (Bramble)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6C01, 48,2, 0,0, "Bear Cabin BG (Blizzard)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6A01, 49,2, 0,0, "Bear Cabin BG (Barter)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D7201, 50,2, 0,0, "Bear Cabin BG (Benny)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D7201, 51,2, 0,0, "Bear Cabin BG (Bjorn)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6A01, 52,2, 0,0, "Bear Cabin BG (Baffle)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D6E01, 53,2, 0,0, "Bear Cabin BG (Boomer)" },
    { { 0x3B6897, 0,0x1000, 1 }, { 0x29305A, 0,0, 3}, 0x3D7301, 53,2, 0,0, "Bear Cabin BG (Boomer) (AK)" },

    { { 0x350000, 0,0, 1 }, { 0x320000, 0,0, 1}, 0x3D7421, 55,2, 0,0, "Overworld" },
    { { 0x321DA4, 0,0, 1 }, { 0x3215A9, 0,0, 1}, 0x3D7421,  0,3, 0,0, "Overworld (Water)" },
    { { 0x0FFF7E, 0,0, 1 }, { 0x286F2B, 0,0, 2}, 0x3D7D41,  0,2, 0,0, "Lake Orangatanga" },
    { { 0x10FE01, 0,0, 1 }, { 0x2871D7, 0,0, 2}, 0x3D7D41,  0,3, 0,0, "Lake Orangatanga (Water)" },
    { { 0x0AFDB5, 0,0, 1 }, { 0x286B7F, 0,0, 2}, 0x3D6801,  0,2, 0,0, "Kremwood Forest" },
    { { 0x11FF7D, 0,0, 1 }, { 0x28734F, 0,0, 1}, 0x3D6801,  0,3, 0,0, "Kremwood Forest (Water)" },
    { { 0x04F05F, 0,0, 1 }, { 0x2AFEAB, 0,0, 2}, 0x3D8BE1,  0,2, 0,0, "Cotton-Top Cove" },
    { { 0x36688A, 0,0, 1 }, { 0x3666BB, 0,0, 2}, 0x3D8BE1,  0,3, 0,0, "Cotton-Top Cove (Water)" },
    { { 0x14FFD7, 0,0, 1 }, { 0x2A93BD, 0,0, 2}, 0x3D8CE1,  0,2, 0,0, "Mekanos" },
    { { 0x367029, 0,0, 1 }, { 0x366EE1, 0,0, 1}, 0x3D8CE1,  0,3, 0,0, "Mekanos (Water)" },
    { { 0x06FEFE, 0,0, 1 }, { 0x2867D8, 0,0, 2}, 0x3D6B01,  0,2, 0,0, "K3" },
    { { 0x37266A, 0,0, 1 }, { 0x372353, 0,0, 2}, 0x3D8DE1,  0,2, 0,0, "Razor Ridge" },
    { { 0x3673D5, 0,0, 1 }, { 0x3672D8, 0,0, 2}, 0x3D8DE1,  0,3, 0,0, "Razor Ridge (Water)" },
    { { 0x01FC3C, 0,0, 1 }, { 0x2B017D, 0,0, 2}, 0x3D8EE1,  0,2, 0,0, "KAOS Kore" },
    { { 0x367816, 0,0, 1 }, { 0x3676E3, 0,0, 2}, 0x3D8EE1,  0,3, 0,0, "KAOS Kore (Water)" },
    { { 0x1BFE0E, 0,0, 1 }, { 0x2A9638, 0,0, 2}, 0x3D8FF9,  0,2, 0,0, "Krematoa" },
    { { 0x367B9A, 0,0, 1 }, { 0x367A94, 0,0, 1}, 0x3D8FF9,  0,3, 0,0, "Krematoa (Water)" },
    { { 0x1BFE0E, 0,0, 1 }, { 0x2A9638, 0,0, 2}, 0x3D90F9,  0,2, 0,0, "Krematoa (AK)" },
    { { 0x367B9A, 0,0, 1 }, { 0x367A94, 0,0, 1}, 0x3D90F9,  0,3, 0,0, "Krematoa (Water) (AK)" },
    { { 0x2B0B39, 0,0, 1 }, { 0x2B081C, 0,0, 1 }, 0x3D8FF9, 0,2, 0,0, "Water" },

    { { 0x28BDEC, 0,0, 1 }, { 0x28B5AE, 0,0, 2 }, 0x3D97F9, 0,2, 0,0, "Kastle KAOS (Left)" },
    { { 0x28BDEC, 0,0, 1 }, { 0x28B989, 0,0, 2 }, 0x3D97F9, 0,2, 0,0, "Kastle KAOS (Right)" },
    { { 0x28BDEC, 0,0, 1 }, { 0x28B989, 0,0, 2 }, 0x3D97F9,56,2, 0,0, "Kastle KAOS (Right)(2)" },

    { { 0x29E146, 0x8A0,0, 3 }, { 0x2C451C, 0x240,0x200, 3 }, 0x3D97F9,57,3,48,1, "Kastle KAOS BG3 (1)" },
    { { 0x29E146, 0x8A0,0, 3 }, { 0x2C451C, 0x240,0x200, 3 }, 0x3D97F9,57,3,49,1, "Kastle KAOS BG3 (2)" },

    { { 0x2BDC2A, 0,0,  1 }, { 0x2C3EB2, 0,0,  1 }, 0x3D9939, 0,2, 0,0, "Knautilus (Left)" },
    { { 0x2BDC2A, 0,0,  1 }, { 0x2C3825, 0,0,  1 }, 0x3D9939, 0,2, 0,0, "Knautilus (Right)" },

    { { 0x345F40, 0x1800,0, 0 }, { 0x345CC0, 0,0, 3 }, 0x3D9939, 58,2, 0,1, "Knautilus BG1" },

    { { 0x2BD1D2, 0,0, 1 }, { 0x2BCD52, 0,0, 3 }, 0x3D9939, 59,3,41,0, "Knautilus BG (Left)" },
    { { 0x2BD1D2, 0,0, 1 }, { 0x2BCA52, 0,0, 3 }, 0x3D9939, 60,3,41,0, "Knautilus BG (Right)" }
};
static const int screen_count = sizeof(dkc3) / sizeof(struct DKC3_SCREEN);


static int extend (unsigned char **data, size_t *size, size_t add) {
    unsigned char *z = realloc(*data, *size + add);
    if (z == NULL)
        return 1;
    *data = z;
    *size += add;
    return 0;
}

static int decomp (unsigned char **data, size_t *size, unsigned char *src, size_t src_size, const struct DATA *d) {
    switch (d->type) {
        case 0: { /* raw */
            if (d->size > src_size
            ||  d->size <= 0) {
                fprintf(stderr, "Invalid size for memcpy. ");
                return 1;
            }

            unsigned char *z = calloc(d->size, 1);
            if (z == NULL)
                return 1;
            *data = z;
            memcpy(*data, src, d->size);
            *size = d->size;
            break;
        }
        case 1: { /* big data */
            if (dk_decompress_mem_to_mem(BD_COMP, data, size, src, src_size))
                return 1;
            break;
        }
        case 2: { /* small data */
            if (dk_decompress_mem_to_mem(SD_COMP, data, size, src, src_size))
                return 1;
            break;
        }
        case 3:  { return 0; }
        default: { return 1; }
    }

    /* offset */
    if (d->move > 0) {
        if (extend(data, size, d->move)) {
            fprintf(stderr, "Failed to reallocate a buffer.\n");
            return 1;
        }
        memmove(*data + d->move, *data, *size - d->move);
        *size = *size + d->move;
    }
    else if (d->move < 0) { /* dangerous */
        memmove(*data, *data - d->move, *size + d->move);
        *size = *size - d->move;
    }

    return 0;
}

void spec3 (unsigned char *rom, size_t rom_size, char *dir, int region) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < screen_count; i++) {

        const struct DKC3_SCREEN *d = &dkc3[i];
        unsigned char *set_data = NULL;
        unsigned char *map_data = NULL;

        size_t set_size = 0;
        size_t map_size = 0;

        /* region */
        unsigned char *set = rom + d->set.addr;
        unsigned char *map = rom + d->map.addr;

        if (decomp(&set_data, &set_size, set, rom_size - (set-rom), &d->set))
            goto error;
        if (decomp(&map_data, &map_size, map, rom_size - (map-rom), &d->map))
            goto error;


        /* layout fixes */
        switch (d->spec) {
            case 0: { break; }

            case  1: case  2: case  3:
            case  4: case  5: case  6: { /* boss photos */
                unsigned char *temp = calloc(map_size, 1);
                if (temp == NULL)
                    goto error;
                for (int j = 0; j < 14; j++)
                    memcpy(&temp[0x214 + (j*0x40)], &map_data[(d->spec-1)*0x150 + (j*24)], 24);
                free(map_data);
                map_data = temp;
                break;
            }
            case  7: case  8: case  9:
            case 10: case 11: case 12: { /* bear photos */
                map_size = 0x700;
                unsigned char *temp = calloc(map_size, 1);
                if (temp == NULL)
                    goto error;
                for (int j = 0; j < 7; j++) {
                    memcpy(&temp[0x28C + j*0x40], &map_data[(d->spec-7)*0x8C + (j*10)],        10);
                    memcpy(&temp[0x2AA + j*0x40], &map_data[(d->spec-7)*0x8C + (j*10) + 0x46], 10);
                }
                free(map_data);
                map_data = temp;
                break;
            }
            case 13: { /* arich */
                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                memcpy(&map_data[0x21C], map, 0x8A);
                memcpy(&map_data[0x2D8], map+ 0x8A, 0x1CE);
                break;
            }
            case 14: { /* riverside BG */
                int m = map_size;
                for (int j = 1; j < m; j+=2)
                    map_data[j] -= 1;
                break;
            }

            case 15: case 16: case 17: case 18:
            case 19: case 20: case 21: case 22:
            case 23: case 24: case 25: { /* waterfalls */
                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                waterfall_layout(rom, map_data, d->spec - 15);
                break;
            }

            case 26: { /* bleak */
                unsigned char *t;
                size_t ts;
                if (dk_decompress_mem_to_mem(BD_COMP, &t, &ts, rom+0x370000, rom_size-0x370000))
                    goto error;
                if (extend(&set_data, &set_size, 0x5140 + ts)
                ||  extend(&map_data, &map_size, 0x200)) {
                    free(t);
                    goto error;
                }
                memcpy(&set_data[0x5140], t, ts);
                free(t);
                memmove(&map_data[0x200], map_data, map_size - 0x200);
                map = rom + 0x3625CB;
                for (int j = 0; j < 0x200; j+=2) {
                    map_data[j]   = map[(j^0x20)  ];
                    map_data[j+1] = map[(j^0x20)+1] + 2;
                }
                break;
            }

            case 27: case 28: case 29: case 30:
            case 31: case 32: case 33: case 34:
            case 35: case 36: case 37: case 38:
            case 39: case 40: {
                static const struct BEAR_DATA bears[] = {
                    {  0, 0x0F, 0xFAC7 }, {  0, 0x10, 0xFACA }, {  0, 0x07, 0xFACD },
                    {  0, 0x01, 0xFAD0 }, {  0, 0x04, 0xFAD3 },
                    {  1, 0x91, 0xFB35 }, {  1, 0x92, 0xFB38 }, {  1, 0x04, 0xFB3B },
                    {  2, 0x13, 0xFB4F }, {  2, 0x14, 0xFB52 }, {  2, 0x15, 0xFB55 },
                    {  2, 0x02, 0xFB58 }, {  2, 0x25, 0xFB5B },
                    {  3, 0x16, 0xFAEA }, {  3, 0x17, 0xFAED }, {  3, 0x02, 0xFAF0 },
                    {  4, 0x18, 0xFB6C }, {  4, 0x19, 0xFB6F }, {  4, 0x04, 0xFB72 },
                    {  4, 0x00, 0xFB75 },
                    {  5, 0x82, 0xFB80 }, {  5, 0x1D, 0xFB83 }, {  5, 0x1E, 0xFB86 },
                    {  5, 0x00, 0xFB89 },
                    {  6, 0x80, 0xFB04 }, {  6, 0x23, 0xFB07 }, {  6, 0x04, 0xFB0A },
                    {  7, 0x1A, 0xFB9D }, {  7, 0x1B, 0xFBA0 }, {  7, 0x04, 0xFBA3 },
                    {  7, 0x07, 0xFBA6 }, {  7, 0x01, 0xFBA9 },
                    {  8, 0x1C, 0xFB1B }, {  8, 0x00, 0xFB1E }, {  8, 0x03, 0xFB21 },
                    {  9, 0xA2, 0xFBC3 }, {  9, 0x03, 0xFBC6 }, {  9, 0x03, 0xFBC9 },
                    { 10, 0x22, 0xFBDD }, { 10, 0x02, 0xFBE0 }, { 10, 0x04, 0xFBE3 },
                    { 10, 0x00, 0xFBE6 },
                    { 11, 0x1F, 0xFBFA }, { 11, 0x06, 0xFBFD }, { 11, 0x24, 0xFC00 },
                    { 12, 0x02, 0xFC14 }, { 12, 0x20, 0xFC17 }, { 12, 0x21, 0xFC1A },
                    { 12, 0x26, 0xFC1D }
                };
                static const int bear_count = sizeof(bears) / sizeof(struct BEAR_DATA);
                int n = d->spec - 27;

                if (n ==  1
                ||  n ==  6
                ||  n ==  9
                ||  n == 12)
                    flip_bear_layout(map_data);

                for (int j = 0; j < bear_count; j++) {
                    const struct BEAR_DATA *bear = &bears[j];
                    if (bear->n != n)
                        continue;
                    bear_layout(map_data, rom, bear->a1, bear->a2, 0x350000);
                }
                break;
            }
            case 41: case 42: case 43: case 44:
            case 45: case 46: case 47: case 48:
            case 49: case 50: case 51: case 52:
            case 53: case 54: { /* bear cabin backgrounds */
                static const struct BEAR_DATA bears[] = {
                    {  0, 0x49, 0xFAD6 }, {  0, 0x4D, 0xFAD9 }, {  0, 0x4A, 0xFADC },
                    {  0, 0x4B, 0xFADF }, {  0, 0x4C, 0xFAE2 }, {  0, 0x0E, 0xFAE5 },
                    {  1, 0xCD, 0xFB3E }, {  1, 0xCA, 0xFB41 }, {  1, 0x48, 0xFB44 },
                    {  1, 0x49, 0xFB47 }, {  1, 0x0E, 0xFB4A },
                    {  2, 0x4D, 0xFB5E }, {  2, 0xCB, 0xFB61 }, {  2, 0x4B, 0xFB64 },
                    {  2, 0x0E, 0xFB67 },
                    {  3, 0x4D, 0xFAF3 }, {  3, 0xCC, 0xFAF6 }, {  3, 0xCB, 0xFAF9 },
                    {  3, 0x4A, 0xFAFC }, {  3, 0x0E, 0xFAFF },
                    {  4, 0x4D, 0xFB78 }, {  4, 0x0E, 0xFB7B },
                    {  5, 0x48, 0xFB8C }, {  5, 0x49, 0xFB8F }, {  5, 0x4B, 0xFB92 },
                    {  5, 0x4B, 0xFB95 }, {  5, 0x0E, 0xFB98 },
                    {  6, 0x4B, 0xFB0D }, {  6, 0x4D, 0xFB10 }, {  6, 0xC9, 0xFB13 },
                    {  6, 0x0E, 0xFB16 },
                    {  7, 0xCB, 0xFBAC }, {  7, 0x4D, 0xFBAF }, {  7, 0x4B, 0xFBB2 },
                    {  7, 0x4C, 0xFBB5 }, {  7, 0xCC, 0xFBB8 }, {  7, 0xCB, 0xFBBB },
                    {  7, 0x0E, 0xFBBE },
                    {  8, 0x49, 0xFB24 }, {  8, 0x4A, 0xFB27 }, {  8, 0xCD, 0xFB2A },
                    {  8, 0x4B, 0xFB2D }, {  8, 0x0E, 0xFB30 },
                    {  9, 0xC9, 0xFBCC }, {  9, 0xC8, 0xFBCF }, {  9, 0x4A, 0xFBD2 },
                    {  9, 0x4B, 0xFBD5 }, {  9, 0x0E, 0xFBD8 },
                    { 10, 0xCB, 0xFBE9 }, { 10, 0x4A, 0xFBEC }, { 10, 0x4D, 0xFBEF },
                    { 10, 0x4B, 0xFBF2 }, { 10, 0x0E, 0xFBF5 },
                    { 11, 0x4A, 0xFC03 }, { 11, 0xCD, 0xFC06 }, { 11, 0xC9, 0xFC09 },
                    { 11, 0xC8, 0xFC0C }, { 11, 0x0E, 0xFC0F },
                    { 12, 0xCC, 0xFC20 }, { 12, 0xCB, 0xFC23 }, { 12, 0x0E, 0xFC26 }
                };
                static const int bear_count = sizeof(bears) / sizeof(struct BEAR_DATA);
                int n = d->spec - 41;

                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;

                for (int j = 0; j < bear_count; j++) {
                    const struct BEAR_DATA *bear = &bears[j];
                    if (bear->n != n)
                        continue;
                    bear_layout(map_data, rom, bear->a1, bear->a2, 0x350000);
                }
                break;
            }

            case 55: { /* overworld */
                size_t bp_size = map_size*8*8*4; /* probably wrong */
                unsigned char *bp[] = { malloc(bp_size), malloc(bp_size) };
                if (bp[0] == NULL || bp[1] == NULL) {
                    free(bp[0]); free(bp[1]);
                    goto error;
                }
                decode_bitplane(rom, set_data, map_data, bp[0], d->palette, map_size, set_size, d->mode,  0, d->bg);
                decode_bitplane(rom, set_data, map_data, bp[1], d->palette, map_size, set_size, d->mode, 13, d->bg);
                memcpy(bp[0], bp[1], 90*4*256);
                assemble_screen(bp[0], map_size, 32, dir, d->name);
                free(set_data);
                free(map_data);
                free(bp[0]);
                free(bp[1]);
                continue;
            }
            case 56: { /* Kastle KAOS (R2) */
                int j;
                for (j = 0; j < 10; j++)
                    memcpy(&map_data[0x098 + j*64], &rom[0x28B2EC+j*12], 12);
                for (j = 0; j < 11; j++)
                    memcpy(&map_data[0x318 + j*64], &rom[0x28B364+j*20], 20);
                break;
            }
            case 57: { /* Kastle KAOS BG3 (L/R) */
                set_size = 0x1760 + 0x8A0;
                set_data = calloc(set_size, 1);
                if (set_data == NULL)
                    goto error;
                memcpy(set_data, set, 0x1200);
                switch (region) {
                    case 0: { set = &rom[0x2C4494]; break; }
                    case 1: { set = &rom[0x2C475C]; break; }
                    case 2: { set = &rom[0x2C475B]; break; }
                }
                memcpy(&set_data[0x1760], set, 0x8A0);

                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                memcpy(&map_data[0x200], map, 0x240);
                break;
            }
            case 58: { /* Knautilus BG1 */
                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                memcpy(&map_data[0x2C0], map, 0x1C0);
                memcpy(&map_data[0x440], map, 0x1C0);
                break;
            }
            case 59: { /* Knautilus (Left)*/
                if (extend(&set_data, &set_size, 0x13D0))
                    goto error;
                memmove(&set_data[0x13D0], set_data, set_size-0x13D0);
                memcpy(set_data, &set_data[0x130], 0x10);
                set_size += 0x13D0;

                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                memcpy(&map_data[0x180], map, 0x300);
                memcpy(&map_data[0x5D0], map+0x310, 0xB0);
                break;
            }
            case 60: { /* Knautilus (Right) */
                if (extend(&set_data, &set_size, 0x13D0))
                    goto error;
                memmove(&set_data[0x13D0], set_data, set_size-0x13D0);
                memcpy(set_data, &set_data[0x130], 0x10);
                set_size += 0x13D0;

                map_size = 0x800;
                map_data = calloc(map_size, 1);
                if (map_data == NULL)
                    goto error;
                memcpy(&map_data[0x180], map, 0x300);
                memcpy(&map_data[0x5C0], map+0x6C0, 0xC0);
                break;
            }
            case 61: { /* Murky Mill Lights */
                int m = map_size;
                for (int j = 0; j < m; j+=2) {
                    if (map_data[j] > 0x8D)
                        map_data[j+1] += 1;
                    map_data[j]   += 0x72;
                    map_data[j+1] += 1;
                }
                break;
            }
            case 62: { map_size -= 0x180; break; }
            case 63: { map_size  = 0xC0; break; }
            case 64: {
                for (int j = 1; j < (int)map_size; j+=2)
                    map_data[j] |= 0x20;
                break;
            }
            case 65: {
                memcpy(&map_data[0x200], set_data, 0x300);
                if (extend(&set_data, &set_size, 0x3700))
                    goto error;
                memcpy(&set_data[0x1E00], &rom[0x2A160B], 0x1900);
                break;
            }
        }

        size_t bp_size = map_size*8*8*4; /* probably wrong */
        unsigned char *bp = malloc(bp_size);
        if (bp == NULL)
            goto error;

        decode_bitplane(rom, set_data, map_data, bp, d->palette, map_size, set_size, d->mode, d->fix, d->bg);
        assemble_screen(bp, map_size, 32, dir, d->name);

        free(set_data);
        free(map_data);
        free(bp);
        continue;
error:
        fprintf(stderr, "Failed %d (%s).\n", i, d->name);
        free(set_data);
        free(map_data);
    }

}

