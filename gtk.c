#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "main.h"
#include "rom.h"

#define IMG_W 24
#define IMG_H 24
#define IMG_SIZE (IMG_W*IMG_H*3)

struct MAIN_WIN {
    GtkWidget *win;
    GtkWidget *parent;
    GtkWidget *top; /* top level widget */
    char *dir;
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

    struct COLOUR_NODE { /* DKL */
        GtkWidget *entry; /* text representation */
        GtkWidget *btn;   /* button containing the colour, (sensitive) */
        GtkWidget *img;   /* we need this to update the colour */
        GdkPixbuf *buf;   /* pixbuf, update with gdk_pixbuf_fill */
        unsigned palette; /* numeric representation */
    } cn[4];
};

static void quick_messagebox (char *msg, int type) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, 0, type, GTK_BUTTONS_CLOSE, msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static int select_output (struct MAIN_WIN *mw) { (void)mw;
    /* select output directory*/
    GtkWidget *select = gtk_file_chooser_dialog_new(
        "Choose output folder...",
        GTK_WINDOW(mw->win),
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_Select",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );
    gtk_widget_show_all(select);

    if (gtk_dialog_run(GTK_DIALOG(select)) == GTK_RESPONSE_OK) {
        mw->dir = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(select));
        gtk_widget_destroy(select);
        return 0;
    }
    gtk_widget_destroy(select);
    return 1;
}

static GtkWidget *make_cbtn (struct COLOUR_NODE *cn) {
    cn->buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0,8, IMG_W,IMG_H);
    gdk_pixbuf_fill(cn->buf, (cn->palette << 8) | 0xFF);
    cn->img = gtk_image_new_from_pixbuf(cn->buf);
    cn->btn = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(cn->btn), cn->img);
    return cn->btn;
}




struct PANEL_INFO {
    int group;
    struct PANEL_ITEM {
        int type;
        int group;
        char *name;
        void *func;
    } item[2];
};

static void generic_panel (
    struct MAIN_WIN *mw,
    const struct PANEL_INFO *panel,
    int size,
    char *name,
    void *extract
) {
    if (mw->top != NULL)
        gtk_widget_destroy(mw->top);
    mw->top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_box_pack_start(GTK_BOX(mw->top), gtk_label_new(name), 0,0,0);
    gtk_box_pack_start(GTK_BOX(mw->top), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0,0,0);

    struct RADIO_GROUP {
        GtkWidget *prev;
        const struct PANEL_ITEM *p;
    } radio = { NULL, NULL };

    struct COLOUR_NODE *cn = mw->cn;
    GtkSizeGroup *sz[2] = { NULL, NULL };
    int group = -1;

    for (int i = 0; i < size; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        for (int j = 0; j < 2; j++) {
            const struct PANEL_ITEM *p = &panel[i].item[j];
            GtkWidget *node = NULL;
            switch (p->type) {
                case 0: { /* label */
                    node = gtk_label_new(p->name);
                    gtk_label_set_xalign(GTK_LABEL(node), 0.0);
                    break;
                }
                case 1: { /* checkbox */
                    node = gtk_check_button_new();
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(node), p->group);
                    break;
                }
                case 2: { /* radio */
                    if (radio.prev != NULL && radio.p->group == p->group) {
                        node = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio.prev), p->name);
                    }
                    else { /* new node */
                        node = gtk_radio_button_new_with_label(NULL, p->name);
                        radio.prev = node;
                        radio.p    = p;
                    }
                    break;
                }
                case 3: { /* entry */
                    cn->entry = node = gtk_entry_new();
                    char colour[9];
                    sprintf(colour, "%06X", cn->palette & 0xFFFFFF);
                    gtk_entry_set_max_length(GTK_ENTRY(node), 6);
                    gtk_entry_set_text(GTK_ENTRY(node), colour);
                    break;
                }
                case 4: { /* picker */
                    node = make_cbtn(cn++);
                    break;
                }
            }
            if (p->func != NULL) {
                char *event = NULL;
                switch (p->type) {
                    case 1: { event = "toggled";  break; } /* label */
                    case 2: { event = "toggled";  break; } /* radio */
                    case 3: { event = "activate"; break; } /* entry */
                    case 4: { event = "clicked";  break; } /* picker */
                }
                g_signal_connect(G_OBJECT(node), event, G_CALLBACK(p->func), mw);
            }
            if (group != panel[i].group) {
                sz[0] = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
                sz[1] = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
                group = panel[i].group;
            }
            gtk_size_group_add_widget(sz[j], node);

            gtk_box_pack_start(GTK_BOX(row), node, 0,0,0);
        }
        gtk_box_pack_start(GTK_BOX(mw->top), row, 0,0,0);
    }

    GtkWidget *btn = gtk_button_new_with_mnemonic("_Extract...");
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(extract), mw);
    gtk_box_pack_start(GTK_BOX(mw->top), btn, 0,0,8);
    gtk_box_pack_start(GTK_BOX(mw->parent), mw->top, 0,0,0);
    gtk_widget_show_all(mw->parent);


    GtkRequisition wsize;
    gtk_widget_get_preferred_size(mw->parent, &wsize, NULL);
    gtk_window_resize(GTK_WINDOW(mw->win), wsize.width, wsize.height);
}


