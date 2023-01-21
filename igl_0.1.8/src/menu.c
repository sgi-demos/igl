#include "igl.h"
#include "iglcmn.h"
#include <stdarg.h>




#if PLATFORM_WIN32
static unsigned char menuBitmapUnchecked[] =
{
    0x00, 0x07, 0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7,
    0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7,
    0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7,
    0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static unsigned char menuBitmapChecked[] =
{
    0x00, 0x07, 0x7f, 0xf7, 0x7f, 0xf7, 0x7f, 0xb7,
    0x7f, 0x37, 0x6e, 0x37, 0x64, 0x77, 0x60, 0xf7,
    0x71, 0xf7, 0x7b, 0xf7, 0x7f, 0xf7, 0x7f, 0xf7,
    0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
#else
/* bitmaps for checkboxes. fukken X11 reversed bytes/bits, LSB/MSB this... */
static char menuBitmapUnchecked[] =
{
    0xff, 0xff, 0xff, 0xff, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0xff, 0xff, 0xff, 0xff
};

static char menuBitmapChecked[] =
{
    0xff, 0xff, 0xff, 0xff, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc8, 0x03, 0xcc, 0x23, 0xce,
    0x63, 0xc7, 0xe3, 0xc3, 0xc3, 0xc1, 0x83, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0xff, 0xff, 0xff, 0xff
};
#endif




#if PLATFORM_X11
#if 0
____________________________ X11 helpers ____________________________
() {}
#endif
#define MENU_BORDERWIDTH        2       /* width of the border frame */
#define MENU_LRPADDING          12      /* left/right padding of menu item */
#define MENU_VPADDING           5       /* vertical padding of menu item */

static void
_igl_X11createPopupMenu (igl_menuT *menu)
{
    int wflags, fontIdx;
    XSetWindowAttributes swAttribs;
    XGCValues gcv;
    XColor color;
    XFontStruct *fontStruct;
    char *fontNames[] =
    {
        "-*-*-*-i-*-sans-12-*-*-*-*-*-*-*",
        "-*-fixed-*-*-*-*-*-*-*-*-*-*-iso8859-1",
        "-*-*-*-*-*-*-*-*-*-*-*-*-*-*"
    };


    /* menu background color is light gray */
    color.red = color.green = color.blue = 49152;
    XAllocColor (igl->appInstance, DefaultColormap (igl->appInstance, DefaultScreen (igl->appInstance)), &color);

    wflags = CWBackPixel | CWEventMask | CWOverrideRedirect; /* | CWCursor; */
    memset (&swAttribs, 0, sizeof (XSetWindowAttributes));
    swAttribs.background_pixel = color.pixel;
    swAttribs.event_mask = KeyPressMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
        EnterWindowMask | LeaveWindowMask; /* | ExposureMask; */
    swAttribs.override_redirect = True;

    /* create the menu window */
    menu->width = 0;
    menu->height = 0;
    menu->hmenu = XCreateWindow (igl->appInstance, RootWindow (igl->appInstance, igl->visualInfo.screen),
        10, 10, 10, 10, 0, igl->visualInfo.depth, InputOutput, igl->visualInfo.visual, wflags, &swAttribs);

    if (menu->hmenu == IGL_NULLMENU)
    {
        return;
    }


    /* allocate menu GCs */
    /* text */
    wflags = GCFont;

    /*
        now this is utterly stupid. X returns XFontStruct, which we don't want to keep
        in memory, so we free it. but if it's free'd, we have to load the font again.
        if, on the other hand, only XLoadFont() is used to load the font and the font
        doesn't exist, X returns an 'error' and the program quits. aargh, API designers...
    */
    fontIdx = 0;
    while (1)
    {
        fontStruct = XLoadQueryFont (igl->appInstance, fontNames[fontIdx]);
        if (fontStruct != NULL)
        {
            break;
        }
        fontIdx ++;
    }
    XFreeFont (igl->appInstance, fontStruct);

    gcv.font = XLoadFont (igl->appInstance, fontNames[fontIdx]);
    menu->textGC = XCreateGC (igl->appInstance, menu->hmenu, wflags, &gcv);

    wflags = GCForeground;
    /* same as window background_pixel - light gray */
    gcv.foreground = color.pixel;
    menu->backgroundGC = XCreateGC (igl->appInstance, menu->hmenu, wflags, &gcv);

    /* white border */
    gcv.foreground = WhitePixel (igl->appInstance, DefaultScreen (igl->appInstance));
    menu->borderWhiteGC = XCreateGC (igl->appInstance, menu->hmenu, wflags, &gcv);

    /* dark gray border + font for the title */
    wflags |= GCFont;
    color.red = color.green = color.blue = 32768;
    XAllocColor (igl->appInstance, DefaultColormap (igl->appInstance, DefaultScreen (igl->appInstance)), &color);
    gcv.foreground = color.pixel;
    menu->borderGrayGC = XCreateGC (igl->appInstance, menu->hmenu, wflags, &gcv);

    /* allocate pixmaps for (un)checked menu items */
    XGetGCValues (igl->appInstance, menu->backgroundGC, GCForeground, &gcv);
    menu->itemUnchecked = XCreatePixmapFromBitmapData (igl->appInstance, menu->hmenu, menuBitmapUnchecked,
        16, 16, BlackPixel (igl->appInstance, DefaultScreen (igl->appInstance)),
        gcv.foreground, DefaultDepth (igl->appInstance, DefaultScreen (igl->appInstance)));
    menu->itemChecked = XCreatePixmapFromBitmapData (igl->appInstance, menu->hmenu, menuBitmapChecked,
        16, 16, BlackPixel (igl->appInstance, DefaultScreen (igl->appInstance)),
        gcv.foreground, DefaultDepth (igl->appInstance, DefaultScreen (igl->appInstance)));
}


static void
_igl_X11calculatePopupMenuSize (igl_menuT *menu)
{
    int i, maxItemWidth, maxItemHeight, titleWidth, direction, ascent, descent;
    XCharStruct charStruct;
    GContext gcontext;
    igl_menuItemT *item;


    gcontext = XGContextFromGC (menu->textGC);

    /* first pass: calculate max item width and height */
    maxItemWidth = maxItemHeight = 0;
    menu->hasCheckboxes = False;
    if (menu->title[0] != '\0')
    {
        XQueryTextExtents (igl->appInstance, gcontext, menu->title, strlen (menu->title), &direction,
            &ascent, &descent, &charStruct);
        titleWidth = maxItemWidth = charStruct.width;
        maxItemHeight = charStruct.ascent + charStruct.descent;
    }
    for (i=0; i < menu->numItems; i++)
    {
        item = &menu->items[i];

        if (item->title[0] != '\0')
        {
            XQueryTextExtents (igl->appInstance, gcontext, item->title, strlen (item->title), &direction,
                &ascent, &descent, &charStruct);

            if (charStruct.width > maxItemWidth)
            {
                maxItemWidth = charStruct.width;
            }
            if (charStruct.ascent + charStruct.descent > maxItemHeight)
            {
                maxItemHeight = charStruct.ascent + charStruct.descent;
            }

            if (item->submenu != -1)
            {
                maxItemWidth += MENU_LRPADDING;
            }
            /* these are mutually exclusive with submenu and delimiter */
            else if ((item->flags & IGL_MENUITEMFLAGS_UNCHECKED) || (item->flags & IGL_MENUITEMFLAGS_CHECKED))
            {
                menu->hasCheckboxes = True;
            }
        }
    }

    if (maxItemWidth != 0)
    {
        menu->width = maxItemWidth;
    }
    menu->width += 2*MENU_BORDERWIDTH + 2*MENU_LRPADDING + (menu->hasCheckboxes ? 20 : 0);  /* 16 for pixmap + 4 for padding */
    menu->itemHeight = maxItemHeight + 2*MENU_BORDERWIDTH + 2*MENU_VPADDING;  /* 2 for selected item border */


    /* second pass: calculate item positions and menu height */
    menu->height = MENU_BORDERWIDTH;
    if (menu->title[0] != '\0')
    {
        menu->height += menu->itemHeight;
        /* menu title should be centered */
        menu->titlePosX = (int)((menu->width - titleWidth)/2.0f);
    }
    for (i=0; i < menu->numItems; i++)
    {
        item = &menu->items[i];
        item->y = menu->height;
        if (item->title[0] != '\0')
        {
            menu->height += menu->itemHeight;
        }
        else if (item->flags & IGL_MENUITEMFLAGS_DELIMITER)
        {
            menu->height += 2;  /* 1 for shadow */
        }
    }
    menu->height += MENU_BORDERWIDTH;
}


static void
_igl_X11drawPopupMenu (igl_menuT *menu, int selectedItem)
{
    int i, x0, x1, y0, y1;
    igl_menuItemT *item;


    /* 1. draw menu border/frame - 4 white and 4 gray lines for 3D effect */
    y0 = menu->height - 1;
    x1 = menu->width - 1;
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, 0, 0, x1, 0);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, 1, 1, x1-1, 1);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, 0, 0, 0, y0);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, 1, 1, 1, y0-1);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1, 0, x1, y0);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1-1, 1, x1-1, y0-1);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, 0, y0, x1, y0);
    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, 1, y0-1, x1-1, y0-1);

    /* 2. draw menu title */
    if (menu->title[0] != '\0')
    {
        y0 = MENU_BORDERWIDTH + menu->itemHeight - 2*MENU_VPADDING;
        XDrawString (igl->appInstance, menu->hmenu, menu->borderGrayGC, menu->titlePosX, y0, menu->title,
            strlen (menu->title));

        y0 += MENU_VPADDING + 2;
        /* double separator */
        x0 = MENU_BORDERWIDTH+1;
        x1 = menu->width - MENU_BORDERWIDTH - 2;
        XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x0, y0, x1, y0);
        XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x0, y0+1, x1, y0+1);
        XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y0+2, x1, y0+2);
    }
    /* 3. draw menu items */
    for (i=0; i < menu->numItems; i++)
    {
        item = &menu->items[i];
        if (item->title[0] != '\0')
        {
            if (i == selectedItem)
            {
                /* draw a selection frame around our selected item, much like the menu window frame */
                x0 = MENU_BORDERWIDTH + 1;
                x1 = menu->width - MENU_BORDERWIDTH - 2;
                y0 = item->y + 2;
                y1 = y0 + menu->itemHeight - 4;

                XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y0, x1, y0);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0+1, y0+1, x1-1, y0+1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y0, x0, y1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0+1, y0+1, x0+1, y1-1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1, y0, x1, y1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1-1, y0+1, x1-1, y1-1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1, y1, x0, y1);
                XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x1-1, y1-1, x0+1, y1-1);
            }
            else
            {
                XFillRectangle (igl->appInstance, menu->hmenu, menu->backgroundGC, MENU_BORDERWIDTH, item->y,
                    menu->width - MENU_BORDERWIDTH - 2, menu->itemHeight);
            }

            XDrawString (igl->appInstance, menu->hmenu, menu->textGC,
                MENU_BORDERWIDTH + MENU_LRPADDING + (menu->hasCheckboxes ? 20 : 0),
                item->y + menu->itemHeight - MENU_BORDERWIDTH - MENU_VPADDING, item->title, strlen (item->title));

            /* draw the arrow for the submenu */
            if (item->submenu != -1)
            {
                x0 = menu->width - MENU_LRPADDING - 4;
                y0 = item->y + MENU_VPADDING + 4;
                y1 = y0 + 10;

                if (i == selectedItem)
                {
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x0, y0, x0, y1);
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x0, y0, x0 + 7, y0 + 5);
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y1, x0 + 7, y0 + 5);
                }
                else
                {
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y0, x0, y1);
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, x0, y0, x0 + 7, y0 + 5);
                    XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, x0, y1, x0 + 7, y0 + 5);
                }
            }
            else if (item->flags & IGL_MENUITEMFLAGS_UNCHECKED)
            {
                XCopyArea (igl->appInstance, menu->itemUnchecked, menu->hmenu, menu->backgroundGC,
                    0, 0, 16, 16, MENU_LRPADDING, item->y + MENU_VPADDING);
            }
            else if (item->flags & IGL_MENUITEMFLAGS_CHECKED)
            {
                XCopyArea (igl->appInstance, menu->itemChecked, menu->hmenu, menu->backgroundGC,
                    0, 0, 16, 16, MENU_LRPADDING, item->y + MENU_VPADDING);
            }
        }
        else if (item->flags & IGL_MENUITEMFLAGS_DELIMITER)
        {
            XDrawLine (igl->appInstance, menu->hmenu, menu->borderGrayGC, MENU_BORDERWIDTH+1, item->y, menu->width-MENU_BORDERWIDTH-1, item->y);
            XDrawLine (igl->appInstance, menu->hmenu, menu->borderWhiteGC, MENU_BORDERWIDTH+1, item->y+1, menu->width-MENU_BORDERWIDTH-1, item->y+1);
        }
    }
}


