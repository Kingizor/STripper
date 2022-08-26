#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dkcomp.h>
#include "gba_misc.h"
#include "bitplane.h"

struct Level {
    unsigned char mode; // Bit depth and palette mode
    unsigned char index; // Path index
    char *name;    // Area name
    char *palette; // Palette name
};

static void extract_target(char current_target[128], char *target, int *target_pos, int *last_split, int target_length) {

    while (target[*target_pos] != '/' && *target_pos < target_length) {
        (*target_pos)++;
    }

    for (int i = *last_split; i < *target_pos; i++) {
        current_target[i - *last_split] = target[i];
    }
    current_target[*target_pos - *last_split] = '\0';
    (*target_pos)++;
    *last_split = *target_pos;

    return;
}

static int fetch_NDS_file(unsigned char *rom, char *target, size_t *file_start, size_t *file_length) {

    unsigned char *fnt = rom + rom[0x40] + (rom[0x41] << 8) + (rom[0x42] << 16) + (rom[0x43] << 24); // Start of FNT
    unsigned char *fat = rom + rom[0x48] + (rom[0x49] << 8) + (rom[0x4A] << 16) + (rom[0x4B] << 24); // Start of FAT

    // Not the most elegant implementation, but it should work okay.
    // Just specify the path. (Case sensitive)

    char current_target[128]; // Current segment
    int depth = 0;            // How deep is the target?
    int current_depth = 0;    // How deep are we in search of the target?
    int target_pos = 0;       // Where is the next split?
    int last_split = 0;       // When was the split last?
    int target_length = strlen(target);

    int folder_id = 0; // Current Directory
    int file_id = 0; // Position of file in FAT

    // Count depth of target (How many folders deep)
    for (int i = 0; i < target_length; i++) {
        if (target[i] == '/') {
            depth++;
        }
    }

    // Traverse until file located.
    while (current_depth <= depth) {

        // Extract name to search for
        extract_target(current_target, target, &target_pos, &last_split, target_length);
        int target_length = strlen(current_target);

        // Go to the current folder
        unsigned char *current_dir = fnt + fnt[folder_id * 8] + (fnt[(folder_id*8)+1] << 8) + (fnt[(folder_id*8)+2] << 16) + (fnt[(folder_id*8)+3] << 24);
        file_id = fnt[(folder_id * 8) + 4] + (fnt[(folder_id * 8) + 5] << 8);

        while (1) {

            unsigned char fn_length = *current_dir & 0x7F; // Length of current filename
            unsigned char file_type = *current_dir & 0x80; // Folder yes, File no
            unsigned char target_ft = !(current_depth == depth); // Target File Type

            // Exit if no more entries
            if (*current_dir == 0) {
                // printf("Couldn't fild a %s called \"%s\".\n", target_ft ? "Folder" : "File", current_target);
                return -1;
            }

            // Skip entry if target and entry are of different types
            if (!file_type && target_ft) { // Current entry is file, target is folder
                current_dir += fn_length + 1;
                file_id++;
                continue;
            }
            else if (file_type && !target_ft) { // Current entry is folder, target is file.
                current_dir += fn_length + 3;
                continue;
            }
            else if (fn_length != target_length) { // Skip entry if the length doesn't match
                current_dir += fn_length + 1;
                if (file_type) {
                    current_dir += 2; // Folders have an ID here
                }
                else {
                    file_id++;
                }
                continue;
            }
            current_dir++;

            // Compare entry and target filenames, letter by letter
            int state = 0;
            int mismatch = 0;
            do {
                if (*(current_dir++) != current_target[state++]) {
                    state -= fn_length;
                    mismatch = 1;
                    break;
                }
            } while (state > 0 && !mismatch && state < fn_length);

            if (mismatch) { // Names don't match
                while (state++ < 0) current_dir++;
                if (file_type) {
                    current_dir += 2;
                }
                else {
                    file_id++;
                }
                continue;
            }// Filenames do not match

            // Match found

            // Get folder ID for next round
            if (file_type) {
                folder_id = (*current_dir + (*(current_dir + 1) << 8)) & 0xFFF;
                current_dir += 2;
                break;
            }

            // File Found
            break;

        }
        current_depth++;
    }

    // Lookup FAT with file_id
    *file_start = fat[ file_id*8   ] + (fat[(file_id*8)+1] << 8) + (fat[(file_id*8)+2] << 16) + (fat[(file_id*8)+3] << 24);
    int file_end   = fat[(file_id*8)+4] + (fat[(file_id*8)+5] << 8) + (fat[(file_id*8)+6] << 16) + (fat[(file_id*8)+7] << 24);
    *file_length = file_end - *file_start;

    return 0;

}

static void decode_layout(unsigned char *layout, size_t *layout_len) {
    // Jungle Climber layouts are stored as the difference from the previous tile
    // This function converts these values to absolutes

    int tc = 0; // Tile Counter
    int map_length = layout[1] + (layout[2] << 8) + (layout[3] << 16);

    for (int i = 0; i < map_length; i+=2) {

        unsigned short current_half = layout[i+4] + (layout[i+5] << 8);

        // Detect signed negatives
        tc += (current_half & 0x8000) ? 0 - (0x10000 - current_half) : current_half;

        layout[i]   = tc & 0xFF;
        layout[i+1] = tc >> 8;

    }
    *layout_len = map_length;
    return;
}

static const char *folders[5] = {"BG0Data", "BG1Data", "BG2Data", "BG3Data", "BG16"};

