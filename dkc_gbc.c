#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"

static void ram_p(uint8_t *rom, uint8_t *ram, int *ram_count, int *ram_reg) {

    uint8_t a = 0, b = 1, c = ram_reg[1], d = 0, e = 0, h = ram_reg[2], l = ram_reg[3], carry = 0;
    int rom_bank = 4;
    int jmp = 0xB9C;
    int run = 1;
    uint8_t stack[6];
    uint8_t sc = 0;
    uint8_t ram_bank = 0;
    // int run_count = 0;
    *ram_count = 0;
    int ret = 0;

    while (run) {

        // printf("\nr = %d\n", run_count);
        // if (run_count > 10) run = 0;
        // run_count++;
        // printf("j = %X\n", jmp);
        // printf(" a = %02X\n", a);
        // printf("bc = %04X\n", (b*256)+c);
        // printf("de = %04X\n", (d*256)+e);
        // printf("hl = %04X\n", (h*256)+l);

        switch(jmp) {

            case 0x3E4:
                a = rom[((d-0x40)*256)+e+(rom_bank*0x4000)];
                if (a == 0) { 
                    jmp = ret;
                    break;
                }
                if ((e+1) > 255) d++;
                e++;
                if (a & 0x80) {
                    jmp = 0x3F7;
                    break;
                }
                c = a;
                a = rom[((d-0x40)*256)+e+(rom_bank*0x4000)];
                if ((e+1) > 255) d++;
                e++;
                jmp = 0x3EF;
            break;

            case 0x3EF:
                ram[*ram_count] = a;
                ram_count[0]++;
                if (l + 1 > 255) h++;
                l++;
                c--;
                if (c > 0) {
                    jmp = 0x3EF;
                }
                else {
                    jmp = 0x3E4;
                }
            break;

            case 0x3F7:
                if (a & 0x40) {
                    jmp = 0x406;
                    break;
                }
                a &= 0x3F;
                c = a;
                jmp = 0x3FE;
            break;

            case 0x3FE:
                a = rom[((d-0x40)*256)+e+(rom_bank*0x4000)];
                if ((e+1) > 255) d++;
                e++;
                ram[*ram_count] = a;
                ram_count[0]++;
                if (l + 1 > 255) h++;
                l++;
                c--;
                if (c == 0) {
                    jmp = 0x3E4;
                }
                else {
                    jmp = 0x3FE;
                }
            break;

            case 0x406:
                a &= 0x3F;
                c = a;
                a = rom[((d-0x40)*256)+e+(rom_bank*0x4000)];
                if ((e+1) > 255) d++;
                e++;
                b = a;
                stack[sc] = e;
                sc++;
                stack[sc] = d;
                sc++;
                a = l;
                if (b > a) carry = 1;
                a -= b;
                e = a;
                a = h;
                if (carry == 1) {
                    a--;
                    carry = 0;
                }
                d = a;
                jmp = 0x414;
            break;

            case 0x414:
                if (d & 0xD0) {
                    a = ram[ram_count[0]-(ram_count[0]%0x1000)+((d-0xD0)*256)+e];
                }
                else {
                    a = rom[((d-0x40)*256)+e+(rom_bank*0x4000)];
                }
                if ((e+1) > 255) d++;
                e++;
                ram[*ram_count] = a;
                ram_count[0]++;
                if (l + 1 > 255) h++;
                l++;
                c--;
                if (c > 0) {
                    jmp = 0x414;
                    break;
                }
                sc--;
                d = stack[sc];
                sc--;
                e = stack[sc];
                jmp = 0x3E4;
            break;

            case 0xB9C:
                stack[sc] = c;
                sc++;
                stack[sc] = b;
                sc++;
                a = b;
                ram_bank = a;
                a = rom[((h-0x40)*256)+l+(rom_bank*0x4000)];
                if (l + 1 > 255) h++;
                l++;
                e = a;
                a = rom[((h-0x40)*256)+l+(rom_bank*0x4000)];
                if (l + 1 > 255) h++;
                l++;
                d = a;
                a = c;
                rom_bank = a;
                stack[sc] = l;
                sc++;
                stack[sc] = h;
                sc++;
                h = 0xD0;
                l = 0;
                ram_count[0] += (((ram_bank-1)*0x1000) - ram_count[0]);
                jmp = 0x3E4;
                ret = 0xBAF;
            break;

            case 0xBAF:
                sc--;
                h = stack[sc];
                sc--;
                l = stack[sc];
                a = 4;
                rom_bank = a;
                sc--;
                b = stack[sc];
                sc--;
                c = stack[sc];
                b++;
                a = 4;
                if (a == b) {
                    jmp = 0xBBC;
                }
                else {
                    jmp = 0xB9C;
                }
            break;

            case 0xBBC:
                a = ram_reg[0];
                a &= 2;
                if (a == 0) { 
                    run = 0;
                    break;
                }
                b = 5;
                jmp = 0xBC3;
            break;

            case 0xBC3:
                stack[sc] = c;
                sc++;
                stack[sc] = b;
                sc++;
                a = b;
                ram_bank = a;
                a = rom[((h-0x40)*256)+l+(rom_bank*0x4000)];
                if (l + 1 > 255) h++;
                l++;
                e = a;
                a = rom[((h-0x40)*256)+l+(rom_bank*0x4000)];
                if (l + 1 > 255) h++;
                l++;
                d = a;
                a = c;
                rom_bank = a;
                stack[sc] = l;
                sc++;
                stack[sc] = h;
                sc++;
                h = 0xD0;
                l = 0;
                ram_count[0] += (((ram_bank-1)*0x1000) - ram_count[0]);
                if (*ram_count < 0x4000) *ram_count = 0x4000;
                jmp = 0x3E4;
                ret = 0xBD6;
            break;

            case 0xBD6:
                sc--;
                h = stack[sc];
                sc--;
                l = stack[sc];
                a = 4;
                rom_bank = a;
                sc--;
                b = stack[sc];
                sc--;
                c = stack[sc];
                b++;
                a = 8;
                if (a == b) {
                    run = 0;
                }
                else {
                    jmp = 0xBC3;
                }
            break;

        } // switch();
    } // while();

    // printf("\n\tRAM LENGTH = %X\n", (*ram_count)[0]);

    return;

} // ram_p();