static void simple_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    if (select_output(mw))
        return;
    switch (mw->rom.meta->game) {
        case GBC_DKC:   { gbc_levels(mw->rom.buf, mw->rom.meta->size, mw->dir); break; }
        case GBA_DKKOS: { kos_levels(mw->rom.buf, mw->dir); break; }
        case  DS_DKJC:  {  jc_levels(mw->rom.buf, mw->dir); break; }
        default: { return; }
    }
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}

static void gba_dkc_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    void (*extract)(unsigned char*,char*,int,int) = NULL;
    if (select_output(mw))
        return;
    switch (mw->rom.meta->game) {
        case GBA_DKC:  { extract = dkc_gba_levels ; break; }
        case GBA_DKC2: { extract = dkc2_gba_levels; break; }
        case GBA_DKC3: { extract = dkc3_gba_levels; break; }
        default: { return; }
    }
    if (mw->mode & RIP_LEVEL)   extract(mw->rom.buf, mw->dir, mw->priority, 0); 
    if (mw->mode & RIP_TILESET) extract(mw->rom.buf, mw->dir, mw->priority, 1); 
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}

static void snes_dkc3_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    if (select_output(mw))
        return;
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    int region  = mw->rom.meta->region;
    if (mw->mode & RIP_SCREEN)   spec3(rom, size, mw->dir, region);
    if (mw->mode & RIP_LEVEL)   level3(rom, size, mw->dir, mw->priority, mw->damage,   0, region);
    if (mw->mode & RIP_LAYOUT)  level3(rom, size, mw->dir, mw->priority, mw->damage|2, 0, region);
    if (mw->mode & RIP_TILESET) level3(rom, size, mw->dir, mw->priority, mw->damage|1, 1, region);
    if (mw->mode & RIP_RAW_8x8) raw_bitplane3(rom, size, mw->dir);
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}
static void snes_dkc2_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    if (select_output(mw))
        return;
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    int region  = mw->rom.meta->region;
    if (mw->mode & RIP_SCREEN)   spec2(rom, size, mw->dir, region);
    if (mw->mode & RIP_LEVEL)   level2(rom, size, mw->dir, mw->priority, 0, 0, region);
    if (mw->mode & RIP_LAYOUT)  level2(rom, size, mw->dir, mw->priority, 2, 0, region);
    if (mw->mode & RIP_TILESET) level2(rom, size, mw->dir, mw->priority, 1, 1, region);
    if (mw->mode & RIP_RAW_8x8) raw_bitplane2(rom, size, mw->dir);
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}
static void snes_dkc_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    if (select_output(mw))
        return;
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    if (mw->mode & RIP_SCREEN)   spec1(rom, mw->dir);
    if (mw->mode & RIP_LEVEL)   level1(rom, size, mw->dir, mw->priority, 0, 0);
    if (mw->mode & RIP_LAYOUT)  level1(rom, size, mw->dir, mw->priority, 2, 0);
    if (mw->mode & RIP_TILESET) level1(rom, size, mw->dir, mw->priority, 1, 1);
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}
static void gb_dkl_call (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    if (select_output(mw))
        return;
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    void (*extract)(unsigned char*,size_t,char*,int,unsigned*,int) = NULL;
    switch (mw->rom.meta->game) {
        case GB_DKL:  { extract =  dkl_levels; break; }
        case GB_DKL2: { extract = dkl2_levels; break; }
        case GB_DKL3: { extract = dkl3_levels; break; }
        default: { return; }
    }
    unsigned palette[4] = {
        mw->cn[0].palette, mw->cn[1].palette,
        mw->cn[2].palette, mw->cn[3].palette
    };
    int sgb = (mw->mode & RIP_PALETTE);
    if (mw->mode & RIP_LEVEL)   extract(rom, size, mw->dir, sgb, palette, 0);
    if (mw->mode & RIP_TILESET) extract(rom, size, mw->dir, sgb, palette, 1);
    quick_messagebox("Done.", GTK_MESSAGE_INFO);
}


