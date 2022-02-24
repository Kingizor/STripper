#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline uint32_t getHalf(uint8_t *rom, int addr) {
    addr &= 0xFFFFFF;
    return rom[addr] + (rom[addr+1] << 8);
}

static inline uint32_t getSignedHalf(uint8_t *rom, int addr) {
    uint32_t n = getHalf(rom, addr);
    if (n & 0x8000) n += 0xFFFF0000; // Prepend 0xFFFF if negative
    return n;
}

static inline uint32_t getWord(uint8_t *rom, int addr) {
    addr &= 0xFFFFFF;
    return rom[addr] + (rom[addr+1] << 8) + (rom[addr+2] << 16) + (rom[addr+3] << 24);
}

static inline void setHalf(uint8_t *data, uint32_t addr, uint32_t n) {
    data[addr++] = n;
    data[addr++] = n >> 8;
}

static inline void setWord(uint8_t *data, uint32_t addr, uint32_t n) {
    data[addr++] = n;
    data[addr++] = n >> 8;
    data[addr++] = n >> 16;
    data[addr++] = n >> 24;
}

void dkc2_decomp(uint8_t *rom, uint8_t *output, uint32_t *outlen, uint32_t src) {

    uint32_t *r = calloc(16, 4); // Registers
    uint8_t *ram = calloc(0x4000, 1);
    uint8_t bytes[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    int jmp = 0x8AC;
    int rtn = 0;
    int run = 1;
    uint32_t stack[25];
    int sc = 0;
    int carry = 0, zero = 0, negative = 0;
    uint32_t wpos = 0;
    int seed = 0; // ram offset
    int dest = 0; // out offset


    // Setup
    r[11] = seed;
    r[9] = seed + 0x20C;
    r[4] = dest; // Destination
    r[8] = src; // Source

    // B90
    r[1] = 0;
    setWord(ram, r[1], src); // [r[1]] = r[0];
    r[10] = 0;

    // 97C
    r[0] = seed;
    stack[sc++] = r[4];
    r[12] = r[0];
    r[0] = 0x212 + r[12];
    r[4] = 0;
    r[2] = 1;
    ram[r[0]] = r[2];
    r[0] = (0x83 << 2) + r[12];
    r[3] = 2;
    setWord(ram, r[0], r[3]);
    r[1] = (0x84 << 2) + r[12];
    r[0] = 0xFFFF;
    setHalf(ram, r[1], r[0]);
    r[1] = (0x21A) + r[12];
    r[0] = 0x81 << 8;
    setHalf(ram, r[1], r[0]);
    r[0] = (0x85 << 2) + r[12];
    setWord(ram, r[0], r[2]);
    r[0] = (0x86 << 2) + r[12];
    setHalf(ram, r[0], r[4]);
    r[0] = (0x82 << 2) + r[12];
    setHalf(ram, r[0], r[2]);
    r[1] = 0x222 + r[12];
    r[0] = 0x8101;
    setHalf(ram, r[1], r[0]);
    r[0] = (0x87 << 2) + r[12];
    setHalf(ram, r[0], r[2]);
    r[0] = (0x88 << 2) + r[12];
    setHalf(ram, r[0], r[4]);
    r[0] = 0x20A + r[12];
    setHalf(ram, r[0], r[3]);
    r[0] = 3;
    r[1] = r[12];
    setWord(ram, r[1] + 4, r[0]);
    r[0] -= 4;
    r[3] = r[0];
    r[1] += 8;
    r[2] = 0xFF;

    while(r[2] > 0) {
        r[0] = getHalf(ram, r[1]);
        r[0] |= r[3];
        setHalf(ram, r[1], r[0]);
        r[1] += 2;
        r[2] -= 1;
    }

    r[4] = stack[--sc];

    // dump_file("ram.bin", ram, 0x4000);


    while (run) {

        switch(jmp) {

            case 0x8AC:
                jmp = 0x9B0;
                rtn = 0x8B0;
            break;

            case 0x8B0:
                negative = 0;
                if (r[0] == 0x100) {
                    run = 0; // quit;
                    break;
                }
                else if (r[0] < 0x100) {
                    negative = 1;
                }
                output[wpos++] = r[0]; // WRITE
                jmp = 0xA6C;
                rtn = 0x8AC;
            break;

            case 0x9B0:
                stack[sc++] = rtn;
                r[3] = 0;
                jmp = 0x9B8;
            break;

            case 0x9B8:
                r[14] = r[9] + (r[3] << 3); // Check
                r[3] = getHalf(ram, r[14] + 6);
                carry = 0;
                if (r[3] & 0x8000) {
                    jmp = 0x9E0;
                    break;
                }
                r[10] += 1;
                if (!((r[10] - 1) & 0x1F)) {
                    r[12] = getWord(rom, r[8]);
                    r[8] += 4;
                }
                carry = (r[12] & 1);
                r[12] >>= 1;
                if (carry) r[3] += 1;
                jmp = 0x9B8;
            break;

            case 0x9E0:
                r[0] = r[3];
                if (r[0] & 0x8000) r[0] -= 0x8000;
                negative = 0;
                if (r[0] < 0x100) {
                    carry = 0;
                    negative = 1;
                    jmp = stack[--sc]; // stack or rtn? :|
                    break;
                }
                else if (r[0] > 0x100) {
                    carry = 1;
                }
                else {
                    run = 0;
                    break;
                }
                jmp = 0xBAC;
                rtn = 0x9F0;
            break;

            case 0x9F0:
                jmp = 0x9F8;
                rtn = 0x9F4;
            break;

            case 0x9F4:
                jmp = stack[--sc];
            break;

            case 0x9F8:
                stack[sc++] = r[4];
                stack[sc++] = r[5];
                stack[sc++] = r[6];

                r[2] = getWord(ram, r[11] + 4);
                r[3] = r[2] + 2;
                setWord(ram, r[11] + 4, r[3]);
                r[3] = r[2] + 1;

                r[4] = r[11] + 8;
                r[1] = r[0] << 1;
                setHalf(ram, r[4] + r[1], r[3]);

                r[1] = r[11] + 0x20C;
                r[5] = r[1] + (r[3] << 3);
                r[4] = r[2] - 1;
                r[6] = r[0] | 0x8000;
                setHalf(ram, r[5] + 6, r[6]);
                setHalf(ram, r[5] + 4, r[4]);

                r[6] = 0;
                setWord(ram, r[5], r[6]);

                r[5] = r[1] + (r[2] << 3);
                r[6] = r[1] + (r[4] << 3);

                r[3] = getWord(ram, r[6]);
                setWord(ram, r[5], r[3]);

                r[3] = getHalf(ram, r[6] + 6);
                setHalf(ram, r[5] + 6, r[3]);
                setHalf(ram, r[5] + 4, r[4]);
                setHalf(ram, r[6] + 6, r[2]);

                if (r[3] & 0x8000) r[3] -= 0x8000;

                r[5] = r[11] + (r[3] << 1);

                setHalf(ram, r[5] + 8, r[2]);

                r[6] = stack[--sc];
                r[5] = stack[--sc];
                r[4] = stack[--sc];

                jmp = rtn;

            break;

            case 0xA6C:
                stack[sc++] = r[4];
                stack[sc++] = r[5];
                stack[sc++] = rtn;

                r[4] = r[11] + 0x20C;
                r[1] = getWord(ram, r[4]);

                if (r[1] >= 0x8000) {
                    negative = 0;
                    jmp = 0xACC; // Nooooooooooooooooooooooooooo
                    break;
                }
                else {
                    negative = 1;
                }
                jmp = 0xA80;
            break;

            case 0xA80:
                r[0] = r[11] + (r[0] << 1);
                r[0] = getSignedHalf(ram, r[0] + 8); // Signed?
                jmp = 0xA88;
            break;

            case 0xA88:
                r[14] = getWord(ram, r[4] + (r[0] << 3));
                r[14] += 1;
                setWord(ram, r[4] + (r[0] << 3), r[14]);
                r[1] = r[0];

                jmp = 0xA98;
            break;

            case 0xA98:
                r[1] -= 1;

                if (negative) {
                    carry = 1;
                    negative = 0;
                }
                else if ((r[1] + 1) == 0 && !negative) {
                    negative = 1;
                    carry = 0;
                }
                else if (!negative) {
                    carry = 1;
                }

                zero = (r[1] == 0);

                if (negative) {
                    jmp = 0xAAC;
                    break;
                }
                r[5] = getWord(ram, r[4] + (r[1] << 3));
                if (r[5] < r[14]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[5] > r[14]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[5] == r[14]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry) ? 0xAAC : 0xA98;
            break;

            case 0xAAC:
                r[1] += 1;

                r[5] = r[4] + (r[1] << 3);

                if (r[1] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[1] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[1] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }

                if (!zero) {
                    jmp = 0xADC;
                    rtn = 0xABC;
                }
                else {
                    zero = 1;
                    jmp = 0xABC;
                }
            break;

            case 0xABC:
                r[0] = getSignedHalf(ram, r[5] + 4); // Signed?
                if (r[0] == 0xFFFFFFFF) { // CMN
                    zero = 1;
                    negative = 0;
                    // carry = 0;
                }
                else {
                    negative = 0;
                    carry = 0;
                    zero = 0;
                }

                if (!zero) {
                    jmp = 0xA88;
                }
                else {
                    jmp = stack[--sc];
                    r[5] = stack[--sc];
                    r[4] = stack[--sc];
                }
            break;

            case 0xACC:
                stack[sc++] = r[0];
                stack[sc++] = r[4];
                jmp = 0xB5C;
            break;

            case 0xAD4:
                r[4] = stack[--sc];
                r[0] = stack[--sc];
                jmp = 0xA80;
            break;

            case 0xADC:
                stack[sc++] = r[4];
                stack[sc++] = r[5];
                r[4] = r[9] + (r[0] << 3);
                r[5] = getHalf(ram, r[4] + 6);
                // tst?
                zero = 1;
                negative = 0;
                carry = 0;
                if (r[5] & 0x8000) {
                    zero = 0;
                    r[5] -= 0x8000;
                }
                if (!zero) {
                    r[5] = r[11] + (r[5] << 1);
                    setHalf(ram, r[5] + 8, r[1]);
                }
                else {
                    r[5] = r[9] + (r[5] << 3);
                    setHalf(ram, r[5] + 4, r[1]);
                    setHalf(ram, r[5] + 0xC, r[1]);
                }

                r[2] = r[9] + (r[1] << 3);
                r[5] = getHalf(ram, r[2] + 6);

                zero = 1;
                if (r[5] & 0x8000) {
                    zero = 0;
                    r[5] -= 0x8000;
                }
                if (!zero) {
                    r[5] = r[11] + (r[5] << 1);
                    setHalf(ram, r[5] + 8, r[0]);
                }
                else {
                    r[5] = r[9] + (r[5] << 3);
                    setHalf(ram, r[5] + 4, r[0]);
                    setHalf(ram, r[5] + 0xC, r[0]);
                }
                zero = 0; // Wrong

                r[5] = getHalf(ram, r[4] + 6);
                r[3] = getHalf(ram, r[2] + 6);
                setHalf(ram, r[4] + 6, r[3]);
                setHalf(ram, r[2] + 6, r[5]);

                r[5] = getWord(ram, r[4]);
                r[3] = getWord(ram, r[2]);
                setWord(ram, r[4], r[3]);
                setWord(ram, r[2], r[5]);

                r[5] = stack[--sc];
                r[4] = stack[--sc];
                jmp = rtn;
                // 0xB4C
            break;

            case 0xB5C:
                stack[sc++] = r[7];
                stack[sc++] = r[8];
                stack[sc++] = r[9];
                stack[sc++] = r[10];
                stack[sc++] = r[11];
                stack[sc++] = r[12];
                stack[sc++] = r[14];
                r[0] = 0x3003B69;
                jmp = 0xB68;
            break;

            case 0xB68:
                r[14] = 0x801413C;
                r[0] = getWord(ram, 0xA0C);
                carry = 1;
                negative = 0;
                zero = 0;
                r[0] = 0;
                r[1] = 0;
                jmp = 0x3A44; // ROM & THUMB
            break;

            case 0xBAC:
                r[0] = r[10] & 0x1F;
                zero = (!(r[10] & 0x1F));
                r[10] += 8;
                if (zero) {
                    r[12] = getWord(rom, r[8]); // ?
                    r[8] += 4;
                }
                zero = 0;
                negative = 0;
                if (r[0] > 0x18) {
                    jmp = 0xBCC;
                    carry = 1;
                    break;
                }
                else if (r[0] < 0x18) {
                    negative = 1;
                    carry = 0;
                }
                r[0] = r[12] & 0xFF;
                r[12] >>= 8;
                jmp = 0xBE8;
            break;

            case 0xBCC:
                r[1] = r[12] & 0xFF;
                r[12] = getWord(rom, r[8]);
                r[8] += 4;
                r[0] = 0x20 - r[0];
                r[1] = r[1] | (r[12] << r[0]);
                r[0] = 8 - r[0];
                r[12] >>= r[0];
                r[0] = r[1] & 0xFF;
                r[1] = 0x03003C00;
                jmp = 0xBE8;
            break;

            case 0xBE8:
                r[2] = r[0] & 0xF;
                r[2] = bytes[r[2]];
                r[0] = bytes[(r[0] >> 4)];
                r[0] = r[0] | (r[2] << 4);
                jmp = rtn;
            break;

            case 0x3A44:
                stack[sc++] = r[4];
                stack[sc++] = r[5];
                stack[sc++] = r[6];
                stack[sc++] = r[7];
                stack[sc++] = r[14];
                r[7] = r[10];
                r[6] = r[9];
                r[5] = r[8];
                stack[sc++] = r[5];
                stack[sc++] = r[6];
                stack[sc++] = r[7];
                r[12] = r[0];
                r[0] = getWord(ram, r[0] + 4);
                r[5] = r[0] - 1;
                r[6] = r[5];
                // cmp    r5,#0x0
                if ((r[5] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[5] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[5] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (negative) ? 0x3AA0 : 0x3A5A;
            break;

            case 0x3A5A:
                r[0] = 0x80;
                r[0] <<= 8;
                r[9] = r[0];
                r[1] = r[5] << 3;
                r[2] = 0x83;
                r[2] <<= 2;
                r[0] = r[1] + r[2];
                r[2] = r[12];
                r[4] = r[0] + r[2];
                r[7] = r[1] + r[2];
                r[3] = r[4];
                r[0] = 0x212;
                r[8] = r[0];
                jmp = 0x3A74;
            break;

            case 0x3A74:
                r[1] = r[8];
                r[0] = r[7] + r[1];
                r[2] = getHalf(ram, r[0]);
                r[0] = r[9];
                r[2] &= r[0];
                // cmp    r2,#0x0
                if ((r[2] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (zero) ? 0x3A96 : 0x3A82;
            break;

            case 0x3A82:
                r[0] = getWord(ram, r[4]);
                r[1] = getWord(ram, r[4] + 4);
                setWord(ram, r[3], r[0]);
                setWord(ram, r[3] + 4, r[1]);
                r[0] = getWord(ram, r[3]);
                r[0] += 1;
                r[0] >>= 1;
                setWord(ram, r[3], r[0]);
                r[3] -= 8;
                r[6] -= 1;
                jmp = 0x3A96;
            break;

            case 0x3A96:
                r[4] -= 8;
                r[7] -= 8;
                r[5] -= 1;
                // cmp    r5,#0x0
                if ((r[5] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[5] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[5] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!negative) ? 0x3A74 : 0x3AA0;
            break;

            case 0x3AA0:
                r[1] = r[12];
                r[0] = getWord(ram, r[1] + 4);
                r[5] = r[0] - 2;
                // cmp    r6,#0x0
                if ((r[6] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[6] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[6] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (negative) ? 0x3B3C : 0x3AAA;
            break;

            case 0x3AAA:
                r[0] = 0x83;
                r[0] <<= 2;
                r[1] += r[0];
                r[8] = r[1];
                jmp = 0x3AB2;
            break;

            case 0x3AB2:
                r[4] = r[5] + 1;
                r[2] = r[6] << 3;
                r[0] = r[8];
                r[3] = r[0] + r[2];
                r[1] = r[5] << 3;
                r[1] += r[8];
                r[0] = r[4] << 3;
                r[0] += r[8];
                r[1] = getWord(ram, r[1]);
                r[0] = getWord(ram, r[0]);
                r[1] += r[0];
                setWord(ram, r[3], r[1]);
                r[7] = r[1];
                r[2] += r[12];
                r[1] = 0x212;
                r[2] += r[1];
                r[1] = getHalf(ram, r[2]);
                r[0] = 0x7FFF;
                r[0] &= r[1];
                setHalf(ram, r[2], r[0]);
                r[4] = r[6] + 1;
                r[1] = r[4] << 3;
                r[2] = r[8];
                r[0] = r[2] + r[1];
                r[0] = getWord(ram, r[0]);
                r[2] = r[6] - 1;
                r[10] = r[2];
                r[2] = r[5] - 2;
                r[9] = r[2];
                // cmp    r7,r0

                if (r[7] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[7] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[7] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry) ? 0x3B04 : 0x3AF0;
            break;

            case 0x3AF0:
                r[2] = 0x83;
                r[2] <<= 2;
                r[0] = r[1] + r[2];
                r[2] = r[12];
                r[1] = r[0] + r[2];
                jmp = 0x3AFA;
            break;

            case 0x3AFA:
                r[1] += 8;
                r[4] += 1;
                r[0] = getWord(ram, r[1]);
                // cmp    r7,r0

                if (r[7] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[7] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[7] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!carry) ? 0x3AFA : 0x3B04;
            break;

            case 0x3B04:
                r[4] -= 1;
                r[3] = r[6];
                // cmp    r6,r4;
                if (r[6] < r[4]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[6] > r[4]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[6] == r[4]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!negative) ? 0x3B24 : 0x3B0C;
            break;

            case 0x3B0C:
                r[0] = r[6] << 3;
                r[1] = 0x83;
                r[1] <<= 2;
                r[0] += r[1];
                r[1] = r[12];
                r[2] = r[0] + r[1];
                jmp = 0x3B18;
            break;

            case 0x3B18:
                r[0] = getWord(ram, r[2] + 0x8);
                r[1] = getWord(ram, r[2] + 0xC);
                // stmia ?
                setWord(ram, r[2], r[0]);
                r[2] += 4;
                setWord(ram, r[2], r[1]);
                r[2] += 4;
                r[3] += 1;

                // cmp    r3,r4
                if (r[3] < r[4]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[3] > r[4]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[3] == r[4]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (negative) ? 0x3B18 : 0x3B24;
            break;

            case 0x3B24:
                r[0] = r[4] << 3;
                r[2] = r[8];
                r[1] = r[2] + r[0];
                setWord(ram, r[1], r[7]);
                r[0] += r[12];
                r[1] = 0x212;
                r[0] += r[1];
                setHalf(ram, r[0], r[5]);
                r[5] = r[9];
                r[6] = r[10];
                // cmp  r6,#0x0
                if ((r[6] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[6] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[6] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!negative) ? 0x3AB2 : 0x3B3C;
            break;

            case 0x3B3C:
                r[2] = r[12];
                r[0] = getWord(ram, r[2] + 4);
                r[5] = r[0] - 1;
                // cmp  r5,#0x0
                if ((r[5] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[5] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[5] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (negative) ? 0x3B92 : 0x3B46;
            break;

            case 0x3B46:
                r[7] = 0x80;
                r[7] <<= 8;
                r[6] = 0x7FFF;
                r[4] = 0x84;
                r[4] <<= 2;
                r[0] = r[5] << 3;
                r[3] = r[0] + r[2];
                jmp = 0x3B54;
            break;

            case 0x3B54:
                r[1] = 0x212;
                r[0] = r[3] + r[1];
                r[2] = getHalf(ram, r[0]);
                r[0] = r[2];
                r[0] &= r[7];
                // cmp  r0,#0x0
                if ((r[0] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[0] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[0] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (zero) ? 0x3B78 : 0x3B62;
            break;

            case 0x3B62:
                r[2] &= r[6];
                r[1] = r[2] << 1;
                r[0] = r[12];
                r[0] += 8;
                r[0] += r[1];
                setHalf(ram, r[0], r[5]);
                jmp = 0x3B8A;
            break;

            case 0x3B70:
                r[2] <<= 8;
                r[7] = ram[r[7] + 0x1F];
                jmp = 0x3B78;
            break;

            case 0x3B78:
                r[1] = r[2] << 3;
                r[1] += r[12];
                r[0] = r[2] + 1;
                r[0] <<= 3;
                r[0] += r[12];
                r[0] += r[4];
                setHalf(ram, r[0], r[5]);
                r[1] += r[4];
                setHalf(ram, r[1], r[5]);
                jmp = 0x3B8A;
            break;

            case 0x3B8A:
                r[3] -= 8;
                r[5] -= 1;
                // cmp  r5,#0x0
                if ((r[5] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                }
                else if (r[5] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[5] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!negative) ? 0x3B54 : 0x3B92;
            break;

            case 0x3B92:
                r[5] = stack[--sc];
                r[4] = stack[--sc];
                r[3] = stack[--sc];
                r[8] = r[3];
                r[9] = r[4];
                r[10] = r[5];
                r[7] = stack[--sc];
                r[6] = stack[--sc];
                r[5] = stack[--sc];
                r[4] = stack[--sc];
                r[0] = stack[--sc];
                // 0801413C
                // andeq r4,r0,r8,ror,r7
                r[14] = stack[--sc];
                r[12] = stack[--sc];
                r[11] = stack[--sc];
                r[10] = stack[--sc];
                r[9] = stack[--sc];
                r[8] = stack[--sc];
                r[7] = stack[--sc];
                jmp = 0xAD4;
            break;

            default:
                run = 0;
            break;

        } // switch();

    } // while();

    // printf("Program Complete\n");
    // printf("outlen = %X\n", wpos);
    // dump_file("output.bin", output, wpos);
    free(ram);
    *outlen = wpos;

}

void dkc2_decode(uint8_t *rom, uint8_t *output, uint32_t *outlen, uint32_t src) {

    uint32_t *r = calloc(16, 4);
    uint8_t *ram = calloc(0x2000, 1);
    int run = 1;
    int jmp = 0xD38;
    int carry = 0, negative = 0, zero = 0;
    r[5] = src;
    r[4] = 0;
    r[1] = 0; // RAM_START
    r[2] = 0x1000100;
    r[0] = rom[r[5]];
    r[5] += 1;
    r[3] = rom[r[5]];
    r[5] += 1;
    r[6] = 0;


    while (run) {

        switch (jmp) {

            // Phase 1 (Generate low RAM)

            case 0xD38:
                r[0] <<= 2;
                r[1] = 0;
                r[2] = r[1] + r[0];
                r[0] = r[3] << 2;
                r[1] += r[0];
                if (r[2] < r[1]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > r[1]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == r[1]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry && !zero) ? 0xD52 : 0xD46;
            break;

            case 0xD46:
                r[0] = rom[r[5]];
                setHalf(ram, r[2], r[0]);
                r[5] += 1;
                r[2] += 4;
                if (r[2] < r[1]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > r[1]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == r[1]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!carry || zero) ? 0xD46 : 0xD52;
            break;

            case 0xD52:
                r[0] = rom[r[5]];
                r[5] += 1;
                r[3] = rom[r[5]];
                r[5] += 1;

                if ((r[0] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[0] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[0] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!zero) ? 0xD38 : 0xD5E;
            break;

            case 0xD5E:
                r[5] -= 1;
                r[0] = 3;
                r[0] &= r[5];
                if ((r[0] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[0] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[0] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (zero) ? 0xD74 : 0xD68;
            break;

            case 0xD68:
                r[1] = 3;
                jmp = 0xD6A;
            break;

            case 0xD6A:
                r[5] += 1;
                r[0] = r[5];
                r[0] &= r[1];
                if ((r[0] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[0] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[0] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!zero) ? 0xD6A : 0xD74;
            break;

            case 0xD74:
                r[0] = 0;
                r[1] = 0x80;
                r[1] <<= 3;
                r[0] += r[1];
                r[1] = 1;
                setHalf(ram, r[0], r[1]);
                r[0] = r[5];
                // 0xD82 ~ 0xD86
                r[4] = r[0];
                jmp = 0xD94;
            break;

            case 0xD94:
                src = r[0]; // For phase 3
                r[0] = 0;
                r[1] = 0x804;
                r[2] = 0xFFFF;
                setHalf(ram, r[1], r[2]);
                r[1] = 0x101;
                r[12] = r[1];

                jmp = 0xDEE;
            break;


            // Phase 2 (Generate high RAM from low RAM)

            case 0xDC0:
                r[2] = r[8];
                r[0] = 0;
                r[1] = r[12];
                r[4] = r[1] << 2;
                r[5] = r[4] + r[0];
                r[3] = r[7] << 2;
                r[3] += r[0];
                r[1] = r[6] << 2;
                r[1] += r[0];
                r[0] = getHalf(ram, r[1]);
                r[2] = getHalf(ram, r[3]);
                r[0] += r[2];
                r[2] = 0;
                setHalf(ram, r[5], r[0]);
                setHalf(ram, r[3], r[2]);
                setHalf(ram, r[1], r[2]);
                r[0] = 0x810; // ram_start
                r[4] += r[0];
                setHalf(ram, r[4], r[7]);
                setHalf(ram, r[4] + 2, r[6]);
                r[0] = 1;
                r[12] += r[0];
                jmp = 0xDEE;
            break;

            case 0xDEE:
                r[6] = 0x201;
                r[7] = r[6];
                r[1] = r[8];
                r[4] = 0;
                r[3] = 0;
                if (r[3] < r[12]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[3] > r[12]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[3] == r[12]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!negative) ? 0xE34 : 0xDFC;
            break;

            case 0xDFC:
                r[2] = getHalf(ram, r[4]);
                if ((r[2] + 1) == 0) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > 0) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == 0) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (zero) ? 0xE2C : 0xE02;
            break;

            case 0xE02:
                r[0] = r[8];
                r[1] = 0;
                r[0] = r[7] << 2;
                r[0] += r[1];
                r[0] = getHalf(ram, r[0]);
                if (r[2] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry) ? 0xE20 : 0xE10;
            break;

            case 0xE10:
                r[6] = r[7];
                r[7] = r[3];
                jmp = 0xE2C;
            break;

            case 0xE20:
                r[0] = r[6] << 2;
                r[0] += r[1];
                r[0] = getHalf(ram, r[0]);
                if (r[2] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[2] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[2] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry) ? 0xE2C : 0xE2A;
            break;

            case 0xE2A:
                r[6] = r[3];
                jmp = 0xE2C;
            break;

            case 0xE2C:
                r[4] += 4;
                r[3] += 1;
                if (r[3] < r[12]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[3] > r[12]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[3] == r[12]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (negative) ? 0xDFC : 0xE34;
            break;

            case 0xE34:
                r[0] = 0x201;
                if (r[6] < r[0]) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[6] > r[0]) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[6] == r[0]) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (!zero) ? 0xDC0 : 0xE3A;
            break;

            case 0xE3A:
                r[12] -= 1; // ?
                run = 0;
            break;

            default:
                printf("Error: jmp = 0x%X\n", jmp);
                run = 0;
            break;

        }

    } // RAM Population (Phase 1 & Phase 2)

    // Phase 3 (Decode)

    r[6] = 0x810; // Hi-RAM offset
    r[7] = r[12];
    r[0] = src;
    r[11] = src;
    r[12] = 1;
    r[10] = 0;

    run = 1;
    jmp = 0x144;
    negative = 0, zero = 0, carry = 0;

    int wpos = 0;

    while (run) {

        switch(jmp) {
            case 0x144:
                r[8] = r[7];
                jmp = 0x148;
            break;

            case 0x148:
                // ror(r[12], 1); // FLAGS
                carry = (r[12] & 1);
                negative = (r[12] & 1); // ?

                r[12] >>= 1;

                if (negative) {
                    r[12] += 0x80000000; // ror
                    r[10] = getWord(rom, r[11]);
                    r[11] += 4;
                }

                // lsr(r[10], 1); // FLAGS
                carry = (r[10] & 1);
                negative = 0;
                r[10] >>= 1;

                r[0] = r[6] + (r[8] << 2);

                if (carry) {
                    r[8] = getHalf(ram, r[0] + 2);
                }
                else {
                    r[8] = getHalf(ram, r[0]);
                }

                if (r[8] < 0x100) {
                    zero = 0;
                    negative = 1;
                    carry = 0;
                }
                else if (r[8] > 0x100) {
                    zero = 0;
                    negative = 0;
                    carry = 1;
                }
                else if (r[8] == 0x100) {
                    if (negative) carry = 1;
                    zero = 1;
                    negative = 0;
                }
                jmp = (carry && !zero) ? 0x148 : 0x168;
            break;

            case 0x168:
                if (!zero) {
                    // fprintf(out, "\tWritten %02X to %04X.\n", (r[8] & 0xFF), wpos);
                    output[wpos++] = r[8];
                    jmp = 0x144;
                }
                else {
                    run = 0;
                }
            break;

            default:
                printf("Error: jmp = 0x%X\n", jmp);
                run = 0;
            break;

        }
    } // Decode (Phase 3)

    *outlen = wpos;

} // dkc2_decode();

