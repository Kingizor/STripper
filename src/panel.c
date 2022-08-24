
#if defined(GTK3_UI)
#include <gtk/gtk.h>
#elif defined(WIN32_UI)
#include <windows.h>
#endif

#include "rom.h"
#include "gui.h"
#include "panel.h"
#include "main.h"

#if defined(GTK3_UI)
void quick_messagebox (char*,int);
#define quick_messagebox(X) quick_messagebox(X, GTK_MESSAGE_INFO)
void toggle_screen  (GtkWidget*,struct MAIN_WIN*);
void toggle_screen  (GtkWidget*,struct MAIN_WIN*);
void toggle_levels  (GtkWidget*,struct MAIN_WIN*);
void toggle_tileset (GtkWidget*,struct MAIN_WIN*);
void toggle_layout  (GtkWidget*,struct MAIN_WIN*);
void toggle_8x8     (GtkWidget*,struct MAIN_WIN*);
void toggle_damage  (GtkWidget*,struct MAIN_WIN*);
void select_zero    (GtkWidget*,struct MAIN_WIN*);
void select_priority(GtkWidget*,struct MAIN_WIN*);
void select_palette (GtkWidget*,struct MAIN_WIN*);
void retrieve_colour(GtkWidget*,struct MAIN_WIN*);
void     pick_colour(GtkWidget*,struct MAIN_WIN*);
#elif defined(WIN32_UI)
void quick_messagebox (char*);
void toggle_screen  (struct MAIN_WIN*,int);
void toggle_levels  (struct MAIN_WIN*,int);
void toggle_tileset (struct MAIN_WIN*,int);
void toggle_layout  (struct MAIN_WIN*,int);
void toggle_8x8     (struct MAIN_WIN*,int);
void toggle_damage  (struct MAIN_WIN*,int);
void select_zero    (struct MAIN_WIN*,int);
void select_priority(struct MAIN_WIN*,int);
void select_palette (struct MAIN_WIN*,int);
void retrieve_colour(struct MAIN_WIN*);
void     pick_colour(struct MAIN_WIN*);
#endif

void generic_panel (
    struct MAIN_WIN*,
    const struct PANEL_INFO*,
    int, char*
);

static const struct PANEL_INFO dkc3_panel[] = {
  { 0, {{ CTRL_LABEL,0, "Extract:",          NULL }, { CTRL_CHECKBOX, 1, "Screens",       toggle_screen   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 1, "Levels",        toggle_levels   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Tilesets",      toggle_tileset  }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Layouts",       toggle_layout   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "8x8 Tiles",     toggle_8x8      }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Damaged Tiles", toggle_damage   }} },
  { 0, {{ CTRL_LABEL,0, "Palette Index #0:", NULL }, { CTRL_RADIO,    3, "Transparent",   select_zero     }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    3, "Backdrop",      select_zero     }} },
  { 0, {{ CTRL_LABEL,0, "Priority:",         NULL }, { CTRL_RADIO,    2, "Both",          select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    2, "Set",           select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    2, "Clear",         select_priority }} }
};
static const int dkc3_panel_size = sizeof(dkc3_panel)
                                 / sizeof(struct PANEL_INFO);

static const struct PANEL_INFO dkc2_panel[] = {
  { 0, {{ CTRL_LABEL,0, "Extract:",          NULL }, { CTRL_CHECKBOX, 1, "Screens",     toggle_screen   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 1, "Levels",      toggle_levels   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Tilesets",    toggle_tileset  }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Layouts",     toggle_layout   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "8x8 Tiles",   toggle_8x8      }} },
  { 0, {{ CTRL_LABEL,0, "Palette Index #0:", NULL }, { CTRL_RADIO,    2, "Transparent", select_zero     }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    2, "Backdrop",    select_zero     }} },
  { 0, {{ CTRL_LABEL,0, "Priority:",         NULL }, { CTRL_RADIO,    1, "Both",        select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    1, "Set",         select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    1, "Clear",       select_priority }} }
};
static const int dkc2_panel_size = sizeof(dkc2_panel)
                                 / sizeof(struct PANEL_INFO);

