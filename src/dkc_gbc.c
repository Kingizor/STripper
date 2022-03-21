#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "bitplane.h"

struct GBC_LEVEL {
    unsigned short t_width;  // 32x32 tiles per row    
    unsigned char  t_height; // 32x32 tiles per column
    unsigned       bp_addr;  // Address of BP data
    unsigned char  rom_bank; // 32x32 tile bank
    unsigned       map_size; // Normal or extended
    unsigned       map_addr; // Tilemap Address
    unsigned char  vert;     // Horizontal or Vertical?
    unsigned       pal_addr; // Palette Address
    int            width;    // Pixel Width (cropping)
    int            height;   // Pixel Height (cropping)
    char          *name;     // Level Name
};

static const struct GBC_LEVEL dkc[] = {
    {0x083, 0x10, 0x20C000, 0x9C, 1, 0x126DD, 0, 0x200317, 4192,  416, "Jungle Hijinxs"},
    {0x015, 0x10, 0x21DF70, 0xB7, 1, 0x12992, 0, 0x200857,  672,  160, "Jungle Hijinxs Bonus 1"},
    {0x007, 0x10, 0x228000, 0xC9, 1, 0x12BDB, 0, 0x200457,  224,  160, "Jungle Hijinxs Bonus 2"},
    {0x0BB, 0x10, 0x20C000, 0x9C, 1, 0x126FB, 0, 0x200357, 5984,  352, "Ropey Rampage"},
    {0x018, 0x10, 0x20C000, 0x9C, 1, 0x12737, 0, 0x200357,  768,  224, "Ropey Rampage Bonus 1"},
    {0x005, 0x10, 0x228000, 0xC9, 1, 0x12BCC, 0, 0x200497,  160,  160, "Ropey Rampage Bonus 2"},
    {0x096, 0x10, 0x21DF70, 0xB7, 1, 0x12926, 0, 0x200857, 4800,  384, "Reptile Rumble"},
    {0x017, 0x10, 0x21DF70, 0xB7, 1, 0x129A1, 0, 0x200857,  736,  224, "Reptile Rumble Bonus 1"},
    {0x00B, 0x10, 0x21DF70, 0xB7, 1, 0x129B0, 0, 0x200857,  352,  384, "Reptile Rumble Bonus 2"},
    {0x00A, 0x10, 0x21DF70, 0xB7, 1, 0x129BF, 0, 0x200857,  320,  288, "Reptile Rumble Bonus 3"},
    {0x03D, 0x40, 0x2119F0, 0xA5, 1, 0x12782, 1, 0x200D17, 1952, 1216, "Coral Capers"},
    {0x0F7, 0x10, 0x20C000, 0x9C, 1, 0x1270A, 0, 0x200397, 7904,  384, "Barrel Cannon Canyon"},
    {0x016, 0x10, 0x21DF70, 0xB7, 1, 0x129CE, 0, 0x200857,  704,  192, "Barrel Cannon Canyon Bonus 1"},
    {0x007, 0x10, 0x20C000, 0x9C, 1, 0x12746, 0, 0x200397,  224,  352, "Barrel Cannon Canyon Bonus 2"}, // 14

    {0x0C8, 0x10, 0x220000, 0xBD, 1, 0x12AA3, 0, 0x200717,  6400, 448, "Winky's Walkway"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12ADF, 0, 0x200717,   192, 192, "Winky's Walkway Bonus"},
    {0x1DB, 0x10, 0x209ED0, 0x99, 2, 0x126C8, 0, 0x200697, 15200, 512, "Mine Cart Carnage"},
    {0x102, 0x10, 0x21DF70, 0xB7, 2, 0x12935, 0, 0x200897,  8256, 512, "Bouncy Bonanza"},
    {0x005, 0x10, 0x225E10, 0xC3, 1, 0x12C53, 0, 0x200997,   160, 144, "Bouncy Bonanza Bonus 1"},
    {0x007, 0x10, 0x21DF70, 0xB7, 1, 0x129EC, 0, 0x200897,   224, 192, "Bouncy Bonanza Bonus 2"},
    {0x005, 0x10, 0x21DF70, 0xB7, 1, 0x129DD, 0, 0x200897,   160, 320, "Bouncy Bonanza Bonus 3"},
    {0x0D5, 0x10, 0x224000, 0xC0, 1, 0x12AFD, 0, 0x200617,  6816, 320, "Stop & Go Station (Green)"},
    {0x0D5, 0x10, 0x224000, 0xC0, 1, 0x12AFD, 0, 0x200657,  6816, 320, "Stop & Go Station (Red)"},
    {0x009, 0x10, 0x224000, 0xC0, 1, 0x12B63, 0, 0x200617,   288, 320, "Stop & Go Station Bonus 1 (Green)"},
    {0x009, 0x10, 0x224000, 0xC0, 1, 0x12B63, 0, 0x200657,   288, 320, "Stop & Go Station Bonus 1 (Red)"},
    {0x005, 0x10, 0x224000, 0xC0, 1, 0x12B72, 0, 0x200617,   160, 320, "Stop & Go Station Bonus 2 (Green)"},
    {0x005, 0x10, 0x224000, 0xC0, 1, 0x12B72, 0, 0x200657,   160, 320, "Stop & Go Station Bonus 2 (Red)"},
    {0x0FF, 0x10, 0x204000, 0x90, 1, 0x125CC, 0, 0x200F97,  8160, 384, "Millstone Mayhem"},
    {0x005, 0x10, 0x204000, 0x90, 1, 0x125F0, 0, 0x200F97,   160, 320, "Millstone Mayhem Bonus 1"},
    {0x007, 0x10, 0x204000, 0x90, 1, 0x125FF, 0, 0x200F97,   224, 160, "Millstone Mayhem Bonus 2"},
    {0x006, 0x10, 0x204000, 0x90, 1, 0x1261D, 0, 0x200F97,   192, 160, "Millstone Mayhem Bonus 3"}, // 17

    {0x0FB, 0x10, 0x215FC0, 0xAB, 1, 0x12866, 0, 0x200C97, 8032,  320, "Vulture Culture"},
    {0x007, 0x10, 0x215FC0, 0xAB, 1, 0x12884, 0, 0x200C97,  224,  160, "Vulture Culture Bonuses 1 & 2"},
    {0x016, 0x10, 0x215FC0, 0xAB, 1, 0x128A2, 0, 0x200C97,  704,  192, "Vulture Culture Bonus 3"},
    {0x0F6, 0x10, 0x206000, 0x93, 1, 0x1262C, 0, 0x200517, 7872,  352, "Tree Top Town"},
    {0x006, 0x10, 0x225E10, 0xC3, 1, 0x12C71, 0, 0x2006D7,  192,  160, "Tree Top Town Bonus 1"},
    {0x005, 0x10, 0x206000, 0x93, 1, 0x12668, 0, 0x200517,  160,  256, "Tree Top Town Bonus 2"},
    {0x0FF, 0x10, 0x215FC0, 0xAB, 1, 0x12875, 0, 0x200CD7, 8160,  320, "Forest Frenzy"},
    {0x006, 0x10, 0x215FC0, 0xAB, 1, 0x12893, 0, 0x200CD7,  192,  160, "Forest Frenzy Bonus 1"},
    {0x00B, 0x10, 0x215FC0, 0xAB, 1, 0x128B1, 0, 0x200CD7,  352,  288, "Forest Frenzy Bonus 2"},
    {0x130, 0x10, 0x204000, 0x90, 2, 0x125DB, 0, 0x200FD7, 9728,  352, "Temple Tempest"},
    {0x019, 0x10, 0x204000, 0x90, 1, 0x1260E, 0, 0x200FD7,  800,  160, "Temple Tempest Bonus 1"},
    {0x007, 0x10, 0x204000, 0x90, 1, 0x125FF, 0, 0x200FD7,  224,  160, "Temple Tempest Bonus 2"},
    {0x0D0, 0x10, 0x20C000, 0x9C, 1, 0x126EC, 0, 0x2003D7, 6656,  416, "Orang-utan Gang"},
    {0x007, 0x10, 0x20C000, 0x9C, 1, 0x12728, 0, 0x2003D7,  224,  192, "Orang-utan Gang Bonus 1"},
    {0x007, 0x10, 0x228000, 0xC9, 1, 0x12C26, 0, 0x2003D7,  224,  192, "Orang-utan Gang Bonus 2"},
    {0x007, 0x10, 0x228000, 0xC9, 1, 0x12C17, 0, 0x2003D7,  224,  192, "Orang-utan Gang Bonus 3"},
    {0x007, 0x10, 0x228000, 0xC9, 1, 0x12C08, 0, 0x2003D7,  224,  192, "Orang-utan Gang Bonus 4"},
    {0x016, 0x10, 0x20C000, 0x9C, 1, 0x12755, 0, 0x2003D7,  704,  192, "Orang-utan Gang Bonus 5"},
    {0x038, 0x40, 0x2119F0, 0xA5, 1, 0x12791, 1, 0x200D57, 1792, 1408, "Clam City"}, // 19

    {0x148, 0x10, 0x219460, 0xB1, 2, 0x128C0, 0, 0x200E57, 10496,  480, "Snow Barrel Blast"},
    {0x006, 0x10, 0x221B00, 0xC6, 1, 0x12B9F, 0, 0x200F17,   192,  160, "Snow Barrel Blast Bonus 1"},
    {0x005, 0x10, 0x219460, 0xB1, 1, 0x128E4, 0, 0x200E57,   160,  320, "Snow Barrel Blast Bonus 2"},
    {0x016, 0x10, 0x219460, 0xB1, 1, 0x128F3, 0, 0x200E57,   704,  160, "Snow Barrel Blast Bonus 3"},
    {0x033, 0x44, 0x208000, 0x96, 2, 0x12677, 1, 0x200A57,  1632, 2176, "Slipslide Ride"}, // 44
    {0x006, 0x10, 0x208000, 0x96, 1, 0x1269B, 0, 0x200AD7,   192,  160, "Slipslide Ride Bonus 1"},
    {0x005, 0x10, 0x208000, 0x96, 1, 0x1268C, 0, 0x200A97,   160,  160, "Slipslide Ride Bonus 2"},
    {0x011, 0x10, 0x208000, 0x96, 1, 0x126AA, 0, 0x200A57,   544,  224, "Slipslide Ride Bonus 3"},
    {0x0E2, 0x10, 0x219460, 0xB1, 1, 0x128D5, 0, 0x200E97,  7232,  512, "Ice Age Alley"},
    {0x00B, 0x10, 0x219460, 0xB1, 1, 0x12902, 0, 0x200E97,   352,  320, "Ice Age Alley Bonus 1"},
    {0x006, 0x10, 0x221B00, 0xC6, 1, 0x12BAE, 0, 0x200F57,   192,  160, "Ice Age Alley Bonus 2"},
    {0x035, 0x59, 0x2119F0, 0xA5, 2, 0x127A0, 1, 0x200D97,  1696, 2848, "Croctopus Chase"}, // 59
    {0x082, 0x10, 0x21DF70, 0xB7, 1, 0x1294A, 0, 0x200A17,  4160,  384, "Torchlight Trouble (Dark)"},
    {0x082, 0x10, 0x21DF70, 0xB7, 1, 0x1294A, 0, 0x2008D7,  4160,  384, "Torchlight Trouble (Light)"},
    {0x005, 0x10, 0x225E10, 0xC3, 1, 0x12C53, 0, 0x200997,   160,  144, "Torchlight Trouble Bonus 1"},
    {0x007, 0x10, 0x21DF70, 0xB7, 1, 0x12A19, 0, 0x200A17,   224,  192, "Torchlight Trouble Bonus 2 (Dark)"},
    {0x007, 0x10, 0x21DF70, 0xB7, 1, 0x12A19, 0, 0x2008D7,   224,  192, "Torchlight Trouble Bonus 2 (Light)"},
    {0x0C0, 0x10, 0x206000, 0x93, 1, 0x1263B, 0, 0x200557,  6144,  416, "Rope Bridge Rumble"},
    {0x019, 0x10, 0x206000, 0x93, 1, 0x1264A, 0, 0x200557,   800,  192, "Rope Bridge Rumble Bonus 1"},
    {0x006, 0x10, 0x225E10, 0xC3, 1, 0x12C80, 0, 0x2006D7,   192,  160, "Rope Bridge Rumble Bonus 2"}, // 20

    {0x0FF, 0x10, 0x214000, 0xA8, 1, 0x127D9, 0, 0x200B17,  8160,  416, "Oil Drum Alley"},
    {0x005, 0x10, 0x214000, 0xA8, 1, 0x1281B, 0, 0x200B17,   160,  320, "Oil Drum Alley Bonus 1"},
    {0x008, 0x10, 0x228000, 0xC9, 1, 0x12BF9, 0, 0x200C17,   256,  160, "Oil Drum Alley Bonus 2"},
    {0x005, 0x10, 0x214000, 0xA8, 1, 0x1282A, 0, 0x200B17,   160,  256, "Oil Drum Alley Bonus 3"},
    {0x01D, 0x10, 0x214000, 0xA8, 1, 0x1280C, 0, 0x200B17,   928,  160, "Oil Drum Alley Bonus 4"},
    {0x17A, 0x10, 0x220000, 0xBD, 2, 0x12A55, 0, 0x200757, 12096,  448, "Trick Track Trek"},
    {0x00A, 0x10, 0x220000, 0xBD, 1, 0x12AB2, 0, 0x200757,   320,  320, "Trick Track Trek Bonus 1"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12AEE, 0, 0x200757,   192,  192, "Trick Track Trek Bonus 2"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12A94, 0, 0x200757,   192,  160, "Trick Track Trek Bonus 3"},
    {0x128, 0x10, 0x21DF70, 0xB7, 2, 0x1296E, 0, 0x200917,  9472,  512, "Elevator Antics"},
    {0x005, 0x10, 0x225E10, 0xC3, 1, 0x12C44, 0, 0x200997,   160,  144, "Elevator Antics Bonus 1"},
    {0x016, 0x10, 0x21DF70, 0xB7, 1, 0x12A28, 0, 0x200917,   704,  192, "Elevator Antics Bonus 2"},
    {0x005, 0x10, 0x21DF70, 0xB7, 1, 0x129FB, 0, 0x200917,   160,  320, "Elevator Antics Bonus 3"},
    {0x03C, 0x57, 0x2119F0, 0xA5, 2, 0x127B5, 1, 0x200DD7,  1920, 2784, "Poison Pond"}, // 57
    {0x168, 0x10, 0x21C000, 0xB4, 2, 0x12911, 0, 0x200797, 11520,  512, "Mine Cart Madness"},
    {0x016, 0x10, 0x21DF70, 0xB7, 1, 0x12A46, 0, 0x200857,   704,  192, "Mine Cart Madness Bonus 1"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12A94, 0, 0x200757,   192,  160, "Mine Cart Madness Bonus 2"},
    {0x00A, 0x10, 0x220000, 0xBD, 1, 0x12AC1, 0, 0x200797,   320,  224, "Mine Cart Madness Bonus 3"},
    {0x11D, 0x10, 0x214000, 0xA8, 2, 0x127E8, 0, 0x200B57,  9120,  480, "Blackout Basement"},
    {0x00B, 0x10, 0x214000, 0xA8, 1, 0x127FD, 0, 0x200B57,   352,  320, "Blackout Basement Bonus 1"},
    {0x007, 0x10, 0x228000, 0xC9, 1, 0x12BEA, 0, 0x200C57,   224,  160, "Blackout Basement Bonus 2"}, // 21

    {0x128, 0x10, 0x220000, 0xBD, 2, 0x12A7F, 0, 0x2007D7, 9472,  448, "Tanked Up Trouble"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12AD0, 0, 0x2007D7,  192,  160, "Tanked Up Trouble Bonus"},
    {0x108, 0x10, 0x21DF70, 0xB7, 2, 0x12959, 0, 0x200957, 8448,  512, "Manic Mincers"},
    {0x007, 0x10, 0x21DF70, 0xB7, 1, 0x12A37, 0, 0x200957,  224,  192, "Manic Mincers Bonus 1"},
    {0x00B, 0x10, 0x21DF70, 0xB7, 1, 0x12A0A, 0, 0x200957,  352,  160, "Manic Mincers Bonus 2"},
    {0x005, 0x10, 0x225E10, 0xC3, 1, 0x12C44, 0, 0x200997,  160,  144, "Manic Mincers Bonus 3"},
    {0x12B, 0x10, 0x224000, 0xC0, 2, 0x12B0C, 0, 0x2005D7, 9568,  448, "Misty Mine"},
    {0x025, 0x10, 0x224000, 0xC0, 1, 0x12B36, 0, 0x2005D7, 1184,  320, "Misty Mine Bonus 1"},
    {0x006, 0x10, 0x224000, 0xC0, 1, 0x12B45, 0, 0x2005D7,  192,  192, "Misty Mine Bonus 2"},
    {0x0F8, 0x10, 0x21DF70, 0xB7, 1, 0x12983, 0, 0x200857, 7936,  480, "Necky Nutmare"},
    {0x007, 0x10, 0x21DF70, 0xB7, 1, 0x12A37, 0, 0x200957,  224,  192, "Necky Nutmare Bonus"},
    {0x120, 0x10, 0x224000, 0xC0, 2, 0x12B21, 0, 0x2005D7, 9216,  512, "Loopy Lights"},
    {0x016, 0x10, 0x224000, 0xC0, 1, 0x12B54, 0, 0x2005D7,  704,  320, "Loopy Lights Bonus 1"},
    {0x01F, 0x10, 0x224000, 0xC0, 1, 0x12B81, 0, 0x2005D7,  992,  512, "Loopy Lights Bonus 2"},
    {0x11B, 0x10, 0x220000, 0xBD, 2, 0x12A6A, 0, 0x200817, 9056,  512, "Platform Perils"},
    {0x006, 0x10, 0x220000, 0xBD, 1, 0x12AD0, 0, 0x200817,  192,  160, "Platform Perils Bonuses 1 & 2"}, // 16

    {0x033, 0x10, 0x221B00, 0xC6, 1, 0x12B90, 0, 0x200ED7, 1632,  512, "Rambi Bonus Room"},
    {0x063, 0x10, 0x228000, 0xC9, 1, 0x12BBD, 0, 0x200417, 3168,  416, "Expresso Bonus Room"},
    {0x042, 0x10, 0x225E10, 0xC3, 1, 0x12C35, 0, 0x200997, 2112,  384, "Winky Bonus Room"},
    {0x025, 0x40, 0x2119F0, 0xA5, 1, 0x127CA, 1, 0x200E17, 1184, 1184, "Enguarde Bonus Room"},
    {0x007, 0x10, 0x20DFF0, 0x9F, 1, 0x12764, 0, 0x200257,  224,  160, "Boss Room (Small)"},
    {0x00C, 0x10, 0x20DFF0, 0x9F, 1, 0x12773, 0, 0x200257,  384,  160, "Boss Room (Large)"},
    {0x00C, 0x10, 0x210000, 0xA2, 1, 0x12C9E, 0, 0x200297,  384,  160, "Gang-Plank Galleon"},
    {0x005, 0x10, 0x218000, 0xAE, 1, 0x12C8F, 0, 0x2002D7,  160,  160, "DK's Treehouse"},
    {0x005, 0x10, 0x225E10, 0xC3, 1, 0x12C62, 0, 0x200997,  160,  160, "Banana Hoard (Empty)"},
    {0x005, 0x10, 0x229FF0, 0xBA, 1, 0x12CAD, 0, 0x2009D7,  160,  160, "Banana Hoard (Full)"},
    {0x010, 0x10, 0x208000, 0x96, 1, 0x126B9, 0, 0x200A57,  512,  352, "Candy's Challenge - Kremkroc Industries Inc"},
    {0x016, 0x40, 0x214000, 0xA8, 1, 0x12857, 1, 0x200BD7,  704,  640, "Candy's Challenge - Chimp Caverns"} // 12
};
static const int screen_count = sizeof(dkc) / sizeof(struct GBC_LEVEL);


