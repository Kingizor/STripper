#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int decrypt(int dec_a, int dec_b) {
    // printf("decrypt - %d\n", dec_b);
    if (dec_b == 20) {
        return (dec_a - (dec_a % 16)) / 16;
    }
    else { // (dec_b == 21)
        return (dec_a % 16) * 16;
    }
} // decrypt();

/*
void write(char *name, uint8_t **output, unsigned int **counter) {

    FILE *out;
	printf("\nAttempting to save %d (0x%X) bytes...\n", counter, counter);
	out = fopen(name, "wb");
	fwrite(*output, 1, counter, out);
    fclose(out);
    printf("Output file written successfully.\n");

}
*/

void decomp(uint8_t *output, uint8_t *rom, int *length, int offset) {
    
    memset(output, 0, 65535); // Zero all bytes.
    int jmp_one, jmp_two, temp, a, b, c, t24, t26, t28, t2A, t2E, t32, t36, t3A, t3B, t3C, t3E, t3F;
    int run = 1;
    int x = 0, y = 0;
    int t2B = 20;
    int t2F = 21;
    // int breakat = 0x12B0;
    int counter = 0;
    
    y = 1;
    t3A = rom[offset+y];
    y++;
    t3B = rom[offset+y];
    y++;
    t3E = rom[offset+y];
    y++;
    t3F = rom[offset+y];
    y++;
    a = rom[offset+y];
    b = rom[offset+y+1];
    t3C = (b*256) + a;
    y = 0x027;
    t24 = 0;
    t26 = 0;
    a = rom[offset+y];
    y++;
    t2E = a;
    a &= 0xF0;
    a >>= 2;
    t36 = a;
    jmp_one = t36;
    
    
    // while (counter < 0x40) {
    while (run) {
        
        // printf("%X\n", jmp_one);
        // printf("a = %X\n", a);
        // printf("x = %d\n", x);
        // printf("y = %d\n", y);
        
        switch(jmp_one) {
            case 0x00:
                a = t2E;
                a &= 0x0F;
                
                if (a == 0) {
                    run = 0;
                    *length = counter;
                    return;
                }
                
                t24 = a;
                
                while (t24 > 0) {
                    a = rom[offset+y];
                    y++;
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x04:
                a = decrypt(t2E, t2F);
                t26 = a;
                a = rom[offset+y];
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                output[counter++] = a;
                x++;
                jmp_two = 0x86AD;
            break;
            
            case 0x08:
                a = decrypt(t2E, t2F);
                t26 = a;
                a = rom[offset+y];
                y++;
                t2A = a;
                t2E = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                output[counter++] = a;
                x++;
                a = decrypt(t2E, t2F);
                t26 = a;
                a = rom[offset+y];
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                output[counter++] = a;
                x++;
                jmp_two = 0x86AD;
            break;
            
            case 0xC:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                a = rom[offset+y];
                y++;
                while (t24 > 0) {
                    output[counter++] = a;
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
                    output[counter++] = a;
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
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x8692;
            break;
            
            case 0x18:
                a = t3C % 256;
                b = (t3C - (t3C % 256)) / 256;
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                jmp_two = 0x86A0;
            break;
            
            case 0x1C:
                a = t3E;
                output[counter++] = a;
                x++;
                jmp_two = 0x86A0;
            break;
            
            case 0x20:
                a = t3F;
                output[counter++] = a;
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
                
                a = output[y];
                b = output[y+1];
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x28:
                a = t2E;
                a &= 0x0F;
                a += 3;
                t24 = a;
                a = rom[offset+y];
                t26 = a;
                y++;
                temp = y;
                
                y = (x - t24 - t26);
                b = (y - (y % 256) / 256);
                
                while (t24 > 0) {
                    a = output[y];
                    y++;
                    output[counter++] = a;
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
                a = rom[offset+y];
                y++;
                b = a;
                a = rom[offset+y];
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
                    a = output[y];
                    y++;
                    output[counter++] = a;
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
                a = rom[offset+y+1];
                b = rom[offset+y];
                t28 = (b*256) + a;
                y += 2;
                temp = y;
                
                y = x - t28;
                b = (y - (y % 256)) / 256;
                if (y < 0) y += 65536;
                
                while (t24 > 0) {
                    a = output[y];
                    y++;
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x34:
                x--;
                a = output[x];
                output[counter++] = a;
                x += 2;
                jmp_two = 0x86A0;
            break;
            
            case 0x38:
                x--;
                b = output[x];
                x--;
                a = output[x];
                output[counter++] = a;
                output[counter++] = b;
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
                
                a = rom[offset+y];
                b = rom[offset+y+1];
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x3F:
                a = rom[offset+y];
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                
                if (a == 0) {
                    run = 0;
                    *length = counter;
                    return;
                }
                
                t24 = a;
                
                while (t24 > 0) {
                    a = decrypt(t2E, t2F);
                    t26 = a;
                    a = rom[offset+y];
                    y++;
                    t2A = a;
                    t2E = a;
                    a = t26;
                    a |= decrypt(t2A, t2B);
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x86A0;
            break;
            
            case 0x43:
                a = rom[offset+y];
                y++;
                output[counter++] = a;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x47:
                a = rom[offset+y];
                b = rom[offset+y+1];
                y += 2;
                x += 2;
                output[counter++] = a;
                output[counter++] = b;
                jmp_two = 0x8692;
            break;
            
            case 0x4B:
                a = rom[offset+y];
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = decrypt(t2E, t2F);
                t26 = a;
                a = rom[offset+y];
                y++;
                t2A = a;
                a = t26;
                a |= decrypt(t2A, t2B);
                while (t24 > 0) {
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x4F:
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t3A;
                while (t24 > 0) {
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x53:
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t3B;
                while (t24 > 0) {
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                jmp_two = 0x86AD;
            break;
            
            case 0x57:
                a = t3C % 256;
                b = (t3C - (t3C % 256)) / 256;
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                jmp_two = 0x8692;
            break;
            
            case 0x5B:
                a = t3E;
                output[counter++] = a;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x5F:
                a = t3F;
                output[counter++] = a;
                x++;
                jmp_two = 0x8692;
            break;
            
            case 0x63:
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 2;
                t26 = a;
                temp = y;
                y = x - t26;
                a = output[y];
                b = output[y+1];
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x67:
                a = rom[offset+y];
                y++;
                t2A = a;
                t2E = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = decrypt(t2E, t2F);
                t26 = a;
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a |= t26;
                t26 = a;
                temp = y;
                y = x - t24 - t26;
                b = y - (y % 256) / 256;
                while (t24 > 0) {
                    a = output[y];
                    y++;
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x6B:
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t2A;
                a &= 0x0F;
                b = a;
                a = rom[offset+y];
                y++;
                b += 1;
                a += 3;
                t28 = (b*256) + a;
                
                temp = y;
                y = x - ((b*256) + a);
                while (t24 > 0) {
                    a = output[y];
                    y++;
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x8692;
            break;
            
            case 0x6F:
                a = rom[offset+y];
                y++;
                t2A = a;
                a = decrypt(t2A, t2B);
                a += 3;
                t24 = a;
                a = t2A;
                b = a;
                a = rom[offset+y];
                y++;
                c = (b*256 + a);
                c <<= 4;
                while (c > 65536) {
                    c -= 65536;
                }
                t28 = c;
                c &= 0x0FF;
                b = (c - (c % 256)) / 256;
                
                a = rom[offset+y];
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
                    a = output[y];
                    y++;
                    output[counter++] = a;
                    x++;
                    t24--;
                }
                y = temp;
                jmp_two = 0x86AD;
            break;
            
            case 0x73:
                x--;
                a = output[x];
                output[counter++] = a;
                x += 2;
                jmp_two = 0x8692;
            break;
            
            case 0x77:
                x--;
                b = output[x];
                x--;
                a = output[x];
                output[counter++] = a;
                output[counter++] = b;
                x += 4;
                jmp_two = 0x8692;
            break;
            
            case 0x7B:
                a = rom[offset+y];
                y++;
                t2E = a;
                c = (b*256) + a;
                c &= 0xF0;
                c >>= 3;
                temp = y;
                c += 7;
                y = c;
                a = rom[offset+y];
                b = rom[offset+y+1];
                output[counter++] = a;
                output[counter++] = b;
                x += 2;
                y = temp;
                jmp_two = 0x86A0;
            break;
            
            default:
                printf("Invalid jump (%X).", jmp_one);
                return;
            break;
        } // switch(jmp_one);
        
        switch (jmp_two) {
        
            case 0:
                printf("END");
                return;
            break;
        
            case 0x8692:
                a = rom[offset+y];
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
                return;
            break;
        } // switch(jmp_two);
    } // Decompression loop.
    
} // decomp();

/*
void show_usage() {
    printf("\nUsage: decomp [OFFSET] [INPUT/OUTPUT]\n");
    printf("e.g.   decomp DKC3.sfc 254C41\n");
}

int main(int argc, char *argv[]) {

    
    uint8_t *rom;
    uint8_t *output = malloc(65535);
    memset(output, 0, 65535);
    unsigned int *counter = malloc(sizeof(int));

    unsigned int i, romlen;
	size_t offlen;
    FILE * romfile;
    
    printf("\nKingizor's DKC2/DKC3 decompressor\n");
    
    if (argc != 3) {
        show_usage();
        return 0;
    }
    
    offlen = strlen(argv[2]);
    
    for (i = 0; i < offlen; i++) {
        
        if (argv[2][i] > 47 && argv[2][i] < 58) {
            // 0-9
        }
        else if (argv[2][i] > 64 && argv[2][i] < 71) {
            // A-F
        }
        else if (argv[2][i] > 96 && argv[2][i] < 103) {
            // a-f
        }
        else {
            printf("\nInvalid offset - %d at argv[2][%d].\n", argv[2][i], i);
            return 0;
        }
    }
    unsigned int offset = strtol(argv[2], NULL, 16);
    
    romfile = fopen(argv[1], "r");

    if (romfile != NULL) {
        fseek(romfile, 0, SEEK_END);
        romlen = ftell(romfile);
        rewind(romfile);
        
        if (offset > romlen) {
            printf("\nError: Decompression offset is larger than input file.\n");
            return 0;
        }
        
        rom = malloc(romlen);
        // output = malloc(65535);
        fread(rom, 1, romlen, romfile);
        fclose(romfile);
        printf("\nROM file loaded.\n");
    }
    else {
        printf("\nError: Could not open input file.\n");
        return 0;
    }
    
    printf("\nAttempting to decompress data at %X...\n", offset);
    decomp(&output, &rom, &counter, offset);
    fclose(romfile);
    printf("Decompression routine completed.\n");
    write(argv[2], &output, &counter);
    
    return 0;
    
} // main();
*/