static int
_igl_X11getMenuItemId (igl_menuT *menu, int x, int y)
{
    int i;
    igl_menuItemT *item;


    for (i=0; i < menu->numItems; i++)
    {
        item = &menu->items[i];
        if (item->title[0] != '\0' && x >= 0 && x <= menu->width && y >= item->y && y <= (item->y + menu->itemHeight))
        {
            return (i);
        }
    }

    return (-1);
}
#endif


#if 0
____________________________ IGL helpers ____________________________
() {}
#endif
static int
_igl_findMenuPlaceholder ()
{
    int i;


    for (i=0; i < IGL_MAXMENUS; i++)
    {
        if (igl->menus[i].hmenu == IGL_NULLMENU)
        {
            return (i);
        }
    }

    return (-1);
}


static void
_igl_parseMenuItem (char *startPtr, char *endPtr, va_list mlist, igl_menuT *menu, igl_menuItemT *item)
{
    int i;
    char *s;


    /* item 'title' needs special handling as it's not marked with %t as menu title is */
    if (item != NULL)
    {
        strncpy (item->title, startPtr, 255);
        s = strchr (item->title, '%');
        if (s != NULL)
        {
            *s = '\0';
            if (s-1 > item->title && *(s-1) == ' ')
            {
                *(s-1) = '\0';
            }
        }

        item->submenu = -1;
    }

    i = 0;
    s = startPtr;
    while (s != NULL && (s+1 <= endPtr))
    {
        if (*s != '%')
        {
            if (s+1 > endPtr)
            {
                /* end-of-string */
                break;
            }

            s ++;
            continue;
        }

        if (s+1 > endPtr)
        {
            /* end-of-string */
            break;
        }
        s ++;


        switch (*s)
        {
            case 't':
#if 1
                strcpy (menu->title, "<- ");
                strncat (menu->title, startPtr, s-startPtr-2);
                strcat (menu->title, " ->");
#else
                strncpy (menu->title, startPtr, s-startPtr-2);
#endif
                break;

            case 'F':
                menu->routine = (int(*)(int, ...))va_arg (mlist, void *);
                break;

            case 'f':
                if (item != NULL)
                {
                    item->routine = (int(*)(int, ...))va_arg (mlist, void *);
                }
                break;

            case 'l':
                if (item != NULL)
                {
                    item->flags |= IGL_MENUITEMFLAGS_DELIMITER;
                }
                break;

            case 'm':
                if (item != NULL)
                {
                    item->submenu = (long)va_arg (mlist, long);
                    if (item->submenu < 0 || item->submenu >= IGL_MAXMENUS || igl->menus[item->submenu].hmenu == IGL_NULLMENU)
                    {
                        /* invalid menu specified */
                        item->submenu = -1;
                    }
                }
                break;

            case 'n':
                if (item != NULL)
                {
                    item->dontExecuteRoutine = 1;
                }
                break;

            case 'x':
                if (item != NULL && isdigit(*(s+1)))
                {
                    item->id = atoi (s+1);
                    if (item->id <= 0)
                    {
                        item->id = -1;
                    }
                }
                break;
        }

        s ++;
    }
}


