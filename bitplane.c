#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bitplane.h"

void decode_palette(unsigned char *rgb, unsigned char *rom, int count) {
    
    int i;
    for (i = 0; i < count; i++) {
    
        unsigned char low = rom[i*2];
        unsigned char high =  rom[i*2+1];
        
        rgb[i*3]   = (low & 0x1F) * 8;
        rgb[i*3+1] = (((low & 0xE0) >> 5) + ((high & 0x03) << 3)) * 8;
        rgb[i*3+2] = ((high & 0x7C) >> 2) * 8;
        
    }
    
    return;

} // decode_palette();

static void colourfix_pre(unsigned char *rom, unsigned char *pal_data, int fix) {

    if (fix == 2) {
        memcpy(&pal_data[0xE2], &rom[0x3D1832], 0x1E);
        // DMA[0]: CPU->PPU Mode:0 0xFD1832->0x2122 Bytes:1E (inc) V:229 CGRAM: 71 (0)
    } // Target Terror Red Palette (DKC2)
    if (fix == 7) {
        memcpy(&pal_data[0xE2], &rom[0x3D1812], 0x1E);
    } // Target Terror Green Palette (DKC2)
    if (fix == 8) {
        memcpy(&pal_data[0xE2], &rom[0x3D1852], 0x1E);
    } // Target Terror Grey Palette (DKC2)
    if (fix == 3) {
        memcpy(&pal_data[0xE0], &rom[0x3D6426], 0x20);
        memcpy(&pal_data[0x00], &rom[0x3D3DAE], 0x40);
        memcpy(&pal_data[0x40], &rom[0x3D3F6E], 0xC0);
    } // Find the Token Palette (DKC2)
    if (fix == 4) {
        memcpy(&pal_data[0xE0], &rom[0x3D6426], 0x20);
        memcpy(&pal_data[0x00], &rom[0x3D3DAE], 0x40);
        memcpy(&pal_data[0x40], &rom[0x3D3DEE], 0xC0);
    } // Destroy Them All Palette (DKC2)
    if (fix == 5) {
        memcpy(&pal_data[0xE0], &rom[0x3D6426], 0x20);
        memcpy(&pal_data[0x00], &rom[0x3D3DAE], 0x40);
        memcpy(&pal_data[0x40], &rom[0x3D3EAE], 0xC0);
    } // Collect the Stars Palette (DKC2)
    if (fix == 6) {
        memcpy(&pal_data[0x00], &rom[0x3D420E], 0x100);
    } // Ending Screen Palette (DKC2)
    if (fix == 13) {
        if (rom[0xFFD9] == 0) {
            memcpy(&pal_data[0x40], &rom[0x3D73B4], 0x20);
        }
        else {
            memcpy(&pal_data[0x40], &rom[0x3D7401], 0x20);
        }
    } // Overworld (DKC3)
    if (fix == 14) {
        memcpy(&pal_data[0x00], &rom[0x3D8841], 0x20);
    } // Bonus Screens (DKC3)
    if (fix == 15) {
        if (rom[0xFFD9] == 0) {
            memcpy(&pal_data[0xC0], &rom[0x3D6274], 0x40);
            memcpy(&pal_data[0x00], &rom[0x3D7DF4], 0x20);
        }
        else {
            memcpy(&pal_data[0xC0], &rom[0x3D62C1], 0x40);
            memcpy(&pal_data[0x00], &rom[0x3D7E41], 0x20);
        }
    } // Belcha's Barn (DKC3)
    if (fix > 15 && fix < 29) {
        bbc_pal(rom, pal_data, fix);
    } // Banana Bird Caves (DKC3)
    if (fix == 29) {
        memcpy(&pal_data[0x1B*2], &rom[0x339AFB], 0x0B);
        pal_data[8] = 0x08;
        pal_data[9] = 0x21;
        pal_data[10] = 0x10;
        pal_data[11] = 0x42;
        pal_data[12] = 0x18;
        pal_data[13] = 0x63;
        pal_data[2] = 0x18;
        pal_data[3] = 0x63;
        pal_data[4] = 0x9C;
        pal_data[5] = 0x73;
        pal_data[6] = 0xFF;
        pal_data[7] = 0x7F;
    } // Lemguin Lunge BG (DKC3)
    if (fix > 29 && fix < 39) {
        memcpy(&pal_data[0x0A], &rom[0x339555 + ((fix-30)*0x16)], 0x16);
    } // Lightning Look-Out Lightning (DKC3)
    if (fix == 39) {
        pal_data[0]  = 0x00;
        pal_data[1]  = 0x00;
        pal_data[2]  = 0x21;
        pal_data[3]  = 0x00;
        pal_data[4]  = 0x63;
        pal_data[5]  = 0x04;
        pal_data[6]  = 0xE7;
        pal_data[7]  = 0x0C;
        pal_data[8]  = 0x00;
        pal_data[9]  = 0x00;
        pal_data[10] = 0xE7;
        pal_data[11] = 0x0C;
        pal_data[12] = 0x6B;
        pal_data[13] = 0x15;
        pal_data[14] = 0xEF;
        pal_data[15] = 0x1D;
        pal_data[16] = 0x00;
        pal_data[17] = 0x00;
        pal_data[18] = 0xEF;
        pal_data[19] = 0x1D;
        pal_data[20] = 0x52; // $33:F600
        pal_data[21] = 0x26;
        pal_data[22] = 0xD6;
        pal_data[23] = 0x2E;
        pal_data[24] = 0x00;
        pal_data[25] = 0x00;
        pal_data[26] = 0xD6;
        pal_data[27] = 0x2E;
        pal_data[28] = 0x5A;
        pal_data[29] = 0x37;
        pal_data[30] = 0x9C;
        pal_data[31] = 0x3B; 
    } // Murky Mill Lights (DKC3)
    if (fix == 40) {
        if (rom[0xFFD9] == 0) {
            memcpy(&pal_data[0x20], &rom[0x3DA82C], 0x20);
        }
        else {
            memcpy(&pal_data[0x20], &rom[0x3DA879], 0x20);
        }
    } // Photo Album Categories (DKC3)
    if (fix == 41) {
        pal_data[0] = 0x78;
        pal_data[1] = 0x0D;
    } // Knautilus BG3 (DKC3)
    if (fix == 42) {
        if (rom[0xFFD9] == 0) {
            memcpy(&pal_data[0xA0], &rom[0x3DB6EC], 0x60);
        }
        else {
            memcpy(&pal_data[0xA0], &rom[0x3DB739], 0x60);
        }
    } // K.Rool Hovercraft (DKC3)
    if (fix == 43) {
        pal_data[0xE2] = 0x20;
        pal_data[0xE3] = 0x00;
        pal_data[0xE4] = 0x62;
        pal_data[0xE5] = 0x08;
        pal_data[0xE6] = 0xC5;
        pal_data[0xE7] = 0x10;
        pal_data[0xE8] = 0x49;
        pal_data[0xE9] = 0x1D;
        pal_data[0xEA] = 0xEC;
        pal_data[0xEB] = 0x29;
        pal_data[0xEC] = 0x20;
        pal_data[0xED] = 0x00;
        pal_data[0xEE] = 0x62;
        pal_data[0xEF] = 0x08;
        pal_data[0xF0] = 0xC5;
        pal_data[0xF1] = 0x10;
        pal_data[0xF2] = 0x49;
        pal_data[0xF3] = 0x1D;
        pal_data[0xF4] = 0xEC;
        pal_data[0xF5] = 0x29;
        pal_data[0xF6] = 0x20;
        pal_data[0xF7] = 0x00;
        pal_data[0xF8] = 0x62;
        pal_data[0xF9] = 0x08;
        pal_data[0xFA] = 0xC5;
        pal_data[0xFB] = 0x10;
        pal_data[0xFC] = 0x49;
        pal_data[0xFD] = 0x1D;
        pal_data[0xFE] = 0xEC;
        pal_data[0xFF] = 0x29;
    } // Ghostly Grove FG (DKC2)
    if (fix == 44) {
        memcpy(&pal_data[0], &rom[0x3D2DCE], 0x20);
    } // Toxic Tower (DKC2)
    if (fix == 45) {
        pal_data[0xB8] = 0x08;
        pal_data[0xB9] = 0x7D;
        pal_data[0xBA] = 0x63;
        pal_data[0xBB] = 0x0C;
        pal_data[0xBC] = 0x63;
        pal_data[0xBD] = 0x0C;
    } // Swanky's Sideshow (DKC3)
    if (fix == 46) {
        pal_data[0xB8] = 0x63;
        pal_data[0xB9] = 0x0C;
        pal_data[0xBA] = 0x08;
        pal_data[0xBB] = 0x7D;
        pal_data[0xBC] = 0x63;
        pal_data[0xBD] = 0x0C;
    } // Swanky's Sideshow (DKC3)
    if (fix == 47) {
        pal_data[0xB8] = 0x63;
        pal_data[0xB9] = 0x0C;
        pal_data[0xBA] = 0x63;
        pal_data[0xBB] = 0x0C;
        pal_data[0xBC] = 0x08;
        pal_data[0xBD] = 0x7D;
    } // Swanky's Sideshow (DKC3)
    if (fix == 48) {
        pal_data[0x02] = 0x62;
        pal_data[0x03] = 0x14;
        pal_data[0x04] = 0x62;
        pal_data[0x05] = 0x14;
        pal_data[0x06] = 0x62;
        pal_data[0x07] = 0x14;
        pal_data[0x08] = 0x00;
        pal_data[0x09] = 0x00;
        pal_data[0x0A] = 0x62;
        pal_data[0x0B] = 0x14;
        pal_data[0x0C] = 0x62;
        pal_data[0x0D] = 0x14;
        pal_data[0x0E] = 0x62;
        pal_data[0x0F] = 0x14;
        pal_data[0x10] = 0x00;
        pal_data[0x11] = 0x00;
        pal_data[0x12] = 0x62;
        pal_data[0x13] = 0x14;
        pal_data[0x14] = 0x62;
        pal_data[0x15] = 0x14;
        pal_data[0x16] = 0x62;
        pal_data[0x17] = 0x14;
        pal_data[0x18] = 0x00;
        pal_data[0x19] = 0x00;
        pal_data[0x1A] = 0x62;
        pal_data[0x1B] = 0x14;
        pal_data[0x1C] = 0x62;
        pal_data[0x1D] = 0x14;
        pal_data[0x1E] = 0x62;
        pal_data[0x1F] = 0x14;
    } // Kastle KAOS BG (DKC3)
    if (fix == 49) {
        pal_data[0x02] = 0x8B;
        pal_data[0x03] = 0x41;
        pal_data[0x04] = 0xFD;
        pal_data[0x05] = 0x7F;
        pal_data[0x06] = 0x83;
        pal_data[0x07] = 0x18;
        pal_data[0x08] = 0x00;
        pal_data[0x09] = 0x00;
        pal_data[0x0A] = 0x8B;
        pal_data[0x0B] = 0x41;
        pal_data[0x0C] = 0xFD;
        pal_data[0x0D] = 0x7F;
        pal_data[0x0E] = 0x83;
        pal_data[0x0F] = 0x18;
        pal_data[0x10] = 0x00;
        pal_data[0x11] = 0x00;
        pal_data[0x12] = 0x8B;
        pal_data[0x13] = 0x41;
        pal_data[0x14] = 0xFD;
        pal_data[0x15] = 0x7F;
        pal_data[0x16] = 0x83;
        pal_data[0x17] = 0x18;
        pal_data[0x18] = 0x00;
        pal_data[0x19] = 0x00;
        pal_data[0x1A] = 0x8B;
        pal_data[0x1B] = 0x41;
        pal_data[0x1C] = 0xFD;
        pal_data[0x1D] = 0x7F;
        pal_data[0x1E] = 0x83;
        pal_data[0x1F] = 0x18;
    } // Kastle KAOS BG Lightning (DKC3)
    if (fix == 50) {
        pal_data[0x21] = 0x40;
        pal_data[0x36] = 0x94;
        pal_data[0x38] = 0xAF;
        pal_data[0x3A] = 0xEA;
        pal_data[0x3C] = 0x26;
        pal_data[0x3E] = 0x83;
        pal_data[0x41] = 0x40;
        pal_data[0x61] = 0x40;
        pal_data[0x6C] = 0x94;
        pal_data[0x6E] = 0xAF;
        pal_data[0x70] = 0xEA;
        pal_data[0x72] = 0x26;
        pal_data[0x74] = 0x83;
        pal_data[0x81] = 0x40;
        pal_data[0x84] = 0x7D;
        pal_data[0x85] = 0x7F;
        pal_data[0x86] = 0xDB;
        pal_data[0x87] = 0x7A;
        pal_data[0x88] = 0x58;
        pal_data[0x89] = 0x76;
        pal_data[0x8A] = 0xB6;
        pal_data[0x8B] = 0x71;
        pal_data[0x8C] = 0x72;
        pal_data[0x8D] = 0x5D;
        pal_data[0x8E] = 0x0D;
        pal_data[0x8F] = 0x49;
        pal_data[0x90] = 0xC9;
        pal_data[0x91] = 0x30;
        pal_data[0x92] = 0x65;
        pal_data[0x93] = 0x1C;
        pal_data[0x94] = 0x21;
        pal_data[0x95] = 0x08;
        pal_data[0xA1] = 0x40;
        pal_data[0xAA] = 0xB6;
        pal_data[0xAB] = 0x71;
        pal_data[0xAC] = 0x72;
        pal_data[0xAD] = 0x5D;
        pal_data[0xAE] = 0x0D;
        pal_data[0xAF] = 0x49;
        pal_data[0xB0] = 0xC9;
        pal_data[0xB1] = 0x30;
        pal_data[0xB2] = 0x65;
        pal_data[0xB3] = 0x1C;
        pal_data[0xB4] = 0x21;
        pal_data[0xB5] = 0x08;
        pal_data[0xC1] = 0x40;
        pal_data[0xC4] = 0xDB;
        pal_data[0xC5] = 0x7A;
        pal_data[0xC6] = 0xB6;
        pal_data[0xC7] = 0x71;
        pal_data[0xC8] = 0x72;
        pal_data[0xC9] = 0x5D;
        pal_data[0xCA] = 0x0D;
        pal_data[0xCB] = 0x49;
        pal_data[0xCC] = 0xC9;
        pal_data[0xCD] = 0x30;
        pal_data[0xCE] = 0x65;
        pal_data[0xCF] = 0x1C;
        pal_data[0xE1] = 0x40;
    } // 2P Collect the Stars (DKC3)
    if (fix == 51) {
        memcpy(&pal_data[0x00], &rom[0x3D8841], 0x20);
        pal_data[0x41] = 0x60;
        pal_data[0x61] = 0x60;
        pal_data[0x62] = 0x94;
        pal_data[0x64] = 0xF0;
        pal_data[0x66] = 0x6D;
        pal_data[0x68] = 0xEA;
        pal_data[0x6A] = 0x67;
        pal_data[0x81] = 0x60;
        pal_data[0x82] = 0xEF;
        pal_data[0x84] = 0xEA;
        pal_data[0xA1] = 0x60;
        pal_data[0xAA] = 0xD3;
        pal_data[0xAB] = 0x5D;
        pal_data[0xAC] = 0x4E;
        pal_data[0xAD] = 0x49;
        pal_data[0xAE] = 0xE9;
        pal_data[0xAF] = 0x30;
        pal_data[0xB0] = 0x65;
        pal_data[0xB1] = 0x1C;
        pal_data[0xC1] = 0x60;
        pal_data[0xCA] = 0x58;
        pal_data[0xCB] = 0x76;
        pal_data[0xCC] = 0xD3;
        pal_data[0xCD] = 0x5D;
        pal_data[0xCE] = 0x4E;
        pal_data[0xCF] = 0x49;
        pal_data[0xD0] = 0xE9;
        pal_data[0xD1] = 0x30;
        pal_data[0xD2] = 0x65;
        pal_data[0xD3] = 0x1C;
        pal_data[0xD4] = 0x41;
        pal_data[0xD5] = 0x0C;
        pal_data[0xE1] = 0x60;
        pal_data[0xE4] = 0x7D;
        pal_data[0xE5] = 0x7F;
        pal_data[0xE6] = 0xDB;
        pal_data[0xE7] = 0x7A;
        pal_data[0xE8] = 0x58;
        pal_data[0xE9] = 0x76;
        pal_data[0xEA] = 0xD3;
        pal_data[0xEB] = 0x5D;
        pal_data[0xEC] = 0x4E;
        pal_data[0xED] = 0x49;
        pal_data[0xEE] = 0xE9;
        pal_data[0xEF] = 0x30;
        pal_data[0xF0] = 0x65;
        pal_data[0xF1] = 0x1C;
    } // 2P Bonus Screens (DKC3)
    if (fix == 57) {
        unsigned char new_pal[] = {0x00, 0x00, 0x00, 0x00, 0x87, 0x21, 0x4B, 0x36, 0x00, 0x00, 0x64, 0x14, 0x4B, 0x2D, 0xF0, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x04, 0x00, 0x00, 0x03, 0x04, 0x04, 0x04, 0x06, 0x08, 0x06, 0x0C, 0x08, 0x14, 0x2A, 0x0C, 0x4C, 0x0C, 0x6E, 0x08, 0xAF, 0x04, 0xB1, 0x14, 0xB3, 0x20, 0xD5, 0x28, 0x37, 0x2D, 0x00, 0x18, 0x00, 0x00, 0x21, 0x08, 0x23, 0x04, 0x44, 0x04, 0x66, 0x08, 0x89, 0x08, 0xAA, 0x0C, 0xCC, 0x14, 0x0F, 0x19, 0x31, 0x1D, 0x52, 0x21, 0x73, 0x29, 0x95, 0x2D, 0xB5, 0x31, 0xB6, 0x3D, 0x00, 0x18, 0x00, 0x00, 0x20, 0x00, 0x40, 0x00, 0x60, 0x00, 0x80, 0x00, 0xA0, 0x00, 0xC0, 0x00, 0x00, 0x01, 0x20, 0x01, 0x41, 0x01, 0x82, 0x01, 0xC3, 0x05, 0x05, 0x06, 0x46, 0x06, 0x69, 0x0E, 0x00, 0x18, 0x01, 0x00, 0x23, 0x04, 0x23, 0x04, 0x25, 0x04, 0x46, 0x04, 0x67, 0x04, 0x69, 0x04, 0x8A, 0x04, 0xAC, 0x08, 0xAD, 0x08, 0xCF, 0x08, 0xD0, 0x0C, 0x12, 0x0D, 0x13, 0x29, 0x55, 0x3D, 0x00, 0x18, 0x00, 0x00, 0x22, 0x04, 0x43, 0x08, 0x63, 0x0C, 0xA4, 0x14, 0xA6, 0x18, 0xC6, 0x1C, 0x08, 0x21, 0xC6, 0x18, 0xA6, 0x14, 0x86, 0x0C, 0x88, 0x04, 0x87, 0x08, 0x65, 0x08, 0x23, 0x04, 0x00, 0x18, 0x00, 0x00, 0x22, 0x04, 0x43, 0x04, 0x64, 0x04, 0x66, 0x04, 0x87, 0x04, 0xA9, 0x04, 0xA8, 0x04, 0xC6, 0x04, 0x06, 0x05, 0x04, 0x05, 0xE3, 0x04, 0xC3, 0x04, 0xA3, 0x04, 0x63, 0x08, 0x00, 0x18, 0x21, 0x00, 0x61, 0x04, 0xA3, 0x04, 0xC3, 0x04, 0x25, 0x05, 0x66, 0x05, 0xC7, 0x05, 0xC8, 0x05, 0xE9, 0x05, 0x66, 0x08, 0xAA, 0x0C, 0x0F, 0x19, 0x52, 0x21, 0x95, 0x2D, 0xB6, 0x3D};
        memcpy(&pal_data[0], &new_pal[0], 0x100);
    } // Orang-utan Gang
    if (fix == 58) {
        unsigned char new_pal[] = {0x00, 0x00, 0x00, 0x00, 0x87, 0x21, 0x4B, 0x36, 0x00, 0x00, 0x64, 0x14, 0x4B, 0x2D, 0xF0, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x24, 0x04, 0x45, 0x08, 0x66, 0x0C, 0xA6, 0x14, 0xA7, 0x14, 0xA8, 0x14, 0xC9, 0x18, 0x29, 0x25, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x21, 0x04, 0x41, 0x08, 0x62, 0x0C, 0x83, 0x10, 0xA4, 0x14, 0xC5, 0x18, 0x06, 0x21, 0x27, 0x25, 0x47, 0x29, 0x68, 0x2D, 0x88, 0x31, 0xA9, 0x35, 0xA9, 0x35, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x40, 0x08, 0x60, 0x0C, 0x80, 0x10, 0xA0, 0x14, 0xC0, 0x18, 0x00, 0x21, 0x20, 0x25, 0x40, 0x29, 0x81, 0x31, 0xC1, 0x39, 0x02, 0x42, 0x42, 0x4A, 0x63, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0x21, 0x04, 0x22, 0x04, 0x42, 0x08, 0x63, 0x0C, 0x63, 0x0C, 0x84, 0x10, 0xA5, 0x14, 0xA5, 0x14, 0xC6, 0x18, 0xC6, 0x18, 0x07, 0x21, 0x08, 0x21, 0x48, 0x29, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0x41, 0x08, 0x61, 0x0C, 0xA1, 0x14, 0xA2, 0x14, 0xC2, 0x18, 0x03, 0x21, 0xC2, 0x18, 0xA2, 0x14, 0x82, 0x10, 0x83, 0x10, 0x83, 0x10, 0x62, 0x0C, 0x21, 0x04, 0x00, 0x00, 0x00, 0x00, 0x21, 0x04, 0x41, 0x08, 0x61, 0x0C, 0x62, 0x0C, 0x83, 0x10, 0xA3, 0x14, 0xA3, 0x14, 0xC2, 0x18, 0x02, 0x21, 0x01, 0x21, 0xE1, 0x1C, 0xC1, 0x18, 0xA1, 0x14, 0x61, 0x0C, 0x00, 0x00, 0x20, 0x04, 0x60, 0x0C, 0xA1, 0x14, 0xC1, 0x18, 0x22, 0x25, 0x62, 0x2D, 0xC3, 0x39, 0xC3, 0x39, 0xE3, 0x3D, 0x62, 0x0C, 0xA4, 0x14, 0x06, 0x21, 0x47, 0x29, 0x88, 0x31, 0xA9, 0x35};
        memcpy(&pal_data[0], &new_pal[0], 0x100);
    } // Ropey Rampage
    if (fix == 65) {
        unsigned char new_pal[] = {
        0x02, 0x24, 0x21, 0x04, 0x43, 0x04, 0x65, 0x04, 0x62, 0x14, 0x21, 0x04, 0x64, 0x04, 0xA7, 0x08, 0x62, 0x14, 0x65, 0x04, 0x87, 0x04, 0xEB, 0x08, 0x62, 0x14, 0x21, 0x04, 0x63, 0x08, 0xA5, 0x08, 0x02, 0x24, 0x22, 0x24, 0x45, 0x24, 0x67, 0x28, 0x8F, 0x34, 0xCE, 0x24, 0x33, 0x19, 0x76, 0x0D, 0xD8, 0x0D, 0x1A, 0x12, 0x7B, 0x12, 0xDD, 0x16, 0x1D, 0x23, 0x5E, 0x2F, 0x9E, 0x3B, 0xDE, 0x47, 0xAD, 0x21, 0x00, 0x00, 0x22, 0x00, 0x43, 0x04, 0x64, 0x04, 0x65, 0x04, 0x87, 0x08, 0xA8, 0x08, 0xC9, 0x08, 0xEA, 0x08, 0x0C, 0x0D, 0x2D, 0x0D, 0x4E, 0x0D, 0x6F, 0x11, 0x71, 0x11, 0x92, 0x11, 0xAD, 0x21, 0xCF, 0x21, 0x6C, 0x1D, 0x09, 0x15, 0xC7, 0x10, 0x84, 0x0C, 0x63, 0x08, 0x21, 0x04, 0x6F, 0x11, 0x0B, 0x0D, 0xC8, 0x08, 0x0D, 0x0D, 0xEB, 0x08, 0xA9, 0x08, 0x87, 0x04, 0x65, 0x04, 0xAD, 0x21, 0x63, 0x0C, 0xA5, 0x18, 0x06, 0x29, 0x45, 0x04, 0x4A, 0x04, 0x70, 0x08, 0xE9, 0x0C, 0x0C, 0x09, 0x2F, 0x09, 0x72, 0x0D, 0xF4, 0x15, 0x00, 0x00, 0x42, 0x04, 0x63, 0x08, 0xA5, 0x0C, 0xAD, 0x21, 0x64, 0x04, 0xA8, 0x08, 0x0F, 0x0D, 0x31, 0x0D, 0x74, 0x11, 0xD4, 0x19, 0x21, 0x00, 0x23, 0x00, 0x67, 0x08, 0xA9, 0x08, 0xEC, 0x0C, 0x65, 0x0C, 0x21, 0x04, 0x86, 0x08, 0xC8, 0x08, 0xAD, 0x21, 0x62, 0x00, 0x84, 0x00, 0xC6, 0x00, 0x2A, 0x01, 0xAE, 0x01, 0x12, 0x06, 0x00, 0x00, 0x22, 0x00, 0x64, 0x04, 0x87, 0x08, 0xC8, 0x08, 0x2D, 0x0D, 0x21, 0x04, 0x63, 0x08, 0x84, 0x0C, 0xAD, 0x21, 0x21, 0x04, 0x43, 0x04, 0x65, 0x04, 0xC7, 0x0C, 0xE9, 0x0C, 0x0C, 0x11, 0x6E, 0x15, 0xB0, 0x19, 0x45, 0x04, 0x4A, 0x04, 0x70, 0x08, 0x86, 0x08, 0xA4, 0x00, 0xA8, 0x08, 0x0C, 0x0D};
        memcpy(&pal_data[0], &new_pal[0], 0x100);
    } // Gangplank Galley (DKC2)
    
    if (fix >= 66 && fix <= 96) {
        unsigned char new_pal[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0x10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,8,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0x10,0,0x20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,8,0,0x18,0,0x28,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0x10,0,0x20,0,0x30,0,0,0,0,0,0,0,0x10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0x18,0,0x28,0,0x38,0,0,0,0,0,0,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0x20,0,0x30,0,0x40,0,0,0,0,0,8,0,0x20,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,
        0,0x28,0,0x38,0,0x48,0,0,0,0,0,0x10,0,0x28,0,0,0,0,0,0,0,0x10,0,0,0,0,0,0,0,0,
        0,0x30,0,0x40,0,0x50,0,0,0,4,0,0x18,0,0x30,0,0,0,0,0,4,0,0x18,0,0,0,0,0,0,0,0,
        0,0x38,0,0x48,0,0x58,0,0,0,0x0C,0,0x20,0,0x38,0,0,0,0,0,0x0C,0,0x20,0,0,0,0,0,0,0,0,
        0,0x40,0,0x50,0x63,0x60,0,0,0,0x14,0,0x28,0,0x40,0,0,0,0,0,0x14,0,0x28,0,0,0,0,0,0,0,0,
        0x63,0x48,0xE7,0x58,0x6B,0x69,0,0,0,0x1C,0,0x30,0x63,0x48,0,0,0,8,0,0x1C,0,0x30,0,0,0,0,0,0,0,8,
        0x6B,0x51,0xEF,0x61,0x73,0x72,0,0,0,0x24,0xA5,0x38,0x6B,0x51,0,0,0,0x10,0,0x24,0xA5,0x38,0,0,0,0,0,4,0,0x10,
        0x73,0x5A,0xF7,0x6A,0x7B,0x7B,0,0,8,0x2D,0xAD,0x41,0x73,0x5A,0,0,0x63,0x18,8,0x2D,0xAD,0x41,0,0,0,4,0,0x0C,0x63,0x18,
        0xF7,0x5E,0x7B,0x6F,0xFF,0x7F,0,0,0x8C,0x31,0x31,0x46,0xF7,0x5E,0,0,0xE7,0x1C,0x8C,0x31,0x31,0x46,0,0,0x42,8,0x84,0x10,0xE7,0x1C,
        0xEF,0x55,0x73,0x66,0xF7,0x76,0,0,0x84,0x28,0x29,0x3D,0xEF,0x55,0,0,0,0x14,0x84,0x28,0x29,0x3D,0,0,0,0,0,8,0,0x14,
        0xE7,0x4C,0x6B,0x5D,0xEF,0x6D,0,0,0,0x20,0x21,0x34,0xE7,0x4C,0,0,0,0x0C,0,0x20,0x21,0x34,0,0,0,0,0,0,0,0x0C,
        0,0x44,0x63,0x54,0xE7,0x64,0,0,0,0x18,0,0x2C,0,0x44,0,0,0,4,0,0x18,0,0x2C,0,0,0,0,0,0,0,4,
        0,0x3C,0,0x4C,0,0x5C,0,0,0,0x10,0,0x24,0,0x3C,0,0,0,0,0,0x10,0,0x24,0,0,0,0,0,0,0,0,
        0,0x34,0,0x44,0,0x54,0,0,0,8,0,0x1C,0,0x34,0,0,0,0,0,8,0,0x1C,0,0,0,0,0,0,0,0,
        0,0x2C,0,0x3C,0,0x4C,0,0,0,0,0,0x14,0,0x2C,0,0,0,0,0,0,0,0x14,0,0,0,0,0,0,0,0,
        0,0x24,0,0x34,0,0x44,0,0,0,0,0,0x0C,0,0x24,0,0,0,0,0,0,0,0x0C,0,0,0,0,0,0,0,0,
        0,0x1C,0,0x2C,0,0x3C,0,0,0,0,0,4,0,0x1C,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,
        0,0x14,0,0x24,0,0x34,0,0,0,0,0,0,0,0x14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0x0C,0,0x1C,0,0x2C,0,0,0,0,0,0,0,0x0C,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,4,0,0x14,0,0x24,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0x0C,0,0x1C,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,4,0,0x14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0x0C,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        memcpy(&pal_data[2], &new_pal[(fix-66)*0x1E], 0x1E);
    } // Mine Sparkles (DKC2)
    if (fix >= 97 && fix <= 129) {
        unsigned char new_pal[] = {
            0,0x10,2,0x28,4,0x44,6,0x68,0x29,0x3C,3,0x18,
            0,0x10,2,0x28,4,0x44,6,0x68,0x29,0x3C,3,0x18,
            0,0x0C,2,0x24,4,0x40,6,0x64,9,0x38,3,0x14,
            0,0x0C,0,0x24,2,0x40,4,0x64,7,0x38,1,0x14,
            0,0x0C,0,0x24,2,0x40,4,0x64,7,0x38,1,0x14,
            0,8,0,0x20,2,0x3C,4,0x60,7,0x34,1,0x10,
            0,8,0,0x20,2,0x3C,4,0x60,7,0x34,1,0x10,
            0,8,0,0x20,1,0x3C,3,0x60,6,0x34,0,0x10,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,4,0,0x1C,1,0x38,3,0x5C,6,0x30,0,0x0C,
            0,8,0,0x20,1,0x3C,3,0x60,6,0x34,0,0x10,
            0,8,0,0x20,2,0x3C,4,0x60,7,0x34,1,0x10,
            0,8,0,0x20,2,0x3C,4,0x60,7,0x34,1,0x10,
            0,0x0C,0,0x24,2,0x40,4,0x64,7,0x38,1,0x14,
            0,0x0C,0,0x24,2,0x40,4,0x64,7,0x38,1,0x14,
            0,0x0C,2,0x24,4,0x40,6,0x64,9,0x38,3,0x14,
            0,0x0C,2,0x24,4,0x40,6,0x64,9,0x38,3,0x14,
            0,0x10,2,0x28,4,0x44,6,0x68,0x29,0x3C,3,0x18,
            0,0x10,2,0x28,4,0x44,6,0x68,0x29,0x3C,3,0x18,
            1,0x10,3,0x28,5,0x44,7,0x68,0x2A,0x3C,4,0x18,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
            1,0x14,3,0x2C,0x25,0x48,0x27,0x6C,0x4A,0x40,0x24,0x1C,
        };
        /*
        unsigned char new_pal[] = {
            1,0,0x6C,0,0xF1,0,0xB5,1,0xEE,0,0x48,0,
            1,0,0x6C,0,0xF1,0,0xB5,1,0xEE,0,0x48,0,
            1,0,0x6C,0,0xF1,0,0xB5,1,0xEE,0,0x48,0,
            1,0,0x8C,0,0x11,1,0xD5,1,0x0E,1,0x68,0,
            2,0,0x8D,0,0x12,1,0xD6,1,0x0F,1,0x69,0,
            2,0,0x8D,0,0x12,1,0xD6,1,0x0F,1,0x69,0,
            2,0,0xAD,0,0x32,1,0xF6,1,0x2F,1,0x89,0,
            2,0,0xAD,0,0x32,1,0xF6,1,0x2F,1,0x89,0,
            4,0,0xAF,0,0x34,1,0xF8,1,0x31,1,0x8B,0,
            4,0,0xAF,0,0x34,1,0xF8,1,0x31,1,0x8B,0,
            4,0,0xCF,0,0x54,1,0x18,2,0x51,1,0xAB,0,
            4,0,0xCF,0,0x54,1,0x18,2,0x51,1,0xAB,0,
            5,0,0xD0,0,0x55,1,0x19,2,0x52,1,0xAC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            5,0,0xF0,0,0x75,1,0x39,2,0x72,1,0xCC,0,
            4,0,0xCF,0,0x54,1,0x18,2,0x51,1,0xAB,0,
            4,0,0xCF,0,0x54,1,0x18,2,0x51,1,0xAB,0,
            4,0,0xCF,0,0x54,1,0x18,2,0x51,1,0xAB,0,
            4,0,0xAF,0,0x34,1,0xF8,1,0x31,1,0x8B,0,
            4,0,0xAF,0,0x34,1,0xF8,1,0x31,1,0x8B,0,
            2,0,0xAD,0,0x32,1,0xF6,1,0x2F,1,0x89,0,
            2,0,0xAD,0,0x32,1,0xF6,1,0x2F,1,0x89,0,
            2,0,0x8D,0,0x12,1,0xD6,1,0x0F,1,0x69,0,
            1,0,0x8C,0,0x11,1,0xD5,1,0x0E,1,0x68,0,
            1,0,0x8C,0,0x11,1,0xD5,1,0x0E,1,0x68,0,
            1,0,0x8C,0,0x11,1,0xD5,1,0x0E,1,0x68,0,
        }; */
        memcpy(&pal_data[0x42], &new_pal[(fix-97)*0x0C], 0x0C);
    } // Mine Sparkles (Gold)

} // colourfix_pre();

static void colourfix_post(unsigned char *rgb, int fix) {

    int i;
    
    if (fix == 1) {
        for (i = 0; i < 384; i+=3) {
            rgb[i] ^= 248; // R
            rgb[i+1] ^= 248; // G
            rgb[i+2] ^= 248; // B
        }
    } // Invert Palette (Glimmer's Galleon)
    if (fix == 9) {
        for (i = 0; i < 384; i+=3) {
            if (40 - rgb[i] > 0) {
                rgb[i] = 40 - rgb[i];
            }
            else {
                rgb[i] = 0;
            }
            if (72 - rgb[i+1] > 0) {
                rgb[i+1] = 72 - rgb[i+1];
            }
            else {
                rgb[i+1] = 0;
            }
            if (104 - rgb[i+2] > 0) {
                rgb[i+2] = 104 - rgb[i+2];
            }
            else {
                rgb[i+2] = 0;
            }
        }
    } // Dark Palette (Floodlit Fish)
    if (fix == 10) {
        for (i = 0; i < 384; i+=3) {
            if (152 - rgb[i] > 0) {
                rgb[i] = 152 - rgb[i];
            }
            else {
                rgb[i] = 0;
            }
            if (168 - rgb[i+1] > 0) {
                rgb[i+1] = 168 - rgb[i+1];
            }
            else {
                rgb[i+1] = 0;
            }
            if (176 - rgb[i+2] > 0) {
                rgb[i+2] = 176 - rgb[i+2];
            }
            else {
                rgb[i+2] = 0;
            }
        }
    } // Light Palette (Floodlit Fish)
    if (fix == 11) {
        for (i = 0; i < 384; i+=3) {
            rgb[i] = 248 - rgb[i];
            rgb[i+1] = 248 - rgb[i+1];
            rgb[i+2] = 248 - rgb[i+2];
        }
    } // Light Palette (Glimmer's Galleon)
    if (fix == 12) {
        for (i = 0; i < 384; i+=3) {
            if (192 - rgb[i] > 0) {
                rgb[i] = 192 - rgb[i];
            }
            else {
                rgb[i] = 0;
            }
            if (176 - rgb[i+1] > 0) {
                rgb[i+1] = 176 - rgb[i+1];
            }
            else {
                rgb[i+1] = 0;
            }
            if (192 - rgb[i+2] > 0) {
                rgb[i+2] = 192 - rgb[i+2];
            }
            else {
                rgb[i+2] = 0;
            }
        }
    } // Dark Palette (Glimmer's Galleon)
    if (fix == 52 || fix == 53) {
        for (i = 0; i < 384; i+=3) {
            if (48 + rgb[i+2] < 248) {
                rgb[i+2] = 48 + rgb[i+2];
            }
            else {
                rgb[i+2] = 248; // Assumes colour math limit is 248. (0x1F * 8 = 248)
            }
        }
    } // Bazza's Blockade & Fish Food Frenzy // Lockjaw's Locker & Lava Lagoon (UW)
    if (fix == 53) {
        for (i = 0; i < 384; i+=3) {
            if (16 + rgb[i+1] < 248) {
                rgb[i+1] = 16 + rgb[i+1];
            }
            else {
                rgb[i+1] = 248; // See above.
            }
        }
    } // Lockjaw's Locker & Lava Lagoon (UW)
    if (fix == 54) {
        for (i = 0; i < 384; i+=3) {
            rgb[i] = 248;
        }
    } // Lava Lagoon (Lava)
    if (fix == 55) {
        for (i = 0; i < 384; i+=3) {
            if (40 + rgb[i+1] < 248) {
                rgb[i+1] = 40 + rgb[i+1];
            }
            else {
                rgb[i+1] = 248;
            }
            if (24 + rgb[i+2] < 248) {
                rgb[i+2] = 24 + rgb[i+2];
            }
            else {
                rgb[i+2] = 248;
            }
            rgb[i] = ((rgb[i] / 8) / 2) * 8;
            rgb[i+1] = ((rgb[i+1] / 8) / 2) * 8; // Colour math "Half mode".
            rgb[i+2] = ((rgb[i+2] / 8) / 2) * 8;
        }
    } // Slime Climb (UW)
    if (fix == 56) {
        for (i = 0; i < 384; i+=3) {
            rgb[i+1] = 248;
        }
    } // Toxic Tower (UW)
    
    
    if (fix == 59) {
        for (i = 0; i < 384; i++) {
            if (rgb[i] > 23) {
                rgb[i] -= 24;
            }
            else {
                rgb[i] = 0;
            }
        }
    } // Loopy Lights (subtract 030303)
    if (fix == 60) {
        for (i = 0; i < 384; i++) {
            if (rgb[i] > 15) {
                rgb[i] -= 16;
            }
            else {
                rgb[i] = 0;
            }
        }
    } // Bouncy Bonanza (subtract 020202)
    if (fix == 61) {
        for (i = 0; i < 384; i++) {
            if ((i % 3) == 2) continue;
            if (rgb[i] > 15) {
                rgb[i] -= 16;
            }
            else {
                rgb[i] = 0;
            }
        }
    } // Reptile Rumble B3 (subtract 020200)
    if (fix == 62) {
        for (i = 0; i < 384; i++) {
            if (rgb[i] > 31) {
                rgb[i] -= 32;
            }
            else {
                rgb[i] = 0;
            }
            rgb[i] /= 2;
        }
    } // Jungle BG (subtract 040404 and half)
    if (fix == 63) {
        for (i = 0; i < 384; i++) {
            if (rgb[i] > 31) {
                rgb[i] -= 32;
            }
            else {
                rgb[i] = 0;
            }
        }
    } // Jungle BG, Blackout Basement (subtract 040404)
    if (fix == 64) {
        for (i = 0; i < 384; i+=3) {
            if (rgb[i] < 217) rgb[i] += 32;
            if (rgb[i+1] < 201) rgb[i+1] += 48;
        }
    } // Ripsaw Rage External Saw (add 040600)

} // colourfix_post();

static void decode_tile(unsigned char *bp_data, unsigned char *raw_data, unsigned char *rgb, unsigned char *bitplane, int raw_address, int bp_ofs, int bp_len, int mode, int bg) {
    
    unsigned int low =  raw_data[raw_address+1];
    unsigned int high = raw_data[raw_address];
    
    int pal_ofs = 0, vflip = 0, hflip = 0, priority = 0;
    
    if (low & 1)   high += 256;
    if (low & 2)   high += 512;
    if (low & 4)   pal_ofs += 1;
    if (low & 8)   pal_ofs += 2;
    if (low & 16)  pal_ofs += 4;
    if (low & 32)  priority = 1;
    if (low & 64)  hflip = 1;
    if (low & 128) vflip = 1;
    
    int img_ofs;
    
    if (mode == 3) {
        img_ofs = high * 16;
    } // 2bpp
    else {
        img_ofs = high * 32;
    } // 4bpp
    
    int override = 0;
    if (img_ofs > bp_len) override = 1; // Edge case. (Skidda's Row)
    
    if (mode == 3) {
        pal_ofs *= 4;
    } // 2bpp
    else {
        pal_ofs *= 16;
    } // 4bpp
    
    int i, j;
    
    unsigned int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    
    int out_ofs = 128;
    if (mode != 1) out_ofs = 32;
    
    unsigned int xpos = 0, ypos = 0;
    
    unsigned char one, two, three, four;
    
    for (i = 0; i < 8; i++) {
        one = bp_data[img_ofs+(i*2)];
        two = bp_data[img_ofs+(i*2)+1];
        if (mode != 3) {
            three = bp_data[img_ofs+(i*2)+16];
            four  = bp_data[img_ofs+(i*2)+17];
        } // 4bpp
        
        if (vflip == 1) i = 7 - i;
        
        for (j = 0; j < 8; j++) {
        
            int value = 0;
            
            if ( (one   & bits[j]) == bits[j]) value += 1;
            if ( (two   & bits[j]) == bits[j]) value += 2;
            if (mode != 3) {
                if ( (three & bits[j]) == bits[j]) value += 4;
                if ( (four  & bits[j]) == bits[j]) value += 8;
            } // 4bpp
            if (override == 1) {
                value = 0;
            }
            
            if (xpos >= 8) {
                xpos = 0;
                ypos++;
            }
            if (ypos >= 8) ypos = 0;
            
            if (hflip == 1) j = 7 - j;
            if (bg == 2 && priority == 0) {
                bitplane[bp_ofs+(i*out_ofs)+(j*4)]   = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+1] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+2] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+3] = 0;
            } // Don't draw background tiles.
            else if (bg == 3 && priority == 1) {
                bitplane[bp_ofs+(i*out_ofs)+(j*4)]   = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+1] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+2] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+3] = 0;
            } // Don't draw foreground tiles.
            else if (bg == 1 && value == 0) {
                bitplane[bp_ofs+(i*out_ofs)+(j*4)]   = rgb[0];
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+1] = rgb[1];
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+2] = rgb[2];
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+3] = 255;
            } // Opaque Pixel (For special screen backgrounds)
            else if (value == 0 || value % 16 == 0) {
                bitplane[bp_ofs+(i*out_ofs)+(j*4)]   = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+1] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+2] = 255;
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+3] = 0;
            } // Transparent Pixel (For almost everything else)
            else {
                bitplane[bp_ofs+(i*out_ofs)+(j*4)]   = rgb[(pal_ofs+value)*3];   // R
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+1] = rgb[(pal_ofs+value)*3+1]; // G
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+2] = rgb[(pal_ofs+value)*3+2]; // B
                bitplane[bp_ofs+(i*out_ofs)+(j*4)+3] = 255; // A
            } // Normal
            xpos++;
            
            if (hflip == 1) j = 7 - j;
        }
        if (vflip == 1) i = 7 - i;
    
    }
    /*
    if (override == 1) {
        printf("value = %d, img_ofs = %d, high = %d, bp_len = %d.\n", value, img_ofs, high, bp_len);
        getchar();
    }
    */
    return;

} // decode_tile();

