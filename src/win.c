#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <shlobj.h>

#include "rom.h"
#include "gui.h"
#include "panel.h"


struct MAIN_WIN main_win;

static COLORREF acrCustClr[16] = {
    0xE8E8E8, 0xA0A0A0, 0x585858, 0x101010
};

static int select_output (struct MAIN_WIN*);



void quick_messagebox (char *str) {
    MessageBox(NULL, _T(str), _T("STripper"), 0);
}


/* functions for handling input events */
#define TOGGLE_FUNC(X,Y,Z) \
    void toggle_##Y (struct MAIN_WIN *mw, int set) {\
        mw->X &= ~Z;\
        if (set)\
            mw->X |= Z;\
    }
TOGGLE_FUNC(mode, screen,  RIP_SCREEN );
TOGGLE_FUNC(mode, levels,  RIP_LEVEL  );
TOGGLE_FUNC(mode, tileset, RIP_TILESET);
TOGGLE_FUNC(mode, layout,  RIP_LAYOUT );
TOGGLE_FUNC(mode, 8x8,     RIP_RAW_8x8);
TOGGLE_FUNC(damage, damage,0x10);

void select_zero (struct MAIN_WIN *mw, int n) {
    mw->priority &= ~1;
    mw->priority |= !!n;
}
void select_priority (struct MAIN_WIN *mw, int n) {
    mw->priority &= ~6;
    mw->priority |= n << 1;
}
void select_palette (struct MAIN_WIN *mw, int n) {
    mw->mode &= ~RIP_PALETTE;
    if (n)
        mw->mode |= RIP_PALETTE;
}
void retrieve_colour (struct MAIN_WIN *mw) {
    (void)mw;
}
void pick_colour (struct MAIN_WIN *mw) {
    (void)mw;
}





#define CTRL_LIMIT 32
struct WIN_CTRL {
    enum CTRL_TYPE type;
    HWND instance;
    const struct PANEL_ITEM *item;
} ctrl_list[CTRL_LIMIT];
static unsigned control_n = 0;

static void erase_window (void) {
    while (control_n--) {
        struct WIN_CTRL *c = &ctrl_list[control_n];
        if (c->instance)
            DestroyWindow(c->instance);
    }
    for (int i = 0; i < 4; i++)
        if (main_win.cn[i].bitmap != NULL) {
            DeleteObject(main_win.cn[i].bitmap);
            main_win.cn[i].bitmap = NULL;
        }
    control_n = 0;
}

static int ctrl_limit (void) {
    if (control_n >= CTRL_LIMIT) {
        quick_messagebox("Surpassed the designated control limit.");
        return 1;
    }
    return 0;
}
static void add_ctrl (int type, HWND hwnd, const struct PANEL_ITEM *p) {
    struct WIN_CTRL *c = &ctrl_list[control_n++];
    c->type     = type;
    c->instance = hwnd;
    c->item     = p;
}


struct CTRL_POS {
    int x,y,w,h;
};

