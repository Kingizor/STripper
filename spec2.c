#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

void spec2(uint8_t *rom, char dir[255], int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    int bp_counter;
    int raw_counter;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    // Special Screens
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A5978);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A5978);
    }
    decomp(raw_data, rom, &raw_counter, 0x8F99C);
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2170, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "K.Rool's Kabin & Rattly Room");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2C83A0);
        decomp(raw_data, rom, &raw_counter, 0x2C7CF0);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2C83A0);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2C7CF0);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3C6E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "File Screen");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 3, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bonus Screen");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2D0997);
        decomp(raw_data, rom, &raw_counter, 0x2D02A1);
    }
    else if (region == 0) {
        decomp( bp_data, rom,  &bp_counter, 47 + 0x2D0997);
        memcpy(raw_data, &rom[0x2D02C6], 0x700);
        raw_counter = 0x700;
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2D0997);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2D02A1);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D26AE, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Title Screen");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2D8584);
        decomp(raw_data, rom, &raw_counter, 0x2D9C9A);
    }
    else if (region == 0) {
        decomp( bp_data, rom,  &bp_counter, 6939 + 0x2D8584);
        decomp(raw_data, rom, &raw_counter, 6939 + 0x2D9C9A);
    }
    else if (region == 2) {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2D8584);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2D9C9A);
    } // (En, Fr)
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2D8584);
        decomp(raw_data, rom, &raw_counter, 35 + 0x2D9C9A);
    } // (En, De)
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 3, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Find the Token Overlay");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2D9179);
        decomp(raw_data, rom, &raw_counter, 0x2D9E72);
    }
    else if (region == 0) {
        decomp( bp_data, rom,  &bp_counter, 6939 + 0x2D9179);
        decomp(raw_data, rom, &raw_counter, 6939 + 0x2D9E72);
    }
    else if (region == 2) {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2D9179);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2D9E72);
    } // (En, Fr)
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2D9179);
        decomp(raw_data, rom, &raw_counter, 35 + 0x2D9E72);
    } // (En, De)
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 4, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Destroy Them All Overlay");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2D78D3);
        decomp(raw_data, rom, &raw_counter, 0x2DA030);
    }
    else if (region == 0) {
        decomp( bp_data, rom,  &bp_counter, 6939 + 0x2D78D3);
        decomp(raw_data, rom, &raw_counter, 6939 + 0x2DA030);
    }
    else if (region == 2) {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2D78D3);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2DA030);
    } // (En, De)
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2D78D3);
        decomp(raw_data, rom, &raw_counter, 35 + 0x2DA030);
    } // (En, De)
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 5, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Collect the Stars Overlay");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2C4D40);
        decomp(raw_data, rom, &raw_counter, 0x2C4749);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 34 + 0x2C4D40);
        decomp(raw_data, rom, &raw_counter, 33 + 0x2C4749);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3C6E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Game Selection");
    
    raw_counter = 0;
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2C4AAD);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 33 + 0x2C4AAD);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3C6E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Music Test");
    
    raw_counter = 0;
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2C4C1C);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 34 + 0x2C4C1C);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3C6E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Cheat Mode");
    
     bp_counter = 0;
    raw_counter = 0x700;
    decomp( bp_data, rom,  &bp_counter, 0x12B9FD);
    memcpy(raw_data, &rom[0x0EFA9F], 0x700);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D11F0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kong Kollege");
    
     bp_counter = 0;
    raw_counter = 0x700;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2DA1AB);
    }
    else if (region == 0) {
        decomp( bp_data, rom,  &bp_counter, 6939 + 0x2DA1AB);
    }
    else if (region == 2) {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2DA1AB);
    } // (En, Fr)
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2DA1AB);
    } // (En, De)
    decomp(raw_data, rom, &raw_counter, 0x05FAA8);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D12F0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Swanky's Bonus Bonanza");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2BCFD1);
        decomp(raw_data, rom, &raw_counter, 0x2C1B0F);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2BCFD1);
        decomp(raw_data, rom, &raw_counter, 33 + 0x2C1B0F);
    }
    raw_counter = 0x700;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0FF0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Monkey Museum");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2BA27C);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2BA27C);
    }
    decomp(raw_data, rom, &raw_counter, 0x03FD35);
    raw_counter = 0x700;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D10F0, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Funky's Flights");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A121C);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A121C);
    }
    decomp(raw_data, rom, &raw_counter, 0x0AFABE);
    raw_counter = 0x700;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D10F0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Funky's Flights BG");
    
     bp_counter = 0;
    raw_counter = 0x700;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2B50CE);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2B50CE);
    }
    memcpy(raw_data, &rom[0x330000], 0x700);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D13F0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Klubba's Kiosk");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2CD032);
        decomp(raw_data, rom, &raw_counter, 0x2CF8CE);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 37 + 0x2CD032);
        decomp(raw_data, rom, &raw_counter, 37 + 0x2CF8CE);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D402E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Video Game Heroes Screen (Unused Version)");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x361D3E);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2CFDEE);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 37 + 0x2CFDEE);
    }
    raw_counter = 0x700;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D402E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Video Game Heroes Screen");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x399400);
    decomp(raw_data, rom, &raw_counter, 0x39C775);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 6, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ending Screen Background"); // Not broken. There is transparency in the sunset.
    
     bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x3661C1);
    decomp(raw_data, rom, &raw_counter, 0x367D1B);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x000000, raw_counter, bp_counter, 2, 6, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ending Screen Foreground");
    
     bp_counter = 0;
    raw_counter = 0x700;
    decomp( bp_data, rom,  &bp_counter, 0x3444D2);
    memcpy(raw_data, &rom[0x3A56C1], 0x700);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D258E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Game Over Screen");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x1D0AFC);
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2ADA86);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2ADA86);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0B10, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Overworld Map");
    
     bp_counter = 0;
    raw_counter = 0x700;
    memcpy(raw_data, &rom[0x19F97B], 0x700);
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x26FCF3);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x26FCF3);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1870, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gangplank Galleon");
    
     bp_counter = 0;
    raw_counter = 0x700;
    if (region == 1) {
        memcpy(raw_data, &rom[0x274D71], 0x700);
        decomp( bp_data, rom,  &bp_counter, 0x275471);
    }
    else {
        memcpy(raw_data, &rom[35 + 0x274D71], 0x700);
        decomp( bp_data, rom,  &bp_counter, 35 + 0x275471);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1970, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Crocodile Cauldron");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x17F9A7);
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x279E78);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x279E78);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1A70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krem Quay Left");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x04F9C0);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1A70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krem Quay Right");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x1BFB2C);
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x280535);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x280535);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1B70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krazy Kremland Top");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x1A4C4F);
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2909EC);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2909EC);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2070, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krazy Kremland Bottom");
    
     bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2854F8);
        decomp(raw_data, rom, &raw_counter, 0x284E0D);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2854F8);
        decomp(raw_data, rom, &raw_counter, 35 + 0x284E0D);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1C70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gloomy Gulch");
    
     bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x382106);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x28A6D7);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 35 + 0x28A6D7);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1D70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "K.Rool's Keep");
    
     bp_counter = 0;
    raw_counter = 0x700;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x28B924);
        memcpy(raw_data, &rom[0x28B224], 0x700);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x28B924);
        memcpy(raw_data, &rom[35 + 0x28B224], 0x700);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1E70, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "The Flying Krock");
    
     bp_counter = 0;
    raw_counter = 0x700;
    decomp( bp_data, rom,  &bp_counter, 0x0BAC49);
    if (region == 1) {
        memcpy(raw_data, &rom[0x298407], 0x700);
    }
    else {
        memcpy(raw_data, &rom[35 + 0x298407], 0x700);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1F70, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "The Lost World");
    // The Lost World has an overlay that requires some palette alterations and unusual layout generation.
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x29FDB3);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x29FDB3);
    }
    decomp(raw_data, rom, &raw_counter, 0x07FCB8);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0CD0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Pirate Panic Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0CD0, raw_counter, bp_counter, 2, 65, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gangplank Galley Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Rattle Battle Background");
    
    /*
    bp_counter = 0x1C20;
    raw_counter = 0x800;
    if (region == 1) {
        memcpy(&bp_data[0x13E0], &rom[0x352BA7], 0x840);
        FILE *vl = fopen("VL.bin", "rb");
        FILE *vh = fopen("VH.bin", "rb");
        fread(raw_data, 1, 0x800, vl);
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 3, 0, 0);
        assemble_screen(bitplane, raw_counter, 32, dir, "PP_FG_UL");
        fread(raw_data, 1, 0x800, vl);
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 3, 0, 0);
        assemble_screen(bitplane, raw_counter, 32, dir, "PP_FG_UR");
        fread(raw_data, 1, 0x800, vh);
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 3, 0, 0);
        assemble_screen(bitplane, raw_counter, 32, dir, "PP_FG_LL");
        fread(raw_data, 1, 0x800, vh);
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 3, 0, 0);
        assemble_screen(bitplane, raw_counter, 32, dir, "PP_FG_LR");
    }
    */
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A121C);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A121C);
    }
    decomp(raw_data, rom, &raw_counter, 0x0AFABE);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1610, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mainbrace Mayhem Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D14F0, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Topsail Trouble Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D304E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Slime Climb Background");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x29A905);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x29A905);
    }
    decomp(raw_data, rom, &raw_counter, 0x02FAC9);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lockjaw's Locker Background-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 2, 1, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Glimmer's Galleon Background-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lava Lagoon Background-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 2, 44, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Toxic Tower Background-1");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x06FC11);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lockjaw's Locker Background-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 2, 1, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Glimmer's Galleon Background-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lava Lagoon Background-2");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x0CFD95);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lockjaw's Locker Background-3");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 2, 1, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Glimmer's Galleon Background-3");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lava Lagoon Background-3");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x09FF37);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lockjaw's Locker Background-4");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 2, 1, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Glimmer's Galleon Background-4");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lava Lagoon Background-4");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
    decomp( bp_data, rom,  &bp_counter, 0x2A4E35);
    }
    else {
    decomp( bp_data, rom,  &bp_counter, 35 + 0x2A4E35);
    }
    memcpy(&bp_data[0x123*16], bp_data, bp_counter);
    bp_counter += (0x123*16);
    decomp(raw_data, rom, &raw_counter, 0x09F99A);
    memcpy(&raw_data[0x700], &raw_data[0x6C0], 0x40);
    memcpy(&raw_data[0x740], &raw_data[0x6C0], 0x40);
    memcpy(&raw_data[0x780], &raw_data[0x6C0], 0x40);
    memcpy(&raw_data[0x7C0], &raw_data[0x6C0], 0x40);
    raw_counter = 0x800;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D27EE, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Bayou Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D450E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krockhead Klamber Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D460E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mudhole Marsh Background");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A3BEA);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A3BEA);
    }
    decomp(raw_data, rom, &raw_counter, 0x0DFEE1);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D27EE, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Bayou Foreground-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D450E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krockhead Klamber Foreground-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D460E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mudhole Marsh Foreground-1");
    
    raw_counter = 0;
    decomp(raw_data, rom, &raw_counter, 0x1AFBB1);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D27EE, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Barrel Bayou Foreground-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D450E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Krockhead Klamber Foreground-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D460E, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Mudhole Marsh Foreground-2");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x360000);
    decomp(raw_data, rom, &raw_counter, 0x13FAF0);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D228E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kannon's Klaim Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D238E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Squawks's Shaft Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D248E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Windy Well Background");
    
    bp_counter = 0;
    raw_counter = 0;
    decomp( bp_data, rom,  &bp_counter, 0x16F791);
    decomp(raw_data, rom, &raw_counter, 0x14FC2D);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0A10, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Hive Background");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2B2EA0);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2B2EA0);
    }
    memcpy(&bp_data[0x600], bp_data, bp_counter);
    memset(bp_data, 0, 0x600);
    bp_counter += 0x600;
    decomp(raw_data, rom, &raw_counter, 0x18FB53);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0A10, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Hive Foreground");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2AB6F0);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2AB6F0);
    }
    decomp(raw_data, rom, &raw_counter, 0x380000);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D28EE, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bramble Blast Background-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2AEE, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bramble Scramble Background-1");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D29EE, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Screech's Sprint Background-1");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2AA94D);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2AA94D);
    }
    memcpy(&bp_data[0x111*16], bp_data, bp_counter);
    memset(bp_data, 0, 0x111*16);
    bp_counter += 0x111*16;
    decomp(raw_data, rom, &raw_counter, 0x1DFDBA);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D28EE, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bramble Blast Background-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2AEE, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Bramble Scramble Background-2");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D29EE, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Screech's Sprint Background-2");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A23D4);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A23D4);
    }
    decomp(raw_data, rom, &raw_counter, 0x0FFFD9);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1710, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Fair Background");
    
    // Long BG.
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x29943A);
        decomp(raw_data, rom, &raw_counter, 0x298C68);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x29943A);
        decomp(raw_data, rom, &raw_counter, 35 + 0x298C68);
    }
    bp_counter = 0x1000;
    raw_counter = 0x1000;
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2EEE, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Hall Background");
    
    // BG Only
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x295221);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x295221);
    }
    memcpy(&bp_data[0x6A0], bp_data, bp_counter);
    memset(bp_data, 0, 0x6A0);
    bp_counter += 0x6A0;
    decomp(raw_data, rom, &raw_counter, 0x0CFEDF);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2EEE, raw_counter, bp_counter, 2, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Kackle Body");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A972E);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A972E);
    }
    decomp(raw_data, rom, &raw_counter, 0x10FE0D);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D06F0, raw_counter, bp_counter, 2, 43, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Gloomy Gulch Foreground"); // Lights
    
    bp_counter = 0;
    raw_counter = 0;
    memcpy(bp_data, &rom[0x3B6FC0], 0x1000);
    bp_counter = 0x1000;
    decomp( raw_data, rom, &raw_counter, 0x15FEB3);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D06F0, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ghostly Grove & Gusty Glade Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3A4E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Web Woods Background");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x295623);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x295623);
    }
    decomp(raw_data, rom, &raw_counter, 0x1CFA5E);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D398E, raw_counter, bp_counter, 2, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ice Background");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x2A10BB);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x2A10BB);
    }
    memcpy(&bp_data[0x14C*16], bp_data, bp_counter);
    memset(bp_data, 0, 0x14C*16);
    bp_counter += 0x14C*16;
    decomp(raw_data, rom, &raw_counter, 0x01FEC0);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Lava");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Water");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D398E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Ice Water");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 3, 44, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Toxic Tower Water");
    
    bp_counter = 0;
    raw_counter = 0x800;
    memset(raw_data, 0, 0x800);
    if (region == 1) {
        decomp(bp_data, rom,  &bp_counter, 0x2B4916);
        memcpy(&raw_data[0x480], &rom[0x29A745], 0x1C0);
    }
    else {
        decomp(bp_data, rom,  &bp_counter, 35 + 0x2B4916);
        memcpy(&raw_data[0x480], &rom[35 + 0x29A745], 0x1C0);
    }
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 3, 0, 0);
    assemble_screen(bitplane, raw_counter, 32, dir, "Castle Background-1");
    
    bp_counter = 0;
    raw_counter = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x297549);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 35 + 0x297549);
    }
    decomp(raw_data, rom, &raw_counter, 0x330802);
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D430E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Klobber Karnage Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D440E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Jungle Jinx Background");
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3B6E, raw_counter, bp_counter, 3, 0, 1);
    assemble_screen(bitplane, raw_counter, 32, dir, "Animal Antics Background");
    
    // Animated
    // bp_counter = 0;
    // raw_counter = 0;
    // decomp( bp_data, rom,  &bp_counter, 0x2AD7DE);
    // decomp(raw_data, rom, &raw_counter, 0x2694EC);
    // decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2CEE, raw_counter, bp_counter, 2, 0, 1);
    // assemble_screen(bitplane, raw_counter, 32, dir, "Krocodile Kore Background");
    
    free( bp_data);
    free(raw_data);
    free(bitplane);

} // Special Screens (DKC2)