static const struct Level levels[] = {
    {3, 3, "sel_bg_1", "sel_top"},
    {3, 1, "x_start_menu_main", "sel_top"},
    {3, 2, "y_start_menu_main", "sel_top"},
    {3, 1, "x_start_menu", "sel_top"},
    {3, 2, "y_start_menu", "sel_top"},
    {3, 3, "z_start_menu", "sel_top"},
    {1, 4, "map_sel_BG1_b", "p_map_sel_02"},
    {1, 4, "map_sel_BG1", "p_map_sel_02"},
    {0, 4, "map_sel_BG2", "p_map_sel_00"},
    {0, 4, "map_sel_BG3", "p_map_sel_00"},
    {3, 1, "beach1", "beach1-pal"},
    {3, 1, "mboard", "p_mboard"},
    {3, 1, "Lecture_DSLite", "pLecture_DSLite"},
    {0, 3, "z0-0-0", "beach1-pal"},
    {3, 1, "x1-1-1", "p1-1-1"},
    {0, 4, "z1-1-1", "p1-1-1"},
    {3, 1, "x1-1-3a", "p1-1-1"},
    {0, 4, "z0-0-0", "p1-1-1"},
    {3, 1, "x1-1-4a", "p1-1-1"},
    {3, 1, "x1-1-4b", "p1-1-4b"},
    {3, 2, "y1-1-4b", "p1-1-4b"},
    {0, 4, "z1-1-4b", "p1-1-4b"},
    {3, 1, "x1-1-5a", "p1-1-1"},
    {3, 1, "x1-1-5b", "p1-1-4b"},
    {3, 2, "y1-1-5b", "p1-1-4b"},
    {0, 4, "z1-1-5b", "p1-1-4b"},
    {3, 2, "y1-1-6a", "p1-1-4b"},
    {0, 4, "z1-1-6a", "p1-1-4b"},
    {3, 1, "x1-1-7a", "p1-1-4b"},
    {3, 2, "y1-1-7a", "p1-1-4b"},
    {3, 1, "x1-2-1", "p1-1-4b"},
    {3, 1, "x1-2-2", "p1-2-2"},
    {3, 1, "x1-2-3", "p1-2-2"},
    {3, 1, "x1-2-4", "p1-2-2"},
    {3, 1, "x1-3-1", "p1-3-1"},
    {0, 4, "y1-3-1", "p1-3-1"},
    {0, 4, "z1-2-2", "p1-3-1"},
    {3, 0, "w1-3-1", "p1-3-1"},
    {3, 1, "x1-3-2", "p1-3-1"},
    {0, 4, "y1-3-2", "p1-3-1"},
    {3, 1, "x1-3-3", "p1-3-3"},
    {0, 4, "y1-3-3", "p1-3-3"},
    {3, 3, "z1-3-3", "p1-3-3"},
    {0, 4, "x1-3-4", "p1-3-4"},
    {3, 2, "y1-3-4", "p1-3-4"},
    {0, 4, "z1-3-4", "p1-3-4"},
    {0, 4, "x1-3-k", "p1-3-k"},
    {3, 2, "y1-3-k", "p1-3-k"},
    {3, 3, "z1-3-k", "p1-3-k"},
    {3, 1, "x1-4-1", "p1-3-3"},
    {0, 4, "y1-4-1", "p1-3-3"},
    {3, 3, "z1-4-1", "p1-3-3"},
    {3, 1, "x1-4-2", "p1-4-2"},
    {0, 4, "y1-4-2", "p1-4-2"},
    {3, 1, "x1-4-3", "p1-4-3"},
    {0, 4, "y1-4-3", "p1-4-3"},
    {3, 3, "z1-4-3", "p1-4-3"},
    {3, 1, "x1-4-4", "p1-4-4"},
    {0, 4, "y1-4-4", "p1-4-4"},
    {3, 3, "z1-4-4", "p1-4-4"},
    {3, 1, "x1-4-k", "p1-4-4"},
    {0, 4, "y1-4-k", "p1-4-4"},
    {3, 3, "z1-4-k", "p1-4-4"},
    {3, 1, "x1-5-1", "p1-5-1"},
    {3, 2, "y1-5-1", "p1-5-1"},
    {3, 3, "z1-5-1", "p1-5-1"},
    {3, 1, "x1-5-2", "p1-5-2"},
    {3, 2, "y1-5-2", "p1-5-2"},
    {3, 3, "z1-5-2", "p1-5-2"},
    {3, 1, "x1-5-3", "p1-5-3"},
    {0, 4, "y1-5-3", "p1-5-3"},
    {0, 4, "z1-5-3", "p1-5-3"},
    {3, 1, "x1-5-4", "p1-5-4"},
    {0, 4, "y1-5-4", "p1-5-4"},
    {0, 4, "z1-5-4", "p1-5-4"},
    {3, 1, "x1-5-boss", "p1-5-boss"},
    {3, 3, "z1-5-boss", "p1-5-boss"},
    {3, 1, "x1-6-1", "p1-6-1"},
    {0, 4, "y1-6-1", "p1-6-1"},
    {0, 4, "z1-6-2", "p1-6-1"},
    {3, 1, "x1-6-2", "p1-3-1"},
    {0, 4, "y1-6-2", "p1-3-1"},
    {3, 1, "x1-6-3", "p1-6-3"},
    {0, 4, "y1-6-3", "p1-6-3"},
    {3, 3, "z1-6-3", "p1-6-3"},
    {3, 1, "x2-1-1", "p2-1-1"},
    {0, 4, "y2-1-1", "p2-1-1"},
    {0, 4, "z2-1-1", "p2-1-1"},
    {3, 1, "x2-1-2", "p2-1-2"},
    {3, 1, "x2-1-3", "p2-1-3"},
    {0, 4, "y2-1-5", "p2-1-3"},
    {3, 1, "x2-1-4", "p2-1-4"},
    {3, 1, "x2-1-5", "p2-1-5"},
    {0, 4, "y2-1-k", "p2-1-k"},
    {0, 4, "x2-2-1", "p2-2-1"},
    {3, 2, "y2-2-1", "p2-2-1"},
    {0, 4, "z2-2-1", "p2-2-1"},
    {0, 4, "x2-2-10", "p2-2-1"},
    {3, 2, "y2-2-10", "p2-2-1"},
    {0, 4, "z2-2-10", "p2-2-1"},
    {3, 2, "y2-2-2", "p2-2-1"},
    {3, 2, "y2-2-3", "p2-2-3"},
    {3, 3, "z2-2-4", "p2-2-3"},
    {3, 2, "y2-2-4", "p2-2-4"},
    {3, 1, "x2-2-5", "p2-2-4"},
    {3, 2, "y2-2-5", "p2-2-5"},
    {3, 3, "z2-2-5", "p2-2-5"},
    {3, 2, "y2-2-6", "p2-2-6"},
    {3, 3, "z2-2-6", "p2-2-6"},
    {3, 2, "y2-2-7", "p2-2-7"},
    {3, 3, "z2-2-8", "p2-2-7"},
    {3, 1, "x2-2-8", "p2-2-8"},
    {3, 2, "y2-2-8", "p2-2-8"},
    {3, 1, "x2-2-9", "p2-2-9"},
    {3, 2, "y2-2-9", "p2-2-9"},
    {3, 1, "x2-2-9a", "p2-2-9a"},
    {3, 2, "y2-2-9a", "p2-2-9a"},
    {0, 4, "x2-2-k", "p2-2-1"},
    {3, 2, "y2-2-k", "p2-2-1"},
    {0, 4, "z2-2-k", "p2-2-1"},
    {3, 1, "x2-3-1", "p2-3-1"},
    {0, 4, "y2-3-1", "p2-3-1"},
    {3, 3, "z2-3-2", "p2-3-1"},
    {3, 1, "x2-3-2", "p2-3-2"},
    {3, 1, "x2-3-3", "p2-3-3"},
    {0, 4, "y2-3-3", "p2-3-3"},
    {3, 1, "x2-3-4", "p2-3-4"},
    {3, 1, "x2-3-5", "p2-3-5"},
    {3, 1, "x2-4-1", "p2-4-1"},
    {0, 4, "y2-4-1", "p2-4-1"},
    {0, 4, "z2-4-1", "p2-4-1"},
    {3, 1, "x2-4-2", "p2-4-2"},
    {3, 2, "y2-4-2", "p2-4-2"},
    {3, 1, "x2-4-3", "p2-4-3"},
    {3, 2, "y2-4-3", "p2-4-3"},
    {3, 1, "x2-4-4", "p2-4-4"},
    {3, 2, "y2-4-4", "p2-4-4"},
    {3, 1, "x2-4-5", "p2-4-5"},
    {3, 2, "y2-4-5", "p2-4-5"},
    {3, 3, "z2-4-5", "p2-4-5"},
    {3, 2, "y2-4-6", "p2-4-6"},
    {3, 1, "x2-4-6", "p2-4-6"},
    {3, 1, "x2-4-7", "p2-4-7"},
    {3, 2, "y2-4-7", "p2-4-7"},
    {3, 3, "z2-4-7", "p2-4-7"},
    {3, 1, "x2-4-k", "p2-4-k"},
    {3, 2, "y2-4-k", "p2-4-k"},
    {3, 3, "z2-4-k", "p2-4-k"},
    {3, 1, "x2-5-1", "p2-5-1"},
    {0, 4, "y2-5-1", "p2-5-1"},
    {0, 4, "z2-5-1", "p2-5-1"},
    {3, 0, "w2-5-2", "p2-5-2"},
    {3, 1, "x2-5-2", "p2-5-2"},
    {3, 3, "z2-5-2", "p2-5-2"},
    {3, 2, "y2-5-2", "p2-5-2"},
    {3, 1, "x2-5-3", "p2-5-3"},
    {3, 2, "y2-5-3", "p2-5-3"},
    {3, 3, "z2-5-3", "p2-5-3"},
    {3, 1, "x2-5-4", "p2-5-4"},
    {3, 2, "y2-5-4", "p2-5-4"},
    {3, 3, "z2-5-4", "p2-5-4"},
    {3, 1, "x2-5-boss", "p2-5-boss"},
    {3, 2, "y2-5-boss", "p2-5-boss"},
    {3, 3, "z2-5-boss", "p2-5-boss"},
    {3, 1, "x2-6-1", "p2-6-1"},
    {3, 4, "y2-6-2", "p2-6-1"},
    {0, 4, "z2-6-2", "p2-6-1"},
    {3, 1, "x2-6-2", "p2-6-2"},
    {3, 2, "y2-6-2", "p2-6-2"},
    {3, 1, "x2-6-3", "p2-6-3"},
    {0, 4, "w3-1-2", "p3-1-2"},
    {3, 1, "x3-1-1", "p3-1-2"},
    {0, 4, "z3-1-1", "p3-1-2"},
    {3, 1, "x3-1-2", "p3-1-2"},
    {0, 4, "z3-1-2", "p3-1-2"},
    {3, 1, "x3-1-3", "p3-1-3"},
    {0, 2, "y3-1-8", "p3-1-3"},
    {0, 4, "z3-1-3", "p3-1-3"},
    {0, 4, "w3-1-3", "p3-1-3"},
    {3, 1, "x3-1-4", "p3-1-6"},
    {0, 4, "z3-1-6", "p3-1-6"},
    {3, 1, "x3-1-5", "p3-1-5"},
    {3, 2, "y3-1-5", "p3-1-5"},
    {3, 1, "x3-1-6", "p3-1-6"},
    {3, 1, "x3-1-7", "p3-1-7"},
    {3, 1, "x3-1-8", "p3-1-8"},
    {0, 4, "y3-1-8", "p3-1-8"},
    {0, 4, "w3-1-8", "p3-1-8"},
    {3, 1, "x3-1-k", "p3-1-5"},
    {3, 1, "x3-2-1", "p3-2-1"},
    {0, 4, "y3-2-1", "p3-2-1"},
    {0, 4, "z3-2-1", "p3-2-1"},
    {3, 1, "x3-2-2", "p3-2-3"},
    {3, 2, "y3-2-2", "p3-2-3"},
    {3, 3, "z3-2-2", "p3-2-3"},
    {3, 1, "x3-2-3", "p3-2-3"},
    {3, 2, "y3-2-3", "p3-2-3"},
    {3, 3, "z3-2-3", "p3-2-3"},
    {3, 1, "x3-2-4", "p3-2-3"},
    {3, 2, "y3-2-4", "p3-2-3"},
    {3, 3, "z3-2-4", "p3-2-3"},
    {3, 1, "x3-2-5", "p3-2-3"},
    {3, 2, "y3-2-5", "p3-2-3"},
    {3, 3, "z3-2-5", "p3-2-3"},
    {3, 1, "x3-2-6", "p3-2-6"},
    {0, 4, "y3-2-6", "p3-2-6"},
    {0, 4, "z3-2-6", "p3-2-6"},
    {0, 4, "x3-3-1", "p3-3-1"},
    {3, 2, "y3-3-1", "p3-3-1"},
    {3, 3, "z3-3-1", "p3-3-1"},
    {0, 4, "x3-3-2", "p3-3-2"},
    {3, 2, "y3-3-1a", "p3-3-2"},
    {0, 4, "z3-3-2", "p3-3-2"},
    {3, 2, "y3-3-2", "p3-3-2"},
    {3, 0, "w3-3-2", "p3-3-2"},
    {3, 2, "y3-3-3", "p3-3-2"},
    {3, 2, "y3-3-4a", "p3-3-2"},
    {3, 2, "y3-3-4b", "p3-3-2"},
    {3, 2, "y3-3-4c", "p3-3-2"},
    {3, 2, "y3-3-4d", "p3-3-2"},
    {3, 2, "y3-3-4e", "p3-3-2"},
    {3, 2, "y3-3-4f", "p3-3-2"},
    {0, 4, "x3-3-5", "p3-3-1"},
    {3, 2, "y3-3-5", "p3-3-1"},
    {3, 3, "z3-3-5", "p3-3-1"},
    {3, 2, "y3-3-k", "p3-3-2"},
    {3, 1, "x3-4-1", "p3-4-1"},
    {0, 4, "y3-4-1", "p3-4-1"},
    {0, 4, "z3-4-1", "p3-4-1"},
    {3, 1, "x3-4-2", "p3-4-2"},
    {3, 2, "y3-4-2", "p3-4-2"},
    {3, 3, "z3-4-2", "p3-4-2"},
    {3, 1, "x3-4-3", "p3-4-2"},
    {3, 2, "y3-4-3", "p3-4-2"},
    {3, 3, "z3-4-3", "p3-4-2"},
    {3, 2, "y3-4-4", "p3-4-2"},
    {3, 1, "x3-4-5", "p3-4-2"},
    {3, 3, "z3-4-5", "p3-4-2"},
    {3, 1, "x3-4-6", "p3-4-6"},
    {0, 4, "y3-4-8", "p3-4-6"},
    {0, 4, "z3-4-8", "p3-4-6"},
    {3, 1, "x3-4-k", "p3-4-2"},
    {3, 3, "z3-4-k", "p3-4-2"},
    {3, 1, "x3-5-1", "p3-5-1"},
    {0, 4, "y3-5-1", "p3-5-1"},
    {0, 4, "z3-5-1", "p3-5-1"},
    {0, 4, "w3-5-1", "p3-5-1"},
    {0, 4, "x3-5-2", "p3-5-2"},
    {3, 3, "z3-5-2", "p3-5-2"},
    {0, 4, "x3-5-3", "p3-5-3"},
    {3, 2, "y3-5-3", "p3-5-3"},
    {0, 4, "x3-5-4", "p3-5-4"},
    {3, 1, "x3-5-boss", "p3-5-boss"},
    {3, 2, "y3-5-boss", "p3-5-boss"},
    {3, 3, "z3-5-boss", "p3-5-boss"},
    {3, 0, "w3-5-boss", "p3-5-boss"},
    {0, 4, "x3-5-k", "p3-5-3"},
    {0, 2, "y3-5-2", "p3-5-3"},
    {0, 4, "x3-6-1", "p3-6-1"},
    {0, 4, "y3-6-1", "p3-6-1"},
    {0, 4, "x3-6-2", "p3-5-2"},
    {0, 4, "y3-6-2", "p3-5-2"},
    {0, 4, "x3-6-3", "p3-5-2"},
    {3, 0, "w4-1-1", "p4-1-1"},
    {0, 4, "x4-1-1", "p4-1-1"},
    {0, 4, "y4-1-1", "p4-1-1"},
    {0, 4, "z4-1-1", "p4-1-1"},
    {3, 1, "x4-1-2", "p4-1-2"},
    {0, 4, "w4-1-2", "p4-1-2"},
    {3, 2, "y4-1-2", "p4-1-2"},
    {0, 4, "z4-1-2", "p4-1-2"},
    {3, 1, "x4-1-3", "p4-1-3"},
    {0, 4, "y4-1-3", "p4-1-3"},
    {0, 4, "z4-1-3", "p4-1-3"},
    {3, 1, "x4-1-4", "p4-1-6"},
    {0, 4, "z4-1-4", "p4-1-6"},
    {3, 1, "x4-1-5", "p4-1-6"},
    {0, 4, "y4-2-4", "p4-1-6"},
    {0, 4, "z4-2-4", "p4-1-6"},
    {0, 4, "w4-2-4", "p4-1-6"},
    {3, 1, "x4-1-6", "p4-1-6"},
    {0, 4, "x4-2-1", "p4-2-a"},
    {0, 4, "y4-2-b", "p4-2-a"},
    {0, 4, "z4-2-b", "p4-2-a"},
    {0, 4, "x4-2-2", "p4-2-a"},
    {0, 4, "y4-2-a", "p4-2-a"},
    {0, 4, "z4-2-a", "p4-2-a"},
    {0, 4, "x4-2-3", "p4-2-b"},
    {0, 4, "x4-2-4", "p4-2-b"},
    {0, 4, "x4-2-5", "p4-2-b"},
    {0, 4, "x4-3-1", "p4-2-a"},
    {0, 4, "y4-3-1", "p4-2-a"},
    {0, 4, "z4-3-1", "p4-2-a"},
    {3, 2, "y4-3-2a", "p4-3-2a"},
    {3, 1, "x4-3-2b", "p4-3-2a"},
    {3, 2, "y4-3-2b", "p4-3-2a"},
    {3, 1, "x4-3-2c", "p4-3-2a"},
    {3, 2, "y4-3-2c", "p4-3-2a"},
    {0, 4, "x4-3-3", "p4-2-a"},
    {0, 4, "y4-3-3", "p4-2-a"},
    {0, 4, "z4-3-3", "p4-2-a"},
    {0, 4, "x4-3-k", "p4-2-a"},
    {0, 4, "y4-3-k", "p4-2-a"},
    {0, 4, "z4-4-1", "p4-4-1"},
    {3, 2, "y4-4-1", "p4-4-1"},
    {3, 1, "x4-4-1", "p4-4-1"},
    {3, 1, "x4-4-2", "p4-4-4"},
    {3, 2, "y4-4-2", "p4-4-4"},
    {3, 0, "w4-4-2", "p4-4-4"},
    {3, 0, "w4-4-3", "p4-4-3"},
    {3, 2, "y4-4-3", "p4-4-3"},
    {3, 3, "z4-4-3", "p4-4-3"},
    {3, 1, "x4-4-4", "p4-4-4"},
    {3, 2, "y4-4-4", "p4-4-4"},
    {3, 0, "w4-4-4", "p4-4-4"},
    {3, 1, "x4-4-5", "p4-4-5"},
    {0, 4, "y4-4-5", "p4-4-5"},
    {3, 3, "z4-4-5", "p4-4-5"},
    {3, 0, "w4-4-5", "p4-4-5"},
    {3, 1, "x4-4-k", "p4-4-4"},
    {0, 4, "y4-4-k", "p4-4-4"},
    {3, 1, "x4-5-1", "p4-5-1"},
    {0, 4, "y4-5-1", "p4-5-1"},
    {0, 4, "z4-5-1", "p4-5-1"},
    {0, 4, "x4-5-2", "p4-5-2"},
    {3, 2, "y4-5-2", "p4-5-2"},
    {3, 3, "z4-5-2", "p4-5-2"},
    {3, 1, "x4-5-3", "p4-5-3"},
    {3, 2, "y4-5-3", "p4-5-3"},
    {3, 1, "x4-5-4", "p4-5-4"},
    {3, 2, "y4-5-4", "p4-5-4"},
    {3, 1, "x4-5-boss", "p4-5-boss"},
    {3, 2, "y4-5-boss", "p4-5-boss"},
    {0, 4, "z4-5-boss", "p4-5-boss"},
    {0, 4, "x4-6-1", "p4-2-b"},
    {0, 4, "y4-3-a", "p4-2-b"},
    {0, 4, "z4-3-a", "p4-2-b"},
    {0, 4, "x4-6-2", "p4-2-b"},
    {0, 4, "x4-6-3", "p4-2-b"},
    {3, 1, "x5-1-1", "p5-1-1"},
    {0, 4, "y5-1-1", "p5-1-1"},
    {3, 3, "z5-1-1", "p5-1-1"},
    {3, 0, "w5-1-1", "p5-1-1"},
    {3, 1, "x5-1-2", "p5-1-1"},
    {0, 4, "y5-1-2", "p5-1-1"},
    {3, 1, "x5-1-3", "p5-1-1"},
    {0, 4, "y5-1-3", "p5-1-1"},
    {3, 1, "x5-1-3a", "p5-1-3a"},
    {3, 3, "z5-1-4", "p5-1-3a"},
    {3, 1, "x5-1-4", "p5-1-4"},
    {3, 2, "y5-1-4", "p5-1-4"},
    {3, 0, "w5-1-4", "p5-1-4"},
    {0, 4, "y5-1-5", "p5-1-5"},
    {3, 3, "z5-1-5", "p5-1-5"},
    {0, 4, "x5-1-5", "p5-1-5"},
    {0, 4, "x5-1-6", "p5-1-6"},
    {3, 2, "y5-1-6", "p5-1-6"},
    {0, 4, "x1-2-k", "p5-1-6"},
    {0, 4, "x5-2-1", "p5-2-1"},
    {3, 2, "y5-2-1", "p5-2-1"},
    {0, 4, "x5-2-2", "p5-2-1"},
    {0, 4, "y5-2-2", "p5-2-1"},
    {3, 3, "z5-2-2", "p5-2-1"},
    {0, 4, "x5-2-3", "p5-2-1"},
    {3, 3, "z5-2-3", "p5-2-1"},
    {0, 4, "x5-2-4", "p5-2-1"},
    {0, 4, "y5-2-4", "p5-2-1"},
    {3, 3, "z5-2-4", "p5-2-1"},
    {0, 4, "x5-2-5", "p5-2-5"},
    {3, 3, "z5-2-5", "p5-2-5"},
    {0, 4, "x5-2-boss", "p5-2-boss"},
    {3, 2, "y5-2-boss", "p5-2-boss"},
    {3, 3, "z5-2-boss", "p5-2-boss"},
    {3, 0, "w5-2-boss", "p5-2-boss"},
    {0, 4, "x5-6-1", "p5-6-1"},
    {3, 2, "y5-6-1", "p5-6-1"},
    {3, 2, "y5-6-2", "p5-6-1"},
    {0, 4, "x5-6-2", "p5-6-1"},
    {0, 4, "x5-6-3", "p5-6-3"},
    {3, 2, "y5-6-3", "p5-6-3"},
    {0, 4, "w5-6-3", "p5-6-3"},
    {3, 2, "y6-1-1", "p2-2-4"},
    {3, 3, "z6-1-1", "p2-2-4"},
    {3, 2, "y6-1-2", "p2-2-7"},
    {3, 3, "z6-1-2", "p2-2-7"},
    {3, 2, "y6-1-3", "p2-2-6"},
    {3, 3, "z6-1-3", "p2-2-6"},
    {3, 2, "y6-1-4", "p2-2-8"},
    {3, 3, "z6-1-4", "p2-2-8"},
    {3, 1, "x6-1-5", "p4-3-2a"},
    {3, 2, "y6-1-5", "p4-3-2a"},
    {3, 1, "x6-2-1", "p6-2-1"},
    {3, 2, "y6-2-1", "p6-2-1"},
    {3, 3, "z6-2-1", "p6-2-1"},
    {3, 1, "x6-2-2", "p6-2-1"},
    {3, 2, "y6-2-2", "p6-2-1"},
    {3, 3, "z6-2-2", "p6-2-1"},
    {3, 1, "x6-2-3", "p6-2-1"},
    {3, 3, "z6-2-3", "p6-2-1"},
    {3, 1, "x6-3-1", "p6-3-1"},
    {3, 2, "y6-3-1", "p6-3-1"},
    {0, 4, "z6-3-1", "p6-3-1"},
    {3, 1, "x6-3-2", "p6-3-2"},
    {3, 2, "y6-3-2", "p6-3-2"},
    {0, 4, "z6-3-2", "p6-3-2"},
    {3, 1, "x6-3-3", "p6-3-1"},
    {3, 2, "y6-3-3", "p6-3-1"},
    {3, 1, "x6-3-boss1", "p6-3-1"},
    {3, 2, "y6-3-boss1", "p6-3-1"},
    {0, 4, "z6-3-boss1", "p6-3-1"},
    {3, 1, "x6-3-boss2", "p6-3-boss2"},
    {3, 2, "y6-3-boss2", "p6-3-boss2"},
    {3, 4, "z6-3-boss2", "p6-3-boss2"},
    {3, 1, "xs-6-b", "p6-3-boss2a"},
    {1, 4, "w6-3-boss2a", "p6-3-boss2a"},
    {3, 1, "startup", "pstartup"},
    {3, 2, "ystartup", "pstartup"},
    {3, 1, "cha_1", "p_start_menu"},
    {3, 2, "cha_2", "p_start_menu"},
    {3, 3, "cha_3", "p_start_menu"},
    {3, 1, "select", "p_start_menu"},
    {3, 1, "xs-1-1", "ps-1-1"},
    {3, 1, "mboard_l", "p_mboard"},
    {3, 1, "stage_clear_DK_a", "p_stage_clear"},
    {3, 2, "stage_clear_DK_b", "p_stage_clear"},
    {3, 3, "stage_clear_palettechange", "p_stage_clear"},
    {3, 2, "vs_t2_1", "p_vs"},
    {3, 1, "vs_u2_1", "p_vs"},
    {3, 1, "vs_t1_1", "p_vs"},
    {3, 1, "kabegami", "p_vs"},
    {3, 3, "vs_u1_1", "p_vs"},
    {3, 1, "xy-0-0", "p2-1-2"}, // ?
    {3, 3, "z00", "p2-1-2"}, // ?
    {3, 1, "xtest10", "p2-1-2"}, // ?
    {3, 1, "xs-test", "p2-1-2"}, // ?
    {0, 4, "xex01", "pex01"},
    {3, 3, "zex01", "pex01"},
    {0, 4, "yex01", "pex01"},
    {0, 4, "xex02", "pex01"},
    {0, 4, "xex03", "pex01"},
    {0, 4, "xex04", "pex01"},
    {0, 4, "xex05", "pex01"},
    {0, 4, "xex06", "pex01"},
    {0, 4, "xex07", "pex01"},
    {0, 4, "xex08", "pex01"},
    {0, 4, "xex09", "pex01"},
    {0, 4, "xex10", "pex01"},
    {0, 4, "wex10", "pex01"},
    {0, 4, "xex11", "pex11"},
    {0, 4, "zex11", "pex11"},
    {0, 4, "yex11", "pex11"},
    {3, 1, "opt_hand_bg", "p_opt"},
    {3, 2, "ex_u2_2", "p_opt"},
    {3, 2, "ex_u1_2", "p_opt"},
    {3, 1, "xFun-1", "pFun-1"},
    {3, 2, "yFun-1", "pFun-1"},
    {3, 1, "xFun-2", "pFun-2"},
    {3, 2, "yFun-2", "pFun-2"},
    {3, 1, "xFun-3", "pFun-3"},
    {3, 3, "zFun-3", "pFun-3"},
    {3, 1, "xFun-4", "pFun-3"},
    {3, 2, "yFun-4", "pFun-4"},
    {3, 1, "xFun-5", "pFun-5"},
    {3, 2, "yFun-5", "pFun-5"},
    {3, 3, "game_over_BG", "p_game_over"},
    {3, 1, "game_over_DK_a", "p_game_over"},
    {3, 2, "game_over_DK_b", "p_game_over"},
    {3, 1, "cliff1", "cliff1-pal"},
    {3, 1, "beach_jungle1", "beach_jungle1-pal"},
    {3, 1, "doukutsu1", "doukutsu1-pal"},
    {3, 1, "xL04-2", "pLecture_DSLite"},
    {3, 1, "cliff3", "cliff3-pal"},
    {3, 1, "jungle1", "jungle1-pal"},
    {3, 1, "xL10-3", "pL10-3"},
    {3, 1, "cliff2", "cliff2-pal"},
    {3, 1, "garasu1", "garasu1-pal"},
    {3, 1, "iseki_soto1", "iseki_soto1-pal"},
    {3, 1, "xL17-3", "pLecture_DSLite"},
    {3, 1, "iseki-naka1", "iseki-naka1-pal"},
    {3, 1, "hoby1", "hoby1-pal"},
    {3, 1, "ghost_fog1", "ghost_naibu1-pal"},
    {3, 1, "ghost_naibu1", "ghost_naibu1-pal"},
    {3, 1, "ice1", "ice1-pal"},
    {3, 1, "yasai1", "yasai1-pal"},
    {3, 1, "iseki-ue", "iseki-ue-pal"},
    {3, 1, "ghost1", "ghost1-pal"},
    {3, 2, "fukidashi1", "beach1-pal"},
    {3, 1, "messageboard", "messageboard-pal"},
    {3, 1, "xm01", "pm01"},
    {3, 2, "ym01", "pm01"},
    {3, 2, "ym02", "pm02-2"},
    {3, 2, "ym02-3", "pm02-3"},
    {3, 1, "xm03-0", "pm03-0"},
    {0, 4, "ym03-0", "pm03-0"},
    {0, 4, "zm03-0", "pm03-0"},
    {3, 1, "xm03-1", "p1-1-4b"},
    {3, 1, "xm03-2", "p1-3-1"},
    {3, 1, "xm03-3", "pm03-3"},
    {0, 4, "ym03-3", "pm03-3"},
    {3, 3, "zm03-3", "pm03-3"},
    {3, 0, "wm03-4", "pm03-4"},
    {3, 1, "xm03-4", "pm03-4"},
    {3, 2, "ym03-4", "pm03-4"},
    {3, 3, "zm03-4", "pm03-4"},
    {3, 1, "xm04-1", "p1-1-4b"},
    {3, 1, "xm04-2", "p1-3-1"},
    {3, 1, "xm04-3", "pm04-3"},
    {0, 4, "ym04-3", "pm04-3"},
    {3, 3, "zm04-3", "pm04-3"},
    {3, 1, "xm04-4", "pm04-4"},
    {0, 4, "ym09-0", "pm04-4"},
    {3, 3, "zm09-0", "pm04-4"},
    {3, 1, "xm05-1", "pm05-1"},
    {0, 4, "zm05-1", "pm05-1"},
    {3, 1, "xm05-2", "pm05-2"},
    {0, 4, "ym05-2", "pm05-2"},
    {0, 4, "zm05-2", "pm05-2"},
    {3, 1, "xm05-3", "pm05-3"},
    {0, 4, "ym05-3", "pm05-3"},
    {0, 4, "zm05-3", "pm05-3"},
    {3, 1, "xm05-4", "pm05-4"},
    {0, 4, "ym05-4", "pm05-4"},
    {3, 3, "zm05-4", "pm05-4"},
    {3, 1, "xm06-1", "pm06-1"},
    {0, 4, "ym06-1", "pm06-1"},
    {3, 3, "zm06-1", "pm06-1"},
    {0, 4, "xm06-2", "pm06-2"},
    {0, 4, "ym06-2", "pm06-2"},
    {0, 4, "zm06-2", "pm06-2"},
    {3, 1, "xm06-3", "pm06-3"},
    {3, 2, "ym06-3", "pm06-3"},
    {3, 3, "zm06-3", "pm06-3"},
    {3, 3, "sel_bg_02", "p_pause_status"},
    {3, 1, "xvs01-1", "pvs01-1"},
    {3, 1, "op1_1", "p_op1"},
    {3, 2, "op1_2", "p_op1"},
    {3, 3, "op3_3", "p_op3"},
    {3, 2, "op3_2", "p_op3"},
    {3, 1, "op3_1", "p_op3"},
    {3, 0, "op3_0", "p_op3"},
    {3, 3, "op4_3", "p_op4"},
    {3, 2, "op4_2", "p_op4"},
    {3, 1, "op4_1", "p_op4"},
    {3, 3, "op5_3", "p_op5"},
    {3, 2, "op5_2", "p_op5"},
    {3, 1, "op5_1", "p_op5"},
    {0, 3, "opt_u3_1", "p_opt"},
    {3, 3, "zs-1-5", "ps-1-5"},
    {3, 2, "ys-1-5", "ps-1-5"},
    {3, 1, "xs-1-5", "ps-1-5"},
    {3, 1, "xs-1-b", "p1-4-boss"},
    {3, 2, "ys-1-c1", "p1-4-boss"},
    {0, 4, "zs-100", "ps-100"},
    {3, 1, "xs-100", "ps-100"},
    {3, 1, "xs-2-a1", "p2-5-boss"},
    {0, 4, "x3-5-boss", "p3-5-boss"},
    {3, 1, "factry_boss", "factry_boss-pal"},
    {3, 1, "xs-4-a", "p_mboard"},
    {3, 1, "doc_1", "doc_1-pal"},
    {3, 2, "ys-4-a", "p4-5-boss"},
    {3, 1, "x4-5-boss_E", "p4-5-boss"},
    {0, 4, "zs-4-b", "ps-4-b"},
    {3, 2, "ys-4-b", "ps-4-b"},
    {0, 4, "xs-4-b", "ps-4-b"},
    {0, 4, "cloud1", "cloud1-pal"},
    {3, 2, "ys-4-b7", "ps-4-b7"},
    {0, 4, "xs-4-b7", "ps-4-b7"},
    {0, 4, "ws-4-b7", "ps-4-b7"},
    {3, 3, "zs-4-b7", "ps-4-b7"},
    {3, 2, "ys-5-a", "p5-1-3a"},
    {0, 4, "ys-5-b", "ps-5-b"},
    {0, 4, "zs-5-b", "ps-5-b"},
    {3, 1, "kc-boss", "kc-boss-pal"},
    {3, 2, "ys-6-a", "p6-3-1"},
    {3, 1, "xs-6-a", "p6-3-1"},
    {3, 1, "banana1", "banana1-pal"},
    {3, 1, "&-s-test", "kc-boss-pal"},
    {0, 4, "yvs01-1", "pvs01-1"},
    {0, 4, "zvs01-1", "pvs01-1"},
    {3, 1, "xvs01-2", "pvs01-2"},
    {0, 4, "yvs01-2", "pvs01-2"},
    {3, 3, "zvs01-2", "pvs01-2"},
    {3, 1, "xvs01-3", "pvs01-3"},
    {3, 3, "zvs01-3", "pvs01-3"},
    {3, 1, "xvs01-4", "pvs01-4"},
    {0, 4, "yvs01-4", "pvs01-4"},
    {3, 3, "zvs01-4", "pvs01-4"},
    {3, 1, "xvs02-1", "pvs02-1"},
    {0, 4, "yvs02-1", "pvs02-1"},
    {0, 4, "zvs02-1", "pvs02-1"},
    {3, 1, "xvs02-2", "pvs02-2"},
    {0, 4, "yvs02-2", "pvs02-2"},
    {0, 4, "zvs02-2", "pvs02-2"},
    {0, 4, "xvs02-3", "pvs02-3"},
    {0, 4, "yvs02-3", "pvs02-3"},
    {3, 3, "zvs02-3", "pvs02-3"},
    {0, 4, "xvs02-6", "pvs02-6"},
    {0, 4, "yvs02-6", "pvs02-6"},
    {0, 4, "zvs02-6", "pvs02-6"},

    // Unusual entries:
    // {3, 3, "z2-4-3", "p2-4-3"},
    // {3, 3, "z1-2-2", "p2-4-4"},
    // {0, 4, "y3-1-2", "p3-1-2"},
    // {0, 2, "y3-4-k", "p3-4-2"},
    // {0, 4, "y3-5-2", "p3-5-2"},
    // {1, 4, "y4-1-4", "p4-1-6"},
    // {3, 4, "w4-1-4", "p4-1-6"},
    // {3, 1, "x4-3-2a", "p4-3-2a"},
    // {3, 4, "w5-1-5", "p5-1-5"},
    // {0, 4, "z5-2-1", "p5-2-1"},
    // {3, 4, "yFun-3", "pFun-3"},
    // {0, 4, "logo_nintendo", "p_logo"},
    // {0, 4, "logo_paon", "p_logo"},
};

