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
        fclose(out);
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr) {
        fprintf(stderr, "Failed to write output PNG. (Info)\n");
        fclose(out);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (Initialization error)\n");
        fclose(out);
        return -1;
    }

    png_init_io(png_ptr, out);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (PNG header #1)\n");
        fclose(out);
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
        fclose(out);
        return -1;
    }

    png_write_image(png_ptr, rows);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to write output PNG. (Write error)\n");
        fclose(out);
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

#define WORD(X) (X & 0xFF),\
                ((X >>  8) & 0xFF),\
                ((X >> 16) & 0xFF),\
                ((X >> 24) & 0xFF)

    unsigned char bmp_header[HEADER_SIZE] = {
        'B', 'M',   // Sig
        WORD(size), // Size
        WORD(0x00), // Reserved
        WORD(HEADER_SIZE), // Start of pixel data (absolute)

        WORD((HEADER_SIZE - 14)), // Subheader size
        WORD(w),    // Width
        WORD(h),    // Height
        0x01, 0x00, // Planes
        0x20, 0x00, // Bits per pixel
        WORD(3),    // Compression (3 = Bitmask)
        WORD(image_size), // Image size
        WORD(0xB13),// Horizontal resolution
        WORD(0xB13),// Vertical resolution
        WORD(0),    // Number of colours
        WORD(0),    // Important colours
        WORD(0x000000FF), // Bitmask Red
        WORD(0x0000FF00), // Bitmask Green
        WORD(0x00FF0000), // Bitmask Blue
        WORD(0xFF000000), // Bitmask Alpha
        0x00, 0x00  // Align to four bytes
    }; 

    if (fwrite(bmp_header, 1, HEADER_SIZE, bmp) != HEADER_SIZE) {
        fprintf(stderr, "Error writing BITMAP header.\n");
        fclose(bmp);
        return -1;
    }

    unsigned row_size = w * 4;

    for (int i = h-1; i >= 0; i--) {
        if (fwrite(&image[i*row_size], 1, row_size, bmp) != row_size) {
            fprintf(stderr, "Error writing image data.\n");
            fclose(bmp);
            return -1;
        }
    }
    fclose(bmp);

    return 0;
}

#endif

int write_png(char *dir, char *name, unsigned char *image, int w, int h) {

#define MAX_NAME 4096
    size_t dirlen = strlen(dir);
    if ((dirlen + strlen(name) + 8) >= MAX_NAME) {
        fprintf(stderr, "Error: Path is longer than %d characters. (%s)", MAX_NAME, name);
        return -1;
    }
    const char *form = "%s%s.%s";

#if defined(_WIN32)
    if (dirlen && dir[dirlen-1] != '\\')
        form = "%s\\%s.%s";
#else
    if (dirlen && dir[dirlen-1] != '/')
        form = "%s/%s.%s";
#endif

#if defined(LODEPNG) || defined(LIBPNG)
    const char *ext = "png";
#else
    const char *ext = "bmp";
#endif

    printf("Saving %s...\n", name);

    char path[MAX_NAME];
    snprintf(path, MAX_NAME-1, form, dir, name, ext);

    // RGBA assumed
#if defined(LODEPNG)
        lodepng_encode32_file(path, image, w, h);
#elif defined(LIBPNG)
        libpng_write(path, image, w, h);
#else
        bitmap_write32(path, image, w, h);
#endif

    return 0;
}