static void toggle_screen (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->mode |=  RIP_SCREEN;
    else
        mw->mode &= ~RIP_SCREEN;
}
static void toggle_levels (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->mode |=  RIP_LEVEL;
    else
        mw->mode &= ~RIP_LEVEL;
}
static void toggle_tileset (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->mode |=  RIP_TILESET;
    else
        mw->mode &= ~RIP_TILESET;
}
static void toggle_layout (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->mode |=  RIP_LAYOUT;
    else
        mw->mode &= ~RIP_LAYOUT;
}
static void toggle_8x8 (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->mode |=  RIP_RAW_8x8;
    else
        mw->mode &= ~RIP_RAW_8x8;
}
static void toggle_damage (GtkWidget *zz, struct MAIN_WIN *mw) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))
        mw->damage |=  0x10;
    else
        mw->damage &= ~0x10;
}


static int get_radio (GtkWidget *zz) {
    GSList *list = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zz));
    int count = -1;
    GSList *list2 = list;
    while (list2 != NULL) { count++; list2 = list2->next; }
    int n = 0;
    while (list != NULL) {
        GtkWidget *btn = list->data;

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn)))
            break;

        list = list->next;
        n++;
    }
    return count - n;
}

static void select_zero (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return;
    mw->priority &= ~1;
    mw->priority |=  n;
}
static void select_priority (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return;
    printf("n = %d.\n", n);
    mw->priority &= ~6;
    mw->priority |= n << 1;
}
static void select_palette (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return;
    mw->mode &= ~RIP_PALETTE;
    if (n) mw->mode |=  RIP_PALETTE;
    /*
    for (int i = 0; i < 4; i++) {
        if (mw->cn[i].entry != NULL)
            gtk_widget_set_sensitive(mw->cn[i].entry, !!n);
        if (mw->cn[i].btn   != NULL)
            gtk_widget_set_sensitive(mw->cn[i].btn,   !!n);
    }
    */
}

static void retrieve_colour (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    int n;
    for (n = 0; n < 4; n++)
        if (zz == mw->cn[n].entry)
            break;
    if (n == 4)
        return;
    struct COLOUR_NODE *cn = &mw->cn[n];
    const char *cstr = gtk_entry_get_text(GTK_ENTRY(zz));
    unsigned c = strtol(cstr, NULL, 16);
    cn->palette = c;
    gdk_pixbuf_fill(cn->buf, (c << 8) | 0xFF);
    gtk_image_set_from_pixbuf(GTK_IMAGE(cn->img), cn->buf);
}
static void pick_colour (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n;
    for (n = 0; n < 4; n++)
        if (zz == mw->cn[n].btn)
            break;
    if (n == 4)
        return;
    struct COLOUR_NODE *cn = &mw->cn[n];

    GtkWidget *picker = gtk_color_chooser_dialog_new("Pick a shade!", GTK_WINDOW(mw->win));
    GdkRGBA start = {
        (double)((cn->palette >> 16) & 0xFF) / 255.0,
        (double)((cn->palette >>  8) & 0xFF) / 255.0,
        (double)((cn->palette      ) & 0xFF) / 255.0,
        1.0
    };
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(picker), &start);
    gtk_widget_show_all(picker);

    if (gtk_dialog_run(GTK_DIALOG(picker)) == GTK_RESPONSE_OK) {
        GdkRGBA result;
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(picker), &result);
        gtk_widget_destroy(picker);
        unsigned char r = 255.0 * result.red;
        unsigned char g = 255.0 * result.green;
        unsigned char b = 255.0 * result.blue;
        cn->palette = (r << 16) | (g << 8) |  b;

        /* update widgets */
        char c[9];
        sprintf(c, "%06X", cn->palette);
        gtk_entry_set_text(GTK_ENTRY(cn->entry), c);
        gdk_pixbuf_fill(cn->buf, (cn->palette << 8) | 0xFF);
        gtk_image_set_from_pixbuf(GTK_IMAGE(cn->img), cn->buf);
    }
    else {
        gtk_widget_destroy(picker);
    }
}

