#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

static int libpng_write(char *name, unsigned char *image, int w, int h) {

    png_structp png_ptr;
    png_infop info_ptr;
    FILE *out;
    int depth = 4;
    int i;

    out = fopen(name, "wb");
    if (out == NULL) {
        perror("Write error");
        return -1;
    }

    png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL
    );
    if (png_ptr == NULL) {
        fprintf(stderr, "Failed to write output PNG. (Write)\n");
        fclose(out);
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Failed to write output PNG. (Info)\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(out);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error: Failed to save PNG.\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(out);
        return -1;
    }

    png_init_io(png_ptr, out);
    png_set_IHDR(png_ptr, info_ptr,
        w, h, 8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_ptr, info_ptr);
    for (i = 0; i < h; i++)
        png_write_row(png_ptr, &image[w*depth*i]);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(out);

    return 0;

}

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

    const char *ext = "png";

    printf("Saving %s...\n", name);

    char path[MAX_NAME];
    snprintf(path, MAX_NAME-1, form, dir, name, ext);

    // RGBA assumed
    libpng_write(path, image, w, h);

    return 0;
}
