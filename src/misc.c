#include <stdio.h>
#include <string.h>
#include "bitplane.h"

void decode_bitplane_3(int region, unsigned char *rom, unsigned char *bp_data, unsigned char *raw_data, unsigned char *bitplane, int pal_addr, unsigned int raw_len, unsigned int bp_len, int mode, int fix, int bg) {

    // Japanese version(s) of DKC3 has all palettes shifted by -77 (-0x4D).

    if (region == 0) {
        pal_addr -= 77;
    }

    decode_bitplane(rom, bp_data, raw_data, bitplane, pal_addr, raw_len, bp_len, mode, fix, bg);
    return;

} // decode_bitplane_3();

void pipe_layout(unsigned char *raw_data, unsigned int raw_counter) {

    // Smoke effect in Low-G Labyrinth & Poisonous Pipeline.

    unsigned int i;

    for (i = 0; i < raw_counter; i+=2) {
        raw_data[i+1] |= 0x20;
    }

    return;

} // pipe_layout();


void surface_layout(unsigned char *raw_data, unsigned int raw_counter) {

    // Water surface in Pot-Hole Panic & Riverside Levels.
    unsigned int i;

    for (i = 0; i < raw_counter; i+=2) {
        if ((raw_data[i+1] - 1) < 0) {
            raw_data[i+1] += 255;
        }
        else {
            raw_data[i+1] -= 1;
        }
    }

    return;

} // surface_layout();


void lights_layout(unsigned char *raw_data, unsigned int raw_counter) {

    // Lights in Murky Mill

    unsigned int i;

    for (i = 0; i < raw_counter; i+=2) {
        if ((raw_data[i] + 0x72) > 255) raw_data[i+1] += 1;
        raw_data[i]   += 0x72;
        raw_data[i+1] += 0x01;
    }

    return;

} // lights_layout();


void waterfall_layout(unsigned char *rom, unsigned char *raw_data, int type) {

    // WATERFALLS

    unsigned int offset[][32] = {
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338AF6, 0x338B36, 0x338B76, 0x338BB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}, // 128
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x338DF6, 0x338E36, 0x338E76, 0x338EB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}, // 192
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6}, // 256 Left
        {0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338AF6, 0x338B36, 0x338B76, 0x338BB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}, // 256 Right
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x3388F6, 0x338936, 0x338976, 0x3389B6}, // 320 Left
        {0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x338DF6, 0x338E36, 0x338E76, 0x338EB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}, // 320 Right
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6}, // 384 Left
        {0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338AF6, 0x338B36, 0x338B76, 0x338BB6}, // 384 Right
        {0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387F6, 0x338836, 0x338876, 0x3388B6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6}, // 448 & 576 Left
        {0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6}, // 448 & 576 Middle
        {0x338DF6, 0x338E36, 0x338E76, 0x338EB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}, // 448 Right
        {0x3388F6, 0x338936, 0x338976, 0x3389B6, 0x3389F6, 0x338A36, 0x338A76, 0x338AB6, 0x338BF6, 0x338C36, 0x338C76, 0x338CB6, 0x338CF6, 0x338D36, 0x338D76, 0x338DB6, 0x338DF6, 0x338E36, 0x338E76, 0x338EB6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6, 0x3387B6}  // 576 Right
    }; // 32

    unsigned int i, j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            raw_data[(j*64)+(i*2)] = rom[offset[type][i] + (j*2)];
            raw_data[(j*64)+(i*2)+1] = rom[offset[type][i] + (j*2) + 1];
        }
    }

    return;

} // waterfall_layout();


void boss_photo_layout(unsigned char *raw_data, unsigned char *temp_data, int offset) {

    int pos = offset * 0x150;
    int i;
    memset(&raw_data[0], 0, 0x800);

    for (i = 0; i < 14; i++) {
        memcpy(&raw_data[0x214 + (i*0x40)], &temp_data[pos + (i*0x018)], 0x18);
    }

} // boss_photo_layout();


void bear_photo_layout(unsigned char *raw_data, unsigned char *temp_data, int offset) {

    int pos = offset * 0x8C;
    int i;
    memset(&raw_data[0], 0, 0x800);

    for (i = 0; i < 7; i++) {
        memcpy(&raw_data[0x28C + (i*0x40)], &temp_data[pos + (i*0x0A)],        0x0A);
        memcpy(&raw_data[0x2AA + (i*0x40)], &temp_data[pos + (i*0x0A) + 0x46], 0x0A);
    }

} // bear_photo_layout();

/*
void rareware_layout(unsigned char **raw_data) {

    // Reserved for Rareware logo. (Mode 7)

} // rareware_layout();
*/