/* misc win32 helpers */
#define CREATE_CTRL(W,X,Y,Z) \
    static HWND create_##W (\
        const struct PANEL_ITEM *item,\
        struct CTRL_POS *p,\
        int style\
    ) {\
        if (ctrl_limit()) exit(1);\
        HWND zz = CreateWindow(\
            _T(Y),\
            _T((item       == NULL\
            ||  item->name == NULL) ? "" : item->name),\
            WS_CHILD|WS_VISIBLE|Z|style,\
            p->x, p->y, p->w, p->h,\
            main_win.hwnd, (HMENU)control_n, main_win.hinst, NULL\
        );\
        add_ctrl(CTRL_##X, zz, item);\
        return zz;\
    }

CREATE_CTRL(  button,   BUTTON, "Button", BS_DEFPUSHBUTTON);
CREATE_CTRL(   label,    LABEL, "Static", SS_LEFT);
CREATE_CTRL(checkbox, CHECKBOX, "Button", BS_AUTOCHECKBOX);
CREATE_CTRL(   radio,    RADIO, "Button", BS_AUTORADIOBUTTON);
CREATE_CTRL(   group,    GROUP, "Button", BS_GROUPBOX);
CREATE_CTRL(   entry,    ENTRY,   "Edit", WS_BORDER);


#define WIN_BORDER_X 20
#define WIN_BORDER_Y 10
#define CTRL_WIDTH  128
#define CTRL_HEIGHT  24
#define BMP_WIDTH   CTRL_WIDTH -5
#define BMP_HEIGHT  CTRL_HEIGHT-5

static COLORREF swap_rb (unsigned c) {
    return ((c & 0xFF0000) >> 16)
         |  (c & 0x00FF00)
         | ((c & 0x0000FF) << 16);
}
static void update_bitmap (struct COLOUR_NODE *cn) {
    if (cn->bitmap != NULL)
        DeleteObject(cn->bitmap);
    HDC hdc  = GetDC(cn->btn);
    cn->bitmap = CreateCompatibleBitmap(hdc, BMP_WIDTH, BMP_HEIGHT);
    HDC hdcm = CreateCompatibleDC(hdc);
    SelectObject(hdcm, cn->bitmap);
    SetDCBrushColor(hdcm, swap_rb(cn->palette));
    RECT r = { 0, 0, BMP_WIDTH, BMP_HEIGHT };
    FillRect(hdcm, &r, (HBRUSH)GetStockObject(DC_BRUSH));
    DeleteDC(hdcm);
    ReleaseDC(cn->btn, hdc);
    SendMessage(cn->btn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)cn->bitmap);
}


void generic_panel (
    struct MAIN_WIN *mw,
    const struct PANEL_INFO *panel,
    int panel_size,
    char *name
) {

    struct CTRL_POS pos = {
        WIN_BORDER_X, WIN_BORDER_Y, CTRL_WIDTH, CTRL_HEIGHT
    };

    { /* Text containing name of game */
        struct PANEL_ITEM item = { CTRL_LABEL, 0, name, NULL };
        create_label(&item, &pos, 0);
        ctrl_list[0].item = NULL;
    }

    int prev_group = -1;

    /* misc controls */
    /* these are populated vertically because radio buttons */
    /* need to be sequential for navigation to work */
    for (int j = 0; j < 2; j++) {
        pos.x = WIN_BORDER_X + j*CTRL_WIDTH;
        pos.y = WIN_BORDER_Y;
        struct COLOUR_NODE *cn = mw->cn;
        for (int i = 0; i < panel_size; i++) {
            const struct PANEL_ITEM *p = &panel[i].item[j];
            pos.y += CTRL_HEIGHT;

            switch (p->type) {
                case CTRL_LABEL: {
                    create_label(p, &pos, WS_GROUP);
                    break;
                }
                case CTRL_CHECKBOX: {
                    create_checkbox(p, &pos, WS_GROUP|WS_TABSTOP);
                    if (p->group)
                        CheckDlgButton(mw->hwnd, control_n-1, BST_CHECKED);
                    break;
                }
                case CTRL_RADIO: {
                    /* create a new radio group, if needed */
                    if (prev_group != p->group) {
                        prev_group = p->group;
                        int k;
                        for (k = i; k < panel_size; k++) {
                            const struct PANEL_ITEM *p2 = &panel[k].item[j];
                            if (p2->type  != CTRL_RADIO
                            ||  p2->group != prev_group)
                                break;
                        }
                        k -= i;
                        pos.h = k*CTRL_HEIGHT;
                        create_group(p, &pos, WS_GROUP);
                        pos.h =   CTRL_HEIGHT;
                        create_radio(p, &pos, WS_TABSTOP);
                        CheckDlgButton(mw->hwnd, control_n-1, BST_CHECKED);
                    }
                    else {
                        create_radio(p, &pos, 0);
                    }
                    break;
                }
                case CTRL_ENTRY: {
                    char str[8];
                    snprintf(str, 8, "%06X", cn->palette);
                    cn->entry = create_entry(p, &pos, WS_GROUP|WS_TABSTOP);
                    SetWindowText(cn->entry, str);
                    SendMessage(cn->entry, EM_SETLIMITTEXT, 6, 0);
                    cn++;
                    break;
                }
                case CTRL_PICKER: {
                    cn->btn = create_button(p, &pos, WS_GROUP|WS_TABSTOP|BS_BITMAP);
                    ctrl_list[control_n-1].type = CTRL_PICKER;
                    update_bitmap(cn);
                    cn++;
                    break;
                }
                default: { exit(2); break; }
            }

        }
    }

    /* extract button */
    pos.x = WIN_BORDER_X+CTRL_WIDTH/2;
    pos.y = WIN_BORDER_Y+CTRL_HEIGHT*(2+panel_size);
    static struct PANEL_ITEM ext = { CTRL_BUTTON, 0, "&Extract...", NULL };
    ext.func = select_output;
    create_button(&ext, &pos, WS_GROUP|WS_TABSTOP);

    { /* Resize window to fit */
        RECT rect;
        GetWindowRect(ctrl_list[control_n-1].instance, &rect);
        int w = 2*(WIN_BORDER_X + CTRL_WIDTH);
        int h = WIN_BORDER_Y + rect.bottom;

        for (unsigned i = 0; i < control_n; i++) {
            GetWindowRect(ctrl_list[i].instance, &rect);
            int width = 2*WIN_BORDER_X + rect.right - rect.left;
            if (w < width)
                w = width;
        }
        GetWindowRect(mw->hwnd, &rect);
        h -= rect.top;
        SetWindowPos (mw->hwnd, 0,
            rect.left, rect.top, w,h,
            SWP_FRAMECHANGED|SWP_NOZORDER|SWP_NOMOVE
        );
    }
    SetFocus(ctrl_list[0].instance);
}