void decode_bitplane(unsigned char *rom, unsigned char *bp_data, unsigned char *raw_data, unsigned char *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg) {

    unsigned int i, j, k;
    unsigned char *rgb = malloc(384);
    unsigned char *pal_data = malloc(0x200);
    // memset(rgb, 0, 128*3);
    
    memcpy(&pal_data[0], &rom[pal_addr], 0x200);
    
    if (fix) colourfix_pre(rom, pal_data, fix); // Affects SNES palette data.
    
    decode_palette(rgb, pal_data, 128);
    free(pal_data);
    
    if (fix) colourfix_post(rgb, fix); // Affects converted colours.
    
    // End of colour manipulation hell.
    
    if (mode == 1) {
        raw_len /= 32;
        for (i = 0; i < raw_len; i++) {
            for (j = 0; j < 4; j++) {
                for (k = 0; k < 4; k++) {
                    decode_tile(bp_data, raw_data, rgb, bitplane, ((i*32)+(j*2)+(k*8)), ((i*4096)+(j*32)+(k*1024)), bp_len, 1, bg);
                }
            }
        }
    } // Level
    else if (mode == 2 || mode == 3) {
        raw_len /= 2;
        for (i = 0; i < raw_len; i++) {
            decode_tile(bp_data, raw_data, rgb, bitplane, i*2, i*256, bp_len, mode, bg);
        }
    } // 2 = Screen 4bpp, 4 = Screen 2bpp
    else {
        printf("Abnormal termination.");
        exit(1);
    }
    
    free(rgb);
    // printf("Bitplane complete.\n");
    return;
    
} // decode_bitplane();

