#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

static int colour_sort (const void *aa, const void *bb) {
    const png_color *a = aa, *b = bb;
    return (a->red   < b->red  ) ? -1
         : (a->red   > b->red  ) ?  1
         : (a->green < b->green) ? -1
         : (a->green > b->green) ?  1
         : (a->blue  < b->blue ) ? -1
         : (a->blue  > b->blue );
}

static png_color get_colour (unsigned char *img, int index) {
    png_color c;
    c.red   = img[index  ];
    c.green = img[index+1];
    c.blue  = img[index+2];
    return c;
}

static int get_index (
    unsigned char *img,
    png_color *map,
    int colour_count,
    int transparent
) {
    png_color c = get_colour(img, 0);
    if (!img[3]) return 0;
    png_colorp p = bsearch(&c, map, colour_count, sizeof(png_color), colour_sort);
    return p - (map - transparent);
}

/* optimize image */
/* input must be RGBA */
static int image_reduce (
    unsigned char *img,
    int w, int h,
    int *colour_count,
    png_color *map,
    int *transparent,
    int *depth,
    int *stride
) {
    int i,j;
    int exceeded = 0;
    *transparent = 0;
    *colour_count = 0;

    /* count number of colours in image */
    for (i = 0; i < w*h*4; i+=4) {
        png_color c = get_colour(img, i);

        /* this program does not use partial transparency */
        if (img[i+3] == 0) {
            *transparent = 1;
            continue;
        }

        /* image can't be indexed if there are too many colours */
        if ((*transparent + *colour_count) == 256) {
            exceeded = 1;
            continue;
        }

        /* have we seen this colour before? */
        png_colorp p = bsearch(&c, map, *colour_count, sizeof(png_color), colour_sort);
        if (p != NULL)
            continue;

        /* add the new colour */
        map[*colour_count] = c;
        *colour_count += 1;
        qsort(map, *colour_count, sizeof(png_color), colour_sort);
    }

    /* stick with RGB(A) if there are too many colours */
    if ((*transparent + *colour_count) > 256)
        exceeded = 1;

    if (exceeded) {
        if (*transparent) {
            *depth  = 8;
            *stride = w*4;
            return PNG_COLOR_TYPE_RGB_ALPHA;
        }

        /* convert to RGB */
        for (i = 0; i < w*h; i++) {
            img[i*3  ] = img[i*4  ];
            img[i*3+1] = img[i*4+1];
            img[i*3+2] = img[i*4+2];
        }
        *depth  = 8;
        *stride = w*3;
        return PNG_COLOR_TYPE_RGB;
    }

    /* add a transparent colour */
    if (*transparent) {
        png_color c;
        memset(&c, 0, sizeof(png_color));
        memmove(map+1, map, *colour_count * sizeof(png_color));
        *map = c;
        map++; /* careful */
    }

    /* convert to indexed */
    if ((*transparent + *colour_count) <= 4) { /* 2-bit */
        *depth  = 2;
        *stride = w/4;
        if (w & 3) {
            w &= ~3;
            w +=  4;
        }
        for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int ofs = w*i + j;
            int index = get_index(img+4*ofs, map, *colour_count, *transparent);
            int shift = ((ofs & 3) ^ 3) << 1;
            unsigned char *dst = &img[ofs/4];
            *dst &= ~(3 << shift);
            *dst |= index << shift;
        }
        }
    }
    else if ((*transparent + *colour_count) <= 16) { /* 4-bit */
        *depth  = 4;
        *stride = w/2;
        if (w & 1) {
            w &= ~1;
            w +=  2;
        }
        for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int ofs = w*i + j;
            int index = get_index(img+4*ofs, map, *colour_count, *transparent);
            int shift = ((ofs & 1) ^ 1) << 2;
            unsigned char *dst = &img[ofs/2];
            *dst &= ~(15  << shift);
            *dst |= index << shift;
        }
        }
    }
    else { /* 8-bit */
        *depth  = 8;
        *stride = w;
        for (i = 0; i < w*h; i++)
            img[i] = get_index(img+4*i, map, *colour_count, *transparent);
    }

    if (*transparent)
        *colour_count += 1;

    return PNG_COLOR_TYPE_PALETTE;
}

static int libpng_write(char *name, unsigned char *image, int w, int h) {

    png_structp png_ptr;
    png_infop info_ptr;
    FILE *out;
    int i;

    out = fopen(name, "wb");
    if (out == NULL) {
        perror("Write error");
        return 1;
    }

    png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL
    );
    if (png_ptr == NULL) {
        fprintf(stderr, "Failed to write output PNG. (Write)\n");
        fclose(out);
        return 1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Failed to write output PNG. (Info)\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(out);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error: Failed to save PNG.\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(out);
        return 1;
    }

    int depth  = 8;
    int stride = 0;
    int colour_count = 0;
    int transparent = 0;
    png_color map[256];
    int colour_type = image_reduce(image, w, h, &colour_count, map, &transparent, &depth, &stride);

    if (colour_type == PNG_COLOR_TYPE_PALETTE) {
        if (transparent) {
            png_byte alpha = 0;
            png_set_tRNS(png_ptr, info_ptr, &alpha, 1, NULL);
        }
        png_set_PLTE(png_ptr, info_ptr, map, colour_count);
    }

    png_init_io (png_ptr, out);
    png_set_IHDR(png_ptr, info_ptr,
        w, h, depth,
        colour_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_ptr, info_ptr);
    for (i = 0; i < h; i++)
        png_write_row(png_ptr, &image[stride*i]);
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
        return 1;
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