static int
_igl_parseMenu (int pup, String str, va_list args)
{
    int i, j, id;
    char *startPtr, *endPtr, tmpStr[256];
    igl_menuT *menu;
    igl_menuItemT *item;


    /* if pup is specified, we're called from addtopup(), else from defpup() */
    if (pup >= 0 && pup < IGL_MAXMENUS)
    {
        id = pup;
    }
    else
    {
        id = _igl_findMenuPlaceholder ();
        if (id == -1)
        {
            return (-1);
        }
        menu = &igl->menus[id];
        menu->numItems = 0;
    }


    strncpy (tmpStr, str, 255);

    /*
        chop menu defines into pieces
        TBD: if we have a single entry, the code below doesn't work (strtok() = NULL)
    */
    startPtr = strtok (tmpStr, "|");
    endPtr = startPtr+strlen(startPtr)-1;

    /* parse menu and items */
    i = -1;
    item = NULL;
    while (startPtr != NULL)
    {
        _igl_parseMenuItem (startPtr, endPtr, args, menu, item);

        startPtr = strtok (NULL, "|");
        if (startPtr == NULL)
        {
            break;
        }
        endPtr = startPtr+strlen(startPtr)-1;

        if (i+1 >= IGL_MAXMENUITEMS)
        {
            break;
        }

        menu->numItems ++;

        i ++;
        item = &menu->items[i];
    }


#if PLATFORM_WIN32
    if (id != pup) /* if (id == pup) we were called from addtopup() */
    {
        /* create the menu w/items if any */
        menu->hmenu = CreatePopupMenu ();
        igl->menus[id].itemUnchecked = CreateBitmap (16, 16, 1, 1, menuBitmapUnchecked);;
        igl->menus[id].itemChecked = CreateBitmap (16, 16, 1, 1, menuBitmapChecked);
    }

    if (menu->title[0] != '\0')
    {
        AppendMenu (menu->hmenu, MF_STRING | MF_GRAYED, 0, menu->title);
        AppendMenu (menu->hmenu, MF_SEPARATOR, 0, NULL);
    }

    for (j=0; j < menu->numItems; j++)
    {
        if (menu->items[j].title[0] != '\0')
        {
            i = menu->items[j].submenu;

            if (i != -1 && igl->menus[i].hmenu != IGL_NULLMENU)
            {
                AppendMenu (menu->hmenu, MF_STRING | MF_POPUP, (UINT_PTR)igl->menus[i].hmenu,
                    menu->items[j].title);
            }
            else
            {
                i = menu->items[j].id;
                /*
                    menuitem ID is i+1/j+1 because TrackPopupMenu() returns 0 if nothing was selected,
                    so we can't have an ID of zero value
                */
                AppendMenu (menu->hmenu, MF_STRING, i > 0 ? i+1 : j+1, menu->items[j].title);
            }
        }
        else if (menu->items[j].flags & IGL_MENUITEMFLAGS_DELIMITER)
        {
            AppendMenu (menu->hmenu, MF_SEPARATOR, 0, NULL);
        }
    }
#else
    if (id != pup) /* if (id == pup) we were called from addtopup() */
    {
        /* create the menu w/items if any */
        _igl_X11createPopupMenu (menu);
    }

    _igl_X11calculatePopupMenuSize (menu);
#endif

    return (id);
}