void dump_bitplane(unsigned char *bp_data, int bp_len, int bpp, int width, char *dir, char *name) {

    const int tile_count = (bpp == 4) ? (bp_len/32) : (bp_len/16);
    
    int px_height = ((tile_count-(tile_count%width)) / width)*8;
    if (tile_count % width) px_height += 8;
    int px_width  = width  * 8;
    int img_size = (px_width * px_height * 4);
    unsigned char *image = malloc(img_size);
    memset(image, 0, img_size);
    int value, offset = 0, tile_n = 0, i, j, k;
    
    const unsigned int bits[] = {128, 64, 32, 16, 8, 4, 2, 1};
    unsigned char byte[4];
    
    for (i = 0; i < tile_count; i++) {
        
        tile_n = (i%width);
        if (i > width-1) {
            offset = ((i-(i%width))/width) * (px_width*8*4);
        }
        
        
    
        for (j = 0; j < 8; j++) {
            byte[0] = bp_data[i*(bpp*8)+(j*2)];
            byte[1] = bp_data[i*(bpp*8)+(j*2)+1];
            if (bpp == 4) {
                byte[2] = bp_data[i*(bpp*8)+(j*2)+16];
                byte[3] = bp_data[i*(bpp*8)+(j*2)+17];
            }
            
            for (k = 0; k < 8; k++) {
                value = 0;
                
                if ( (byte[0] & bits[k]) == bits[k]) value += 1;
                if ( (byte[1] & bits[k]) == bits[k]) value += 2;
                if (bpp == 4) {
                    if ( (byte[2] & bits[k]) == bits[k]) value += 4;
                    if ( (byte[3] & bits[k]) == bits[k]) value += 8;
                }
                
                if (bpp == 4) {
                    value *= 16;
                }
                else if (bpp == 2) {
                    value *= 64;
                }
                
                image[offset+(j*px_width*4)+(tile_n*32)+(k*4)  ] = value;
                image[offset+(j*px_width*4)+(tile_n*32)+(k*4)+1] = value;
                image[offset+(j*px_width*4)+(tile_n*32)+(k*4)+2] = value;
                if (value == 0) {
                    image[offset+(j*px_width*4)+(tile_n*32)+(k*4)+3] = 0;
                }
                else {
                    image[offset+(j*px_width*4)+(tile_n*32)+(k*4)+3] = 255;
                }
                
            } // Each plane
        } // Each line
    } // Each Tile
    
    write_png(dir, name, image, px_width, px_height);
    free(image);
    
    return;

} // dump_bitplane();

