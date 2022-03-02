#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (defined(LODEPNG))

unsigned lodepng_encode32_file(const char* filename, const unsigned char* image, unsigned w, unsigned h);

#elif (defined(LIBPNG))

#include <png.h>
#include <zlib.h>

static int libpng_write(char *name, unsigned char *image, int w, int h) {

    png_structp png_ptr;
    png_infop info_ptr;
    png_byte *rows[h];
    int depth = 4;

    // Generate row pointers
    for (int i = 0; i < h; i++) {
        rows[i] = (png_byte*)&image[w*i*depth];
    }

    FILE *out = fopen(name, "wb");

    if (out == NULL) {
        perror("Write error");
        return -1;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        fprintf(stderr, "Failed to write output PNG. (Write)\n");
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr) {
        fprintf(stderr, "Failed to write output PNG. (Info)\n");
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (Initialization error)\n");
        return -1;
    }

    png_init_io(png_ptr, out);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (PNG header #1)\n");
        return -1;
    }

    /*
    int ct;
    if (depth == 3) {
        ct = PNG_COLOR_TYPE_RGB;
    }
    else if (depth == 4) {
        ct = PNG_COLOR_TYPE_RGBA;
    }
    */

    png_set_IHDR(png_ptr, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (PNG header #2)\n");
        return -1;
    }

    png_write_image(png_ptr, rows);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (Write error)\n");
        return -1;
    }

    png_write_end(png_ptr, NULL);
    fclose(out);

    return 0;

}

#else

static int bitmap_write32(char *name, unsigned char *image, int w, int h) {

    FILE *bmp = fopen(name, "wb");

    if (bmp == NULL) {
        perror("Write error");
        return -1;
    }

    int image_size = w * h * 4;

    #define HEADER_SIZE 0x48
    int size = image_size + HEADER_SIZE;

    unsigned char bmp_header[HEADER_SIZE] = {
        0x42, 0x4D, // Sig
        size, size>>8, size>>16, size>>24, // Size
        0x00, 0x00, 0x00, 0x00, // Reserved
        0x48, 0x00, 0x00, 0x00, // Start of pixel data (absolute)

        0x3A, 0x00, 0x00, 0x00, // Subheader size
        w, w>>8, w>>16, w>>24, // Width
        h, h>>8, h>>16, h>>24, // Height
        0x01, 0x00, // Planes
        0x20, 0x00, // Bits per pixel
        0x03, 0x00, 0x00, 0x00, // Compression (3 = Bitmask)
        image_size, image_size>>8, image_size>>16, image_size>>24, // Image size
        0x00, 0x00, 0x00, 0x00, // Horizontal resolution
        0x00, 0x00, 0x00, 0x00, // Vertical resolution
        0x00, 0x00, 0x00, 0x00, // Number of colours
        0x00, 0x00, 0x00, 0x00, // Important colours

        0xFF, 0x00, 0x00, 0x00, // Bitmask Red
        0x00, 0xFF, 0x00, 0x00, // Bitmask Green
        0x00, 0x00, 0xFF, 0x00, // Bitmask Blue
        0x00, 0x00, 0x00, 0xFF, // Bitmask Alpha
        0x00, 0x00, // Align to four bytes

        }; 

    if (fwrite(bmp_header, 1, HEADER_SIZE, bmp) != HEADER_SIZE) {
        fprintf(stderr, "Error writing BITMAP header.\n");
        return -1;
    }

    unsigned row_size = w * 4;

    for (int i = h-1; i >= 0; i--) {
        if (fwrite(&image[i*row_size], 1, row_size, bmp) != row_size) {
            fprintf(stderr, "Error writing image data.\n");
            return -1;
        }
    }

    return 0;
}

#endif

int write_png(char *dir, char *name, unsigned char *image, int w, int h) {

    if ((strlen(dir) + strlen(name) + 4) > 255) {
        fprintf(stderr, "Error: Path is longer than 255 characters. (%s)", name);
        return -1;
    }

    printf("Saving %s...\n", name);

    char path[255];
    sprintf(path, "%s%s", dir, name);

    // RGBA assumed
    #if (defined(LODEPNG))
        sprintf(path, "%s.png", name);
        lodepng_encode32_file(path, image, w, h);
    #elif (defined(LIBPNG))
        sprintf(path, "%s.png", name);
        libpng_write(path, image, w, h);
    #else
        sprintf(path, "%s.bmp", name);
        bitmap_write32(path, image, w, h);
    #endif

    return 0;
}
