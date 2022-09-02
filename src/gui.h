/* SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Kingizor
 * STripper - Interface Definitions */

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
    HWND subwnd;
    HWND subwnd_label;
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


enum CTRL_TYPE {
    CTRL_LABEL,
    CTRL_CHECKBOX,
    CTRL_RADIO,
    CTRL_ENTRY,
    CTRL_PICKER,
    CTRL_BUTTON,
    CTRL_GROUP
};

struct PANEL_INFO {
    int group;
    struct PANEL_ITEM {
        enum CTRL_TYPE type;
        int group;
        char *name;
#if defined(GTK3_UI)
        void (*func)(GtkWidget*,struct MAIN_WIN*);
#elif defined(WIN32_UI)
        void (*func)(struct MAIN_WIN*,int);
#endif
    } item[2];
};

void create_panel   (struct MAIN_WIN*);
int generic_extract (void*);

#endif