#if 0
____________________________ menu functions ____________________________
() {}
#endif
/* DESC: defpup - defines a menu */
long
defpup (String str, ...)
{
    int id;
    va_list mlist;


    IGL_CHECKINIT (-1);

    va_start (mlist, str);
    id = _igl_parseMenu (-1, str, mlist);
    va_end (mlist);

    return (id);
}


/* DESC: newpup - allocates and initializes a structure for a new menu */
long
newpup ()
{
    long id;


    IGL_CHECKINIT (-1);

    id = _igl_findMenuPlaceholder ();
    if (id != -1)
    {
#if PLATFORM_WIN32
        igl->menus[id].hmenu = CreatePopupMenu ();
        igl->menus[id].itemUnchecked = CreateBitmap (16, 16, 1, 1, menuBitmapUnchecked);
        igl->menus[id].itemChecked = CreateBitmap (16, 16, 1, 1, menuBitmapChecked);
#else
        _igl_X11createPopupMenu (&igl->menus[id]);
#endif
    }

    return (id);
}


/* DESC: addtopup - adds items to an existing pop-up menu */
void
addtopup (long pup, String str, ...)
{
    va_list mlist;


    IGL_CHECKINITV ();
    if (pup < 0 || pup >= IGL_MAXMENUS || igl->menus[pup].hmenu == IGL_NULLMENU)
    {
        return;
    }

    /* TBD: check if the menu was created with defpup() or newpup() */
    va_start (mlist, str);
    _igl_parseMenu (pup, str, mlist);
    va_end (mlist);
}


