#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gba_misc.h"
#include "bitplane.h"

struct kos_levels {
    uint32_t palette;
    uint32_t tileset;
    uint32_t tilemap;
    char *name;
};

static int crop_kos(uint8_t *layout, uint32_t *tilemap_size, int *width, int height) {

    /*
    Simple cropping.

    Each area is always 128 tiles wide, so start at right of a row and determine the rightmost tile that isn't blank.
    Do this for each row and crop the frame along the rightmost tile.
    */

    *width = 0;

    for (int i = 0; i < height; i++) {
        int row = i * 128;
        for (int j = 127; j >= 0; j--) {
            int rpos = (row + j) * 2;
            if (layout[rpos] != 0 && layout[rpos + 1] != 0) {
                if (j > *width) {
                    *width = j;
                }
                break;
            }
        }
    }
    (*width)++;

    *tilemap_size = *width * height * 2;
    uint8_t *new_layout = malloc(*tilemap_size);

    if (new_layout == NULL) {
        printf("Failed to allocate memory for cropping.\n");
        return -1;
    }
    for (int i = 0; i < height; i++) {
        memcpy(&new_layout[*width * i * 2], &layout[i * 128 * 2], *width * 2);
    }
    memcpy(layout, new_layout, *tilemap_size);
    free(new_layout);

    return 0;

}