static void gbc_layout(uint8_t *rom, uint8_t *raw_data, uint8_t *lay_data, uint8_t *col_data, int bank, int width, int height) {

    int val, alt, ofs, tile, addr;
    int i, j, k, m;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            val = raw_data[(i*width)+j];
            alt = raw_data[(i*width)+j+(height*width)];

            if (val > 511) {
                printf("Error: Tile greater than 255 detected.\n");
            }

            ofs = ((alt - (alt % 4)) * 0x1000) + (((alt % 4) * 0x100));

            for (k = 0; k < 4; k++) {
                for (m = 0; m < 4; m++) {

                    addr = (i*width*16)+(j*4)+(k*width*4)+m;
                    tile = bank+val+(k*0x1000)+(m*0x400) + ofs;

                    lay_data[addr] = rom[tile];

                    tile++;
                    col_data[addr] = rom[tile];
                }
            }
        }
    }

    return;

} // gbc_layout();

struct gbc_levels {
    int t_width;    // 32x32 tiles per row    
    int t_height;   // 32x32 tiles per column
    int bp_addr;    // Address of BP data
    int rom_bank;   // 32x32 tile bank
    int ram_reg[4]; // Raw Generation
    int vert;       // Horizontal or Vertical?
    int pal_addr;   // Palette Address
    int width;      // Pixel Width (cropping)
    int height;     // Pixel Height (cropping)
    char *name;     // Level Name
};