static void snes_dkc3_panel (struct MAIN_WIN *mw, char *name) {
    static const struct PANEL_INFO dkc3[] = {
        { 0, {{0,0, "Extract Screens:",          NULL }, { 1,1, NULL, toggle_screen }} },
        { 0, {{0,0, "Extract Levels:",           NULL }, { 1,1, NULL, toggle_levels }} },
        { 0, {{0,0, "Extract Tilesets:",         NULL }, { 1,0, NULL, toggle_tileset}} },
        { 0, {{0,0, "Extract Layouts:",          NULL }, { 1,0, NULL, toggle_layout }} },
        { 0, {{0,0, "Extract 8x8 Tiles:",        NULL }, { 1,0, NULL, toggle_8x8    }} },
        { 0, {{0,0, "Damaged Tiles:",    NULL }, { 1,0, NULL,         toggle_damage}} },
        { 0, {{0,0, "Palette Index #0:", NULL }, { 2,3, "Transparent", select_zero }} },
        { 0, {{0,0, "",                  NULL }, { 2,3, "Backdrop",    select_zero }} },
        { 0, {{0,0, "Priority:",         NULL }, { 2,2, "Both",        select_priority }} },
        { 0, {{0,0, "",                  NULL }, { 2,2, "Set",         select_priority }} },
        { 0, {{0,0, "",                  NULL }, { 2,2, "Clear",       select_priority }} }
    };
    const int size = sizeof(dkc3) / sizeof(struct PANEL_INFO);
    generic_panel(mw, dkc3, size, name, snes_dkc3_call);
}
static void snes_dkc2_panel (struct MAIN_WIN *mw, char *name) {
    static const struct PANEL_INFO dkc2[] = {
        { 0, {{0,0, "Extract Screens:",  NULL }, { 1,1, NULL,        toggle_screen }} },
        { 0, {{0,0, "Extract Levels:",   NULL }, { 1,1, NULL,        toggle_levels }} },
        { 0, {{0,0, "Extract Tilesets:", NULL }, { 1,0, NULL,        toggle_tileset}} },
        { 0, {{0,0, "Extract Layouts:",  NULL }, { 1,0, NULL,        toggle_layout }} },
        { 0, {{0,0, "Extract 8x8 Tiles:",NULL }, { 1,0, NULL,        toggle_8x8    }} },
        { 0, {{0,0, "Palette Index #0:", NULL }, { 2,2, "Transparent", select_zero }} },
        { 0, {{0,0, "",                  NULL }, { 2,2, "Backdrop",    select_zero }} },
        { 0, {{0,0, "Priority:",         NULL }, { 2,1, "Both",      select_priority}} },
        { 0, {{0,0, "",                  NULL }, { 2,1, "Set",       select_priority}} },
        { 0, {{0,0, "",                  NULL }, { 2,1, "Clear",     select_priority}} }
    };
    const int size = sizeof(dkc2) / sizeof(struct PANEL_INFO);
    generic_panel(mw, dkc2, size, name, snes_dkc2_call);
}
static void snes_dkc1_panel (struct MAIN_WIN *mw, char *name) {
    static const struct PANEL_INFO dkc[] = {
        { 0, {{0,0, "Extract Screens:",  NULL }, { 1,1, NULL,         toggle_screen  }} },
        { 0, {{0,0, "Extract Levels:",   NULL }, { 1,1, NULL,         toggle_levels  }} },
        { 0, {{0,0, "Extract Tilesets:", NULL }, { 1,0, NULL,         toggle_tileset }} },
        { 0, {{0,0, "Extract Layouts:",  NULL }, { 1,0, NULL,         toggle_layout  }} },
        { 0, {{0,0, "Palette Index #0:", NULL }, { 2,2, "Transparent",  select_zero  }} },
        { 0, {{0,0, "",                  NULL }, { 2,2, "Backdrop",     select_zero  }} },
        { 0, {{0,0, "Priority:",         NULL }, { 2,1, "Both",       select_priority}} },
        { 0, {{0,0, "",                  NULL }, { 2,1, "Set",        select_priority}} },
        { 0, {{0,0, "",                  NULL }, { 2,1, "Clear",      select_priority}} }
    };
    const int size = sizeof(dkc) / sizeof(struct PANEL_INFO);
    generic_panel(mw, dkc, size, name, snes_dkc_call);
}
static void gb_dkl_panel (struct MAIN_WIN *mw, char *name) {
    static const struct PANEL_INFO dkl[] = {
        { 0, {{0,0, "Extract Levels:",   NULL }, {1,1, NULL,   toggle_levels }} },
        { 0, {{0,0, "Extract Tilesets:", NULL }, {1,0, NULL,   toggle_tileset}} },
        { 0, {{0,0, "Palette:",  NULL }, {2,1, "SGB",    select_palette }} },
        { 0, {{0,0, "",          NULL }, {2,1, "Custom", select_palette }} },
        { 1, {{3,2, NULL, retrieve_colour }, {4,2, NULL, pick_colour }} }, /* colour selection */
        { 1, {{3,3, NULL, retrieve_colour }, {4,3, NULL, pick_colour }} },
        { 1, {{3,4, NULL, retrieve_colour }, {4,4, NULL, pick_colour }} },
        { 1, {{3,5, NULL, retrieve_colour }, {4,5, NULL, pick_colour }} }
    };
    const int size = sizeof(dkl) / sizeof(struct PANEL_INFO);
    generic_panel(mw, dkl, size, name, gb_dkl_call);
}
static void gba_dkc_panel (struct MAIN_WIN *mw, char *name) {
    static const struct PANEL_INFO gba[] = {
        { 0, {{0,0, "Extract Levels:",   NULL }, {1,1, NULL, toggle_levels }} },
        { 0, {{0,0, "Extract Tilesets:", NULL }, {1,0, NULL, toggle_tileset}} },
        { 0, {{0,0, "Priority:", NULL }, {2,1, "Both",  select_priority}} },
        { 0, {{0,0, "",          NULL }, {2,1, "Set",   select_priority}} },
        { 0, {{0,0, "",          NULL }, {2,1, "Clear", select_priority}} }
    };
    const int size = sizeof(gba) / sizeof(struct PANEL_INFO);
    generic_panel(mw, gba, size, name, gba_dkc_call);
}