void assemble_bitplane(unsigned char *bitplane, unsigned int width, unsigned int raw_len, char *dir, char *name) {

    unsigned int height = (((raw_len/32) - ((raw_len/32) % (width/32))) / (width/32))*32;
    if ((raw_len/32) % (width/32)) height += 32;

    unsigned char *out = malloc(width * height * 4);
    
    unsigned int i, j, k, m = 0;
    
    for (i = 0; i < height/32; i++) {
        
        unsigned int tile_row = i * (width) * 4 * 32;
        
        for (j = 0; j < width/32; j++) {
        
            unsigned int tile_column = j * 128;
            
            for (k = 0; k < 32; k++) {
            
                unsigned int pixel = k * (width) * 4 + tile_row + tile_column;
                
                if (m >= (raw_len/32)) {
                    memset(&out[pixel], 0, 128);
                }
                else {
                    memcpy(&out[pixel], &bitplane[(m*4096)+(k*128)], 128);
                }
            } // 32 rows per tile.
                m++;
        } // j
    } // i
    
    char path[255];
    sprintf(path, "%s Tiles", name);
    write_png(dir, path, out, width, height);
    free(out);
    
} // assemble_bitplane();

void assemble_screen(unsigned char *bitplane, unsigned int raw_len, unsigned int width, char *dir, char *name) {

    unsigned int height = (raw_len / 2) / width;
    
    /*
    256 /  8 = 32
    224 /  8 = 28
    32  * 28 = 896
    896 *  2 = 1792
    */
    
    unsigned char *out = malloc(width * height * 32 * 8 * 4);
    unsigned int i, j, k;
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            for (k = 0; k < 8; k++) {
                memcpy(&out[(i*8192)+(j*32)+(k*1024)], &bitplane[(i*8192)+(j*256)+(k*32)], 32);
            } // Columns
        } // Each column
    } // Each row
    
    write_png(dir, name, out, (width*8), (height*8));
    free(out);
    
} // assemble_screen();

