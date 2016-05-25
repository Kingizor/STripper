#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int decrypt(int dec_a, int dec_b) {
    if (dec_b == 20) {
        return (dec_a - (dec_a % 16)) / 16;
    }
    else { // (dec_b == 21)
        return (dec_a % 16) * 16;
    }
} // decrypt();

static inline int read_output(uint8_t *output, int addr, int *reg) {
    if (addr > 0xFFFF) {
        printf("Error: Output address out of bounds. (decomp)\n");
        return -1;
    }
    *reg = output[addr];
    return 0;
}

static inline int write_output(uint8_t *output, int addr, int value) {
    if (addr > 0xFFFF) {
        printf("Error: Write address out of bounds. (decomp)\n");
        return -1;
    }
    output[addr] = value;
    return 0;
}

static inline int read_rom(uint8_t *rom, int addr, int *reg) {
    if (addr > 0x3FFFFF) {
        printf("Error: ROM address out of bounds. (decomp)\n");
        return -1;
    }
    *reg = rom[addr];
    return 0;
}

int decomp(uint8_t *output, uint8_t *rom, int *length, int offset) {
    
    memset(output, 0, 0xFFFF); // Zero all bytes.
    int jmp_one, jmp_two, temp, a, b, c, t24, t26, t28, t2A, t2E, t32, t36, t3A, t3B, t3C, t3E, t3F;
    int run = 1;
    int x = 0, y = 0;
    int t2B = 20;
    int t2F = 21;
    // int breakat = 0x12B0;
    int counter = 0;
    
    y = 1;
    if (read_rom(rom, offset+y, &t3A)) return -1;
    y++;
    if (read_rom(rom, offset+y, &t3B)) return -1;
    y++;
    if (read_rom(rom, offset+y, &t3E)) return -1;
    y++;
    if (read_rom(rom, offset+y, &t3F)) return -1;
    y++;
    if (read_rom(rom, offset+y, &a)) return -1;
    if (read_rom(rom, offset+y+1, &b)) return -1;
    t3C = (b*256) + a;
    y = 0x027;
    t24 = 0;
    t26 = 0;
    if (read_rom(rom, offset+y, &a)) return -1;
    y++;
    t2E = a;
    a &= 0xF0;
    a >>= 2;
    t36 = a;
    jmp_one = t36;
    
    while (run) {
        
        switch(jmp_one) {
            case 0x00:
                a = t2E;
                a &= 0x0F;
                
                if (a == 0) {
                    run = 0;
                    *length = counter;
                    return 0;
                }
                
                t24 = a;
                
                while (t24 > 0) {
                    if (read_rom(rom, offset+y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x04:
                a = decrypt(t2E, t2F);
                t26 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x86AD;
            break;
            
            case 0x08:
                a = decrypt(t2E, t2F);
                t26 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                t2E = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                if (write_output(output, counter++, a)) return -1;
                x++;
                a = decrypt(t2E, t2F);
                t26 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x86AD;
            break;
            
            case 0xC:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x10:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                a = t3A;
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x14:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                a = t3B;
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x18:
                a = t3C % 256;
                b = (t3C - (t3C % 256)) / 256;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                jmp_two = 0x86A0;
            break;
            
            case 0x1C:
                a = t3E;
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x86A0;
            break;
            
            case 0x20:
                a = t3F;
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x86A0;
            break;
            
            case 0x24:
                temp = y;
                a = t2E;
                a &= 0x0F;
                a += 2;
                t26 = a;
                
                y = x - t26;
                
                if (read_output(output, y, &a)) return -1;
                if (read_output(output, y+1, &b)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x28:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                t26 = a;
                y++;
                temp = y;
                
                y = (x - t24 - t26);
                b = (y - (y % 256) / 256);
                
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x2C:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                b = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2E = a;
                
                c = (b*256) + a;
                c >>= 4;
                c += 0x0103;
                t28 = c;
                
                temp = y;
                
                y = x - t28;
                b = y - (y % 256);
                
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x86A0;
            break;
            
            case 0x30:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                if (read_rom(rom, offset+y+1, &a)) return -1;
                if (read_rom(rom, offset+y, &b)) return -1;
                t28 = (b*256) + a;
                y += 2;
                temp = y;
                
                y = x - t28;
                b = (y - (y % 256)) / 256;
                if (y < 0) y += 65536;
                
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x34:
                x--;
                if (read_output(output, x, &a)) return -1;
                if (write_output(output, counter++, a)) return -1;
                x += 2;
                jmp_two = 0x86A0;
            break;
            
            case 0x38:
                if (read_output(output, --x, &b)) return -1;
                if (read_output(output, --x, &a)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 4;
                jmp_two = 0x86A0;
            break;
            
            case 0x3C:
                temp = y;
                a = t2E;
                
                
                c = (b*256) + a;
                c &= 0x0F;
                c <<= 1;
                c += 7;
                y = c;
                
                if (read_rom(rom, offset+y, &a)) return -1;
                if (read_rom(rom, offset+y+1, &b)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x3F:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                
                if (a == 0) {
                    run = 0;
                    *length = counter;
                    return 0;
                }
                
                t24 = a;
                
                while (t24 > 0) {
                    a = decrypt(t2E, t2F);
                    t26 = a;
                    if (read_rom(rom, offset+y, &a)) return -1;
                    y++;
                    t2A = a;
                    t2E = a;
                    a = t26;
                    a |= decrypt(t2A, t2B);
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x86A0;
            break;
            
            case 0x43:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x47:
                if (read_rom(rom, offset+y, &a)) return -1;
                if (read_rom(rom, offset+y+1, &b)) return -1;
                y += 2;
                x += 2;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                jmp_two = 0x8692;
            break;
            
            case 0x4B:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = decrypt(t2E, t2F);
                t26 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x4F:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t3A;
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x53:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t3B;
                while (t24 > 0) {
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x57:
                a = t3C % 256;
                b = (t3C - (t3C % 256)) / 256;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                jmp_two = 0x8692;
            break;
            
            case 0x5B:
                a = t3E;
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x5F:
                a = t3F;
                if (write_output(output, counter++, a)) return -1;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x63:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 2;
                t26 = a;
                temp = y;
                y = x - t26;
                if (read_output(output, y, &a)) return -1;
                if (read_output(output, y+1, &b)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x67:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = decrypt(t2E, t2F);
                t26 = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a |= t26;
                t26 = a;
                temp = y;
                y = x - t24 - t26;
                b = y - (y % 256) / 256;
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x6B:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t2A;
                a &= 0x0F;
                b = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                b += 1;
                a += 3;
                t28 = (b*256) + a;
                
                temp = y;
                y = x - ((b*256) + a);
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x6F:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t2A;
                b = a;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                c = (b*256 + a);
                c <<= 4;
                while (c > 65536) {
                    c -= 65536;
                }
                t28 = c;
                c &= 0x0FF;
                b = (c - (c % 256)) / 256;
                
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                temp = y;
                t2A = a;
                a = decrypt(t2A, t2B);
                
                c = (b*256) + a;
                c &= 0x0F;
                c |= t28;
                t28 = c;
                c = x;
                c -= t28;
                // if (c < 0) c += 65536;
                y = c;
                b = y - (y % 256) / 256;
                while (t24 > 0) {
                    if (read_output(output, y, &a)) return -1;
                    y++;
                    if (write_output(output, counter++, a)) return -1;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x73:
                x--;
                if (read_output(output, x, &a)) return -1;
                if (write_output(output, counter++, a)) return -1;
                x += 2;
                jmp_two = 0x8692;
            break;
            
            case 0x77:
                x--;
                if (read_output(output, x, &b)) return -1;
                x--;
                if (read_output(output, x, &a)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 4;
                jmp_two = 0x8692;
            break;
            
            case 0x7B:
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2E = a;
                c = (b*256) + a;
                c &= 0xF0;
                c >>= 3;
                temp = y;
                c += 7;
                y = c;
                if (read_rom(rom, offset+y, &a)) return -1;
                if (read_rom(rom, offset+y+1, &b)) return -1;
                if (write_output(output, counter++, a)) return -1;
                if (write_output(output, counter++, b)) return -1;
                x += 2;
                y = temp;
                jmp_two = 0x86A0;
            break;
            
            default:
                printf("Invalid jump (%X).", jmp_one);
                return -2;
            break;
        } // switch(jmp_one);
        
        switch (jmp_two) {
        
            case 0:
                printf("END");
                return -2;
            break;
        
            case 0x8692:
                if ((offset+y) > 0x3FFFFF) return -1;
                if (read_rom(rom, offset+y, &a)) return -1;
                y++;
                t2E = a;
                a &= 0xF0;
                a >>= 2;
                t36 = a;
                jmp_one = t36;
            break;
            
            case 0x86A0:
                a = t2E;
                a &= 0x0F;
                a <<= 2;
                a += 0x3F;
                if (a > 256) a -= 256;
                t32 = a;
                jmp_one = t32;
            break;
            
            case 0x86AD:
                a = t2A;
                a &= 0x0F;
                a <<= 2;
                a += 0x3F;
                if (a >= 256) a -= 256;
                t32 = a;
                jmp_one = t32;
            break;
            
            default:
                printf("Exit\n");
                return -2;
            break;
        } // switch(jmp_two);
    } // Decompression loop.
    
    return -2;
    
} // decomp();