static const struct PANEL_INFO dkc_panel[] = {
  { 0, {{ CTRL_LABEL,0, "Extract:",          NULL }, { CTRL_CHECKBOX, 1, "Screens",      toggle_screen   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 1, "Levels",       toggle_levels   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Tilesets",     toggle_tileset  }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Layouts",      toggle_layout   }} },
  { 0, {{ CTRL_LABEL,0, "Palette Index #0:", NULL }, { CTRL_RADIO,    2, "Transparent",  select_zero     }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    2, "Backdrop",     select_zero     }} },
  { 0, {{ CTRL_LABEL,0, "Priority:",         NULL }, { CTRL_RADIO,    1, "Both",         select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    1, "Set",          select_priority }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_RADIO,    1, "Clear",        select_priority }} }
};
static const int dkc_panel_size = sizeof(dkc_panel)
                                / sizeof(struct PANEL_INFO);

static const struct PANEL_INFO dkl_panel[] = {
  { 0, {{ CTRL_LABEL,0, "Extract:",          NULL }, { CTRL_CHECKBOX, 1, "Levels",   toggle_levels   }} },
  { 0, {{ CTRL_LABEL,0, NULL,                NULL }, { CTRL_CHECKBOX, 0, "Tilesets", toggle_tileset  }} },
  { 0, {{ CTRL_LABEL,0, "Palette:",          NULL }, { CTRL_RADIO,    1, "SGB",    select_palette }} },
  { 0, {{ CTRL_LABEL,0, "",                  NULL }, { CTRL_RADIO,    1, "Custom", select_palette }} },
  { 1, {{ CTRL_ENTRY,2, NULL,     retrieve_colour }, { CTRL_PICKER,   2, NULL,     pick_colour    }} }, /* colour selection */
  { 1, {{ CTRL_ENTRY,3, NULL,     retrieve_colour }, { CTRL_PICKER,   3, NULL,     pick_colour    }} },
  { 1, {{ CTRL_ENTRY,4, NULL,     retrieve_colour }, { CTRL_PICKER,   4, NULL,     pick_colour    }} },
  { 1, {{ CTRL_ENTRY,5, NULL,     retrieve_colour }, { CTRL_PICKER,   5, NULL,     pick_colour    }} }
};
static const int dkl_panel_size = sizeof(dkl_panel)
                                / sizeof(struct PANEL_INFO);

static const struct PANEL_INFO gba_panel[] = {
  { 0, {{ CTRL_LABEL,0, "Extract:",  NULL }, { CTRL_CHECKBOX, 1, "Levels",   toggle_levels   }} },
  { 0, {{ CTRL_LABEL,0, NULL,        NULL }, { CTRL_CHECKBOX, 0, "Tilesets", toggle_tileset  }} },
  { 0, {{ CTRL_LABEL,0, "Priority:", NULL }, { CTRL_RADIO,1, "Both",  select_priority}} },
  { 0, {{ CTRL_LABEL,0, "",          NULL }, { CTRL_RADIO,1, "Set",   select_priority}} },
  { 0, {{ CTRL_LABEL,0, "",          NULL }, { CTRL_RADIO,1, "Clear", select_priority}} }
};
static const int gba_panel_size = sizeof(gba_panel)
                                / sizeof(struct PANEL_INFO);



/* callbacks */
static void simple_call (struct MAIN_WIN *mw) {
    switch (mw->rom.meta->game) {
        case GBC_DKC:   { gbc_levels(mw->rom.buf, mw->rom.meta->size, mw->dir); break; }
        case GBA_DKKOS: { kos_levels(mw->rom.buf, mw->dir); break; }
        case  DS_DKJC:  {  jc_levels(mw->rom.buf, mw->dir); break; }
        default: { return; }
    }
}

static void gba_dkc_call (struct MAIN_WIN *mw) {
    void (*extract)(unsigned char*,char*,int,int) = NULL;
    switch (mw->rom.meta->game) {
        case GBA_DKC:  { extract = dkc_gba_levels ; break; }
        case GBA_DKC2: { extract = dkc2_gba_levels; break; }
        case GBA_DKC3: { extract = dkc3_gba_levels; break; }
        default: { return; }
    }
    if (mw->mode & RIP_LEVEL)   extract(mw->rom.buf, mw->dir, mw->priority, 0); 
    if (mw->mode & RIP_TILESET) extract(mw->rom.buf, mw->dir, mw->priority, 1); 
}