static unsigned int get_position(unsigned char *rom, int address) {

    int high = rom[address+1];
    int low  = rom[address];
    
    int coordinate = ((high*256)+low);
    
    return coordinate;

} // get_position();

void assemble_level(unsigned char *bitplane, unsigned char *rom, unsigned char *layout, unsigned int layout_len, int position_addr, int vert, int layout_size, int fix, char *dir, char *name) {

    /*
    1 = Riverside Level (DKC3). Crop top two rows of tiles (64px).
    */
    
    
    unsigned int i, j, k, k2, k3, k4, m, n;
    
    unsigned int left, right, top, bottom;
    unsigned int px_left, px_right, px_top, px_bottom, crop_left, crop_right, crop_top, crop_bottom;
    
    left   = 0;
    right = layout_size;
    top    = 0;
    bottom  = (layout_len/2)/layout_size;
    px_left = 0;
    px_top = 0;
    px_right = right*32;
    px_bottom = bottom*32;
    
    if (!vert) {
        int temp = px_right;
        px_right = px_bottom;
        px_bottom = temp;
    }
    
    unsigned int width  = (px_right  - px_left);
    unsigned int height = (px_bottom - px_top);
    
    unsigned char *group = malloc(width * height * 4);
    
    
    for (i = 0; i < height; i+=32) {
        
        unsigned int tile_row = (i/32) * width * 4 * 32;
        
        for (j = 0; j < width; j+=32) {
        
            unsigned int tile_column = (j/32) * 128;
            unsigned int high; // Modifier
            unsigned int low;  // Tile
            int vflip = 0;
            int hflip = 0;
            
            if (!vert) {
                high = layout[(((j/32)+left)*layout_size*2)+(((i/32)+top)*2)+1];
                low  = layout[(((j/32)+left)*layout_size*2)+(((i/32)+top)*2)];
            }
            else {
                high = layout[(((i/32)+top)*layout_size*2)+(((j/32)+left)*2)+1];
                low  = layout[(((i/32)+top)*layout_size*2)+(((j/32)+left)*2)];
            }
            
            if (high & 1)   low += 256;
            if (high & 2)   low += 512;
            if (high & 64)  vflip = 1;
            if (high & 128) {
                k = 31;
                k2 = -1;
                hflip = 1;
            }
            else {
                k = 0;   // k  = Start
                k2 = 32; // k2 = Finish
                hflip = 0;
            }
            k3 = 0; // k3 = row_counter.
            if ( px_right % 32 && width-j < 32 ) {
                k4 = width-j;
            } // Intercept right edge, but not the left.
            else {
                k4 = 32; // How many pixels to copy. (final column &< 32)
            }
            
            while (k != k2) {
            
                // if (hflip == 1) k = (31 - k);
                unsigned int pixel = k * width * 4 + tile_row + tile_column;
                
                if (vflip == 1) {
                    for (m = 0; m < k4; m++) {
                        n = 31 - m;
                        memcpy(&group[pixel + (n*4)], &bitplane[(low*4096)+(k3*128)+(m*4)], 4);
                    }
                } // 32 pixels per row.
                else {
                    memcpy(&group[pixel], &bitplane[(low*4096)+(k3*128)], (k4*4));
                }
                
                if (hflip == 1) {
                    k--;
                }
                else {
                    k++;
                }
                k3++;
                
            } // 32 rows per tile.
        } // j
    } // i
    
    if (position_addr) {
        
        if (rom[0] == 0xFF && rom[1] == 0xFF) {
            crop_left   = (rom[3] * 256) + rom[2];
            crop_right  = (rom[5] * 256) + rom[4];
            crop_top    = (rom[7] * 256) + rom[6];
            crop_bottom = (rom[9] * 256) + rom[8]; // For DKC
        }
        else {
            crop_left   = get_position(rom, position_addr);
            crop_right  = get_position(rom, position_addr+2);
            crop_top    = get_position(rom, position_addr+4);
            crop_bottom = get_position(rom, position_addr+6); // For DKC2 & DKC3
        }
        
        if (fix == 1) crop_top = 64; // Riverside (DKC3)
        
        if (crop_right > px_right) {
            printf("Invalid coordinates in level %s.\n", name);
            getchar();
            return;
        } // Extra rows are easier to handle than extra columns.
        
        unsigned int level_width = crop_right - crop_left;
        unsigned int level_height = crop_bottom - crop_top;
        unsigned char *level = malloc(level_width * level_height * 4);
        
        for (i = 0; i < level_height; i++) {
        
            if (i >= height) {
                memset(&level[i*level_width*4], 0, ((level_height-height)*level_width*4));
                break;
            }
        
            memcpy(&level[i*level_width*4], &group[(((i+crop_top)*width)+crop_left)*4], level_width*4);
        }
        free(group);
        width = level_width;
        height = level_height;
        group = level;
    }
    
    write_png(dir, name, group, width, height);
    free(group);

} // assemble_level();