void bear_layout(unsigned char *raw_data, unsigned char *rom, unsigned char a, int x, int bank) {

    /*
        For Bear Cabins

        This function requires:
            The value in the lower byte of the accumulator
            The X register
            The current program bank

        $B4/AF6B = a & x
        $B4/AF71 = bank (Will probably always be $F5:0000)

        $B4/AF6B = Start
        $B4/AF4C = End
    */

    if (rom[0xFFD9] == 0) {
        if (rom[0xFFDB] == 0) {
            x -= 610;
        }
        else {
            x -= 574;
        }
    }
    if (rom[0xFFD9] == 2) x += 188;

    unsigned char offset, b, wA, wB, t1C, t1D, t1E, t1F, t20, t21, t22, t23, t26, t27, t28, t29;
    int y;
    int jmp;
    char run = 1;

    char flip = 0;

    if (a >= 0x80) flip = 1;

    t1C = 0;
    t1D = 0;

    a &= 0x3F;
    b = 0;
    a <<= 2;
    y = a;
    t1E = rom[bank + 0x702E + y];
    t1F = 0;
    t20 = rom[bank + 0x702F + y];
    t21 = 0;
    t22 = rom[bank + 0x7030 + y];
    t23 = rom[bank + 0x7030 + y + 1];
    // t24 = 0xF5;
    // t25 = 0;
    a = rom[0x340000 + 1 + x]; // Code bank
    b = rom[0x340000 + 1 + x + 1];
    t26 = a;
    t27 = b;
    t28 = a;
    t29 = b;

    jmp = 0xAF07;

    // int count = 0;

    while (run) {

        // printf("\ncount = %d\n", count);
        // printf("jmp = %X\n", jmp);
        // printf("\tc = %X\n", (b*256)+a);
        // printf("\tx = %X\n", x);
        // printf("\ty = %X\n", y);
        // count++;

        switch(jmp) {

            case 0xAF07:
                y = (t21*256)+t20;
                jmp = 0xAF0A;
            break;

            case 0xAF0A:
                x = (t1F*256)+t1E;

                // $B4/AF0E

                if (flip == 1) {
                    jmp = 0xAF27;
                    // jmp = 0xAF10;
                }
                else {
                    jmp = 0xAF10;
                }

            break;

            case 0xAF10:
                do {
                    a = t26;
                    b = t27;
                    wA = a;
                    offset = b;
                    while (offset >= 4) {
                        offset -= 4;
                    }
                    wB = offset;
                    a = rom[bank+(t23*256)+t22];
                    b = rom[bank+(t23*256)+t22+1];
                    a |= t1C;
                    b |= t1D;
                    raw_data[(((wB*256)+wA)*2)] = a;
                    raw_data[(((wB*256)+wA)*2)+1] = b;
                    if ((wA + 2) > 255) wB++;
                    wA += 2;
                    if ((t22 + 2) > 255) t23++;
                    t22 += 2;
                    if ((t26 + 1) > 255) t27++;
                    t26++;
                    x--;
                } while (x > 0);
                jmp = 0xAF3F;
            break;

            case 0xAF27:
                do {
                    a = x;
                    b = 0;
                    if ((a + t26) > 255) b++;
                    a += t26;
                    b += t27;
                    wA = a;
                    offset = b;
                    while (offset > 4) {
                        offset -= 4;
                    }
                    wB = offset;
                    a = rom[bank + (t23*256) + t22];
                    b = rom[bank + (t23*256) + t22 + 1];
                    b ^= 0x40;
                    a |= t1C;
                    b |= t1D;
                    raw_data[(((wB*256)+wA)*2)] = a;
                    raw_data[(((wB*256)+wA)*2)+1] = b;
                    if ((t22 + 2) > 255) t23++;
                    t22 += 2;
                    x--;
                } while (x > 0);
                jmp = 0xAF3F;
            break;

            case 0xAF3F:
                a = t28;
                b = t29;
                if ((a + 0x20) > 255) b++;
                a += 0x20;
                t26 = a;
                t27 = b;
                t28 = a;
                t29 = b;
                y--;
                if (y > 0) {
                    jmp = 0xAF0A;
                }
                else {
                    jmp = 0xAF4C;
                    run = 0;
                }
            break;

            default:
                printf("Invalid jump - %X\n", jmp);
                return;
            break;

        } // Switch
    } // While

    return;  

} // bear_layout();


void flip_bear_layout(unsigned char *raw_data) {

    /*
    Some bear cabins are flipped horizontally.

    $B4:A21D - JSL
    $BC:F84C - RTL
    */

    int c = 0x200, x = 0x7400, tempX, y = 0x1C, t1A, t1C, t1E;
    int write;
    do {
        t1E = x;
        t1A = 0;

        do {
            write = (x-0x7400)*2;
            c = (raw_data[write+1]*256) + raw_data[write];
            // write += 2;
            c ^= 0x4000;
            t1C = c;
            c = 0x1F;
            c -= t1A;
            while (c < 0) {
                c += 65536;
            }
            c += t1E;
            while (c > 65535) {
                c -= 65536;
            }
            x = c;
            write = (x-0x7400)*2;
            tempX = x;
            c = (raw_data[write+1]*256) + raw_data[write];
            c ^= 0x4000;
            x = t1E;
            write = (x-0x7400)*2;
            raw_data[write]   = (c % 256);
            raw_data[write+1] = (c - (c % 256)) / 256;
            x = tempX;
            write = (x-0x7400)*2;
            c = t1C;
            raw_data[write]   = (c % 256);
            raw_data[write+1] = (c - (c % 256)) / 256;
            t1E++;
            x = t1E;
            t1A += 2;
            c = t1A;
        } while(c < 0x1F);

        c = x;
        c += 0x10;
        while (c > 65535) {
            c -= 65536;
        }
        x = c;
        y--;

    } while(y > 0);

} // flip_bear_layout();