static void gbc_layout (unsigned char *rom, unsigned char *raw_data, unsigned char *lay_data, unsigned char *col_data, int bank, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int val = raw_data[(i*width)+j];
            int alt = raw_data[(i*width)+j+(height*width)];
            int ofs = ((alt - (alt % 4)) * 0x1000) + (((alt % 4) * 0x100));
            for (int k = 0; k < 4; k++) {
                for (int m = 0; m < 4; m++) {
                    int addr = (i*width*16)+(j*4)+(k*width*4)+m;
                    int tile = bank+val+(k*0x1000)+(m*0x400) + ofs;
                    lay_data[addr] = rom[tile];
                    tile++;
                    col_data[addr] = rom[tile];
                }
            }
        }
    }
} // gbc_layout();

int tidy_gbc (unsigned char **map_data, size_t *rawlen, int width, int px_height, int vert) {

    unsigned char *raw_data = *map_data;
    int div = (vert) ? 0x40 : 0x100;
    int i;

    int length = 0x2000 / div;

    unsigned char *new_data = calloc(0x6000, 1);
    if (new_data == NULL)
        return 1;

    if ((vert == 0 &&     width <= div)
    ||  (vert == 1 && px_height <= 2048)) { // Normal Layout
        // printf("Normal Mode\n");
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], width);
        }
    }
    else if (!vert) { // Extended Layout for horizontal levels.
        int shortwidth = (width-div);
        // printf("Extended Mode H\n");
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], div);
            memcpy(&new_data[((i+1) * width) - shortwidth], &raw_data[0x4000 + (i * div)], shortwidth);
        }
    }
    else { // Extended Layout for vertical levels.
        // printf("Extended Mode V\n");
        memcpy(&new_data[0], &raw_data[0], 0x6000);
        memcpy(&raw_data[0x1000], &new_data[0x4000], 0x1000);
        memcpy(&raw_data[(px_height/32)*0x40], &new_data[0x1000], 0x1000);
        memcpy(&raw_data[((px_height/32)*0x40)+0x1000], &new_data[0x5000], 0x1000);
        length = px_height/16;
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], width);
        }
    }

    free(raw_data);
    *map_data = new_data;
    *rawlen = width * length;

    return 0;

} // tidy_gbc();



