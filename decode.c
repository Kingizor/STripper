#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void ror(uint8_t *a, uint8_t *b, int count) {

    int carry = 0, i;
    unsigned int c;
    
    c = ((*b)*256) + *a;
    
    for (i = 0; i < count; i++) {
        if (carry == 1) {
            c += 65536;
            carry = 0;
        }
        if (c & 1) carry = 1;
        c >>= 1;
    }
    
    *a = (c % 256);
    *b = (c - (c % 256))/256;
    
    return;

} // ror(); ~ Rotate Right


static void swap(uint8_t *a, uint8_t *b) {

    int temp = *a;
    *a = *b;
    *b = temp;
    return;
    
} // swap();


void decode(uint8_t *rom, uint8_t *output, unsigned int *outlen, unsigned int x, unsigned int bank) {

    memset(output, 0, 65535);
    unsigned int c = 0, y = 0, tempA = 0, tempB = 0, tempX = 0;
    uint8_t a = 0, b = 0, t1C = 0, t1D = 0, t1E = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t28 = 0, t29 = 0, t3E = 0, t3F = 0, t40 = 0, t41 = 0, pushX = 0;
    int recurse = 0; // Index of origin
    int origin[255]; // RTS
    int jmp = 0; // Next
    int run = 1;
    
    // $BB:8D03
    a = rom[0x3D1B03 + x];
    b = rom[0x3D1B03 + x + 1];
    
    if (a == 0) {
        // $8D6F;
        if (x == 0xD62) {
            a = 0xBB;
            b = 0x19;
        } // Boss Photos (US)
        else if (x == 0xD3D) {
            a = 0xE2;
            b = 0x12;
        } // Bear Photos (US)
        else if (x == 0xD15) {
            a = 0xF3;
            b = 0x16;
        } // Boss Photos (JP)
        else if (x == 0xCF0) {
            a = 0x1A;
            b = 0x10;
        } // Bear Photos (JP)
        
        else {
            printf("Unknown jump.\n");
            return;
        }
        if (rom[0xFFD9] == 2) a--; // EU
        
        x = 0x00;
        y = 0xEB;
        jmp = 0x8A73;
    }
    else {
        a = rom[0x3D1B07 + x];
        b = rom[0x3D1B07 + x + 1];
        
        if (a > 0x80) {
            jmp = 0x8D73;
        }
        else {
            printf("\nError @decode();\n");
            printf("a = %X\n", a);
            printf("b = %X\n\n", b);
            return;
        }
    }
    // int count = 0;
    
    while (run) {
        
        /*
        printf("\n%04d:\n", count);
        printf("\tjmp = %X\n", jmp);
        printf("\tc = %02X%02X\n", b, a);
        printf("\tx = %04X\n", x);
        printf("\ty = %04X\n", y);
        count++;
        */
        
        switch (jmp) {
            case 0x8A73:
                t24 = a;
                t25 = b;
                // t26 = y;
                y = 0;
                t3E = 0;
                t3F = 0;
                t40 = 0;
                t41 = 0;
                a = rom[bank + (t24+(t25*256))];
                b = rom[bank + (t24+(t25*256)) + 1];
                y++;
                t28 = a;
                t29 = b;
                a &= 0xE0;
                b = 0;
                b = a;
                a = 0; // swap bytes
                tempA = a;
                tempB = b;
                
                a = rom[bank + t24 + (t25*256) + y];
                b = rom[bank + t24 + (t25*256) + y + 1];
                y = (b*256)+a;
                *outlen = y * 2; // Limit
                if (t24 + 3 > 255) t25 += 1;
                t24 += 3;
                
                a = tempA;
                b = tempB;
                
                do {
                    output[x]   = a;
                    output[x+1] = b;
                    x += 2;
                    y--;
                } while (y > 0);
                
                a = t28;
                b = t29;
                
                if ((a & 1) == 0) {
                    jmp = 0x8AA5;
                }
                else {
                    jmp = 0x8ABF;
                    origin[recurse] = 0x8AA5;
                    recurse++;
                }
            break;
        
            case 0x8AA5:
                a = t28;
                b = t29;
                if ((a & 2) == 0) {
                    jmp = 0x8AAF;
                }
                else {
                    jmp = 0x8B4F;
                    origin[recurse] = 0x8AAF;
                    recurse++;
                }
            break;
        
        
            case 0x8AAF:
                a = t28;
                b = t29;
                if ((a & 4) == 0) {
                    jmp = 0x8AB9;
                }
                else {
                    jmp = 0x8B09;
                    origin[recurse] = 0x8AB9;
                    recurse++;
                }
            break;
        
            case 0x8AB9:
                jmp = 0x8B97;
                origin[recurse] = 0x8ABC;
                recurse++;
            break;
        
            case 0x8ABC:
                jmp = 0x8BDD;
            break;
            
            case 0x8ABF:
                b = 0;
                a = 0;
                tempA = 0;
                jmp = 0x8AC3;
            break;
            
            case 0x8AC3:
                a = 2;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8AC9;
                recurse++;
            break;
        
            case 0x8AC9:
                if ((a & 2) == 0) {
                    jmp = 0x8AF4;
                }
                else {
                    a &= 1;
                    ror(&a, &b, 4);
                    
                    t22 = a;
                    t23 = b;
                    a = 6;
                    b = 0;
                    jmp = 0x8C76;
                    origin[recurse] = 0x8ADE;
                    recurse++;
                }
            break;

            case 0x8ADE:
                if (a == 0) {
                    jmp = 0x8B07;
                }
                else {
                    y = (b*256)+a;
                    if (pushX == 1) {
                        x = tempX; 
                    } 
                    else { 
                        x = 0; 
                    }
                    pushX = 0;
                    do {
                        a = output[x];
                        b = output[x+1];
                        a |= t22;
                        b |= t23;
                        output[x]   = a;
                        output[x+1] = b;
                        x += 2;
                        y--;
                    } while (y > 0);
                    tempX = x;
                    pushX = 1;
                    jmp = 0x8AC3;
                }
            break;
        
            case 0x8AF4:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                ror(&a, &b, 4);
                
                a |= output[x];
                b |= output[x+1];
                output[x] = a;
                output[x+1] = b;
                x += 2;
                tempX = x;
                pushX = 1;
                jmp = 0x8AC3;
            break;
        
            case 0x8B07:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                recurse--;
                jmp = origin[recurse];
            break;
            
            case 0x8B09:
                a = 0;
                b = 0;
                tempA = a;
                jmp = 0x8B0D;
            break;
            
            case 0x8B0D:
                a = 2;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8B13;
                recurse++;
            break;
            
            case 0x8B13:
                if ((a & 2) == 0) {
                    jmp = 0x8B3C; // $BB/8B16 (Check these)
                }
                else {
                    a &= 1;
                    
                    ror(&a, &b, 2);
                    
                    t22 = a;
                    t23 = b;
                    b = 0;
                    a = 6;
                    
                    jmp = 0x8C76;
                    origin[recurse] = 0x8B26;
                    recurse++;
                }
            break;
            
            case 0x8B26:
                if (a == 0) {
                    jmp = 0x8B4D;
                }
                else {
                    y = (b*256)+a;
                    if (pushX == 1) {
                        x = tempX; 
                    } 
                    else { 
                        x = 0; 
                    }
                    pushX = 0;
                    
                    do {
                        a = output[x];
                        b = output[x + 1];
                        a |= t22;
                        b |= t23;
                        output[x]   = a;
                        output[x+1] = b;
                        x += 2;
                        y--;
                    } while (y > 0);
                    
                    tempX = x;
                    pushX = 1;
                    jmp = 0x8B0D;
                }
            break;
            
            case 0x8B3C:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                ror(&a, &b, 2);
                a |= output[x];
                b |= output[x+1];
                output[x]   = a;
                output[x+1] = b;
                x += 2;
                tempX = x;
                pushX = 1;
                jmp = 0x8B0D;
            break;
            
            case 0x8B4D:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                recurse--;
                jmp = origin[recurse];
            break;
            
            case 0x8B4F:
                a = 0;
                b = 0;
                tempA = a;
                jmp = 0x8B53;
            break;
            
            case 0x8B53:
                b = 0;
                a = 2;
                jmp = 0x8C76;
                origin[recurse] = 0x8B59;
                recurse++;
            break;
        
            case 0x8B59:
                if ((a & 2) == 0) {
                    jmp = 0x8B83;
                }
                else {
                    a &= 1;
                    
                    ror(&a, &b, 3);
                    
                    t22 = a;
                    t23 = b;
                    a = 6;
                    b = 0;
                    jmp = 0x8C76;
                    origin[recurse] = 0x8B6D;
                    recurse++;
                }
            break;
        
            case 0x8B6D:
                if (a == 0) {
                    jmp = 0x8B95;
                }
                else {
                    y = (b*256)+a;
                    if (pushX == 1) {
                        x = tempX; 
                    } 
                    else { 
                        x = 0; 
                    }
                    pushX = 0;
                    do {
                        a = output[x];
                        b = output[x+1];
                        a |= t22;
                        b |= t23;
                        output[x] = a;
                        output[x+1] = b;
                        x += 2;
                        y--;
                    } while (y > 0);
                    tempX = x;
                    pushX = 1;
                    jmp = 0x8B53;
                }
            break;
            
            case 0x8B83:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                
                ror(&a, &b, 3);
                
                a |= output[x];
                b |= output[x+1];
                output[x] = a;
                output[x+1] = b;
                x += 2;
                tempX = x;
                pushX = 1;
                jmp = 0x8B53;
                
            break;
            
            case 0x8B95:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                recurse--;
                jmp = origin[recurse];
            break;
            
            case 0x8B97:
                b = 0;
                a = 0;
                tempA = a;
                jmp = 0x8B9B;
            break;
            
            case 0x8B9B:
                a = 4;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8BA1;
                recurse++;
            break;
            
            case 0x8BA1:
                if ((a & 8) == 0) {
                    jmp = 0x8BCA;
                }
                else {
                    a &= 7;
                    swap(&a, &b);
                    
                    c = (b*256) + a;
                    c <<= 2;
                    while (c > 65535) {
                        c -= 65536;
                    }
                    a = (c % 256);
                    b = (c - (c % 256))/256;
                    t22 = a;
                    t23 = b;
                    a = 4;
                    b = 0;
                    jmp = 0x8C76;
                    origin[recurse] = 0x8BB4;
                    recurse++;
                }
            break;
            
            case 0x8BB4:
                if (a == 0) {
                    jmp = 0x8BDB;
                }
                else {
                    y = (b*256)+a;
                    if (pushX == 1) {
                        x = tempX; 
                    } 
                    else { 
                        x = 0; 
                    }
                    pushX = 0;
                    do {
                        a = output[x];
                        b = output[x+1];
                        a |= t22;
                        b |= t23;
                        output[x]   = a;
                        output[x+1] = b;
                        x += 2;
                        y--;
                    } while (y > 0);
                    tempX = x;
                    pushX = 1;
                    jmp = 0x8B9B;
                }
            break;
            
            case 0x8BCA:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                swap(&a, &b);
                c = (b*256)+a;
                c <<= 2;
                while (c > 65535) {
                    c -= 65536;
                }
                a = (c % 256);
                b = (c - (c % 256))/256;
                a |= output[x];
                b |= output[x+1];
                output[x] = a;
                output[x+1] = b;
                x += 2;
                tempX = x;
                pushX = 1;
                jmp = 0x8B9B;
            break;
        
            case 0x8BDB:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                recurse--;
                jmp = origin[recurse];
            break;
        
            case 0x8BDD:
                b = 0;
                a = 0;
                tempA = a;
                jmp = 0x8BE1;
            break;
            
            case 0x8BE1:
                a = 4;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8BE7;
                recurse++;
            break;
            
            case 0x8BE7:
                swap(&a, &b);
                t20 = a;
                t21 = b;
                a = 8;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8BF0;
                recurse++;
            break;
            
            case 0x8BF0:
                a |= t20;
                b |= t21;
                if ((b & 12) == 0) {
                    jmp = 0x8C65;
                }
                else if ((b & 8) == 0) {
                    jmp = 0x8C43;
                }
                else if ((b & 4) == 0) {
                    jmp = 0x8C22;
                }
                else {
                    a &= 0xFF;
                    b &= 3;
                    t22 = a;
                    t23 = b;
                    b = 0;
                    a = 4;
                    jmp = 0x8C76;
                    origin[recurse] = 0x8C0C;
                    recurse++;
                }
            break;
            
            case 0x8C01:
                a &= 0xFF;
                b &= 0x03;
                t22 = a;
                t23 = b;
                a = 4;
                b = 0;
                jmp = 0x8C76;
                origin[recurse] = 0x8C0C;
                recurse++;
            break;
            
            case 0x8C0C:
                y = (b*256)+a;
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                do {
                    a = output[x];
                    b = output[x+1];
                    a |= t22;
                    b |= t23;
                    output[x] = a;
                    output[x+1] = b;
                    if (t22 - 1 < 0) t23--;
                    t22--;
                    x += 2;
                    y--;
                } while (y > 0);
                tempX = x;
                pushX = 1;
                jmp = 0x8BE1;
            break;
            
            case 0x8C22:
                a &= 0xFF;
                b &= 0x03;
                t22 = a;
                t23 = b;
                b = 0;
                a = 4;
                jmp = 0x8C76;
                origin[recurse] = 0x8C2D;
                recurse++;
            break;
            
            case 0x8C2D:
                y = (b*256)+a;
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                do {
                    a = output[x];
                    b = output[x+1];
                    a |= t22;
                    b |= t23;
                    output[x] = a;
                    output[x+1] = b;
                    if (t22 + 1 > 255) t23++;
                    t22++;
                    x += 2;
                    y--;
                } while (y > 0);
                tempX = x;
                pushX = 1;
                jmp = 0x8BE1;
            break;
            
            case 0x8C43:
                a &= 0xFF;
                b &= 3;
                t22 = a;
                t23 = b;
                b = 0;
                a = 6;
                jmp = 0x8C76;
                origin[recurse] = 0x8C4E;
                recurse++;
            break;
            
            case 0x8C4E:
                if (a == 0) {
                    jmp = 0x8C74;
                }
                else {
                    y = (b*256)+a;
                    if (pushX == 1) {
                        x = tempX; 
                    } 
                    else { 
                        x = 0; 
                    }
                    pushX = 0;
                    do {
                        a = output[x];
                        b = output[x+1];
                        a |= t22;
                        b |= t23;
                        output[x]   = a;
                        output[x+1] = b;
                        x += 2;
                        y--;
                    } while (y > 0);
                    tempX = x;
                    pushX = 1;
                    jmp = 0x8BE1;
                }
            break;
            
            case 0x8C65:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                a |= output[x];
                b |= output[x+1];
                output[x]   = a;
                output[x+1] = b;
                x += 2;
                tempX = x;
                pushX = 1;
                jmp = 0x8BE1;
            break;
            
            case 0x8C74:
                run = 0;
            break;
            
            case 0x8C76:
                t1C = a;
                t1D = b;
                a = t3E;
                b = t3F;
                x = (b*256)+a;
                b = 0;
                a &= 6;
                if ((a + t1C) > 0xFF) b += 1;
                a += t1C;
                b += t1D;
                if ((a + 0xA5) > 0xFF) b += 1;
                a += 0xA5;
                b += 0x8C;
                t1E = a; // jmp
                // t1F = b;
                a = t40;
                b = t41;
                c = (b*256)+a;
                c >>= 2;
                b = (c - (c%256)) / 256;
                a = (c % 256);
                if ((a + t3E) > 0xFF) b += 1;
                a += t3E;
                b += t3F;
                c = (b*256)+a;
                c >>= 3;
                b = (c - (c%256)) / 256;
                a = (c % 256);
                y = (b*256)+a;
                b = (x - (x % 256)) / 256;
                a = (x % 256);
                if ((a + t1C) > 0xFF) b += 1;
                a += t1C;
                b += t1D;
                t3E = a;
                t3F = b;
                // Skip LDA, ADC, STA
                b = rom[bank + (t25*256) + t24 + y];
                a = rom[bank + (t25*256) + t24 + y + 1]; // XBA
                // jmp
                c = (b*256)+a;
                c >>= (0xB5 - t1E);
                b = (c - (c%256)) / 256;
                a = (c % 256);
                x = (t1D*256)+t1C;
                a &= rom[0x3B8CBA + x];
                b &= rom[0x3B8CBA + x + 1];
                recurse--;
                jmp = origin[recurse];
            break;
            
            case 0x8D73:
            
                // 16-bit A
                y = rom[0x3D1B04 + x] + (rom[0x3D1B04 + x + 1]*256);
                a = rom[0x3D1B03 + x];
                
                a &= 0xC0;
                b = 0;
                
                a >>= 5;
                tempX = x;
                pushX = 1;
                x = a;
                
                jmp = 0x8DCC;
                
            break;
        
            case 0x8DCC:
                if (pushX == 1) {
                    x = tempX; 
                } 
                else { 
                    x = 0; 
                }
                pushX = 0;
                // PLX, PHX ??
                tempX = 0; // ?
                a = rom[0x3D1B03 + x];
                a |= 0xC0;
                a &= 0xFF;
                b = 0;
                y = (b*256)+a;
                a = rom[0x3D1B04 + x];
                b = rom[0x3D1B04 + x + 1];
                x = 0;
                jmp = 0x8A73;
                origin[recurse] = 0x8A73;
                recurse++;
            break;
        
            default:
                printf("Unexpected jump - %X", jmp);
                run = 0;
            break;
            
        } // switch(jmp);
        
    
    } // while(...);
    
    
    return;

} // decode();

/*
int main(int argc, char *argv[]) {

    // if (argc != 3) return 0;
    
    // at.exe [INPUT] [OUTPUT] [X] [BANK]
    
    FILE * romfile;
    romfile = fopen(argv[1], "rb");
    uint8_t *rom = malloc(4194304);
    fread(rom, 1, 4194304, romfile);
    fclose(romfile);
    
    uint8_t *output = malloc(0xFFFF);
    unsigned int *outlen = malloc(sizeof(int));
    
    unsigned int x    = strtol(argv[3], NULL, 16);
    unsigned int bank = strtol(argv[4], NULL, 16); // No error checking
    
    decode(&rom, &output, &outlen, x, bank);
    
    FILE * out;
    out = fopen(argv[2], "wb");
    fwrite(output, 1, outlen[0], out);
    
    printf("\nOutput Length = %X bytes.\n", outlen[0]);
    printf("Program Complete\n");
    
    return 0;

}
*/