void tidy_gbc(uint8_t *raw_data, int *rawlen, int width, int px_height, int vert) {

    int i, shortwidth, ex;
    int div = (vert) ? 0x40 : 0x100;

    int length = 0x2000 / div;

    if (vert == 0 && width <= div) {
        ex = 0;
    }
    else if (vert == 1 && px_height <= 2048) {
        ex = 0;
    }
    else {
        ex = 1;
    }

    uint8_t *new_data = calloc(0x6000, 1);

    if (!ex) {
        // printf("Normal Mode\n");
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], width);
        }
    } // Normal Layout
    else if (!vert) {
        shortwidth = (width-div);
        // printf("Extended Mode H\n");
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], div);
            memcpy(&new_data[((i+1) * width) - shortwidth], &raw_data[0x4000 + (i * div)], shortwidth);
        }
    } // Extended Layout for horizontal levels.
    else {
        // printf("Extended Mode V\n");
        memcpy(&new_data[0], &raw_data[0], 0x6000);
        memcpy(&raw_data[0x1000], &new_data[0x4000], 0x1000);
        memcpy(&raw_data[(px_height/32)*0x40], &new_data[0x1000], 0x1000);
        memcpy(&raw_data[((px_height/32)*0x40)+0x1000], &new_data[0x5000], 0x1000);
        length = px_height/16;
        for (i = 0; i < length; i++) {
            memcpy(&new_data[i * width], &raw_data[i * div], width);
        }
    } // Extended Layout for vertical levels.

    memcpy(raw_data, new_data, 0x6000);
    free(new_data);
    *rawlen = width * length;

    return;

} // tidy_gbc();