void gbc_levels(unsigned char *rom, size_t rom_size, char *dir) {

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < screen_count; i++) { // 119

        const struct GBC_LEVEL *d = &dkc[i];
        unsigned char *lay_data = NULL;
        unsigned char *col_data = NULL;
        unsigned char *bitplane = malloc(d->t_width * d->t_height * 1024 * 4);
        unsigned char  rgb[384];
        unsigned char *map_data = NULL;
        unsigned char *map_pos  = NULL;
        size_t map_size = 0;
        int block_count = 3*d->map_size;
        struct DKCGBC_MAP {
            unsigned char *data;
            size_t size;
        } mapd[7];

        memset(&mapd, 0, sizeof(mapd));

        if (bitplane == NULL)
            goto error;

        /* decompress each block */
        for (int j = 0; j < block_count; j++) {
            unsigned char *r = &rom[d->map_addr];
            unsigned addr =   (r[0] << 14)
                          | (((r[2+2*j]) & ~0xC0) << 8)
                          |    r[1+2*j];
            if (dk_decompress_mem_to_mem(DKCGBC_DECOMP, &mapd[j].data, &mapd[j].size, rom+addr, rom_size-addr))
                goto error;
            map_size += 4096;
        }

        /* big levels have an empty block in the middle, apparently */
        if (d->map_size == 2) {
            block_count++;
            mapd[6] = mapd[5];
            mapd[5] = mapd[4];
            mapd[4] = mapd[3];
            mapd[3].data = calloc(mapd[3].size, 1);
            if (mapd[3].data == NULL)
                goto error;
            map_size += mapd[3].size;
        }

        /* concatenate */
        map_data = calloc(map_size, 1);
        if (map_data == NULL)
            goto error;
        map_pos = map_data;
        for (int j = 0; j < block_count; j++) {
            struct DKCGBC_MAP *md = &mapd[j];
            memcpy(map_pos, md->data, md->size);
            free(md->data);
            md->data = NULL;
            map_pos += (j == block_count-1) ? md->size : 4096;
        }

        if (tidy_gbc(&map_data, &map_size, d->t_width, d->height, d->vert))
            goto error;

        lay_data = malloc(map_size*8); /* Guessing */
        col_data = malloc(map_size*8);
        if (lay_data == NULL
        ||  col_data == NULL)
            goto error;

        gbc_layout(rom, map_data, lay_data, col_data, d->rom_bank*0x4000, d->t_width, d->t_height);
        decode_palette(rgb, &rom[d->pal_addr], 128);

        gbc_assemble(bitplane, &rom[d->bp_addr], lay_data, col_data, rgb, d->t_width, d->t_height, 1);

        arrange_gbc(bitplane, d->width, d->height, dir, d->name);

cleanup:
        for (int j = 0; j < block_count; j++)
            free(mapd[j].data);
        free(bitplane);
        free(map_data);
        free(lay_data);
        free(col_data);
        continue;
error:
        printf("Error: (%d)\n", i);
        goto cleanup;
    }
}