static void simple_panel (struct MAIN_WIN *mw, char *name) {
    generic_panel(mw, NULL, 0, name, simple_call);
}




static void create_panel (struct MAIN_WIN *mw) {
    /* set defaults */
    mw->mode     = RIP_LEVEL | RIP_SCREEN;
    mw->damage   = 0;
    mw->priority = 0;
    for (int i = 0; i < 4; i++) {
        mw->cn[i].entry = NULL;
        mw->cn[i].btn   = NULL;
    }

    switch (mw->rom.meta->game) {
        case SNES_DKC:  { snes_dkc1_panel(mw, "DKC SNES" ); break; }
        case SNES_DKC2: { snes_dkc2_panel(mw, "DKC2 SNES"); break; }
        case SNES_DKC3: { snes_dkc3_panel(mw, "DKC3 SNES"); break; }
        case   GB_DKL:  {    gb_dkl_panel(mw, "DKL GB"   ); break; }
        case   GB_DKL2: {    gb_dkl_panel(mw, "DKL2 GB"  ); break; }
        case   GB_DKL3: {    gb_dkl_panel(mw, "DKL3 GB"  ); break; }
        case  GBA_DKC:  {   gba_dkc_panel(mw, "DKC GBA"  ); break; }
        case  GBA_DKC2: {   gba_dkc_panel(mw, "DKC2 GBA" ); break; }
        case  GBA_DKC3: {   gba_dkc_panel(mw, "DKC3 GBA" ); break; }
        case  GBC_DKC:  {    simple_panel(mw, "DKC GBC"  ); break; }
        case  GBA_DKKOS:{    simple_panel(mw, "King of Swing GBA"); break; }
        case   DS_DKJC: {    simple_panel(mw, "Jungle Climber DS"); break; }
        default: { return; }
    }
}