void bbc_pal(unsigned char *rom, unsigned char *palette, int seed) {

/*

// Palettes for Banana Bird Caves

(A in $80:C05D)

List of known "seeds":

10 - K's Kache 0
11 - Hill-Top Hoard 2
12 - Undercover Cove 4
13 - Kong Cove 6
14 - Bounty Beach 0
15 - Belcha's Burrow 2
16 - Smuggler's Cove 4
17 - Arich's Hoard 6
18 - Bounty Bay 0
19 - Sky-High Secret 2
1A - Glacial Grotto 4
1B - Clifftop Cache 6
1C - Sewer Stockpile 0

*/

    unsigned char a = 0, b = 0, run = 1, t1A = 0, t1B = 0, t1C = 0, t1D = 0, t1E = 0, t1F = 0, t20 = 0, t21 = 0;
    int c = 0, x = 0, y = 0;
    int origin[5];
    int recurse = 0; // Stack
    int jmp;
    char temp;
    
    a = seed;
    b = 0;
    a &= 0x0C;
    a >>= 1;
    x = a;
    if (rom[0xFFD9] == 1) {
        a = rom[0xC0A8 + x];
        b = rom[0xC0A8 + x + 1]; // NA
    }
    else if (rom[0xFFD9] == 2) {
        a = rom[0xC0DE + x];
        b = rom[0xC0DE + x + 1]; // EU
    }
    else {
        if (rom[0xFFDB] == 0) {
            a = rom[0xC06C + x];
            b = rom[0xC06C + x + 1]; // JP Rev0
        }
        else {
            a = rom[0xC08E + x];
            b = rom[0xC08E + x + 1]; // JP Rev1
        }
    }
    t1E = a;
    t1F = b;
    a ^= 0xFF;
    b ^= 0xFF;
    t20 = a;
    t21 = b;
    a = seed;
    b = 0;
    a &= 3;
    a <<= 1;
    x = a;
    y = 0;
    // 16-bit A, X, Y
    jmp = 0xC07A;
    
    // int count = 0;
    
    while (run) {
        /*
        printf("\ncount: %d\n", count);
        printf("jmp = %X\n", jmp);
        printf("\tc = %04X\n", (b*256)+a);
        printf("\tx = %04X\n", x);
        printf("\ty = %04X\n", y);
        count++;
        */
        switch(jmp) {
            
            case 0xC07A:
                a = palette[y*2];
                b = palette[(y*2)+1];
                t1A = a;
                t1B = b;
                if (x == 0) {
                    jmp = 0xC0B8;
                }
                else if (x == 2) {
                    jmp = 0xC0C8;
                }
                else if (x == 4) {
                    jmp = 0xC0D9;
                }
                else if (x == 6) {
                    jmp = 0xC0EB;
                }
                else {
                    // printf("Error @%X %X\n", seed, x);
                    return;
                }
                origin[recurse] = 0xC08C;
                recurse++;
            break;
            
            case 0xC08C:
                t1C = a;
                t1D = b;
                a = t1A;
                b = t1B;
                a &= t20;
                b &= t21;
                a |= t1C;
                b |= t1D;
                palette[y*2] = a;
                palette[(y*2)+1] = b;
                temp = a;
                a = b;
                b = temp;
                y++;
                if (y == 0x80) {
                    run = 0;
                }
                else {
                    jmp = 0xC07A;
                }
            break;
            
            case 0xC0B8:
                a &= t1E;
                b &= t1F;
                t1C = a;
                t1D = b;
                c = (b*256) + a;
                c >>= 2;
                c ^= 0xFFFF;
                c++;
                
                c += ((t1D*256) + t1C);
                if (c > 65535) {
                    c -= 65536;
                }
                a = (c % 256);
                b = (c - (c % 256)) / 256;
                
                if (c > (t1F*256)+t1E) {
                    a = t1E;
                    b = t1F;
                }
                
                a &= t1E;
                b &= t1F;
                                
                recurse--;
                jmp = origin[recurse];
                
            break;
            
            case 0xC0C8:
                a &= t1E;
                b &= t1F;
                t1C = a;
                t1D = b;
                c = (b*256) + a;
                c >>= 1;
                c += ((t1D*256) + t1C);
                if (c > 65535) {
                    c -= 65536;
                }
                a = (c % 256);
                b = (c - (c % 256)) / 256;
                
                if (c > (t1F*256)+t1E) { // CMP?
                    a = t1E;
                    b = t1F;
                }
                
                a &= t1E;
                b &= t1F;
                
                recurse--;
                jmp = origin[recurse];
                
            break;
            
            case 0xC0D9:
                a &= t1E;
                b &= t1F;
                t1C = a;
                t1D = b;
                c = (b*256) + a;
                c >>= 2;
                c += ((t1D*256) + t1C);
                if (c > 65535) {
                    c -= 65536;
                }
                a = (c % 256);
                b = (c - (c % 256)) / 256;
                
                if (c > (t1F*256)+t1E) {
                    a = t1E;
                    b = t1F;
                }
                
                a &= t1E;
                b &= t1F;
                
                recurse--;
                jmp = origin[recurse];
                
            break;
            
            case 0xC0EB:
                a &= t1E;
                b &= t1F;
                c = (b*256)+a;
                c >>= 1;
                a = c % 256;
                b = (c - (c%256)) / 256;
                a &= t1E;
                b &= t1F;
                recurse--;
                jmp = origin[recurse];
            break;
            
        } // switch
    
    } // while
    
    
    
} // bbc_pal();

