#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "bitplane.h"
#include "dkc2_decomp.h"
#include "gba_misc.h"

int gba_data(
    unsigned char *rom, size_t romsize,
    unsigned char **output, size_t *outsize,
    unsigned loc, unsigned offset,
    int data_type
) {
    int e = 0;

    if (loc >= romsize) {
        fprintf(stderr, "Error: data offset is larger than the ROM.\n");
        return 1;
    }

    switch (data_type) {
        case GBA_COMP_NONE: {
            *output = calloc(offset, 1);
            if (*output == NULL) {
                fprintf(stderr, "Failed to allocate memory for data.\n");
                return 1;
            }
            memcpy(*output, &rom[loc], offset); *outsize = offset;
            break;
        }
        case GBA_COMP_BIOS:
        case GBA_COMP_DKC2:
        case GBA_COMP_DKC3: {
            if ((e = dk_decompress_mem_to_mem(GBA_COMP, output, outsize, rom + loc, romsize - loc))) {
                fprintf(stderr, "Error: %s.\n", dk_get_error(e));
                return e;
            }
            break;
        }
        default: {
            fprintf(stderr, "Error: %X\n", loc);
            *output = NULL;
            return 1;
        }
    }
    if (offset) {
        unsigned char *dat = realloc(*output, *outsize + offset);
        if (dat == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for data.\n");
            free(*output); *output = NULL;
            return 1;
        }
        *output = dat;
        memmove(*output, &(*output)[offset], *outsize - offset);
    }

    return e;
}

int gba_layout(unsigned char **lay_data, unsigned char *raw_data, unsigned char *att_data, int *width, int *height, int mode) {

    raw_data[1] &= ~0xC0;

    int raw_size = (raw_data[0] + (raw_data[1] * 256)) * 0x12;
    *width  = (*lay_data)[0] + ((*lay_data)[1] * 256);
    *height = (*lay_data)[2] + ((*lay_data)[3] * 256);
    int lay_size = *width * *height * 0x12 * 2;

    unsigned char *lev_data = malloc(lay_size);
    if (lev_data == NULL) {
        printf("Error allocating memory for tile conversion.\n");
        return 1;
    }

    int i, j, ij, k, tile_group, write;

    for (i = 0; i < *height; i++) {
        for (j = 0; j < *width; j++) {

        ij = (i * (*width)) + j;
        tile_group = (*lay_data)[(ij * 2) + 4] + ((*lay_data)[(ij * 2) + 5] * 256);

            for (k = 0; k < 3; k++) {
                write = (i*(*width)*9) + (j*3) + (k*(*width)*3);
                memcpy(&lev_data[write*2], &raw_data[(tile_group * 0x12) + (k*6) + 2], 6);
                if (!mode) memcpy(&att_data[write], &raw_data[raw_size + (tile_group * 9) + (k*3) + 2], 3);
            }
        }
    }
    free(*lay_data);
    *lay_data = lev_data;
    return 0;
} // gba_layout();

void gba_split(unsigned char *lay_data, unsigned char *att_data, int size) {

    int i;
    for (i = 0; i < size; i++) {
        att_data[i] = lay_data[((i*2)+1)] & 0xFC;
        lay_data[(i*2)+1] &= 3;
    }

} // gba_split();

void gba_tiles(unsigned char *bitplane, unsigned char *bp_data, unsigned char *lay_data, unsigned char *att_data, unsigned char *rgb, int lay_len, int priority, int mode) {

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

                    bitplane[ofs]   = rgb[((pal_ofs + lp) * 3)];
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

int gba_tileset(
    unsigned char **lay_data, size_t *lay_len,
    unsigned char  *raw_data
) {
    raw_data[1] &= ~0xC0;
    int total = raw_data[0] + (raw_data[1]*256); // Number of tile groups.
    int i;

    size_t rlen = 4*(total + 24);
    if (rlen > *lay_len) {
        unsigned char *data = realloc(*lay_data, rlen);
        if (data == NULL) {
            printf("Error: Failed to allocate memory for something.\n");
            return 1;
        }
        *lay_data = data;
        *lay_len = rlen;
    }
    memset(*lay_data, 0, *lay_len);

    int width = 24;
    int height = total / width;
    if (total % width) height++;

    (*lay_data)[0] = width;
    (*lay_data)[1] = 0;
    (*lay_data)[2] = height;
    (*lay_data)[3] = height >> 8;

    for (i = 0; i < total; i++) {
        (*lay_data)[4+(i*2)]   = i % 256;
        (*lay_data)[4+(i*2)+1] = (i-(i % 256))/256;
    }
    return 0;
}

