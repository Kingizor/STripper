#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bitplane.h"
#include "decomp.h"

void level2(uint8_t *rom, char dir[255], int priority, int special, int region) {

    uint8_t  *bp_data = malloc(65535);
    uint8_t *raw_data = malloc(65535);
    uint8_t *lay_data_a = malloc(65535);
    uint8_t *lay_data_b = malloc(65535);
    int bp_counter;
    int raw_counter;
    int lay_counter_a;
    int lay_counter_b;
    
    int bp_width = 512;
    
    uint8_t *bitplane = malloc(512 * 2000 * 4); // width * height * RGBA
    
    // Ship Deck (9 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1F8116);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25C627);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23E006);
    } // 1
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25C627);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23E006);
    } // 2
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0CD0, raw_counter, bp_counter, 1, 0, priority);
    
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Ship Deck.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Pirate Panic & Gangplank Galley Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE29, 0, 16, 0, dir, "Pirate Panic.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE5B, 0, 16, 0, dir, "Pirate Panic Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE65, 0, 16, 0, dir, "Pirate Panic Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE51, 0, 16, 0, dir, "Pirate Panic Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE33, 0, 16, 0, dir, "Gangplank Galley.png");
        // Bonus 1 uses K.Rool's Kabin tiles.
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE6F, 0, 16, 0, dir, "Gangplank Galley Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0CD0, raw_counter, bp_counter, 1, 65, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Gangplank Galley (Sunset) Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE33, 0, 16, 0, dir, "Gangplank Galley (Sunset).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D324E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Rattle Battle Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE3D, 0, 16, 0, dir, "Rattle Battle.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE79, 0, 16, 0, dir, "Rattle Battle Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE83, 0, 16, 0, dir, "Rattle Battle Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE47, 0, 16, 0, dir, "Glimmer's Galleon Exit Room.png");
    }
    
    // Ship Mast (14 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1FD537);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25E0C8);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23F9B2);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25E0C8);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23F9B2);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1610, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 32, 0, dir, "Ship Mast.png");
    }
    else {
        
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Mainbrace Mayhem Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEB1, 1, 32, 0, dir, "Mainbrace Mayhem.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BECF, 1, 32, 0, dir, "Mainbrace Mayhem Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BED9, 1, 32, 0, dir, "Mainbrace Mayhem Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEF7, 1, 32, 0, dir, "Mainbrace Mayhem Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEC5, 1, 32, 0, dir, "Mainbrace Mayhem Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE93, 1, 32, 0, dir, "Krow's Nest.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D14F0, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Topsail Trouble Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEA7, 1, 32, 0, dir, "Topsail Trouble.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEED, 1, 32, 0, dir, "Topsail Trouble Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEF7, 1, 32, 0, dir, "Topsail Trouble Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEC5, 1, 32, 0, dir, "Topsail Trouble Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEBB, 1, 32, 0, dir, "Kreepy Krow.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D304E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Slime Climb Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE9D, 1, 32, 0, dir, "Slime Climb.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BEE3, 1, 32, 0, dir, "Slime Climb Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF0B, 1, 32, 0, dir, "Slime Climb Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF15, 1, 32, 0, dir, "Rattle Battle Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D304E, raw_counter, bp_counter, 1, 55, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Slime Climb Tiles (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE9D, 1, 32, 0, dir, "Slime Climb (Underwater).png");
    }
    
    // Ship Hold (7 areas)

     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1F0870);
    
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2597E1);
        decomp(lay_data_a, rom, &lay_counter_a, 0x236F39);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x2597E1);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x236F39);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Ship Hold.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lockjaw's Locker Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD91, 1, 80, 0, dir, "Lockjaw's Locker.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD9B, 1, 80, 0, dir, "Lockjaw's Locker Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDAF, 1, 80, 0, dir, "Lockjaw's Locker Warp Room.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0910, raw_counter, bp_counter, 1, 53, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lockjaw's Locker Tiles (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD91, 1, 80, 0, dir, "Lockjaw's Locker (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD9B, 1, 80, 0, dir, "Lockjaw's Locker Bonus 1 (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDAF, 1, 80, 0, dir, "Lockjaw's Locker Warp Room (Underwater).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Glimmer's Galleon Tiles (Natural).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD7D, 1, 80, 0, dir, "Glimmer's Galleon (Natural).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDB9, 1, 80, 0, dir, "Glimmer's Galleon Bonus 1 (Natural).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDA5, 1, 80, 0, dir, "Glimmer's Galleon Bonus 2 (Natural).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 1, 12, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Glimmer's Galleon Tiles (Dark).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD7D, 1, 80, 0, dir, "Glimmer's Galleon (Dark).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDB9, 1, 80, 0, dir, "Glimmer's Galleon Bonus 1 (Dark).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDA5, 1, 80, 0, dir, "Glimmer's Galleon Bonus 2 (Dark).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2BEE, raw_counter, bp_counter, 1, 11, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Glimmer's Galleon Tiles (Light).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD7D, 1, 80, 0, dir, "Glimmer's Galleon (Light).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDB9, 1, 80, 0, dir, "Glimmer's Galleon Bonus 1 (Light).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDA5, 1, 80, 0, dir, "Glimmer's Galleon Bonus 2 (Light).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 1, 53, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lava Lagoon Tiles (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD87, 1, 80, 0, dir, "Lava Lagoon (Underwater).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDAF, 1, 80, 0, dir, "Lava Lagoon Bonus 1 & Warp Room (Underwater).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 1, 54, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lava Lagoon Tiles (Lava).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD87, 1, 80, 0, dir, "Lava Lagoon (Lava).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDAF, 1, 80, 0, dir, "Lava Lagoon Bonus 1 & Warp Room (Lava).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D07F0, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Lava Lagoon Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD87, 1, 80, 0, dir, "Lava Lagoon.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDAF, 1, 80, 0, dir, "Lava Lagoon Bonus 1 & Warp Room.png");
        
    }
    
    // Forest (8 areas + 3 unused)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1EA932);
    
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2572C5);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23438B);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x2572C5);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23438B);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D06F0, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Forest.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Ghostly Grove Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD1D, 0, 16, 0, dir, "Ghostly Grove.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD3B, 0, 16, 0, dir, "Ghostly Grove Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD59, 0, 16, 0, dir, "Ghostly Grove Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0610, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Gusty Glade Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD13, 0, 16, 0, dir, "Gusty Glade.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD59, 0, 16, 0, dir, "Gusty Glade Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD4F, 0, 16, 0, dir, "Gusty Glade Bonus 2.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3A4E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Web Woods Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD09, 0, 16, 0, dir, "Web Woods.png");
        // Bonus 1 is brambles.
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD63, 0, 16, 0, dir, "Web Woods Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD31, 0, 16, 0, dir, "Web Woods - Squitter Room.png");
        
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BD6D, 0, 16, 0, dir, "Forest Unused Area.png");
    }
    
    // Hive (8 areas + 4 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x1F3059);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25A71C);
        decomp(lay_data_a, rom, &lay_counter_a, 0x23B5D4);
        decomp(lay_data_b, rom, &lay_counter_b, 0x23A1C4);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25A71C);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x23B5D4);
        decomp(lay_data_b, rom, &lay_counter_b, 25 + 0x23A1C4);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0A10, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Hive-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 16, 0, dir, "Hive-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Hive Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDD3, 1, 80, 0, dir, "Hornet Hole.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDF1, 1, 80, 0, dir, "Hornet Hole Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFC7, 1, 16, 0, dir, "Hornet Hole Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE05, 1, 80, 0, dir, "Hornet Hole Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDC9, 1, 80, 0, dir, "Rambi Rumble.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFD1, 1, 16, 0, dir, "Rambi Rumble Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE19, 1, 80, 0, dir, "Rambi Rumble Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDDD, 1, 80, 0, dir, "Rambi Rumble Rambi Room.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFB3, 1, 16, 0, dir, "Parrot Chute Panic.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BE0F, 1, 80, 0, dir, "Parrot Chute Panic Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BDFB, 1, 80, 0, dir, "Parrot Chute Panic Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35BFBD, 1, 16, 0, dir, "King Zing Sting.png");
    }
    
    // Fair (3 areas + 7 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x202195);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x25F52D);
        decomp(lay_data_a, rom, &lay_counter_a, 0x24186E);
        decomp(lay_data_b, rom, &lay_counter_b, 0x243D8A);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x25F52D);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x24186E);
        decomp(lay_data_b, rom, &lay_counter_b, 25 + 0x243D8A);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1710, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Fair-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 0, 16, 0, dir, "Fair-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Rickety Race Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF2F, 0, 16, 0, dir, "Rickety Race.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C2CF, 0, 16, 0, dir, "Rickety Race Bonus 1.png");
        // Rickety Race Exit Room is Ship Deck
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1710, raw_counter, bp_counter, 1, 2, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Target Terror Tiles (Red).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF25, 0, 16, 0, dir, "Target Terror (Red).png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C2BB, 0, 16, 0, dir, "Target Terror Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C29D, 0, 16, 0, dir, "Target Terror & Rickety Race Exit Room.png");
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1710, raw_counter, bp_counter, 1, 7, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Target Terror Tiles (Green).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF25, 0, 16, 0, dir, "Target Terror (Green).png");
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D1710, raw_counter, bp_counter, 1, 8, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Target Terror Tiles (Grey).png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF25, 0, 16, 0, dir, "Target Terror (Grey).png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2EEE, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Haunted Hall Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C293, 0, 16, 0, dir, "Haunted Hall.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF39, 0, 16, 0, dir, "Haunted Hall Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C2C5, 0, 16, 0, dir, "Haunted Hall Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C2B1, 0, 16, 0, dir, "Haunted Hall Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C2A7, 0, 16, 0, dir, "Haunted Hall Exit Room.png");
    }
    
    // Lava (10 areas)
    // Lava levels have the "numbers" problem where the animated tiles have placeholders.
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x207174);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x260FB1);
        decomp(lay_data_a, rom, &lay_counter_a, 0x245ADE);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x260FB1);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x245ADE);
    }
    
    memcpy(&bp_data[0x1A0], &rom[0x3B3420], 0x1A0);
    memcpy(&bp_data[0x340], &rom[0x3B4C80], 0x1A0);
    memcpy(&bp_data[0x020], &rom[0x350147], 0x180);
    memcpy(&bp_data[0x4E0], &rom[0x3B0DA0], 0x2C0); // ...
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0DF0, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Lava.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Hot Head Hop & Red-Hot Ride Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF49, 0, 16, 0, dir, "Hot Head Hop.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF8F, 0, 16, 0, dir, "Hot Head Hop Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF67, 0, 16, 0, dir, "Hot Head Hop Bonus 2 & Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF85, 0, 16, 0, dir, "Hot Head Hop Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF53, 0, 16, 0, dir, "Red-Hot Ride.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF99, 0, 16, 0, dir, "Red-Hot Ride Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFA3, 0, 16, 0, dir, "Red-Hot Ride Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF7B, 0, 16, 0, dir, "Kleever's Kiln.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF71, 0, 16, 0, dir, "Lava Room.png"); // ?
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D0EF0, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Fiery Furnace Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BF5D, 0, 16, 0, dir, "Fiery Furnace.png");
        // FF B1 is Brambles.
    }
    
    // Mine (9 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x20CF31);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x26303A);
        decomp(lay_data_a, rom, &lay_counter_a, 0x2479EE);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x26303A);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x2479EE);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D228E, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 24, 0, dir, "Mine.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Kannon's Klaim Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFEB, 1, 24, 0, dir, "Kannon's Klaim.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C01D, 1, 24, 0, dir, "Kannon's Klaim Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C031, 1, 24, 0, dir, "Kannon's Klaim Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C013, 1, 24, 0, dir, "Kannon's Klaim Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFF5, 1, 24, 0, dir, "Kannon's Klaim Warp Room.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D238E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Squawks's Shaft Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFE1, 1, 24, 0, dir, "Squawks's Shaft.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C027, 1, 24, 0, dir, "Squawks's Shaft Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C031, 1, 24, 0, dir, "Squawks's Shaft Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C009, 1, 24, 0, dir, "Squawks's Shaft Bonus 3.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFF5, 1, 24, 0, dir, "Squawks's Shaft Warp Room.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D248E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Windy Well Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35BFFF, 1, 24, 0, dir, "Windy Well.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C009, 1, 24, 0, dir, "Windy Well Bonus 1.png");
        // WW B2 is Brambles
    }
    
    // Swamp (12 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x2106AF);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x264C7F);
        decomp(lay_data_a, rom, &lay_counter_a, 0x249E44);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x264C7F);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x249E44);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D27EE, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Swamp.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Barrel Bayou Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C041, 0, 16, 0, dir, "Barrel Bayou.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C073, 0, 16, 0, dir, "Barrel Bayou Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C07D, 0, 16, 0, dir, "Barrel Bayou Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C055, 0, 16, 0, dir, "Barrel Bayou Warp Room.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0AF, 0, 16, 0, dir, "Kudgel's Kontest.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D450E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Krockhead Klamber Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C04B, 0, 16, 0, dir, "Krockhead Klamber.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C087, 0, 16, 0, dir, "Krockhead Klamber Bonus.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D460E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Mudhole Marsh Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C05F, 0, 16, 0, dir, "Mudhole Marsh.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C091, 0, 16, 0, dir, "Mudhole Marsh Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C09B, 0, 16, 0, dir, "Mudhole Marsh Bonus 2.png");
        
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C069, 0, 16, 0, dir, "Swamp Unused Area 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0A5, 0, 16, 0, dir, "Swamp Unused Area 2.png");
    }
    
    // Brambles (5 areas + 9 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    decomp( bp_data, rom,  &bp_counter, 0x215B05);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2665F8);
        decomp(lay_data_a, rom, &lay_counter_a, 0x24B767);
        decomp(lay_data_b, rom, &lay_counter_b, 0x24E649);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x2665F8);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x24B767);
        decomp(lay_data_b, rom, &lay_counter_b, 25 + 0x24E649);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D28EE, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 96, 0, dir, "Brambles-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 96, 0, dir, "Brambles-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Bramble Blast Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0BF, 1, 96, 0, dir, "Bramble Blast.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C279, 1, 96, 0, dir, "Bramble Blast Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C23D, 1, 96, 0, dir, "Bramble Blast Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C233, 1, 96, 0, dir, "Animal Antics - Squawks.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0DD, 1, 96, 0, dir, "Animal Antics Bonus.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C26F, 1, 96, 0, dir, "Toxic Tower Bonus 1.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2AEE, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Bramble Scramble Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0C9, 1, 96, 0, dir, "Bramble Scramble.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C251, 1, 96, 0, dir, "Bramble Scramble Bonus.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C247, 1, 96, 0, dir, "Target Terror Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C25B, 1, 96, 0, dir, "Windy Well Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C265, 1, 96, 0, dir, "Web Woods Bonus 1.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D29EE, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Screech's Sprint Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0D3, 1, 96, 0, dir, "Screech's Sprint.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C283, 1, 96, 0, dir, "Screech's Sprint Bonus.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0E7, 1, 96, 0, dir, "Fiery Furnace Bonus.png");
    }
    
    // Castle (9 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp( bp_data, rom,  &bp_counter, 0x29A905);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x268077);
        decomp(lay_data_a, rom, &lay_counter_a, 0x24F714);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x268077);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x24F714);
    }
    
    memcpy(bp_data, &rom[0x2199BE], 0x257F);
    memcpy(&bp_data[0x4840], &rom[0x21BF3E], 0x37C0);
    bp_counter = 0x8000;
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 16, 0, dir, "Castle.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Castle Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C0F7, 1, 16, 0, dir, "Castle Crush.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C133, 1, 16, 0, dir, "Castle Crush Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C13D, 1, 16, 0, dir, "Castle Crush Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C10B, 1, 16, 0, dir, "Chain Link Chamber.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C11F, 1, 16, 0, dir, "Chain Link Chamber Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C129, 1, 16, 0, dir, "Chain Link Chamber Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C115, 1, 16, 0, dir, "Toxic Tower.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C147, 1, 16, 0, dir, "Stronghold Showdown.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C101, 1, 16, 0, dir, "Castle BG.png");

    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2DEE, raw_counter, bp_counter, 1, 56, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Toxic Tower Tiles (Underwater).png");
    assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C115, 1, 16, 0, dir, "Toxic Tower (Underwater).png");
    
    // Krocodile Kore
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    decomp(bp_data, rom,  &bp_counter, 0x21EFDE);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x2694EC);
        decomp(lay_data_a, rom, &lay_counter_a, 0x25264B);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x2694EC);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x25264B);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D2CEE, raw_counter, bp_counter, 1, 0, priority);
    assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Krocodile Kore Tiles.png");
    assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C157, 1, 16, 0, dir, "Krocodile Kore.png");
    
    
    // K.Rool Duel
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0x100;
    decomp( bp_data, rom,  &bp_counter, 0x223790);
    if (region == 1) {
        decomp(raw_data, rom, &raw_counter, 0x269CE2);
        memcpy(lay_data_a, &rom[0x252732], 0x100);
    }
    else {
        decomp(raw_data, rom, &raw_counter, 25 + 0x269CE2);
        memcpy(lay_data_a, &rom[25 + 0x252732], 0x100);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D314E, raw_counter, bp_counter, 1, 0, priority);
    assemble_bitplane(bitplane, bp_width, raw_counter, dir, "K.Rool Duel Tiles.png");
    assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C167, 1, 16, 0, dir, "K.Rool Duel.png");
    
    }
    
    // Ice (8 areas + 1 area)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    lay_counter_b = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x229E5F);
        decomp(raw_data, rom, &raw_counter, 0x26A896);
        decomp(lay_data_a, rom, &lay_counter_a, 0x252832);
        decomp(lay_data_b, rom, &lay_counter_b, 0x254351);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 25 + 0x229E5F);
        decomp(raw_data, rom, &raw_counter, 35 + 0x26A896);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x252832);
        decomp(lay_data_b, rom, &lay_counter_b, 25 + 0x254351);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D398E, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 1, 80, 0, dir, "Ice-1.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0, 1, 16, 0, dir, "Ice-2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Ice Tiles.png");
        assemble_level(bitplane, rom, lay_data_b, lay_counter_b, 0x35C223, 1, 16, 0, dir, "Black Ice Battle.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1B3, 1, 80, 0, dir, "Black Ice Battle Bonus.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C177, 1, 80, 0, dir, "Arctic Abyss.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1A9, 1, 80, 0, dir, "Arctic Abyss Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1BD, 1, 80, 0, dir, "Arctic Abyss Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C181, 1, 80, 0, dir, "Clapper's Cavern.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C195, 1, 80, 0, dir, "Clapper's Cavern Bonus 1.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C19F, 1, 80, 0, dir, "Clapper's Cavern Bonus 2.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C18B, 1, 80, 0, dir, "Animal Antics - Enguarde.png");
    }
    
    // Jungle (8 areas)
    
     bp_counter = 0;
    raw_counter = 0;
    lay_counter_a = 0;
    if (region == 1) {
        decomp( bp_data, rom,  &bp_counter, 0x22E556);
        decomp(raw_data, rom, &raw_counter, 0x26C9FF);
        decomp(lay_data_a, rom, &lay_counter_a, 0x254E8A);
    }
    else {
        decomp( bp_data, rom,  &bp_counter, 25 + 0x22E556);
        decomp(raw_data, rom, &raw_counter, 35 + 0x26C9FF);
        decomp(lay_data_a, rom, &lay_counter_a, 25 + 0x254E8A);
    }
    
    decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D430E, raw_counter, bp_counter, 1, 0, priority);
    if (special == 2) {
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0, 0, 16, 0, dir, "Jungle-DKC2.png");
    }
    else {
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Klobber Karnage Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1CD, 0, 16, 0, dir, "Klobber Karnage.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1EB, 0, 16, 0, dir, "Klobber Karnage Bonus.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D440E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Jungle Jinx Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1FF, 0, 16, 0, dir, "Jungle Jinx.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1F5, 0, 16, 0, dir, "Jungle Jinx Bonus.png");
        
        decode_bitplane(rom, bp_data, raw_data, bitplane, 0x3D3B6E, raw_counter, bp_counter, 1, 0, priority);
        assemble_bitplane(bitplane, bp_width, raw_counter, dir, "Animal Antics Jungle Tiles.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1E1, 0, 16, 0, dir, "Animal Antics - Rambi.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C209, 0, 16, 0, dir, "Animal Antics - Squitter.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C213, 0, 16, 0, dir, "Animal Antics - Rattly.png");
        assemble_level(bitplane, rom, lay_data_a, lay_counter_a, 0x35C1D7, 0, 16, 0, dir, "Jungle BG.png");
    }
    
    free( bp_data);
    free(raw_data);
    free(lay_data_a);
    free(lay_data_b);
    free(bitplane);

} // Levels (DKC2)