void gbc_levels(uint8_t *rom, char dir[255]) {

    struct gbc_levels dkc[] = {
        {0x083, 0x10, 0x20C000, 0x9C, {0x01, 0xE2, 0x66, 0xDE}, 0, 0x200317, 4192, 416, "Jungle Hijinxs"},
        {0x015, 0x10, 0x21DF70, 0xB7, {0x01, 0xE8, 0x69, 0x93}, 0, 0x200857, 672, 160, "Jungle Hijinxs Bonus 1"},
        {0x007, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6B, 0xDC}, 0, 0x200457, 224, 160, "Jungle Hijinxs Bonus 2"},
        {0x0BB, 0x10, 0x20C000, 0x9C, {0x01, 0xE2, 0x66, 0xFC}, 0, 0x200357, 5984, 352, "Ropey Rampage"},
        {0x018, 0x10, 0x20C000, 0x9C, {0x01, 0xEB, 0x67, 0x38}, 0, 0x200357, 768, 224, "Ropey Rampage Bonus 1"},
        {0x005, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6B, 0xCD}, 0, 0x200497, 160, 160, "Ropey Rampage Bonus 2"},
        {0x096, 0x10, 0x21DF70, 0xB7, {0x01, 0xE6, 0x69, 0x27}, 0, 0x200857, 4800, 384, "Reptile Rumble"},
        {0x017, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xA2}, 0, 0x200857, 736, 224, "Reptile Rumble Bonus 1"},
        {0x00B, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xB1}, 0, 0x200857, 352, 384, "Reptile Rumble Bonus 2"},
        {0x00A, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xC0}, 0, 0x200857, 320, 288, "Reptile Rumble Bonus 3"},
        {0x03D, 0x40, 0x2119F0, 0xA5, {0x01, 0xE3, 0x67, 0x83}, 1, 0x200D17, 1952, 1216, "Coral Capers"},
        {0x0F7, 0x10, 0x20C000, 0x9C, {0x01, 0xE3, 0x67, 0x0B}, 0, 0x200397, 7904, 384, "Barrel Cannon Canyon"},
        {0x016, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xCF}, 0, 0x200857, 704, 192, "Barrel Cannon Canyon Bonus 1"},
        {0x007, 0x10, 0x20C000, 0x9C, {0x01, 0xEB, 0x67, 0x47}, 0, 0x200397, 224, 352, "Barrel Cannon Canyon Bonus 2"}, // 14

        {0x0C8, 0x10, 0x220000, 0xBD, {0x01, 0xE9, 0x6A, 0xA4}, 0, 0x200717, 6400, 448, "Winky's Walkway"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xEC, 0x6A, 0xE0}, 0, 0x200717, 192, 192, "Winky's Walkway Bonus"},
        {0x1DB, 0x10, 0x209ED0, 0x99, {0x02, 0xE1, 0x66, 0xC9}, 0, 0x200697, 15200, 512, "Mine Cart Carnage"},
        {0x102, 0x10, 0x21DF70, 0xB7, {0x02, 0xE7, 0x69, 0x36}, 0, 0x200897, 8256, 512, "Bouncy Bonanza"},
        {0x005, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x54}, 0, 0x200997, 160, 144, "Bouncy Bonanza Bonus 1"},
        {0x007, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xED}, 0, 0x200897, 224, 192, "Bouncy Bonanza Bonus 2"},
        {0x005, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xDE}, 0, 0x200897, 160, 320, "Bouncy Bonanza Bonus 3"},
        {0x0D5, 0x10, 0x224000, 0xC0, {0x01, 0xEA, 0x6A, 0xFE}, 0, 0x200617, 6816, 320, "Stop & Go Station (Green)"},
        {0x0D5, 0x10, 0x224000, 0xC0, {0x01, 0xEA, 0x6A, 0xFE}, 0, 0x200657, 6816, 320, "Stop & Go Station (Red)"},
        {0x009, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x64}, 0, 0x200617, 288, 320, "Stop & Go Station Bonus 1 (Green)"},
        {0x009, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x64}, 0, 0x200657, 288, 320, "Stop & Go Station Bonus 1 (Red)"},
        {0x005, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x73}, 0, 0x200617, 160, 320, "Stop & Go Station Bonus 2 (Green)"},
        {0x005, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x73}, 0, 0x200657, 160, 320, "Stop & Go Station Bonus 2 (Red)"},
        {0x0FF, 0x10, 0x204000, 0x90, {0x01, 0xE0, 0x65, 0xCD}, 0, 0x200F97, 8160, 384, "Millstone Mayhem"},
        {0x005, 0x10, 0x204000, 0x90, {0x01, 0xE0, 0x65, 0xF1}, 0, 0x200F97, 160, 320, "Millstone Mayhem Bonus 1"},
        {0x007, 0x10, 0x204000, 0x90, {0x01, 0xEB, 0x66, 0x00}, 0, 0x200F97, 224, 160, "Millstone Mayhem Bonus 2"},
        {0x006, 0x10, 0x204000, 0x90, {0x01, 0xEB, 0x66, 0x1E}, 0, 0x200F97, 192, 160, "Millstone Mayhem Bonus 3"}, // 17

        {0x0FB, 0x10, 0x215FC0, 0xAB, {0x01, 0xE5, 0x68, 0x67}, 0, 0x200C97, 8032, 320, "Vulture Culture"},
        {0x007, 0x10, 0x215FC0, 0xAB, {0x01, 0xE5, 0x68, 0x85}, 0, 0x200C97, 224, 160, "Vulture Culture Bonuses 1 & 2"},
        {0x016, 0x10, 0x215FC0, 0xAB, {0x01, 0xEB, 0x68, 0xA3}, 0, 0x200C97, 704, 192, "Vulture Culture Bonus 3"},
        {0x0F6, 0x10, 0x206000, 0x93, {0x01, 0xE0, 0x66, 0x2D}, 0, 0x200517, 7872, 352, "Tree Top Town"},
        {0x006, 0x10, 0x225E10, 0xC3, {0x01, 0xEC, 0x6C, 0x72}, 0, 0x2006D7, 192, 160, "Tree Top Town Bonus 1"},
        {0x005, 0x10, 0x206000, 0x93, {0x01, 0xEB, 0x66, 0x69}, 0, 0x200517, 160, 256, "Tree Top Town Bonus 2"},
        {0x0FF, 0x10, 0x215FC0, 0xAB, {0x01, 0xE5, 0x68, 0x76}, 0, 0x200CD7, 8160, 320, "Forest Frenzy"},
        {0x006, 0x10, 0x215FC0, 0xAB, {0x01, 0xEB, 0x68, 0x94}, 0, 0x200CD7, 192, 160, "Forest Frenzy Bonus 1"},
        {0x00B, 0x10, 0x215FC0, 0xAB, {0x01, 0xEB, 0x68, 0xB2}, 0, 0x200CD7, 352, 288, "Forest Frenzy Bonus 2"},
        {0x130, 0x10, 0x204000, 0x90, {0x02, 0xE0, 0x65, 0xDC}, 0, 0x200FD7, 9728, 352, "Temple Tempest"},
        {0x019, 0x10, 0x204000, 0x90, {0x01, 0xEB, 0x66, 0x0F}, 0, 0x200FD7, 800, 160, "Temple Tempest Bonus 1"},
        {0x007, 0x10, 0x204000, 0x90, {0x01, 0xEB, 0x66, 0x00}, 0, 0x200FD7, 224, 160, "Temple Tempest Bonus 2"},
        {0x0D0, 0x10, 0x20C000, 0x9C, {0x01, 0xE2, 0x66, 0xED}, 0, 0x2003D7, 6656, 416, "Orang-utan Gang"},
        {0x007, 0x10, 0x20C000, 0x9C, {0x01, 0xEB, 0x67, 0x29}, 0, 0x2003D7, 224, 192, "Orang-utan Gang Bonus 1"},
        {0x007, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6C, 0x27}, 0, 0x2003D7, 224, 192, "Orang-utan Gang Bonus 2"},
        {0x007, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6C, 0x18}, 0, 0x2003D7, 224, 192, "Orang-utan Gang Bonus 3"},
        {0x007, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6C, 0x09}, 0, 0x2003D7, 224, 192, "Orang-utan Gang Bonus 4"},
        {0x016, 0x10, 0x20C000, 0x9C, {0x01, 0xEC, 0x67, 0x56}, 0, 0x2003D7, 704, 192, "Orang-utan Gang Bonus 5"},
        {0x038, 0x40, 0x2119F0, 0xA5, {0x01, 0xE3, 0x67, 0x92}, 1, 0x200D57, 1792, 1408, "Clam City"}, // 19

        {0x148, 0x10, 0x219460, 0xB1, {0x02, 0xE6, 0x68, 0xC1}, 0, 0x200E57, 10496, 480, "Snow Barrel Blast"},
        {0x006, 0x10, 0x221B00, 0xC6, {0x01, 0xEA, 0x6B, 0xA0}, 0, 0x200F17, 192, 160, "Snow Barrel Blast Bonus 1"},
        {0x005, 0x10, 0x219460, 0xB1, {0x01, 0xE6, 0x68, 0xE5}, 0, 0x200E57, 160, 320, "Snow Barrel Blast Bonus 2"},
        {0x016, 0x10, 0x219460, 0xB1, {0x01, 0xEB, 0x68, 0xF4}, 0, 0x200E57, 704, 160, "Snow Barrel Blast Bonus 3"},
        {0x033, 0x44, 0x208000, 0x96, {0x02, 0xE1, 0x66, 0x78}, 1, 0x200A57, 1632, 2176, "Slipslide Ride"}, // 44
        {0x006, 0x10, 0x208000, 0x96, {0x01, 0xEB, 0x66, 0x9C}, 0, 0x200AD7, 192, 160, "Slipslide Ride Bonus 1"},
        {0x005, 0x10, 0x208000, 0x96, {0x01, 0xE1, 0x66, 0x8D}, 0, 0x200A97, 160, 160, "Slipslide Ride Bonus 2"},
        {0x011, 0x10, 0x208000, 0x96, {0x01, 0xEB, 0x66, 0xAB}, 0, 0x200A57, 544, 224, "Slipslide Ride Bonus 3"},
        {0x0E2, 0x10, 0x219460, 0xB1, {0x01, 0xE6, 0x68, 0xD6}, 0, 0x200E97, 7232, 512, "Ice Age Alley"},
        {0x00B, 0x10, 0x219460, 0xB1, {0x01, 0xEB, 0x69, 0x03}, 0, 0x200E97, 352, 320, "Ice Age Alley Bonus 1"},
        {0x006, 0x10, 0x221B00, 0xC6, {0x01, 0xEA, 0x6B, 0xAF}, 0, 0x200F57, 192, 160, "Ice Age Alley Bonus 2"},
        {0x035, 0x59, 0x2119F0, 0xA5, {0x02, 0xE4, 0x67, 0xA1}, 1, 0x200D97, 1696, 2848, "Croctopus Chase"}, // 59
        {0x082, 0x10, 0x21DF70, 0xB7, {0x01, 0xE7, 0x69, 0x4B}, 0, 0x200A17, 4160, 384, "Torchlight Trouble (Dark)"},
        {0x082, 0x10, 0x21DF70, 0xB7, {0x01, 0xE7, 0x69, 0x4B}, 0, 0x2008D7, 4160, 384, "Torchlight Trouble (Light)"},
        {0x005, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x54}, 0, 0x200997, 160, 144, "Torchlight Trouble Bonus 1"},
        {0x007, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x1A}, 0, 0x200A17, 224, 192, "Torchlight Trouble Bonus 2 (Dark)"},
        {0x007, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x1A}, 0, 0x2008D7, 224, 192, "Torchlight Trouble Bonus 2 (Light)"},
        {0x0C0, 0x10, 0x206000, 0x93, {0x01, 0xE0, 0x66, 0x3C}, 0, 0x200557, 6144, 416, "Rope Bridge Rumble"},
        {0x019, 0x10, 0x206000, 0x93, {0x01, 0xE0, 0x66, 0x4B}, 0, 0x200557, 800, 192, "Rope Bridge Rumble Bonus 1"},
        {0x006, 0x10, 0x225E10, 0xC3, {0x01, 0xEC, 0x6C, 0x81}, 0, 0x2006D7, 192, 160, "Rope Bridge Rumble Bonus 2"}, // 20

        {0x0FF, 0x10, 0x214000, 0xA8, {0x01, 0xE5, 0x67, 0xDA}, 0, 0x200B17, 8160, 416, "Oil Drum Alley"},
        {0x005, 0x10, 0x214000, 0xA8, {0x01, 0xEB, 0x68, 0x1C}, 0, 0x200B17, 160, 320, "Oil Drum Alley Bonus 1"},
        {0x008, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6B, 0xFA}, 0, 0x200C17, 256, 160, "Oil Drum Alley Bonus 2"},
        {0x005, 0x10, 0x214000, 0xA8, {0x01, 0xEB, 0x68, 0x2B}, 0, 0x200B17, 160, 256, "Oil Drum Alley Bonus 3"},
        {0x01D, 0x10, 0x214000, 0xA8, {0x01, 0xEB, 0x68, 0x0D}, 0, 0x200B17, 928, 160, "Oil Drum Alley Bonus 4"},
        {0x17A, 0x10, 0x220000, 0xBD, {0x02, 0xE9, 0x6A, 0x56}, 0, 0x200757, 12096, 448, "Trick Track Trek"},
        {0x00A, 0x10, 0x220000, 0xBD, {0x01, 0xEB, 0x6A, 0xB3}, 0, 0x200757, 320, 320, "Trick Track Trek Bonus 1"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xEC, 0x6A, 0xEF}, 0, 0x200757, 192, 192, "Trick Track Trek Bonus 2"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xE9, 0x6A, 0x95}, 0, 0x200757, 192, 160, "Trick Track Trek Bonus 3"},
        {0x128, 0x10, 0x21DF70, 0xB7, {0x02, 0xE8, 0x69, 0x6F}, 0, 0x200917, 9472, 512, "Elevator Antics"},
        {0x005, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x45}, 0, 0x200997, 160, 144, "Elevator Antics Bonus 1"},
        {0x016, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x29}, 0, 0x200917, 704, 192, "Elevator Antics Bonus 2"},
        {0x005, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x69, 0xFC}, 0, 0x200917, 160, 320, "Elevator Antics Bonus 3"},
        {0x03C, 0x57, 0x2119F0, 0xA5, {0x02, 0xE4, 0x67, 0xB6}, 1, 0x200DD7, 1920, 2784, "Poison Pond"}, // 57
        {0x168, 0x10, 0x21C000, 0xB4, {0x02, 0xE6, 0x69, 0x12}, 0, 0x200797, 11520, 512, "Mine Cart Madness"},
        {0x016, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x47}, 0, 0x200857, 704, 192, "Mine Cart Madness Bonus 1"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xE9, 0x6A, 0x95}, 0, 0x200757, 192, 160, "Mine Cart Madness Bonus 2"},
        {0x00A, 0x10, 0x220000, 0xBD, {0x01, 0xEB, 0x6A, 0xC2}, 0, 0x200797, 320, 224, "Mine Cart Madness Bonus 3"},
        {0x11D, 0x10, 0x214000, 0xA8, {0x02, 0xE5, 0x67, 0xE9}, 0, 0x200B57, 9120, 480, "Blackout Basement"},
        {0x00B, 0x10, 0x214000, 0xA8, {0x01, 0xE5, 0x67, 0xFE}, 0, 0x200B57, 352, 320, "Blackout Basement Bonus 1"},
        {0x007, 0x10, 0x228000, 0xC9, {0x01, 0xEC, 0x6B, 0xEB}, 0, 0x200C57, 224, 160, "Blackout Basement Bonus 2"}, // 21

        {0x128, 0x10, 0x220000, 0xBD, {0x02, 0xE9, 0x6A, 0x80}, 0, 0x2007D7, 9472, 448, "Tanked Up Trouble"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xEB, 0x6A, 0xD1}, 0, 0x2007D7, 192, 160, "Tanked Up Trouble Bonus"},
        {0x108, 0x10, 0x21DF70, 0xB7, {0x02, 0xE7, 0x69, 0x5A}, 0, 0x200957, 8448, 512, "Manic Mincers"},
        {0x007, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x38}, 0, 0x200957, 224, 192, "Manic Mincers Bonus 1"},
        {0x00B, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x0B}, 0, 0x200957, 352, 160, "Manic Mincers Bonus 2"},
        {0x005, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x45}, 0, 0x200997, 160, 144, "Manic Mincers Bonus 3"},
        {0x12B, 0x10, 0x224000, 0xC0, {0x02, 0xEA, 0x6B, 0x0D}, 0, 0x2005D7, 9568, 448, "Misty Mine"},
        {0x025, 0x10, 0x224000, 0xC0, {0x01, 0xEA, 0x6B, 0x37}, 0, 0x2005D7, 1184, 320, "Misty Mine Bonus 1"},
        {0x006, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x46}, 0, 0x2005D7, 192, 192, "Misty Mine Bonus 2"},
        {0x0F8, 0x10, 0x21DF70, 0xB7, {0x01, 0xE8, 0x69, 0x84}, 0, 0x200857, 7936, 480, "Necky Nutmare"},
        {0x007, 0x10, 0x21DF70, 0xB7, {0x01, 0xEB, 0x6A, 0x38}, 0, 0x200957, 224, 192, "Necky Nutmare Bonus"},
        {0x120, 0x10, 0x224000, 0xC0, {0x02, 0xEA, 0x6B, 0x22}, 0, 0x2005D7, 9216, 512, "Loopy Lights"},
        {0x016, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x55}, 0, 0x2005D7, 704, 320, "Loopy Lights Bonus 1"},
        {0x01F, 0x10, 0x224000, 0xC0, {0x01, 0xEC, 0x6B, 0x82}, 0, 0x2005D7, 992, 512, "Loopy Lights Bonus 2"},
        {0x11B, 0x10, 0x220000, 0xBD, {0x02, 0xE9, 0x6A, 0x6B}, 0, 0x200817, 9056, 512, "Platform Perils"},
        {0x006, 0x10, 0x220000, 0xBD, {0x01, 0xEB, 0x6A, 0xD1}, 0, 0x200817, 192, 160, "Platform Perils Bonuses 1 & 2"}, // 16

        {0x033, 0x10, 0x221B00, 0xC6, {0x01, 0xEA, 0x6B, 0x91}, 0, 0x200ED7, 1632, 512, "Rambi Bonus Room"},
        {0x063, 0x10, 0x228000, 0xC9, {0x01, 0xEB, 0x6B, 0xBE}, 0, 0x200417, 3168, 416, "Expresso Bonus Room"},
        {0x042, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x36}, 0, 0x200997, 2112, 384, "Winky Bonus Room"},
        {0x025, 0x40, 0x2119F0, 0xA5, {0x01, 0xE4, 0x67, 0xCB}, 1, 0x200E17, 1184, 1184, "Enguarde Bonus Room"},
        {0x007, 0x10, 0x20DFF0, 0x9F, {0x01, 0xE3, 0x67, 0x65}, 0, 0x200257, 224, 160, "Boss Room (Small)"},
        {0x00C, 0x10, 0x20DFF0, 0x9F, {0x01, 0xE3, 0x67, 0x74}, 0, 0x200257, 384, 160, "Boss Room (Large)"},
        {0x00C, 0x10, 0x210000, 0xA2, {0x01, 0xE3, 0x6C, 0x9F}, 0, 0x200297, 384, 160, "Gang-Plank Galleon"},
        {0x005, 0x10, 0x218000, 0xAE, {0x01, 0xE5, 0x6C, 0x90}, 0, 0x2002D7, 160, 160, "DK's Treehouse"},
        {0x005, 0x10, 0x225E10, 0xC3, {0x01, 0xEB, 0x6C, 0x63}, 0, 0x200997, 160, 160, "Banana Hoard (Empty)"},
        {0x005, 0x10, 0x229FF0, 0xBA, {0x01, 0xEC, 0x6C, 0xAE}, 0, 0x2009D7, 160, 160, "Banana Hoard (Full)"},
        {0x010, 0x10, 0x208000, 0x96, {0x01, 0xEC, 0x66, 0xBA}, 0, 0x200A57, 512, 352, "Candy's Challenge - Kremkroc Industries Inc"},
        {0x016, 0x40, 0x214000, 0xA8, {0x01, 0xEC, 0x68, 0x58}, 1, 0x200BD7, 704, 640, "Candy's Challenge - Chimp Caverns"} // 12
    };

    int size = sizeof(dkc) / sizeof(struct gbc_levels);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) { // 119

        uint8_t *bp_data = malloc(0x2000); // Tiles Bank 0:1
        uint8_t *raw_data = malloc(0x8000);
        uint8_t *lay_data = malloc(0x20000);
        uint8_t *col_data = malloc(0x20000);
        uint8_t *pal_data = malloc(0x200);
        uint8_t *rgb = malloc(384);
        int rawlen = 0;
        int t_width = dkc[i].t_width;
        int t_height = dkc[i].t_height;
        int width = dkc[i].width;
        int height = dkc[i].height;

        memcpy(bp_data, &rom[dkc[i].bp_addr], 0x2000);

        ram_p(rom, raw_data, &rawlen, dkc[i].ram_reg);
        tidy_gbc(raw_data, &rawlen, t_width, height, dkc[i].vert);

        // memset(raw_data, 0, 0x8000);
        // tile_generator(raw_data, &rawlen, 255, 0);
        // t_width = 16;
        // t_height = rawlen / t_width;
        // if (rawlen % t_width) t_height++;
        // width = t_width * 32;
        // height = t_height * 32;

        gbc_layout(rom, raw_data, lay_data, col_data, dkc[i].rom_bank*0x4000, t_width, t_height);

        memcpy(pal_data, &rom[dkc[i].pal_addr], 128);
        decode_palette(rgb, pal_data, 128);

        uint8_t *bitplane = malloc(t_width * t_height * 1024 * 4);

        gbc_assemble(bitplane, bp_data, lay_data, col_data, rgb, t_width, t_height, 1);

        arrange_gbc(bitplane, width, height, dir, dkc[i].name);

        free(bp_data);
        free(lay_data);
        free(raw_data);
        free(col_data);
        free(pal_data);
        free(rgb);
        free(bitplane);
    }



    /*
    FILE * lay_file = fopen("lay.bin", "wb");
    fwrite(lay_data, 0x400, 1, lay_file);
    fclose(lay_file);
    */

    return;

} // gbc_levels();