/* DESC: dopup - displays the specified pop-up menu */
long
dopup (long pup)
{
    long ret = -1, id;
    igl_menuT *menu;
    igl_menuItemT *item;
#if PLATFORM_WIN32
    POINT p;
#else
    XEvent event;
    Cursor cursor;
    /* <sarcasm>luckily newer compilers provide about a meg of stack.</sarcasm> */
    int i, mousex, mousey, width, height, border_width, depth, rx, ry, x, y, selectedItem, newSelectedItem;
    Boolean processMenu;
    Window rootWnd, childWnd;
    unsigned int maskRet;
    igl_menuT *currentMenu, *childMenu;
#endif

    IGL_CHECKINIT (-1);
    if (pup < 0 || pup >= IGL_MAXMENUS || igl->menus[pup].hmenu == IGL_NULLMENU)
    {
        return (-1);
    }

    menu = &igl->menus[pup];

#if PLATFORM_WIN32
    GetCursorPos (&p);

    id = TrackPopupMenu (menu->hmenu, TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD | TPM_NONOTIFY,
            p.x, p.y, 0, IGL_CTX()->wnd, NULL);
    id --;
#else
    /* grab the mouse pointer */
    cursor = XCreateFontCursor (igl->appInstance, XC_arrow);
    if (XGrabPointer (igl->appInstance, RootWindow (igl->appInstance, igl->visualInfo.screen),
            True, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
            RootWindow (igl->appInstance, igl->visualInfo.screen), cursor, CurrentTime) != GrabSuccess)
    {
        /* the cursor is grabbed by someone else, so bail out */
        /* TBD: how do we destroy the cursor? */
        return;
    }


    /* show the topmost menu window */
    XQueryPointer (igl->appInstance, menu->hmenu, &rootWnd, &childWnd, &mousex, &mousey, &x, &y, &maskRet);
    XMoveResizeWindow (igl->appInstance, menu->hmenu, mousex, mousey, menu->width, menu->height);
    XMapRaised (igl->appInstance, menu->hmenu);
    _igl_X11drawPopupMenu (menu, -1);

    /* event processing loop */
    selectedItem = -1;
    processMenu = True;
    currentMenu = menu;
    while (processMenu)
    {
        if (XQueryPointer (igl->appInstance, currentMenu->hmenu, &rootWnd, &childWnd, &rx, &ry, &x, &y, &maskRet) &&
            (rx != mousex || ry != mousey))
        {
            /* if mouse moved, update everything necessary on the screen */
            mousex = rx;
            mousey = ry;

            newSelectedItem = _igl_X11getMenuItemId (currentMenu, x, y);
            if (newSelectedItem != selectedItem)
            {
                /* if previous selected item had a submenu, unmap/hide it */
                if (selectedItem != -1 && currentMenu->items[selectedItem].submenu != -1)
                {
                    childMenu = &igl->menus[currentMenu->items[selectedItem].submenu];
                    XUnmapWindow (igl->appInstance, childMenu->hmenu);
                }
                /* if new selected item has a submenu, map/show it */
                if (newSelectedItem != -1 && currentMenu->items[newSelectedItem].submenu != -1)
                {
                    childMenu = &igl->menus[currentMenu->items[newSelectedItem].submenu];
                    /*
                        yes, one must love API calls with at least 5 params. especially since none can
                        be NULL or we dump core. X11 must be a cult of non-NULL worshipers or something.
                        no, they don't redirect to /dev/null either... =)
                    */
                    XGetGeometry (igl->appInstance, currentMenu->hmenu, &rootWnd, &rx, &ry, &width, &height,
                        &border_width, &depth);

                    /* position the 'child' menu beside parent's selected item and draw it */
                    XMoveResizeWindow (igl->appInstance, childMenu->hmenu, rx + currentMenu->width,
                        ry + currentMenu->items[newSelectedItem].y, childMenu->width, childMenu->height);
                    XMapWindow (igl->appInstance, childMenu->hmenu);
                    _igl_X11drawPopupMenu (childMenu, -1);
                }

                selectedItem = newSelectedItem;
                _igl_X11drawPopupMenu (currentMenu, selectedItem);
            }
        }

        
        XNextEvent (igl->appInstance, &event);
        switch (event.type)
        {
            case KeyPress:
            case KeyRelease:
            break;

/*
    XQueryPointer() seems to be in sync only when a button is pressed,
    hence we use ButtonRelease and track the menu only when a button is held!
*/
#if 0
            case ButtonPress:
#else
            case ButtonRelease:
#endif
                id = _igl_X11getMenuItemId (currentMenu, event.xbutton.x, event.xbutton.y);
                /* hmm... if we release the mouse over an item with a submenu, should we return its id? */
                if (id != -1 && currentMenu->items[id].submenu != -1)
                {
                    id = -1;
                }
                processMenu = False;
            break;

            case EnterNotify:
                if (event.xcrossing.window != None)
                {
                    for (i=0; i < IGL_MAXMENUS; i++)
                    {
                        if (igl->menus[i].hmenu == event.xcrossing.window)
                        {
                            currentMenu = &igl->menus[i];
                            selectedItem = -1;
                            break;
                        }
                    }
                }
            break;

            case LeaveNotify:
                selectedItem = -1;
            break;

            default:
            break;
        }
    }

    /* release the mouse pointer and hide the menu window (plus any children!) */
    XUngrabPointer (igl->appInstance, CurrentTime);
    for (i=0; i < IGL_MAXMENUS; i++)
    {
        if (igl->menus[i].hmenu != IGL_NULLMENU)
        {
            XUnmapWindow (igl->appInstance, igl->menus[i].hmenu);
        }
    }

    /* TBD: the code below relies on menu rather than currentMenu (because of Win32) */
    menu = currentMenu;
#endif

    if (id < 0 || id >= IGL_MAXMENUITEMS)
    {
        return (-1);
    }


    item = &menu->items[id];

    /* 1. menu item has a routine() defined -> call it */
    if (item->routine != NULL)
    {
        ret = (*item->routine)(id);
    }
    else
    {
        if (item->id > 0)
        {
            ret = item->id;
        }
        else
        {
            ret = id;
        }
    }

    /* 2. menu has a routine() defined -> call it, unless the item should not execute it */
    /* NOTE: if 1. is true, return value is passed as an argument to this routine */
    if (menu->routine != NULL && !item->dontExecuteRoutine)
    {
        ret = (*menu->routine)(ret);
    }
    else
    {
        if (item->id > 0)
        {
            ret = item->id;
        }
        else
        {
            ret = id;
        }
    }


    return (ret);
}


