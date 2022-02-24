#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "dkc2_decomp.h"

void huff_decomp(uint8_t *rom, uint8_t *output, int len, int rpos) {

    int wpos = 0, tpos = 0, tree, value, i, j, node_l, node_r, write = 0, dir;

    tree = rpos+2;
    rpos += (rom[rpos]+1) * 2;
    rpos -= 5;

    while (wpos < len) {
        rpos += 8;
        for (i = 0; i < 4; i++) {
            value = rom[rpos--];
            for (j = 7; j >= 0; j--) {
                node_l = rom[tree+(tpos*2)];
                node_r = rom[tree+(tpos*2)+1];
                dir = (value >> j) & 1;
                if ((write & 1 && dir) || (write & 2 && !dir)) {
                    output[wpos++] = (dir) ? node_r : node_l;
                    write = 0;
                    tpos = 0;
                    continue;
                }
                write = 0;
                if (dir) {
                    tpos += (node_r & 0x3F)+1;
                    if (node_r & 0x40) write += 1;
                    if (node_r & 0x80) write += 2;
                } // Right
                else {
                    tpos += (node_l & 0x3F)+1;
                    if (node_l & 0x40) write += 1;
                    if (node_l & 0x80) write += 2;
                } // Left
            }
        }
    }

} // huff_decomp();

void rle_decomp(uint8_t *rom, uint8_t *output, int len, int rpos) {

    int wpos = 0, size, i;

    while (wpos < len) {

        size = rom[rpos++] & 0x7F;

        if (rom[rpos-1] & 0x80) {
            size += 3;
            for (i = 0; i < size; i++) {
                output[wpos++] = rom[rpos];
            }
            rpos++;
        } // Compressed
        else {
            size++;
            for (i = 0; i < size; i++) {
                output[wpos++] = rom[rpos++];
            }
        } // Not compressed
    }

} // rle_decomp();

void lz77_decomp(uint8_t *rom, uint8_t *output, int len, int rpos) {

    int i, j, wpos = 0, flag, cpos, sz, ofs;
    uint8_t bit_table[] = {128, 64, 32, 16, 8, 4, 2, 1};

    while (wpos < len) {

        flag = rom[rpos++];

        for (i = 0; i < 8; i++) {

            if (flag & bit_table[i]) {
                sz   = (rom[rpos] >> 4) + 3;
                ofs  = ((rom[rpos] & 0x0F) << 8) + rom[rpos+1];
                rpos+=2;
                cpos = wpos - ofs - 1;
                for (j = 0; j < sz; j++) {
                    if (cpos < 0 || cpos >= wpos) {
                        output[wpos++] = 0;
                    }
                    else {
                        output[wpos++] = output[cpos++];
                    }
                }
            }
            else {
                output[wpos++] = rom[rpos++];
            }
        } // 8 Blocks
    }

} // lz77_decomp();

void gba_decomp(uint8_t *rom, uint8_t *output, int *len, int address) {

    int type = rom[address] & 0x30; // Compression Type
    *len = (rom[address+3] << 16) + (rom[address+2] << 8) + rom[address+1];
    // printf("Length = 0x%X bytes.\n", *len);
    address += 4;

    if (type == 0x10) {
        // printf("Decompressing data using LZ77...");
        lz77_decomp(rom, output, *len, address);
    }
    else if (type == 0x20) {
        // printf("Decompressing data using Huffman...");
        huff_decomp(rom, output, *len, address);
    }
    else if (type == 0x30) {
        // printf("Decompressing data using RLE...");
        rle_decomp(rom, output, *len, address);
    }
    else {
        printf("Unknown decompression scheme. (%X)\n", address-4);
        return;
    }
    // printf("Done!\n");

} // gba_decomp();

void gba_data(uint8_t *rom, uint8_t *output, int *length, uint32_t location, uint32_t offset, uint8_t type) {

        switch (type) {
            case 0:
                memcpy(output, &rom[location], offset);
                *length = offset;
            break;

            case 1:
                gba_decomp(rom, output, length, location);
            break;

            case 2:
                dkc2_decomp(rom, output, length, location);
            break;

            case 3:
                dkc2_decode(rom, output, length, location);
            break;

            default:
                printf("Error: %X\n", location);
            break;
        }
        if (type && offset) memmove(output, &output[offset], *length - offset);
}

