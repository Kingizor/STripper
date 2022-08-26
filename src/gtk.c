#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "rom.h"
#include "gui.h"
#include "panel.h"

#define IMG_W 24
#define IMG_H 24
#define IMG_SIZE (IMG_W*IMG_H*3)

void quick_messagebox (char *msg, int type) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, 0, type, GTK_BUTTONS_CLOSE, msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

#define TOGGLE_FUNC(X,Y,Z) \
    gboolean toggle_##Y (GtkWidget *zz, struct MAIN_WIN *mw) {\
        mw->X &= ~Z;\
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zz)))\
            mw->X |= Z;\
        return 0;\
    }
TOGGLE_FUNC(mode, screen,  RIP_SCREEN )
TOGGLE_FUNC(mode, levels,  RIP_LEVEL  )
TOGGLE_FUNC(mode, tileset, RIP_TILESET)
TOGGLE_FUNC(mode, layout,  RIP_LAYOUT )
TOGGLE_FUNC(mode, 8x8,     RIP_RAW_8x8)
TOGGLE_FUNC(damage, damage,0x10)

static gboolean get_radio (GtkWidget *zz) {
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

gboolean select_zero (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return 0;
    mw->priority &= ~1;
    mw->priority |=  n;
    return 0;
}
gboolean select_priority (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return 0;
    mw->priority &= ~6;
    mw->priority |= n << 1;
    return 0;
}
gboolean select_palette (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n = get_radio(zz);
    if (n == -1) return 0;
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
    return 0;
}
gboolean retrieve_colour (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n;
    for (n = 0; n < 4; n++)
        if (zz == mw->cn[n].entry)
            break;
    if (n == 4)
        return 0;
    struct COLOUR_NODE *cn = &mw->cn[n];
    const char *cstr = gtk_entry_get_text(GTK_ENTRY(zz));
    unsigned c = strtol(cstr, NULL, 16);
    cn->palette = c;
    gdk_pixbuf_fill(cn->buf, (c << 8) | 0xFF);
    gtk_image_set_from_pixbuf(GTK_IMAGE(cn->img), cn->buf);
    return 0;
}
gboolean pick_colour (GtkWidget *zz, struct MAIN_WIN *mw) {
    int n;
    for (n = 0; n < 4; n++)
        if (zz == mw->cn[n].btn)
            break;
    if (n == 4)
        return 0;
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
    return 0;
}

static void end_extractor (GObject *s, GAsyncResult *r, gpointer arg) {
    (void)s;
    (void)r;
    struct MAIN_WIN *mw = arg;
    gtk_widget_destroy(GTK_WIDGET(mw->wwin));
    quick_messagebox("Done", GTK_MESSAGE_INFO);
}
static void run_extractor (GTask *task, gpointer sdata, gpointer tdata, GCancellable *cancellable) {
    (void)task;
    (void)sdata;
    (void)tdata;
    (void)cancellable;
    generic_extract(tdata);
}
static void begin_extractor (struct MAIN_WIN *mw) {
    GTask *task = g_task_new(NULL, NULL, end_extractor, mw);

    mw->wwin = gtk_dialog_new_with_buttons(
        "Extracting...",
        mw->win,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        NULL,
        NULL
    );
    gtk_window_set_default_size(GTK_WINDOW(mw->wwin), 200, -1);
    gtk_window_set_modal(GTK_WINDOW(mw->wwin), 1);
    GtkWidget *box = gtk_dialog_get_content_area(GTK_DIALOG(mw->wwin));
    GtkWidget *spin = gtk_spinner_new();
    gtk_container_add(GTK_CONTAINER(box), spin);
    gtk_spinner_start(GTK_SPINNER(spin));
    gtk_widget_show_all(mw->wwin);

    g_task_set_task_data(task, mw, NULL);
    g_task_run_in_thread(task, run_extractor);
}

/* select output directory and queue extraction */
static gboolean select_output (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
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
        begin_extractor(mw);
        return 0;
    }
    gtk_widget_destroy(select);
    return 0;
}



static GtkWidget *make_cbtn (struct COLOUR_NODE *cn) {
    cn->buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0,8, IMG_W,IMG_H);
    gdk_pixbuf_fill(cn->buf, (cn->palette << 8) | 0xFF);
    cn->img = gtk_image_new_from_pixbuf(cn->buf);
    cn->btn = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(cn->btn), cn->img);
    return cn->btn;
}

void generic_panel (
    struct MAIN_WIN *mw,
    const struct PANEL_INFO *panel,
    int size,
    char *name
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
                case CTRL_LABEL: {
                    node = gtk_label_new(p->name);
                    gtk_label_set_xalign(GTK_LABEL(node), 0.0);
                    break;
                }
                case CTRL_CHECKBOX: {
                    node = gtk_check_button_new_with_label(p->name);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(node), p->group);
                    break;
                }
                case CTRL_RADIO: {
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
                case CTRL_ENTRY: {
                    cn->entry = node = gtk_entry_new();
                    char colour[9];
                    sprintf(colour, "%06X", cn->palette & 0xFFFFFF);
                    gtk_entry_set_max_length(GTK_ENTRY(node), 6);
                    gtk_entry_set_text(GTK_ENTRY(node), colour);
                    break;
                }
                case CTRL_PICKER: {
                    node = make_cbtn(cn++);
                    break;
                }
                default: { exit(2); }
            }
            if (p->func != NULL) {
                char *event = NULL;
                switch (p->type) {
                    case CTRL_CHECKBOX:
                    case CTRL_RADIO:  { event = "toggled";  break; }
                    case CTRL_ENTRY:  { event = "activate"; break; }
                    case CTRL_PICKER: { event = "clicked";  break; }
                    default: { exit(3); }
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
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(select_output), mw);
    gtk_box_pack_start(GTK_BOX(mw->top), btn, 0,0,8);
    gtk_box_pack_start(GTK_BOX(mw->parent), mw->top, 0,0,0);
    gtk_widget_show_all(mw->parent);


    GtkRequisition wsize;
    gtk_widget_get_preferred_size(mw->parent, &wsize, NULL);
    gtk_window_resize(GTK_WINDOW(mw->win), wsize.width, wsize.height);
}


static gboolean open_file (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;

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
    gtk_file_filter_add_pattern(filter, "*.nds");

    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(open), filter);

    if (gtk_dialog_run(GTK_DIALOG(open)) == GTK_RESPONSE_OK) {
        char *rom_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(open));
        gtk_widget_destroy(open);

        if (rom_name == NULL) {
            gtk_widget_destroy(open);
            free(rom_name);
            return 0;
        }

        /* load and verify ROM */
        char *error;
        struct ROM_DATA rom;
        if ((error = verify_rom(&rom, rom_name)) != NULL) {
            quick_messagebox(error, GTK_MESSAGE_ERROR);
            free(rom_name);
            return 0;
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
    return 0;
}
static gboolean quit_program (GtkWidget *zz, struct MAIN_WIN *mw) { (void)zz;
    free(mw->dir);
    free(mw->rom.buf);
    gtk_main_quit();
    return 0;
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
