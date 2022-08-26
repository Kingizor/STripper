#ifndef GUI_H
#define GUI_H

struct MAIN_WIN {
#if defined(GTK3_UI)
    void *win;  /* main window */
    void *wwin; /* progress dialog */
    void *parent;
    void *top; /* top level widget */
    struct COLOUR_NODE { /* DKL */
        void *entry; /* text representation */
        void *btn;   /* button containing the colour, (sensitive) */
        void *img;   /* we need this to update the colour */
        void *buf;   /* pixbuf, update with gdk_pixbuf_fill */
        unsigned palette; /* numeric representation */
    } cn[4];
    char *dir;
#elif defined(WIN32_UI)
    HWND hwnd;
    HINSTANCE hinst;
    struct COLOUR_NODE {
        HWND entry;
        HWND btn;
        HBITMAP bitmap;
        unsigned palette;
    } cn[4];
    LPSTR dir;
#endif

    struct ROM_DATA rom;

    enum {
        RIP_LEVEL   =  1,
        RIP_SCREEN  =  2,
        RIP_TILESET =  4,
        RIP_LAYOUT  =  8,
        RIP_RAW_8x8 = 16,
        RIP_PALETTE = 32, /* Use a custom palette (GB) */
    } mode;
    int damage; /* 0x10 */
    int priority;
};

#endif
