#ifndef PANEL_H
#define PANEL_H

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