static void snes_dkc3_call (struct MAIN_WIN *mw) {
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    int region  = mw->rom.meta->region;
    if (mw->mode & RIP_SCREEN)   spec3(rom, size, mw->dir, region);
    if (mw->mode & RIP_LEVEL)   level3(rom, size, mw->dir, mw->priority, mw->damage,   0, region);
    if (mw->mode & RIP_LAYOUT)  level3(rom, size, mw->dir, mw->priority, mw->damage|2, 0, region);
    if (mw->mode & RIP_TILESET) level3(rom, size, mw->dir, mw->priority, mw->damage|1, 1, region);
    if (mw->mode & RIP_RAW_8x8) raw_bitplane3(rom, size, mw->dir);
}
static void snes_dkc2_call (struct MAIN_WIN *mw) {
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    int region  = mw->rom.meta->region;
    if (mw->mode & RIP_SCREEN)   spec2(rom, size, mw->dir, region);
    if (mw->mode & RIP_LEVEL)   level2(rom, size, mw->dir, mw->priority, 0, 0, region);
    if (mw->mode & RIP_LAYOUT)  level2(rom, size, mw->dir, mw->priority, 2, 0, region);
    if (mw->mode & RIP_TILESET) level2(rom, size, mw->dir, mw->priority, 1, 1, region);
    if (mw->mode & RIP_RAW_8x8) raw_bitplane2(rom, size, mw->dir);
}
static void snes_dkc_call (struct MAIN_WIN *mw) {
    unsigned char *rom = mw->rom.buf;
    size_t size = mw->rom.meta->size;
    if (mw->mode & RIP_SCREEN)   spec1(rom, size, mw->dir);
    if (mw->mode & RIP_LEVEL)   level1(rom, size, mw->dir, mw->priority, 0, 0);
    if (mw->mode & RIP_LAYOUT)  level1(rom, size, mw->dir, mw->priority, 2, 0);
    if (mw->mode & RIP_TILESET) level1(rom, size, mw->dir, mw->priority, 1, 1);
}
static void gb_dkl_call (struct MAIN_WIN *mw) {
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
}

int generic_extract (void *zz) {
    struct MAIN_WIN *mw = zz;
    switch (mw->rom.meta->game) {
        case SNES_DKC:  {  snes_dkc_call(mw); break; }
        case SNES_DKC2: { snes_dkc2_call(mw); break; }
        case SNES_DKC3: { snes_dkc3_call(mw); break; }
        case   GB_DKL:  
        case   GB_DKL2: 
        case   GB_DKL3: { gb_dkl_call(mw); break; }
        case  GBA_DKC:  
        case  GBA_DKC2:
        case  GBA_DKC3: { gba_dkc_call(mw); break; }
        case  GBC_DKC:
        case  GBA_DKKOS:
        case   DS_DKJC: { simple_call(mw); break; }
        default: { quick_messagebox("Unknown extractor?"); return 1; }
    }
    quick_messagebox("Done.");
    return 0;
}

/* neutral UI generation */
#define MAKE_PANEL(X,Y) \
    void Y##_panel (struct MAIN_WIN *mw, char *name) {\
        generic_panel(mw, X, X##_size, name);\
    }

MAKE_PANEL(dkc3_panel,snes_dkc3)
MAKE_PANEL(dkc2_panel,snes_dkc2)
MAKE_PANEL( dkc_panel,snes_dkc)
MAKE_PANEL( dkl_panel,  gb_dkl)
MAKE_PANEL( gba_panel, gba_dkc)

void simple_panel (struct MAIN_WIN *mw, char *name) {
    generic_panel(mw, NULL, 0, name);
}

void create_panel (struct MAIN_WIN *mw) {
    /* set defaults */
    mw->mode     = RIP_LEVEL | RIP_SCREEN;
    mw->damage   = 0;
    mw->priority = 0;

    switch (mw->rom.meta->game) {
        case SNES_DKC:  {  snes_dkc_panel(mw, "DKC SNES" ); break; }
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


