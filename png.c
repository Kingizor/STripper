#include <stdio.h>
#include <stdint.h>

#if (defined(LODEPNG))

unsigned lodepng_encode32_file(const char* filename, const unsigned char* image, unsigned w, unsigned h);

#elif (defined(LIBPNG))

#include <png.h>

static int libpng_write(char *name, uint8_t *image, int w, int h) {
    
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
        perror("Error: ");
        return -1;
    }
    
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (!png_ptr) {
        printf("Failed to write output PNG. (Write)\n");
        return -1;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    
    if (!info_ptr) {
        printf("Failed to write output PNG. (Info)\n");
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("Failed to write output PNG. (Initialization error)\n");
        return -1;
    }
            
    png_init_io(png_ptr, out);
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("Failed to write output PNG. (PNG header #1)\n");
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
        printf("Failed to write output PNG. (PNG header #2)\n");
        return -1;
    }
    
    png_write_image(png_ptr, rows);
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("Failed to write output PNG. (Write error)\n");
        return -1;
    }
    
    png_write_end(png_ptr, NULL);
    fclose(out);
    
    return 0;
    
}

#else
    #error "No PNG library defined. libpng or lodepng is required."
#endif

void write_png(char *name, uint8_t *image, int w, int h) {
    
    // RGBA assumed
    #if (defined(LODEPNG))
        lodepng_encode32_file(name, image, w, h);
    #elif (defined(LIBPNG))
        libpng_write(name, image, w, h);
    #else
        #error "No PNG library defined."
    #endif
    
}