void arrange_gbc(unsigned char *source, int width, int height, char *dir, char *name) {

    int laylen = width * height / 64;
    unsigned char *image = malloc(width * height * 4);
    int i, j, k;
    
    for (j = 0; j < laylen/(width/8); j++) {
        // printf("j = %X\n", j);
        for (i = 0; i < width/8; i++) {
            for (k = 0; k < 8; k++) {
                memcpy(&image[((i%(width/8))*32)+(j*width*8*4)+(k*width*4)], &source[(i*256)+(j*width*8*4)+(k*32)], 32);
            }
        }
    }
    source = image;
    
    write_png(dir, name, source, width, height);
    free(image);

} // arrange_gbc();

void gbc_assemble(unsigned char *bitplane, unsigned char *bp_data, unsigned char *lay_data, unsigned char *col_data, unsigned char *rgb, int t_width, int t_height, int mode) {

    int bank; // VRAM Bank
    int pal_val; // 0-7
    int bp_val;
    int hflip;
    int vflip;
    int bits[] = {128, 64, 32, 16, 8, 4, 2, 1};
    int one, two;
    int i, j, k, m;
    int tile;
    int offset;
    int laylen = 16 * t_width * t_height;
    
    for (i = 0; i < laylen; i+=16) {
        for (j = 0; j < 16; j++) {
            
            pal_val = 0;
            bank = 0;
            hflip = 0;
            vflip = 0;
            
            if (mode == 1) {
                if (col_data[i+j] & 1) pal_val += 1;
                if (col_data[i+j] & 2) pal_val += 2;
                if (col_data[i+j] & 4) pal_val += 4;
                if (col_data[i+j] & 8) bank = 0x1000;
                if (col_data[i+j] & 32) hflip = 1;
                if (col_data[i+j] & 64) vflip = 1;
                // ((*col_data)[i+j] & 128) priority = 1: priority = 0;
            } // GBC
            
            tile = lay_data[i+j] * 16;
            
            for (k = 0; k < 8; k++) {
                one = bp_data[bank + tile + (k*2) + 0];
                two = bp_data[bank + tile + (k*2) + 1];
                if (vflip) k = 7 - k;
                // Layout data is signed.
                for (m = 0; m < 8; m++) {
                    
                    bp_val = 0;
                    if (one & bits[m]) bp_val += 1;
                    if (two & bits[m]) bp_val += 2;
                    if (hflip) m = 7 - m;
                    
                    offset = ((i*256)+(j*256)+(k*32)+(m*4));
                    
                    if (mode == 0) {
                        bitplane[offset+0] = rgb[9-(bp_val*3)+0];
                        bitplane[offset+1] = rgb[9-(bp_val*3)+1];
                        bitplane[offset+2] = rgb[9-(bp_val*3)+2];
                        bitplane[offset+3] = 255;
                    } // GB 4 colours
                    else if (mode == 1) {
                        bitplane[offset+0] = rgb[((pal_val*4)+bp_val)*3];
                        bitplane[offset+1] = rgb[((pal_val*4)+bp_val)*3+1];
                        bitplane[offset+2] = rgb[((pal_val*4)+bp_val)*3+2];
                        bitplane[offset+3] = 255;
                    } // GBC 256 colours
                    if (hflip) m = 7 - m;
                }
                if (vflip) k = 7 - k;
            }
        }
    }
    
    // write_png("tiles.png", image, 8, lay_len*8);
    
    // free(image);
    
    return;

} // gbc_assemble();

void tile_generator(unsigned char *raw_data, int *rawlen, int tiles, int mode) {
    
    int i, j = 0;
    for (i = 0; i < tiles; i++) {
        
        raw_data[j++] = i;
        if (mode) i++; // 16-bit
        
    }
    
    *rawlen = tiles;
    
    return;
    
} // tile_generator