/* DESC: freepup - deallocates a menu */
void
freepup (long pup)
{
    IGL_CHECKINITV ();
    if (pup < 0 || pup >= IGL_MAXMENUS || igl->menus[pup].hmenu == IGL_NULLMENU)
    {
        return;
    }

    /* TBD: if pup is a submenu, update its 'parent', too!!! */
#if PLATFORM_WIN32
    DeleteObject (igl->menus[pup].itemUnchecked);
    DeleteObject (igl->menus[pup].itemChecked);
    DestroyMenu (igl->menus[pup].hmenu);
#else
    XFreeGC (igl->appInstance, igl->menus[pup].textGC);
    XFreeGC (igl->appInstance, igl->menus[pup].backgroundGC);
    XFreeGC (igl->appInstance, igl->menus[pup].borderWhiteGC);
    XFreeGC (igl->appInstance, igl->menus[pup].borderGrayGC);
    XFreePixmap (igl->appInstance, igl->menus[pup].itemUnchecked);
    XFreePixmap (igl->appInstance, igl->menus[pup].itemChecked);
    XDestroyWindow (igl->appInstance, igl->menus[pup].hmenu);
#endif
    memset (&igl->menus[pup], 0, sizeof(igl_menuT));
}


void
setpup (long pup, long entry, unsigned long mode)
{
    igl_menuT *menu;
    igl_menuItemT *item;
#if PLATFORM_WIN32
    int i, win32_itemOffset, igl_itemOffset;
    unsigned int menuState;
#else
    Boolean recalculateMenu;
#endif


    IGL_CHECKINITV ();
    if (pup < 0 || pup >= IGL_MAXMENUS || igl->menus[pup].hmenu == IGL_NULLMENU)
    {
        return;
    }

    menu = &igl->menus[pup];
    if (entry < 0 || entry > menu->numItems)
    {
        return;
    }

    item = &menu->items[entry];
    /* currently we can only change items with titles which can't have submenus dunno about SGI though */
    if (item->title[0] == '\0' || item->submenu != -1)
    {
        return;
    }


#if PLATFORM_WIN32
    /* find the entry-th menu item and calculate Win32 item offset. delimiters and menu titles do not count */
    igl_itemOffset = 0;
    win32_itemOffset = (menu->title[0] == '\0' ? 0 : 2);
    for (i=0; i < menu->numItems; i++)
    {
        if (igl_itemOffset == entry)
        {
            break;
        }

        if (menu->items[i].title[0] != '\0')
        {
            igl_itemOffset ++;
        }
        win32_itemOffset ++;
    }


    menuState = GetMenuState (menu->hmenu, win32_itemOffset, MF_BYPOSITION);
    switch (mode)
    {
        case PUP_NONE:
            EnableMenuItem (menu->hmenu, win32_itemOffset, MF_BYPOSITION);
            CheckMenuItem (menu->hmenu, win32_itemOffset, MF_BYPOSITION | MF_UNCHECKED);
            SetMenuItemBitmaps (menu->hmenu, win32_itemOffset, MF_BYPOSITION, NULL, NULL);
        break;

        case PUP_GREY:
            if (!(menuState & MF_GRAYED))
            {
                EnableMenuItem (menu->hmenu, win32_itemOffset, MF_BYPOSITION | MF_GRAYED);
            }
        break;

        case PUP_BOX:
        case PUP_CHECK:
            SetMenuItemBitmaps (menu->hmenu, win32_itemOffset, MF_BYPOSITION,
                menu->itemUnchecked, menu->itemChecked);
            CheckMenuItem (menu->hmenu, win32_itemOffset,
                MF_BYPOSITION | (mode == PUP_BOX ? MF_UNCHECKED : MF_CHECKED));
        break;
    }
#else
    recalculateMenu = False;
    switch (mode)
    {
        case PUP_NONE:
            if ((item->flags & IGL_MENUITEMFLAGS_UNCHECKED) || (item->flags & IGL_MENUITEMFLAGS_CHECKED))
            {
                recalculateMenu = True;
            }
            item->flags &= ~IGL_MENUITEMFLAGS_UNCHECKED;
            item->flags &= ~IGL_MENUITEMFLAGS_CHECKED;
        break;

        case PUP_GREY:
            item->flags |= IGL_MENUITEMFLAGS_GRAYED;
        break;

        case PUP_BOX:
            if (!(item->flags & IGL_MENUITEMFLAGS_UNCHECKED) && !(item->flags & IGL_MENUITEMFLAGS_CHECKED))
            {
                recalculateMenu = True;
            }
            item->flags &= ~IGL_MENUITEMFLAGS_CHECKED;
            item->flags |= IGL_MENUITEMFLAGS_UNCHECKED;
        break;

        case PUP_CHECK:
            if (!(item->flags & IGL_MENUITEMFLAGS_UNCHECKED) && !(item->flags & IGL_MENUITEMFLAGS_CHECKED))
            {
                recalculateMenu = True;
            }
            item->flags &= ~IGL_MENUITEMFLAGS_UNCHECKED;
            item->flags |= IGL_MENUITEMFLAGS_CHECKED;
        break;
    }

    if (recalculateMenu)
    {
        _igl_X11calculatePopupMenuSize (menu);
    }
#endif
}


/* DESC: pupmode, endpupmode - obsolete routines */
void
pupmode ()
{
}


/* DESC: pupmode, endpupmode - obsolete routines */
void
endpupmode ()
{
}


/* DESC: ??? */
void
pupcolor (long clr)
{
}



