/* SPDX-License-Identifier: MIT
 * Copyright (c) 2014-2022 Kingizor
 * STripper - Function Definitions for Tile Manipulation */

#ifndef bitplane_h
#define bitplane_h

void assemble_level(unsigned char *bitplane, unsigned char *rom, unsigned char *layout, unsigned int layout_len, int position_addr, int vert, int layout_size, int fix, char *dir, char *name);
void assemble_screen(unsigned char *bitplane, unsigned int raw_len, unsigned int width, char *dir, char *name);
void assemble_bitplane(unsigned char *bitplane, unsigned int width, unsigned int raw_len, char *dir, char *name);
void dump_bitplane(unsigned char *bp_data, int bp_len, int bpp, int width, char *dir, char *name);
void decode_bitplane(unsigned char *rom, unsigned char *bp_data, unsigned char *raw_data, unsigned char *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg);
void decode_palette(unsigned char *rgb, const unsigned char *rom, int count);
void bbc_pal(unsigned char *rom, unsigned char *palette, int seed);
void gbc_assemble(unsigned char *bitplane, unsigned char *bp_data, unsigned char *lay_data, unsigned char *col_data, unsigned char *rgb, int t_width, int t_height, int mode);
void arrange_gbc(unsigned char *source, int width, int height, char *dir, char *name);
void write_png(char *dir, char *name, unsigned char *image, int w, int h);

#endif