void kos_levels(uint8_t *rom, char *dir) {

    struct kos_levels levels[] = {

        {0x509EC0, 0x240758, 0x013644, "Banana Bungalow 1"},
        {0x50A080, 0x240758, 0x017684, "Banana Bungalow 2"},
        {0x508F00, 0x240758, 0x0064B4, "Tropical Treetops 1"},
        {0x5090C0, 0x240758, 0x007CE4, "Tropical Treetops 2"},
        {0x50A5C0, 0x240758, 0x021F34, "Tropical Treetops B"},
        {0x509280, 0x240758, 0x008D14, "Tropical Treetops 3"},
        {0x509980, 0x240758, 0x00E5C4, "Contraption Cave 1"},
        {0x509B40, 0x240758, 0x00FDF4, "Contraption Cave 2"},
        {0x50A780, 0x240758, 0x022F64, "Contraption Cave B"},
        {0x509D00, 0x240758, 0x011624, "Contraption Cave 3"},
        {0x509440, 0x240758, 0x00AD54, "Puzzling Pyramid 1"},
        {0x509600, 0x240758, 0x00C584, "Puzzling Pyramid 2"},
        {0x5097C0, 0x240758, 0x00D5A4, "Puzzling Pyramid 3"},
        {0x50A940, 0x240758, 0x023F84, "Congazuma's Castle"},

        {0x50AB00, 0x240758, 0x024F94, "Necky's Canyon 1"},
        {0x50ACC0, 0x240758, 0x0257A4, "Necky's Canyon 2"},
        {0x50AE80, 0x240758, 0x026FD4, "Necky's Canyon 3"},
        {0x50B040, 0x240758, 0x028804, "Cactus Woods 1"},
        {0x50C1C0, 0x24405C, 0x0359D4, "Cactus Woods B"},
        {0x50B200, 0x240758, 0x02A034, "Cactus Woods 2"},
        {0x50B3C0, 0x240758, 0x02B844, "Cactus Woods 3"},
        {0x50B580, 0x24405C, 0x02D074, "Madcap Mine 1"},
        {0x50B740, 0x24405C, 0x02F094, "Madcap Mine 2"},
        {0x50B900, 0x24405C, 0x0300B4, "Madcap Mine 3"},
        {0x50C380, 0x24405C, 0x0379F4, "Madcap Mine B"},
        {0x50BAC0, 0x24405C, 0x0318F4, "Treacherous Twister 1"},
        {0x50BC80, 0x24405C, 0x032934, "Treacherous Twister 2"},
        {0x50C540, 0x24405C, 0x03A244, "Fire Necky's Nest"},

        {0x50C700, 0x24405C, 0x03B264, "Risky Reef 1"},
        {0x50DDC0, 0x247EE0, 0x0544B4, "Risky Reef B"},
        {0x50C8C0, 0x24405C, 0x03CAA4, "Risky Reef 2"},
        {0x50CA80, 0x24405C, 0x03E2D4, "Risky Reef 3"},
        {0x50CC40, 0x24405C, 0x042314, "Lockjaw Falls 1"},
        {0x50CE00, 0x24405C, 0x043B44, "Lockjaw Falls 2"},
        {0x50CFC0, 0x24405C, 0x045374, "Lockjaw Falls 3"},
        {0x50D6C0, 0x24405C, 0x04CC44, "Ship of Souls 1"},
        {0x50D880, 0x247EE0, 0x04EC64, "Ship of Souls 2"},
        {0x50DA40, 0x247EE0, 0x051C84, "Ship of Souls 3"},
        {0x50D180, 0x24405C, 0x0473B4, "Kremling Kamp 1"},
        {0x50DF80, 0x247EE0, 0x0554D4, "Kremling Kamp B"},
        {0x50D340, 0x24405C, 0x0493E4, "Kremling Kamp 2"},
        {0x50D500, 0x24405C, 0x04AC14, "Kremling Kamp 3"},
        {0x50E140, 0x247EE0, 0x057D04, "Davy Bones Locker"},

        {0x50E300, 0x247EE0, 0x058514, "Cold Cold Forest 1"},
        {0x50E4C0, 0x247EE0, 0x059D44, "Cold Cold Forest 2"},
        {0x50E680, 0x247EE0, 0x05BD64, "Cold Cold Forest 3"},
        {0x50FB80, 0x24F728, 0x0767C4, "Cold Cold Forest B"},
        {0x50E840, 0x247EE0, 0x05FDA4, "Raging Ravine 1"},
        {0x50EA00, 0x247EE0, 0x061DE4, "Raging Ravine 2"},
        {0x50EBC0, 0x24BAE4, 0x063E24, "Raging Ravine 3"},
        {0x50ED80, 0x24BAE4, 0x065E64, "Underwater Ruins 1"},
        {0x50EF40, 0x24BAE4, 0x067E84, "Underwater Ruins 2"},
        {0x50F100, 0x24BAE4, 0x069EC4, "Underwater Ruins 3"},
        {0x50F2C0, 0x24F728, 0x06CEE4, "Ice Castle 1"},
        {0x50F9C0, 0x24F728, 0x074794, "Ice Castle B"},
        {0x50F480, 0x24F728, 0x06E724, "Ice Castle 2"},
        {0x50F640, 0x24F728, 0x06FF44, "Ice Castle 3"},
        {0x50FD40, 0x24F728, 0x0787E4, "Sassy Squatch's Lair"},

        {0x50FF00, 0x24F728, 0x07F8D4, "Booster Barrel Skyway"},
        {0x5100C0, 0x24F728, 0x083144, "K.Kruizer III Artillery 1"},
        {0x510280, 0x24F728, 0x084974, "K.Kruizer III Artillery 2"},
        {0x510440, 0x2536CC, 0x087194, "K.Kruizer III Hull 1"},
        {0x5115C0, 0x2536CC, 0x0A6424, "K.Kruizer III Hull B"},
        {0x510600, 0x2536CC, 0x0891B4, "K.Kruizer III Hull 2"},
        {0x5107C0, 0x2536CC, 0x08D1F4, "K.Kruizer III Hull 3"},
        {0x510B40, 0x2536CC, 0x092254, "K.Kruizer III Engine 1"},
        {0x510D00, 0x2536CC, 0x096294, "K.Kruizer III Engine 2"},
        {0x511CC0, 0x2536CC, 0x0A9C94, "King K.Rool 1"},
        {0x512900, 0x255D70, 0x0ACD14, "King K.Rool 2"},
        {0x512200, 0x2536CC, 0x0AB4C4, "King K.Rool 3"},
        {0x512AC0, 0x255D70, 0x0AF564, "King K.Rool 4"},
        {0x5123C0, 0x2536CC, 0x0ABCD4, "King K.Rool 5"},
        {0x512740, 0x255D70, 0x0AC504, "King K.Rool 6"},
        // {0x512580, 0x2536CC, 0x0AC4F4, "Credits"},

        {0x511B00, 0x2536CC, 0x0A9474, "Jungle Jam"},
        // {0x512C80, 0x2536CC, 0x0A9474, "Jungle Jam 2"},
        // {0x507300, 0x240758, 0x0A9474, "Cranky 0"},
        {0x5074C0, 0x240758, 0x001BF4, "Cranky 1"},
        {0x507680, 0x240758, 0x002404, "Cranky 2"},
        {0x507840, 0x240758, 0x002C24, "Cranky 3"},
        {0x507A00, 0x240758, 0x003444, "Cranky 4"},
        {0x507BC0, 0x240758, 0x003C54, "Cranky 5"},
        {0x507D80, 0x240758, 0x004464, "Cranky 6"},
        {0x507F40, 0x240758, 0x004C74, "Cranky 7"},

        {0x513700, 0x259D34, 0x0B4604, "Climbing Race 1"},
        {0x5138C0, 0x259D34, 0x0B6E54, "Climbing Race 2"},
        {0x513A80, 0x259D34, 0x0B96A4, "Climbing Race 3"},
        {0x513C40, 0x259D34, 0x0BBEF4, "Climbing Race 4"},
        {0x513E00, 0x259D34, 0x0BE744, "Climbing Race 5 (Unused)"},
        {0x513FC0, 0x259D34, 0x0C0F94, "Obstacle Race 1"},
        {0x514180, 0x259D34, 0x0C37E4, "Obstacle Race 2"},
        {0x514340, 0x259D34, 0x0C6034, "Obstacle Race 3"},
        {0x514500, 0x259D34, 0x0C8884, "Obstacle Race 4 (Unused)"},
        {0x5146C0, 0x259D34, 0x0CB0D4, "Attack Battle 1"},
        {0x514880, 0x259D34, 0x0CC0F4, "Attack Battle 2"},
        {0x514A40, 0x259D34, 0x0CD104, "Attack Battle 3 (Unused)"},
        {0x514C00, 0x259D34, 0x0CE114, "Barrel Blast 1"},
        {0x514DC0, 0x259D34, 0x0CF134, "Barrel Blast 2"},
        {0x514F80, 0x259D34, 0x0D1154, "Color Capture"},

        {0x510980, 0x2536CC, 0x090234, "Unused 1"},
        {0x510EC0, 0x2536CC, 0x09A2D4, "Unused 2"},
        {0x511400, 0x2536CC, 0x09E324, "Unused 3"},
        {0x511780, 0x2536CC, 0x0A8454, "Unused 4"},
        {0x515140, 0x259D34, 0x0D1964, "Unused 5"},
    };

    uint32_t length = sizeof(levels) / sizeof(struct kos_levels);

    #pragma omp parallel for schedule(dynamic)
    for (uint32_t i = 0; i < length; i++) {
        uint32_t index = levels[i].tilemap;
        uint32_t tilemap_counter = 0;

        // Heuristically determine the height by checking for pointers. (Not guaranteed to be reliable)
        while (rom[index+3] == 8) {
            tilemap_counter += 4;
            index += 16;
        }

        index = levels[i].tilemap;
        uint32_t tilemaps[tilemap_counter];

        // Load pointers
        for (uint32_t j = 0; j < tilemap_counter; j++) {
            tilemaps[j] = rom[index] + (rom[index+1] << 8) + (rom[index+2] << 16);
            index += 4;
        }

        // A single nametable is 32x32, each tile is 2 bytes.
        // 32 * 32 * 2 = 2048 (0x800)
        uint32_t tilemap_size = tilemap_counter * 0x800;
        uint8_t *layout = malloc(tilemap_size);
        index = 0; // Write index
        int width = 0, height = (tilemap_counter / 4) * 32;

        if (layout == NULL) {
            printf("Failed to allocate memory for layout.\n");
            continue;
        }



        // Each row of nametables
        for (uint32_t j = 0; j < (tilemap_counter / 4); j++) {
            // Each row of tiles
            for (uint8_t k = 0; k < 32; k++) {
                // Particular row within nametable
                for (uint8_t m = 0; m < 4; m++) {
                    memcpy(&layout[index], &rom[tilemaps[(j*4)+m]+(k*0x40)], 0x40);
                    index += 0x40;
                }
            }
        }

        if (crop_kos(layout, &tilemap_size, &width, height)) {
            continue;
        }

        uint8_t *att_data = malloc(tilemap_size/2);

        if (att_data == NULL) {
            printf("Failed to allocate memory for layout data.\n");
            continue;
        }

        gba_split(layout, att_data, tilemap_size/2); // Damn DKC

        uint8_t *rgb = malloc(768);
        uint8_t *bitplane = malloc(width*height*64*4);

        if (rgb == NULL || bitplane == NULL) {
            printf("Failed to allocate memory for output image data.\n");
            continue;
        }

        decode_palette(rgb, &rom[levels[i].palette] - 0x40, 256);

        // Convert tiles to rgba
        gba_tiles(bitplane, &rom[levels[i].tileset], layout, att_data, rgb, tilemap_size/2, 0, 0);
        arrange_gbc(bitplane, width*8, height*8, dir, levels[i].name);

        free(layout);
        free(att_data);
        free(rgb);
        free(bitplane);

    }

}