static void open_file (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    (void)mw;

    /* create an open dialog */
    GtkWidget *open = gtk_file_chooser_dialog_new(
        "Open File...",
        GTK_WINDOW(mw->win),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Open",
        GTK_RESPONSE_OK,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );
    gtk_widget_show_all(open);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.sfc");
    gtk_file_filter_add_pattern(filter, "*.smc");
    gtk_file_filter_add_pattern(filter, "*.gb");
    gtk_file_filter_add_pattern(filter, "*.gbc");
    gtk_file_filter_add_pattern(filter, "*.gba");
    gtk_file_filter_add_pattern(filter, "*.ds");

    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(open), filter);

    if (gtk_dialog_run(GTK_DIALOG(open)) == GTK_RESPONSE_OK) {
        char *rom_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(open));
        gtk_widget_destroy(open);

        if (rom_name == NULL) {
            gtk_widget_destroy(open);
            free(rom_name);
            return;
        }

        /* load and verify ROM */
        char *error;
        struct ROM_DATA rom;
        if ((error = verify_rom(&rom, rom_name)) != NULL) {
            quick_messagebox(error, GTK_MESSAGE_ERROR);
            free(rom_name);
            return;
        }
        free(mw->rom.buf);
        mw->rom = rom;
        free(rom_name);

        /* create a new panel */
        create_panel(mw);
    }
    else {
        gtk_widget_destroy(open);
    }
}
static void quit_program (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    free(mw->dir);
    free(mw->rom.buf);
    gtk_main_quit();
}

static void create_window (struct MAIN_WIN *mw) {
    mw->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(mw->win), 320, 240);
    gtk_window_set_title(GTK_WINDOW(mw->win), "STripper");
    g_signal_connect(G_OBJECT(mw->win), "destroy", G_CALLBACK(quit_program), mw);
    mw->parent = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    /* generate menu */
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *bar = gtk_menu_bar_new();

    GtkWidget *file = gtk_menu_item_new_with_mnemonic("_File");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), file);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(file), menu);

    GtkWidget *open = gtk_menu_item_new_with_mnemonic("_Open");
    GtkWidget *quit = gtk_menu_item_new_with_mnemonic("_Quit");
    g_signal_connect(G_OBJECT(open), "activate", G_CALLBACK(open_file),    mw);
    g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(quit_program), mw);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), open);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit);

    /* list supported ROM versions */
    /*
    GtkWidget *help = gtk_menu_item_new_with_mnemonic("_Help");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), help);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(help), menu);

    GtkWidget *about = gtk_menu_item_new_with_mnemonic("_About");
    g_signal_connect(G_OBJECT(about), "activate", G_CALLBACK(show_about), mw);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), about);
    */

    gtk_box_pack_start(GTK_BOX(mw->parent), bar, 0,0,0);

    /* Top level container */
    mw->top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(mw->parent), mw->top, 0,0,0);

    gtk_container_add(GTK_CONTAINER(mw->win), mw->parent);
    gtk_widget_show_all(mw->win);
}


int main (int argc, char *argv[]) {
    struct MAIN_WIN mw;
    memset(&mw, 0, sizeof(struct MAIN_WIN));

    /* default palette */
    mw.cn[0].palette = 0xE8E8E8;
    mw.cn[1].palette = 0xA0A0A0;
    mw.cn[2].palette = 0x585858;
    mw.cn[3].palette = 0x101010;

    gtk_init(&argc, &argv);

    create_window(&mw);

    gtk_main();
    return 0;
}