static void resolve_button_press (struct MAIN_WIN *mw, WPARAM wParam) {
    unsigned id = LOWORD(wParam);
    if (id >= control_n)
        return;
    struct WIN_CTRL *ctrl = &ctrl_list[id];
    if (ctrl->item->func == NULL)
        return;

    switch (ctrl->item->type) {
        case CTRL_CHECKBOX: {
            void (*func)(struct MAIN_WIN*, int) = ctrl->item->func;
            func(mw, !!IsDlgButtonChecked(mw->hwnd, id));
            break;
        }
        case CTRL_BUTTON: {
            if (LOWORD(wParam) == (control_n-1)) { /* extraction */
                int  (*func)(struct MAIN_WIN*) = ctrl->item->func;
                func(mw);
            }
            else { /* something else */
                void (*func)(struct MAIN_WIN*) = ctrl->item->func;
                func(mw);
            }
            break;
        }
        case CTRL_RADIO: {
            void (*func)(struct MAIN_WIN*, int) = ctrl->item->func;
            struct WIN_CTRL *c2 = ctrl;
            int n = -2;
            while (c2->item->group == ctrl->item->group)
                { c2--; n++; }
            func(mw, n);
            break;
        }
        case CTRL_ENTRY: {
            int i;
            for (i = 0; i < 4; i++)
                if (mw->cn[i].entry == ctrl->instance)
                    break;
            if (i == 4)
                return;
            struct COLOUR_NODE *cn = &mw->cn[i];
            char str[32];
            GetWindowText(ctrl->instance, str, 7);
            cn->palette = strtol(str, NULL, 16);
            update_bitmap(cn);
            break;
        }
        case CTRL_PICKER: {
            int i;
            for (i = 0; i < 4; i++)
                if (mw->cn[i].btn == ctrl->instance)
                    break;
            if (i == 4)
                return;
            struct COLOUR_NODE *cn = &mw->cn[i];
            CHOOSECOLOR colour;
            colour.lStructSize  = sizeof(colour);
            colour.hwndOwner    = mw->hwnd;
            colour.rgbResult    = swap_rb(cn->palette);
            colour.lpCustColors = (LPDWORD)acrCustClr;
            colour.Flags        = CC_RGBINIT | CC_FULLOPEN;
            if (ChooseColor(&colour)) {
                cn->palette = swap_rb(colour.rgbResult);
                char str[8];
                snprintf(str, 8, "%06X", cn->palette);
                SetWindowText(cn->entry, str);
                /* updating the entry also updates the bitmap */
                /* update_bitmap(cn); */
            }
            break;
        }
        default: { return; }
    }

}

enum WIN_COMMANDS {
    APP_QUIT,
    APP_OPEN
};

static void create_menu (HWND hwnd) {
    HMENU hmenu = CreateMenu();
    HMENU hfile = CreateMenu();

    AppendMenu(hmenu, MF_STRING,    APP_OPEN, _T("&Open..."));
    AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hmenu, MF_STRING,    APP_QUIT, _T("&Quit"));

    AppendMenu(hfile, MF_POPUP, (UINT_PTR)hmenu, _T("&File"));
    SetMenu(hwnd, hfile);
}