void jc_levels(unsigned char *rom, size_t romsize, char *dir) {

    int length = sizeof(levels) / sizeof(struct Level);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < length; i++) {

        unsigned char *tileset_d = NULL;
        unsigned char *tilemap_d = NULL;
        unsigned char *att_data  = NULL;
        unsigned char *bitplane  = NULL;
        size_t tileset, tilemap, palette; // Addresses within ROM
        size_t tileset_len, tilemap_len, palette_len; // Length of data, may be compressed
        char name[255];
        int e;

        // Open tilemap, tileset and palette files.
        sprintf(name, "%s/%s.nbfc", folders[levels[i].index], levels[i].name);
        if (fetch_NDS_file(rom, name, &tileset, &tileset_len)) {
            printf("Failed to locate file: \"%s\".\n", name);
            continue;
        }
        sprintf(name, "%s/%s.nbfs", folders[levels[i].index], levels[i].name);
        if (fetch_NDS_file(rom, name, &tilemap, &tilemap_len)) {
            printf("Failed to locate file: \"%s\".\n", name);
            continue;
        }
        sprintf(name, "%s/%s.nbfp", "BGPal", levels[i].palette);
        if (fetch_NDS_file(rom, name, &palette, &palette_len)) {
            printf("Failed to locate file: \"%s\".\n", name);
            continue;;
        }

        // Decompress Tileset
        // Decompress Tilemap (first word is width&height)
        if ((e = dk_decompress_mem_to_mem(GBA_COMP, &tileset_d, &tileset_len, rom+tileset,   romsize-tileset))
        ||  (e = dk_decompress_mem_to_mem(GBA_COMP, &tilemap_d, &tilemap_len, rom+tilemap+4, romsize-tilemap))
        ) {
            fprintf(stderr, "%d: %s.\n", i, dk_get_error(e));
            goto next;
        }

        decode_layout(tilemap_d, &tilemap_len);


        // Do things here
        // 8bpp GBA-type tiles, etc

        int width  = rom[tilemap+0] + (rom[tilemap+1] << 8);
        int height = rom[tilemap+2] + (rom[tilemap+3] << 8);

        att_data = malloc(tilemap_len/2);
        bitplane = malloc(width * height * 4);

        if (att_data == NULL || bitplane == NULL) {
            fprintf(stderr, "Error: Failed to allocate data for something.\n");
            goto next;
        }

        gba_split(tilemap_d, att_data, tilemap_len/2); // DKC ruined my life

        unsigned char rgb[768];
        decode_palette(rgb, &rom[palette], 256);

        gba_tiles(bitplane, tileset_d, tilemap_d, att_data, rgb, tilemap_len/2, 0, levels[i].mode);
        arrange_gbc(bitplane, width, height, dir, levels[i].name);

next:
        free(att_data);
        free(bitplane);
        free(tileset_d);
        free(tilemap_d);

    }

}