void gba_layout(uint8_t *lay_data, uint8_t *raw_data, uint8_t *att_data, int *width, int *height, int mode) {

    raw_data[1] &= ~0xC0;

    int raw_size = (raw_data[0] + (raw_data[1] * 256)) * 0x12;
    *width  = lay_data[0] + (lay_data[1] * 256);
    *height = lay_data[2] + (lay_data[3] * 256);
    int lay_size = *width * *height * 0x12 * 2;

    uint8_t *lev_data = calloc(lay_size, 1);

    if (lev_data == NULL) {
        printf("Error allocating memory for tile conversion.\n");
    }

    int i, j, ij, k, tile_group, write;

    for (i = 0; i < *height; i++) {
        for (j = 0; j < *width; j++) {

        ij = (i * (*width)) + j;
        tile_group = lay_data[(ij * 2) + 4] + (lay_data[(ij * 2) + 5] * 256);

            for (k = 0; k < 3; k++) {
                write = (i*(*width)*9) + (j*3) + (k*(*width)*3);
                memcpy(&lev_data[write*2], &raw_data[(tile_group * 0x12) + (k*6) + 2], 6);
                if (!mode) memcpy(&att_data[write], &raw_data[raw_size + (tile_group * 9) + (k*3) + 2], 3);
            }
        }
    }

    memcpy(&lay_data[0], &lev_data[0], lay_size);

    free(lev_data);

} // gba_layout();

void gba_split(uint8_t *lay_data, uint8_t *att_data, int size) {

    int i;
    for (i = 0; i < size; i++) {
        att_data[i] = lay_data[((i*2)+1)] & 0xFC;
        lay_data[(i*2)+1] &= 3;
    }

} // gba_split();

void gba_tiles(uint8_t *bitplane, uint8_t *bp_data, uint8_t *lay_data, uint8_t *att_data, uint8_t *rgb, int lay_len, int priority, int mode) {

    // mode flags:
    // 0 = 16 palettes, 1 = 256 colour mode
    // 0 = 4bpp, 2 = 8pp

    int pal_val, pal_ofs, hflip, vflip, ofs, i, j, k, m, lp, rp, tile, temp;

    for (i = 0; i < lay_len; i+=9) {
        for (j = 0; j < 9 && i+j < lay_len; j++) {

            hflip   = (att_data[i+j] & 4) ? 1 : 0;
            vflip   = (att_data[i+j] & 8) ? 1 : 0;
            pal_val = att_data[i+j] >> 4;
            pal_ofs = (mode & 1) ? 0 : pal_val * 16;
            tile = (lay_data[((i+j)*2)+1]*256) + lay_data[((i+j)*2)];
            for (k = 0; k < 8; k++) {


                for (m = 0; m < 4; m++) {

                    if (hflip) m = 3 - m;
                    if (mode & 2) {
                        lp = bp_data[(tile*64)+(k*8)+(m*2)];
                        rp = bp_data[(tile*64)+(k*8)+(m*2)+1];
                    }
                    else {
                        lp =  bp_data[(tile*32)+(k*4)+m] & 0x0F;
                        rp = (bp_data[(tile*32)+(k*4)+m] & 0xF0) >> 4;
                    }
                    if (hflip) m = 3 - m;

                    if (vflip) k = 7 - k;
                    ofs = ((i+j)*256)+(k*32)+(m*8);
                    if (vflip) k = 7 - k;

                    if (hflip) {
                        temp = lp;
                        lp = rp;
                        rp = temp;
                    }

                    bitplane[ofs] = rgb[((pal_ofs + lp) * 3)];
                    bitplane[ofs+1] = rgb[((pal_ofs + lp) * 3) + 1];
                    bitplane[ofs+2] = rgb[((pal_ofs + lp) * 3) + 2];
                    bitplane[ofs+3] = 255;

                    bitplane[ofs+4] = rgb[((pal_ofs + rp) * 3)];
                    bitplane[ofs+5] = rgb[((pal_ofs + rp) * 3) + 1];
                    bitplane[ofs+6] = rgb[((pal_ofs + rp) * 3) + 2];
                    bitplane[ofs+7] = 255;

                    if (!lp && !priority) {
                        bitplane[ofs+0] = 0;
                        bitplane[ofs+1] = 0;
                        bitplane[ofs+2] = 0;
                        bitplane[ofs+3] = 0;
                    }
                    if (!rp && !priority) {
                        bitplane[ofs+4] = 0;
                        bitplane[ofs+5] = 0;
                        bitplane[ofs+6] = 0;
                        bitplane[ofs+7] = 0;
                    }

                }
            }
        }
    }
    // printf("ofs = %X\n", ofs);

}

void gba_tileset(uint8_t *lay_data, uint8_t *raw_data) {

    raw_data[1] &= ~0xC0;
    int total = raw_data[0] + (raw_data[1]*256); // Number of tile groups.
    int i;

    int width = 24;
    int height = total / width;
    if (total % width) height++;

    lay_data[0] = width;
    lay_data[1] = 0;
    lay_data[2] = height;
    lay_data[3] = height >> 8;

    for (i = 0; i < total; i++) {
        lay_data[4+(i*2)]   = i % 256;
        lay_data[4+(i*2)+1] = (i-(i % 256))/256;
    }
    memset(&lay_data[4+(total*2)], 0, (width*height*2)-(total*2));

} // gba_tileset();