/* select an output folder */
static int select_output (struct MAIN_WIN *mw) {
    BROWSEINFO bi;
    bi.hwndOwner      = mw->hwnd;
    bi.pidlRoot       = NULL;
    bi.pszDisplayName = mw->dir;
    bi.lpszTitle      = _T("Select an output folder...");
    bi.ulFlags        = BIF_NEWDIALOGSTYLE;
    bi.lpfn           = NULL;
    bi.lParam         = 0;
    bi.iImage         = 0;
    LPITEMIDLIST folder = SHBrowseForFolder(&bi);
    if (folder == NULL)
        return 1;
    SHGetPathFromIDList(folder, mw->dir);
    CoTaskMemFree(folder);
    generic_extract((void*)mw);
    return 0;
}

/* select a ROM to open */
static int open_rom (struct MAIN_WIN *mw) {
    TCHAR szFile[MAX_PATH];
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = mw->hwnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT(
    "ROM File\0*.sfc;*.smc;*.gb;*.gba;*.ds;*.nds\0"
    "SNES ROM\0*.sfc;*.smc\0"
    "Game Boy ROM\0*.gb\0"
    "Game Boy Advance ROM\0*.gba\0"
    "DS ROM\0*.ds;*.nds\0"
    "All Files\0*.*\0"
    );
    ofn.nFilterIndex = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle  = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        char *error;
        if ((error = verify_rom(&mw->rom, ofn.lpstrFile)) != NULL) {
            quick_messagebox(error);
            return 1;
        }
    }
    else {
        return 1;
    }
    return 0;
}

LRESULT CALLBACK WndProc (
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (message) {
        case WM_COMMAND: {
            if (lParam
            && ((HIWORD(wParam) == BN_CLICKED)
            ||  (HIWORD(wParam) == EN_CHANGE))
            ) {
                resolve_button_press(&main_win, wParam);
                break;
            }
            else {
                switch (LOWORD(wParam)) {
                    case APP_OPEN: {
                        if (open_rom(&main_win))
                            return 0;
                        erase_window();
                        create_panel(&main_win);
                        break;
                    }
                    case APP_QUIT: {
                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
                    }
                }
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CREATE: {
            create_menu(hwnd);
            break;
        }
        case WM_DESTROY: {
            erase_window();
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return 0;
}

int WINAPI WinMain (
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPSTR     lpCmdLine,
    _In_     int       nCmdShow
) {
    (void)lpCmdLine;
    (void)hPrevInstance;

    memset(&main_win, 0, sizeof(struct MAIN_WIN));

    /* create a class and register it */
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = GetSysColorBrush(COLOR_3DFACE);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = _T("STripper");
    wcex.hIconSm        = LoadIcon(hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        quick_messagebox("Call to RegisterClassEx failed.");
        return 1;
    }

    /* create a window */
    HWND hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW | WS_EX_CONTROLPARENT,
        _T("STripper"),
        _T("STripper"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, /* x,y */
        320, 240, /* w,h */
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hwnd) {
        quick_messagebox("Call to CreateWindow failed.");
        return 1;
    }
    main_win.hwnd = hwnd;
    main_win.hinst = hInstance;

    /* default palette */
    for (int i = 0; i < 4; i++)
        main_win.cn[i].palette = swap_rb(acrCustClr[i]);

    main_win.dir = calloc(MAX_PATH, 2);
    if (main_win.dir == NULL) {
        quick_messagebox("Failed to allocate memory for directory name buffer.");
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* menu focus doesn't work properly if there are no other elements */
    struct PANEL_ITEM item = { CTRL_LABEL, 0, "", NULL };
    struct CTRL_POS pos = { 20, 20, CTRL_WIDTH, CTRL_HEIGHT };
    create_label(&item, &pos, WS_GROUP|WS_TABSTOP);
    SetFocus(ctrl_list[0].instance);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(hwnd, &msg)) {
            TranslateMessage(&msg);
             DispatchMessage(&msg);
        }
    }
    free(main_win.dir);
    return (int)msg.wParam;
}

