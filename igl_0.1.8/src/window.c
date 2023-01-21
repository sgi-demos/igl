#include "igl.h"
#include "iglcmn.h"




#if PLATFORM_WIN32
WGLSWAPINTERVALEXT wglSwapIntervalEXT;
WGLGETSWAPINTERVALEXT wglGetSwapIntervalEXT;

LRESULT CALLBACK
igl_Win32processEvents (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif


#if 0
____________________________ helpers ____________________________
() {}
#endif
/* find a space for a new window in igl->openWindows. returns index (>= 0) if ok, -1 on error */
static int
_igl_findWindowPlaceholder ()
{
    int i;


    for (i=0; i < IGL_MAXWINDOWS; i++)
    {
        if (igl->openWindows[i].wnd == IGL_NULLWND)
        {
            return (i);
        }
    }

    return (-1);
}


/* initializes text/character properties of a window for use with lcharstr()/charstr() */
static void
_igl_textInit (igl_windowT *wptr)
{
#if PLATFORM_WIN32
    HFONT oldFont;


    wptr->fontBase = glGenLists (96);
    oldFont = (HFONT)SelectObject (wptr->hdc, GetStockObject (SYSTEM_FONT));
    wglUseFontBitmaps (wptr->hdc, 32, 96, wptr->fontBase);
    SelectObject (wptr->hdc, oldFont);
#else
    Font font;


    font = XLoadFont (igl->appInstance, "-*-fixed-*-*-*-*-*-*-*-*-*-*-iso8859-1");
    wptr->fontBase = glGenLists (96);
    glXUseXFont (font, 32, 96, wptr->fontBase);
#endif
}


static long
_igl_createWindow (char *windowTitle, long parentId)
{
    int idx, format, wflags;
    igl_windowT *wptr, *pptr;
#if PLATFORM_WIN32
    WNDCLASS wc;
    PIXELFORMATDESCRIPTOR pfd;
#else
    XSetWindowAttributes swAttribs;
    XGCValues gcv;
    XSizeHints sizeHints;
#endif


    if (igl->appInstance == NULL)
    {
        return (-1);
    }

    idx = _igl_findWindowPlaceholder ();
    if (idx == -1)
    {
        return (-1);
    }
    igl->currentWindow = idx;
    wptr = &igl->openWindows[idx];

    pptr = (parentId != -1) ? &igl->openWindows[parentId] : NULL;


    /* init + set default window params */
    memset (wptr, 0, sizeof (igl_windowT));
    wptr->width = igl->winWidth;
    wptr->height = igl->winHeight;
    wptr->aspectX = igl->winAspectX;
    wptr->aspectY = igl->winAspectY;
    wptr->acPlanes = 64;    /* Win32 default i guess */
    wptr->stenPlanes = 0;   /* IrisGL default */
    wptr->zbPlanes = 24;    /* IrisGL default */
    wptr->rgbPlanes = 32;

    if (pptr == NULL)
    {
        wptr->x = igl->winPosx;
        wptr->y = igl->winPosy;
        strncpy (wptr->title, windowTitle, 255);
    }
    else
    {
        /* TBD: position beside the parent window! */
        wptr->x = igl->winPosx;
        wptr->y = igl->winPosy;
        strcpy (wptr->title, pptr->title);
        wptr->parentWnd = pptr->wnd;
    }


#if PLATFORM_WIN32
    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = igl_Win32processEvents;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = igl->appInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = IGL_WINDOWCLASS;
    RegisterClass (&wc);

    /*
        create the window
        NOTE: window size and position will be set upon calling winconstraints()
        if prefposition() was not called, default position/size will be used
    */
    wflags = WS_POPUPWINDOW;
    if (igl->windowFlags & IGL_WFLAGS_NOPORT)
    {
        /* reset for each window */
        wptr->flags |= IGL_WFLAGS_NOPORT;
        igl->windowFlags &= ~IGL_WFLAGS_NOPORT;
    }
    else
    {
        wflags |= WS_VISIBLE;
    }

    if (pptr != NULL || (igl->windowFlags & IGL_WFLAGS_NOBORDER))
    {
        /* if it's a swinopen() window OR noborder() was called, the window will not have a border */
        wptr->flags |= IGL_WFLAGS_NOBORDER;
        if (pptr == NULL)
        {
            igl->windowFlags &= ~IGL_WFLAGS_NOBORDER;   /* reset for each window */
        }
    }
    else
    {
        wflags |= WS_CAPTION;
    }

    wptr->wnd = CreateWindow (IGL_WINDOWCLASS, wptr->title, wflags,
        wptr->x, wptr->y, wptr->width, wptr->height, NULL, NULL, igl->appInstance, NULL);
    if (wptr->wnd == IGL_NULLWND)
    {
        return (-1);
    }

    /*
        when Win32 creates the window, the borders/caption are calculated into the window size,
        so we have to adjust it by calculating the client area/rectangle.
    */
#   if 0
    if (wptr->aspectX != 0 && wptr->aspectY != 0)
    {
        /* adjust the size according to aspect ratio */
        if (wptr->aspectX < wptr->aspectY)
        {
            SetWindowPos (wptr->wnd, NULL, 0, 0,
                (wptr->width + igl->winOffsetX)*wptr->aspectX/wptr->aspectY,
                wptr->height + igl->winOffsetY, SWP_NOZORDER | SWP_NOMOVE);
        }
        else
        {
            SetWindowPos (wptr->wnd, NULL, 0, 0, wptr->width + igl->winOffsetX,
                (wptr->height + igl->winOffsetY)*wptr->aspectY/wptr->aspectX, SWP_NOZORDER | SWP_NOMOVE);
        }
    }
    else
#   endif
    {
        SetWindowPos (wptr->wnd, NULL, 0, 0, wptr->width + igl->winOffsetX,
            wptr->height + igl->winOffsetY, SWP_NOZORDER | SWP_NOMOVE);
    }


    wptr->hdc = GetDC (wptr->wnd);
    memset (&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof (PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_LAYER_BUFFERS;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = wptr->rgbPlanes;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cAccumBits = wptr->acPlanes;
    pfd.cStencilBits = wptr->stenPlanes;
    pfd.cDepthBits = wptr->zbPlanes;
    pfd.bReserved = 49; /* = 0x31 = 00110001 = 1 underlay + 3 overlays */
    format = ChoosePixelFormat (wptr->hdc, &pfd);
    SetPixelFormat (wptr->hdc, format, &pfd);

    /* correct wptr's settings if pixel format differs */
    DescribePixelFormat (wptr->hdc, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    wptr->rgbPlanes = pfd.cColorBits;
    wptr->acPlanes = pfd.cAccumBits;
    wptr->stenPlanes = pfd.cStencilBits;
    wptr->zbPlanes = pfd.cDepthBits;

    /* create and enable the rendering contexts (RCs) for main and under/overlay planes */
    wptr->hrcs[IGL_WLAYER_NORMALDRAW] = wglCreateLayerContext (wptr->hdc, 0);
    wptr->hrcs[IGL_WLAYER_UNDERDRAW] = wglCreateLayerContext (wptr->hdc, -1);
    wptr->hrcs[IGL_WLAYER_OVERDRAW] = wglCreateLayerContext (wptr->hdc, 1);
    wptr->hrcs[IGL_WLAYER_PUPDRAW] = wglCreateLayerContext (wptr->hdc, 2);
    wptr->hrcs[IGL_WLAYER_CURSORDRAW] = wglCreateLayerContext (wptr->hdc, 3);

    /* main plane is the default */
    wglMakeCurrent (wptr->hdc, wptr->hrcs[IGL_WLAYER_NORMALDRAW]);
    SetActiveWindow (wptr->wnd);

#else   /* PLATFORM_WIN32 */
    memset (&swAttribs, 0, sizeof (XSetWindowAttributes));
    swAttribs.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask |
        StructureNotifyMask | FocusChangeMask | PointerMotionMask | PointerMotionHintMask | ButtonMotionMask;
    wflags = CWEventMask;
    wptr->wnd = XCreateWindow (igl->appInstance,
        wptr->parentWnd != IGL_NULLWND ? wptr->parentWnd : RootWindow (igl->appInstance, igl->visualInfo.screen),
        wptr->x, wptr->y, wptr->width, wptr->height,
        0, igl->visualInfo.depth, InputOutput, igl->visualInfo.visual, wflags, &swAttribs);
    if (wptr->wnd == IGL_NULLWND)
    {
        return (-1);
    }

    /* windows aren't resizeable until winconstraints() is called */
    sizeHints.flags = PMinSize | PMaxSize;
    sizeHints.min_width = sizeHints.max_width = wptr->width;
    sizeHints.min_height = sizeHints.max_height = wptr->height;

    /* keepaspect() affects winopen() and winconstraints() */
    if (wptr->aspectX != 0 && wptr->aspectY != 0)
    {
        sizeHints.flags |= PAspect;
        sizeHints.min_aspect.x = sizeHints.max_aspect.x = wptr->aspectX;
        sizeHints.min_aspect.y = sizeHints.max_aspect.y = wptr->aspectY;
    }

    XSetWMNormalHints (igl->appInstance, wptr->wnd, &sizeHints);


    wptr->hrcs[IGL_WLAYER_NORMALDRAW] = glXCreateContext (igl->appInstance, &igl->visualInfo, NULL, True);
#if 0   // TBD
    wptr->hrcs[IGL_WLAYER_UNDERDRAW] = glXCreateContext (igl->appInstance, <visualInfo1>, NULL, TRUE);
    wptr->hrcs[IGL_WLAYER_OVERDRAW] = glXCreateContext (igl->appInstance, <visualInfo2>, NULL, TRUE);
    wptr->hrcs[IGL_WLAYER_PUPDRAW] = glXCreateContext (igl->appInstance, <visualInfo3>, NULL, TRUE);
    wptr->hrcs[IGL_WLAYER_CURSORDRAW] = glXCreateContext (igl->appInstance, <visualInfo4>, NULL, TRUE);
#endif
    if (wptr->hrcs[IGL_WLAYER_NORMALDRAW] == NULL)
    {
        return (-1);
    }

    wflags = GCGraphicsExposures;
    gcv.graphics_exposures = False;
    wptr->hdc = XCreateGC (igl->appInstance, wptr->wnd, wflags, &gcv);
    if (wptr->hdc == NULL)
    {
        return (-1);
    }

    XStoreName (igl->appInstance, wptr->wnd, wptr->title);
    if (igl->windowFlags & IGL_WFLAGS_NOPORT)
    {
        /* reset for each window */
        wptr->flags |= IGL_WFLAGS_NOPORT;
        igl->windowFlags &= ~IGL_WFLAGS_NOPORT;
    }
    else
    {
        XMapWindow (igl->appInstance, wptr->wnd);
    }
    glXMakeCurrent (igl->appInstance, wptr->wnd, wptr->hrcs[IGL_WLAYER_NORMALDRAW]);
#endif

    /* OpenGL heaven begins... */
    glMatrixMode (GL_PROJECTION);
    _igl_textInit (wptr);

    /* grab OpenGL extensions if present */
    /* NOTE: we have to do it here, since we don't have OpenGL in WinMain() yet! */
    if (!igl->initialized)
    {
#if PLATFORM_WIN32      // TBD
        wglSwapIntervalEXT = (WGLSWAPINTERVALEXT)wglGetProcAddress ("wglSwapIntervalEXT");
        wglGetSwapIntervalEXT = (WGLGETSWAPINTERVALEXT)wglGetProcAddress ("wglGetSwapIntervalEXT");
#else
#endif
        igl->initialized = TRUE;
    }


    return (idx);
}


/* find ID of the window by HWND */
static short
_igl_findWindowByHandle (HWND wnd)
{
    short i;


    if (wnd != IGL_NULLWND)
    {
        for (i=0; i < IGL_MAXWINDOWS; i++)
        {
            if (igl->openWindows[i].wnd == wnd)
            {
                return (i);
            }
        }
    }

    return (-1);
}


#if PLATFORM_WIN32
#if 0
____________________________ WIN32 stuph ____________________________
() {}
#endif
/* convert Win32 msgs (key/mouse events) to IrisGL devices */
#define KEYTR(_key) case #@_key: ret=_key##KEY; break
static int
_igl_Win32deviceToIris (UINT msg, WPARAM wp, short *state)
{
    int ret = -1;


    if (msg == WM_KEYDOWN || msg == WM_KEYUP)
    {
        *state = (msg == WM_KEYDOWN);
        switch (wp)
        {
            KEYTR(A);
            KEYTR(B);
            KEYTR(C);
            KEYTR(D);
            KEYTR(E);
            KEYTR(F);
            KEYTR(G);
            KEYTR(H);
            KEYTR(I);
            KEYTR(J);
            KEYTR(K);
            KEYTR(L);
            KEYTR(M);
            KEYTR(N);
            KEYTR(O);
            KEYTR(P);
            KEYTR(Q);
            KEYTR(R);
            KEYTR(S);
            KEYTR(T);
            KEYTR(U);
            KEYTR(V);
            KEYTR(W);
            KEYTR(X);
            KEYTR(Y);
            KEYTR(Z);

            case '0': ret = ZEROKEY; break;
            case '1': ret = ONEKEY; break;
            case '2': ret = TWOKEY; break;
            case '3': ret = THREEKEY; break;
            case '4': ret = FOURKEY; break;
            case '5': ret = FIVEKEY; break;
            case '6': ret = SIXKEY; break;
            case '7': ret = SEVENKEY; break;
            case '8': ret = EIGHTKEY; break;
            case '9': ret = NINEKEY; break;

            case VK_NUMPAD0: ret = PAD0; break;
            case VK_NUMPAD1: ret = PAD1; break;
            case VK_NUMPAD2: ret = PAD2; break;
            case VK_NUMPAD3: ret = PAD3; break;
            case VK_NUMPAD4: ret = PAD4; break;
            case VK_NUMPAD5: ret = PAD5; break;
            case VK_NUMPAD6: ret = PAD6; break;
            case VK_NUMPAD7: ret = PAD7; break;
            case VK_NUMPAD8: ret = PAD8; break;
            case VK_NUMPAD9: ret = PAD9; break;
            case VK_DECIMAL: ret = PADPERIOD; break;
            case VK_SUBTRACT: ret = PADMINUS; break;

            case VK_F1: ret = F1KEY; break;
            case VK_F2: ret = F2KEY; break;
            case VK_F3: ret = F3KEY; break;
            case VK_F4: ret = F4KEY; break;
            case VK_F5: ret = F5KEY; break;
            case VK_F6: ret = F6KEY; break;
            case VK_F7: ret = F7KEY; break;
            case VK_F8: ret = F8KEY; break;
            case VK_F9: ret = F9KEY; break;
            case VK_F10: ret = F10KEY; break;
            case VK_F11: ret = F11KEY; break;
            case VK_F12: ret = F12KEY; break;

            case VK_ESCAPE: ret = ESCKEY; break;
            case VK_TAB: ret = TABKEY; break;
            case VK_RETURN: ret = RETKEY; break; /* should it set PADENTER, too? */
            case VK_SPACE: ret = SPACEKEY; break;
            case VK_BACK: ret = BACKSPACEKEY; break;
            case VK_LEFT: ret = LEFTARROWKEY; break;
            case VK_DOWN: ret = DOWNARROWKEY; break;
            case VK_RIGHT: ret = RIGHTARROWKEY; break;
            case VK_UP: ret = UPARROWKEY; break;
            case VK_PRINT: ret = PRINTSCREENKEY; break;
            case VK_SCROLL: ret = SCROLLLOCKKEY; break;
            case VK_PAUSE: ret = PAUSEKEY; break;
            case VK_INSERT: ret = INSERTKEY; break;
            case VK_HOME: ret = HOMEKEY; break;
            case VK_END: ret = ENDKEY; break;
            case VK_NUMLOCK: ret = NUMLOCKKEY; break;

            /* mouse buttons */
            case VK_LBUTTON: ret = LEFTMOUSE; break;
            case VK_RBUTTON: ret = RIGHTMOUSE; break;
            case VK_MBUTTON: ret = MIDDLEMOUSE; break;
        }
    }
    else if (msg == WM_LBUTTONUP || msg == WM_LBUTTONDOWN)
    {
        *state = (msg == WM_LBUTTONDOWN);
        ret = LEFTMOUSE;
    }
    else if (msg == WM_RBUTTONUP || msg == WM_RBUTTONDOWN)
    {
        *state = (msg == WM_RBUTTONDOWN);
        ret = RIGHTMOUSE;
    }
    else if (msg == WM_MBUTTONUP || msg == WM_MBUTTONDOWN)
    {
        *state = (msg == WM_MBUTTONDOWN);
        ret = MIDDLEMOUSE;
    }


    return (ret);
}
#undef KEYTR


/*
    process WM_KEY/MOUSE events and place them in appropriate device queues.
    events are removed once user chooses to process them (getvaluator()/getbutton()/getdev()/qread())
*/
static void
_igl_Win32processDevice (UINT msg, WPARAM wp, LPARAM lp)
{
    int dev, devTie;
    short state;
    POINT p;


    /* refresh the mouse position for 'internal' purposes =) */
    GetCursorPos (&p);
    igl->devices[MOUSEX] = p.x;
    igl->devices[MOUSEY] = p.y;


    dev = _igl_Win32deviceToIris (msg, wp, &state);
    /* normal devices */
    if (dev != -1)
    {
        /* if device is queued, we have to qenter() it */
        if (igl->deviceQueue[dev])
        {
            /* NOTE: state indicates whether a device is 'up' or 'down' */
            qenter ((Device)dev, state);

            /* if it's a KEYBD device and it's 'down', qenter KEYBD as well */
            if (ISKEYBD(dev) && state == 1 && igl->deviceQueue[KEYBD])
            {
                qenter (KEYBD, (short)dev);
            }

            /* if device is tied, we have to qenter() its ties */
            devTie = igl->tiedValuators[dev*2];
            if (devTie != NULLDEV)
            {
                qenter ((Device)devTie, (short)igl->devices[devTie]);

                devTie = igl->tiedValuators[dev*2+1];
                if (devTie != NULLDEV)
                {
                    qenter ((Device)devTie, (short)igl->devices[devTie]);
                }
            }
        }
    }
    /* 'abnormal' devices */
    else
    {
        /* this one has two connected devices - MOUSEX and MOUSEY */
        if (msg == WM_MOUSEMOVE)
        {
            if (igl->deviceQueue[MOUSEX])
            {
                qenter (MOUSEX, (short)igl->devices[MOUSEX]);
            }
            if (igl->deviceQueue[MOUSEY])
            {
                qenter (MOUSEY, (short)igl->devices[MOUSEY]);
            }
        }
    }
}


LRESULT CALLBACK
igl_Win32processEvents (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    short wid = -1;
    igl_windowT *wptr;
    RECT r;
    MINMAXINFO *mminfo;


    switch (msg)
    {
        case WM_CREATE:
        case WM_DESTROY:
            return (0);

        case WM_CLOSE:
            PostQuitMessage (0);
            return (0);

        case WM_PAINT:
            /* manual says: the REDRAW token is queued automatically */
            qenter (REDRAW, _igl_findWindowByHandle (wnd));
            return (DefWindowProc(wnd, msg, wParam, lParam));

        case WM_MOVE:
        case WM_SIZE:
            wid = _igl_findWindowByHandle (wnd);
            if (wid != -1)
            {
                wptr = &igl->openWindows[wid];
                if (msg == WM_MOVE)
                {
                    wptr->x = LOWORD (lParam);
                    wptr->y = HIWORD (lParam);
                }
                else
                {
                    GetClientRect (wptr->wnd, &r);
                    wptr->width = r.right;
                    wptr->height = r.bottom;
                }
            }
            return (0);

#if 1
        case WM_EXITSIZEMOVE:
            wid = _igl_findWindowByHandle (wnd);
            if (wid != -1)
            {
                /* this probably has to be forced here? */
                qenter (REDRAW, wid);
            }
            return (0);
#endif

        case WM_KEYUP:
        case WM_KEYDOWN:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            _igl_Win32processDevice (msg, wParam, lParam);
#if 1
            /* debug mode - ESC kills the app */
            if (msg == WM_KEYDOWN && wParam == VK_ESCAPE)
            {
                PostQuitMessage (0);
            }
#endif
            return (0);

        case WM_SETFOCUS:
            wid = _igl_findWindowByHandle (wnd);
            if (wid != -1)
            {
#if 0   /* TBD: do we need this here or is it done automatically? */
                wglMakeCurrent (igl->openWindows[wid].hdc,
                    igl->openWindows[wid].hrcs[IGL_WLAYER_NORMALDRAW]);
#endif
                if (igl->deviceQueue[INPUTCHANGE])
                {
                    qenter (INPUTCHANGE, wid);
                }
                if (igl->deviceQueue[REDRAW])
                {
                    qenter (REDRAW, wid);
                }
            }
            return (0);

#if 0
        case WM_SIZING:
            /* WM_GETMINMAXINFO is received before WM_SIZING, so here we only process aspect ratio */
            wid = _igl_findWindowByHandle (wnd);
            if (wid != -1)
            {
                wptr = &igl->openWindows[wid];
                if (wptr->aspectX != 0 && wptr->aspectY != 0)
                {
                    pr = (RECT *)lParam;
                    w = pr->right - pr->left;
                    h = pr->bottom - pr->top;

                    switch (wParam)
                    {
                        case WMSZ_TOP:
                            pr->left = pr->right - h*wptr->aspectX/wptr->aspectY;
                        break;

                        case WMSZ_BOTTOM:
                            pr->right = pr->left + h*wptr->aspectX/wptr->aspectY;
                        break;

                        case WMSZ_LEFT:
                            pr->top = pr->bottom - w*wptr->aspectY/wptr->aspectX;
                        break;

                        case WMSZ_RIGHT:
                            pr->bottom = pr->top + w*wptr->aspectY/wptr->aspectX;
                        break;

                        default:
                            return (FALSE);
                        break;
                    }
                    return (TRUE);
                }
            }
            return (DefWindowProc (wnd, msg, wParam, lParam));
        break;
#endif

        case WM_GETMINMAXINFO:
            wid = _igl_findWindowByHandle (wnd);
            if (wid != -1)
            {
                mminfo = (MINMAXINFO *)lParam;
                wptr = &igl->openWindows[wid];

                /* force minsize()/maxsize() */
                if (wptr->minWidth > 0)
                {
                    mminfo->ptMinTrackSize.x = wptr->minWidth;
                }
                if (wptr->minHeight > 0)
                {
                    mminfo->ptMinTrackSize.y = wptr->minHeight;
                }
                if (wptr->maxWidth > 0)
                {
                    mminfo->ptMaxTrackSize.x = wptr->maxWidth;
                }
                if (wptr->maxHeight > 0)
                {
                    mminfo->ptMaxTrackSize.y = wptr->maxHeight;
                }
            }
            return (0);

        default:
            return (DefWindowProc (wnd, msg, wParam, lParam));
    }
}
#else   /* PLATFORM_WIN32 */

#if 0
____________________________ X11 stuph ____________________________
() {}
#endif
/* convert X11 msgs (key/mouse events) to IrisGL devices */
/* NOTE: X11 differentiates between XK_Q and XK_q (which sucks i guess). IGL doens't */
#define KEYTR(_key,_skey) case XK_##_key: case XK_##_skey: ret=_key##KEY; break
static int
_igl_X11deviceToIris (XEvent *event, short *state)
{
    int ret = -1;


    if (event->type == KeyPress || event->type == KeyRelease)
    {
        *state = (event->type == KeyPress);

        switch (XLookupKeysym (&event->xkey, 0))
        {
            KEYTR(A, a);
            KEYTR(B, b);
            KEYTR(C, c);
            KEYTR(D, d);
            KEYTR(E, e);
            KEYTR(F, f);
            KEYTR(G, g);
            KEYTR(H, h);
            KEYTR(I, i);
            KEYTR(J, j);
            KEYTR(K, k);
            KEYTR(L, l);
            KEYTR(M, m);
            KEYTR(N, n);
            KEYTR(O, o);
            KEYTR(P, p);
            KEYTR(Q, q);
            KEYTR(R, r);
            KEYTR(S, s);
            KEYTR(T, t);
            KEYTR(U, u);
            KEYTR(V, v);
            KEYTR(W, w);
            KEYTR(X, x);
            KEYTR(Y, y);
            KEYTR(Z, z);

            case XK_0: ret = ZEROKEY; break;
            case XK_1: ret = ONEKEY; break;
            case XK_2: ret = TWOKEY; break;
            case XK_3: ret = THREEKEY; break;
            case XK_4: ret = FOURKEY; break;
            case XK_5: ret = FIVEKEY; break;
            case XK_6: ret = SIXKEY; break;
            case XK_7: ret = SEVENKEY; break;
            case XK_8: ret = EIGHTKEY; break;
            case XK_9: ret = NINEKEY; break;

            case XK_KP_0: ret = PAD0; break;
            case XK_KP_1: ret = PAD1; break;
            case XK_KP_2: ret = PAD2; break;
            case XK_KP_3: ret = PAD3; break;
            case XK_KP_4: ret = PAD4; break;
            case XK_KP_5: ret = PAD5; break;
            case XK_KP_6: ret = PAD6; break;
            case XK_KP_7: ret = PAD7; break;
            case XK_KP_8: ret = PAD8; break;
            case XK_KP_9: ret = PAD9; break;
            case XK_KP_Decimal: ret = PADPERIOD; break;
            case XK_KP_Subtract: ret = PADMINUS; break;

            case XK_F1: ret = F1KEY; break;
            case XK_F2: ret = F2KEY; break;
            case XK_F3: ret = F3KEY; break;
            case XK_F4: ret = F4KEY; break;
            case XK_F5: ret = F5KEY; break;
            case XK_F6: ret = F6KEY; break;
            case XK_F7: ret = F7KEY; break;
            case XK_F8: ret = F8KEY; break;
            case XK_F9: ret = F9KEY; break;
            case XK_F10: ret = F10KEY; break;
            case XK_F11: ret = F11KEY; break;
            case XK_F12: ret = F12KEY; break;

            case XK_Escape: ret = ESCKEY; break;
            case XK_Tab: ret = TABKEY; break;
            case XK_Linefeed: ret = RETKEY; break; /* should it set PADENTER, too? */
            case XK_space: ret = SPACEKEY; break;
            case XK_BackSpace: ret = BACKSPACEKEY; break;
            case XK_Left: ret = LEFTARROWKEY; break;
            case XK_Down: ret = DOWNARROWKEY; break;
            case XK_Right: ret = RIGHTARROWKEY; break;
            case XK_Up: ret = UPARROWKEY; break;
            case XK_Print: ret = PRINTSCREENKEY; break;
            case XK_Scroll_Lock: ret = SCROLLLOCKKEY; break;
            case XK_Pause: ret = PAUSEKEY; break;
            case XK_Insert: ret = INSERTKEY; break;
            case XK_Home: ret = HOMEKEY; break;
            case XK_End: ret = ENDKEY; break;
            case XK_Num_Lock: ret = NUMLOCKKEY; break;
        }
    }
    else if (event->type == ButtonPress || event->type == ButtonRelease)
    {
        *state = (event->type == ButtonPress);
        switch (event->xbutton.button)
        {
            case Button1:
                ret = LEFTMOUSE;
            break;

            case Button2:
                ret = MIDDLEMOUSE;
            break;

            case Button3:
            default:
                ret = RIGHTMOUSE;
            break;
        }
    }

    return (ret);
}
#undef KEYTR


void
_igl_X11processDevice (XEvent *event)
{
    int dev, devTie;
    short state;


    dev = _igl_X11deviceToIris (event, &state);
    if (dev != -1 && igl->deviceQueue[dev])
    {
        /* if device is queued, we have to qenter() it */
        /* NOTE: state indicates whether a device is 'up' or 'down' */
        qenter ((Device)dev, state);

        /* if it's a KEYBD device and it's 'down', qenter KEYBD as well */
        if (ISKEYBD(dev) && state == 1 && igl->deviceQueue[KEYBD])
        {
            qenter (KEYBD, (short)dev);
        }

        /* if device is tied, we have to qenter() its ties */
        devTie = igl->tiedValuators[dev*2];
        if (devTie != NULLDEV)
        {
            qenter ((Device)devTie, (short)igl->devices[devTie]);

            devTie = igl->tiedValuators[dev*2+1];
            if (devTie != NULLDEV)
            {
                qenter ((Device)devTie, (short)igl->devices[devTie]);
            }
        }
    }
}


void
igl_X11processEvents ()
{
    short wid = -1;
    XEvent event;
    XWindowAttributes wa;
    igl_windowT *wptr;
    Window rootWnd, childWnd;
    int rx, ry, x, y;
    unsigned int maskRet;


    /* this seems to be the best place to retrieve the cursor/mouse position */
    if (XQueryPointer (igl->appInstance, IGL_CTX ()->wnd, &rootWnd, &childWnd, &rx, &ry, &x, &y, &maskRet))
    {
        igl->devices[MOUSEX] = rx;
        igl->devices[MOUSEY] = ry;
    }

    while (XPending (igl->appInstance) > 0)
    {
        XNextEvent (igl->appInstance, &event);
        switch (event.type)
        {
            case KeyPress:
            case KeyRelease:
            case ButtonPress:
            case ButtonRelease:
                if (event.type == ButtonPress || event.type == ButtonRelease)
                {
                    igl->devices[MOUSEX] = event.xmotion.x_root;
                    igl->devices[MOUSEY] = event.xmotion.y_root;
                }
                _igl_X11processDevice (&event);
            break;

            case MotionNotify:
                if (event.xmotion.is_hint == NotifyNormal || event.xmotion.is_hint == NotifyHint)
                {
                    igl->devices[MOUSEX] = event.xmotion.x_root;
                    igl->devices[MOUSEY] = event.xmotion.y_root;
#if 1   // do we need this?
                    if (igl->deviceQueue[MOUSEX])
                    {
                        qenter (MOUSEX, (short)igl->devices[MOUSEX]);
                    }
                    if (igl->deviceQueue[MOUSEY])
                    {
                        qenter (MOUSEY, (short)igl->devices[MOUSEY]);
                    }
#endif
                }
            break;

            case Expose:
                if (event.xexpose.count == 0)
                {
                    qenter (REDRAW, _igl_findWindowByHandle (event.xexpose.window));
                }
            break;

            case ConfigureNotify:
                if (!event.xconfigure.override_redirect)
                {
                    /* get the new window position/size */
                    wid = _igl_findWindowByHandle (event.xconfigure.window);
                    if (wid != -1)
                    {
                        wptr = &igl->openWindows[wid];
                        wptr->x = event.xconfigure.x;
                        wptr->y = event.xconfigure.y;
                        wptr->width = event.xconfigure.width;
                        wptr->height = event.xconfigure.height;

                        /* this probably has to be forced here? */
                        qenter (REDRAW, wid);
                    }
                }
            break;

            case FocusIn:
#if 0   // probably not needed
            case FocusOut:
#endif
                wid = _igl_findWindowByHandle (event.xfocus.window);
                if (wid != -1)
                {
                    if (igl->deviceQueue[INPUTCHANGE])
                    {
                        qenter (INPUTCHANGE, wid);
                    }
                    if (igl->deviceQueue[REDRAW])
                    {
                        qenter (REDRAW, wid);
                    }
                }
            break;

            case ReparentNotify:
            {
                Window rootWnd, parentWnd, *clientWnds;
                int frameSize, clientCount;


                /*
                    this is a bit tricky:
                    X11 window managers usually reparent each client
                    window - add its own window as a parent and add a
                    title frame/border to the window.
                    when you then say XMoveWindow(0, 0), the window manager
                    repositions OUR window to (0, 0) and the frame is positioned
                    beyond (maybe i got this wrong, but this seems to be the case).
                    so to overcome this problem, we calculate the delta between
                    parent's and our own Y-coord position and move the window
                    to that location...
                */
                if (!event.xreparent.override_redirect)
                {
                    wid = _igl_findWindowByHandle (event.xreparent.window);
                    if (wid != -1)
                    {
                        wptr = &igl->openWindows[wid];
                        XQueryTree (igl->appInstance, wptr->wnd, &rootWnd, &parentWnd, &clientWnds, &clientCount);
                        if (parentWnd != RootWindow (igl->appInstance, igl->visualInfo.screen))
                        {
                            XGetWindowAttributes (igl->appInstance, parentWnd, &wa);
                            frameSize = wa.y - wptr->y;
                            wptr->y += frameSize;
                            XMoveWindow (igl->appInstance, wptr->wnd, wptr->x, wptr->y);
                        }
                    }
                }
            }
            break;

            default:
            break;
        }
    }

#if 0       /* uncomment to ease up X server banging */
    usleep (1000);
#endif
}
#endif  /* PLATFORM_WIN32 */


#if 0
____________________________ window functions ____________________________
() {}
#endif
/* DESC: greset - resets graphics state */
void
greset ()
{
    IGL_CHECKINITV ();


    backface (FALSE);
    /* blink (0, a, b, c, d); */
    singlebuffer ();
    color (BLACK);
    onemap ();
    concave (FALSE);
    /* setcursor (0, a, b); */
    /* depthrange (Zmin, Zmax); */
    depthcue (FALSE);
    IGL_CTX ()->flags &= ~IGL_WFLAGS_RGBA; /* cmode (); */
    /* drawmode (NORMALDRAW); */
    font (0);
    setlinestyle (0);
    linewidth (1);
    lsrepeat (1);
    setpattern (0);
    /* picking size = 10 */
    /* RGB color = undefined */
    /* RGB shaderange = undefined */
    /* RGB writemask = undefined */
    shademodel (GOURAUD);
    lshaderange (0, 7, getgdesc (GD_ZMIN), getgdesc (GD_ZMAX));
    viewport (0, 0, getgdesc (GD_XPMAX)-1, getgdesc (GD_YPMAX)-1);
    /* writemask (all enabled); */
    zbuffer (FALSE);

#ifdef IRIS_4D
    lsbackup (FALSE);
    /* resetls (TRUE); */
#endif
}


/* DESC: ginit, gbegin - create a window that occupies the entire screen */
void
gbegin ()
{
    prefsize (getgdesc (GD_XPMAX), getgdesc (GD_YPMAX));
    winopen ("");
    greset ();
    igl->devices[MOUSEX] = getgdesc (GD_XPMAX)/2;
    igl->devices[MOUSEY] = getgdesc (GD_YPMAX)/2;
    // TBD: gbegin() does the same as ginit(), except it doesn't alter the colormap
}


/* DESC: ginit, gbegin - create a window that occupies the entire screen */
void
ginit ()
{
    prefsize (getgdesc (GD_XPMAX), getgdesc (GD_YPMAX));
    winopen ("");
    greset ();
    igl->devices[MOUSEX] = getgdesc (GD_XPMAX)/2;
    igl->devices[MOUSEY] = getgdesc (GD_YPMAX)/2;
}


/* DESC: gexit - exits graphics */
void
gexit ()
{
    int i, j;


#if PLATFORM_WIN32
    DEVMODE devMode;


    if (igl == NULL)
    {
        return;
    }

    /* close fullscreen mode */
    memset (&devMode, 0, sizeof(DEVMODE));
    devMode.dmSize = sizeof (DEVMODE);
    ChangeDisplaySettings (&devMode, 0);

    /* cleanup && destroy all menus */
    for (i=0; i < IGL_MAXMENUS; i++)
    {
        if (igl->menus[i].hmenu != NULL)
        {
            DestroyMenu (igl->menus[i].hmenu);
        }
    }

    /* release wgl */
    wglMakeCurrent (NULL, NULL);

    /* cleanup && destroy all open windows */
    for (i=0; i < IGL_MAXWINDOWS; i++)
    {
        igl_windowT *wptr = &igl->openWindows[i];


        if (wptr->wnd != NULL)
        {
            /* deallocate display lists (allocated winopen()) */
            if (wptr->fontBase != 0)
            {
                glDeleteLists (wptr->fontBase, 96);
            }

            for (j=0; j < IGL_WLAYER_MARKER; j++)
            {
                if (wptr->hrcs[j] != NULL)
                {
                    wglDeleteContext (wptr->hrcs[j]);
                }
            }

            if (wptr->hdc != NULL)
            {
                ReleaseDC (wptr->wnd, wptr->hdc);
            }
            DestroyWindow (wptr->wnd);
        }
    }

#else
    if (igl == NULL)
    {
        return;
    }

    /*
        TBD:
        1. close fullscreen mode
        2. destroy all menus
        #3. release glx
        4. destroy all windows
            a. delete GL lists
            b. delete glx contexts
            c. release GC
            d. close window
        #5. close X display
    */
    if (igl->appInstance != NULL)
    {
        glXMakeCurrent (igl->appInstance, None, NULL);

        /* cleanup && destroy all open windows */
        for (i=0; i < IGL_MAXWINDOWS; i++)
        {
            igl_windowT *wptr = &igl->openWindows[i];


            if (wptr->wnd != IGL_NULLWND)
            {
                /* deallocate display lists (allocated winopen()) */
                if (wptr->fontBase != 0)
                {
                    glDeleteLists (wptr->fontBase, 96);
                }

                for (j=0; j < IGL_WLAYER_MARKER; j++)
                {
                    if (wptr->hrcs[j] != NULL)
                    {
                        glXDestroyContext (igl->appInstance, wptr->hrcs[j]);
                    }
                }

                if (wptr->hdc != NULL)
                {
                    XFreeGC (igl->appInstance, wptr->hdc);
                }
                XDestroyWindow (igl->appInstance, wptr->wnd);
            }
        }

        XCloseDisplay (igl->appInstance);
    }
#endif
}


/* DESC: prefsize - specifies the preferred size of a graphics window */
void
prefsize (int width, int height)
{
    if (width > 0)
    {
        igl->winWidth = width;
    }
    if (height > 0)
    {
        igl->winHeight = height;
    }
}


/* DESC: maxsize - specifies the maximum size of a graphics window */
void
maxsize (long width, long height)
{
    if (width > 0 && (igl->winMinWidth == 0 || width >= igl->winMinWidth))
    {
        igl->winMaxWidth = width;
    }
    if (height > 0 && (igl->winMinHeight == 0 || width >= igl->winMinHeight))
    {
        igl->winMaxHeight = height;
    }
}


/* DESC: minsize - specifies the minimum size of a graphics window */
void
minsize (long width, long height)
{
    if (width > 0 && (igl->winMaxWidth == 0 || width <= igl->winMaxWidth))
    {
        igl->winMinWidth = width;
    }
    if (height > 0 && (igl->winMaxHeight == 0 || height <= igl->winMaxHeight))
    {
        igl->winMinHeight = height;
    }
}


/* DESC: prefposition - specifies the preferred location and size of a graphics window */
void
prefposition (int x0, int x1, int y0, int y1)
{
    if (x1 < x0 || y1 < y0)
    {
        return;
    }

    /*
        this is probably wrong, but we do it nonetheless (for ep). the window should be
        forced to + coordinate space. on X11 this seems to somehow work (window manager?).
    */
    if (x0 < 0)
    {
        x1 -= x0;
        x0 = 0;
    }
    if (y0 < 0)
    {
        y1 -= y0;
        y0 = 0;
    }

    igl->winPosx = x0;
    igl->winPosy = y0;
    if (x1-x0 >= 0)
    {
        igl->winWidth = x1-x0;
    }
    if (y1-y0 >= 0)
    {
        igl->winHeight = y1-y0;
    }
}


/* DESC: winopen - creates a graphics window */
long
winopen (String windowTitle)
{
    return (_igl_createWindow(windowTitle, -1));
}


/* DESC: swinopen - creates a graphics subwindow */
long
swinopen (long parent)
{
    IGL_CHECKINIT (-1);
    if (parent < 0 || parent > IGL_MAXWINDOWS || igl->openWindows[parent].wnd == IGL_NULLWND)
    {
        return (-1);
    }

    return (_igl_createWindow(NULL, parent));
}


/* DESC: winclose -  closes the identified graphics window */
void
winclose (long gwid)
{
    int i;


    IGL_CHECKINITV ();

    if (gwid < 0 || gwid >= IGL_MAXWINDOWS)
    {
        return;
    }

    /*
        TBD: does parent's close also close al children?
        since we passed parentWnd to CreateWindow() in swinopen(), it should...
    */
#if PLATFORM_WIN32
    DestroyWindow (igl->openWindows[gwid].wnd);
#else
    XDestroyWindow (igl->appInstance, igl->openWindows[gwid].wnd);
#endif
    memset (&igl->openWindows[gwid], 0, sizeof(igl_windowT));

    /* find the new current window! */
    for (i=0; i < IGL_MAXWINDOWS; i++)
    {
        if (igl->openWindows[i].wnd != IGL_NULLWND)
        {
            igl->currentWindow = i;
#if PLATFORM_WIN32
            wglMakeCurrent (igl->openWindows[i].hdc, igl->openWindows[i].hrcs[IGL_WLAYER_NORMALDRAW]);
#else
            glXMakeCurrent (igl->appInstance, igl->openWindows[i].wnd,
                igl->openWindows[i].hrcs[IGL_WLAYER_NORMALDRAW]);
#endif
            return;
        }
    }
    igl->currentWindow = -1;    /* this was the last window that we just closed */
    /* TBD: what do we do here? should the app exit? */
}


/* DESC: winconstraints - binds window constraints to the current window */
void
winconstraints ()
{
    long wndStyle;
    igl_windowT *wptr;
#if PLATFORM_X11
    XSizeHints sizeHints;
#endif


    IGL_CHECKINITV ();

    wptr = IGL_CTX ();
    wptr->x = igl->winPosx;
    wptr->y = igl->winPosy;
    wptr->width = igl->winWidth;
    wptr->height = igl->winHeight;
    wptr->minWidth = igl->winMinWidth;
    wptr->minHeight = igl->winMinHeight;
    wptr->maxWidth = igl->winMaxWidth;
    wptr->maxHeight = igl->winMaxHeight;
    wptr->aspectX = igl->winAspectX;
    wptr->aspectY = igl->winAspectY;

    /*
        winconstraints() man page says:
        "After binding constraints to a window, winconstraints resets the window
         constraints to their default values, if any."
        the question is, what are the default values, if any =)

        the reset below is commented out, since it seems to screw up ep's trip
    */
#if 0
    igl->winPosx = igl->winPosy = 0;
    igl->winWidth = IGL_WINDOWWIDTH;
    igl->winHeight = IGL_WINDOWHEIGHT;
    igl->winMinWidth = igl->winMinHeight = igl->winMaxWidth = igl->winMaxHeight = 0;
#endif
    igl->winAspectX = igl->winAspectY = 0;
 

#if PLATFORM_WIN32
#   if 0
    if (wptr->aspectX != 0 && wptr->aspectY != 0)
    {
        if (wptr->aspectX < wptr->aspectY)
        {
            SetWindowPos (wptr->wnd, NULL, 0, 0,
                (wptr->width + igl->winOffsetX)*wptr->aspectX/wptr->aspectY,
                wptr->height + igl->winOffsetY, SWP_NOZORDER | SWP_NOMOVE);
        }
        else
        {
            SetWindowPos (wptr->wnd, NULL, 0, 0, wptr->width + igl->winOffsetX,
                (wptr->height + igl->winOffsetY)*wptr->aspectY/wptr->aspectX, SWP_NOZORDER | SWP_NOMOVE);
        }
    }
    else
#   endif
    {
        SetWindowPos (wptr->wnd, HWND_NOTOPMOST,
            wptr->x, wptr->y, wptr->width + igl->winOffsetX, wptr->height + igl->winOffsetY, SWP_SHOWWINDOW);
    }

    wndStyle = GetWindowLong (wptr->wnd, GWL_STYLE);
    wndStyle |= WS_SIZEBOX;
    SetWindowLong (wptr->wnd, GWL_STYLE, wndStyle);
#else
    sizeHints.flags = 0;
    if (wptr->aspectX != 0 && wptr->aspectY != 0)
    {
        sizeHints.flags |= PAspect;
        sizeHints.min_aspect.x = sizeHints.max_aspect.x = wptr->aspectX;
        sizeHints.min_aspect.y = sizeHints.max_aspect.y = wptr->aspectY;
    }
    if (wptr->minWidth != 0 && wptr->minHeight != 0)
    {
        sizeHints.flags |= PMinSize;
        sizeHints.min_width = wptr->minWidth;
        sizeHints.min_height = wptr->minHeight;
    }
    if (wptr->maxWidth != 0 && wptr->maxHeight != 0)
    {
        sizeHints.flags |= PMaxSize;
        sizeHints.max_width = wptr->maxWidth;
        sizeHints.max_height = wptr->maxHeight;
    }

    if (sizeHints.flags != 0)
    {
        XSetWMNormalHints (igl->appInstance, wptr->wnd, &sizeHints);
    }
    XMoveResizeWindow (igl->appInstance, wptr->wnd, wptr->x, wptr->y, wptr->width, wptr->height);
#endif
}


/* DESC: winposition - changes the size and position of the current graphics window */
void
winposition (int x0, int x1, int y0, int y1)
{
    igl_windowT *wptr;


    IGL_CHECKINITV ();

    if (x1 < x0 || y1 < y0)
    {
        return;
    }

    wptr = IGL_CTX ();
    wptr->x = x0;
    wptr->y = y0;
    wptr->width = x1-x0;
    wptr->height = y1-y0;

    if (wptr->maxWidth > 0 && wptr->width > wptr->maxWidth)
    {
        wptr->width = wptr->maxWidth;
    }
    else if (wptr->minWidth > 0 && wptr->width < wptr->minWidth)
    {
        wptr->width = wptr->minWidth;
    }
    if (wptr->maxHeight > 0 && wptr->height > wptr->maxHeight)
    {
        wptr->height = wptr->maxHeight;
    }
    else if (wptr->minHeight > 0 && wptr->height < wptr->minHeight)
    {
        wptr->height = wptr->minHeight;
    }


#if 1
    /* seems that IrisGL functions in this way, otherwise ep main window gets resized incorrectly */
    igl->winWidth = wptr->width;
    igl->winHeight = wptr->height;
    igl->winPosx = wptr->x;
    igl->winPosy = wptr->y;
#endif


#if PLATFORM_WIN32
    SetWindowPos (wptr->wnd, HWND_NOTOPMOST, wptr->x, wptr->y,
        wptr->width + igl->winOffsetX, wptr->height + igl->winOffsetY, SWP_SHOWWINDOW);
#else
    XMoveResizeWindow (igl->appInstance, wptr->wnd, wptr->x, wptr->y, wptr->width, wptr->height);
#endif
}


/* DESC: winattach - obsolete routine */
long
winattach ()
{
    /* obsoleted by SGI */
    return (0);
}


/* DESC: winget - returns the identifier of the current graphics window */
long
winget ()
{
    return (igl->currentWindow);
}


/* DESC: winset - sets the current graphics window */
void
winset (long gwid)
{
    IGL_CHECKINITV ();

    if (gwid >= 0 && gwid < IGL_MAXWINDOWS && gwid != igl->currentWindow)
    {
        igl->currentWindow = gwid;
#if PLATFORM_WIN32
        wglMakeCurrent (igl->openWindows[gwid].hdc, igl->openWindows[gwid].hrcs[IGL_WLAYER_NORMALDRAW]);
#else
        glXMakeCurrent (igl->appInstance, igl->openWindows[gwid].wnd,
            igl->openWindows[gwid].hrcs[IGL_WLAYER_NORMALDRAW]);
#endif
    }
}


/* DESC: winpush - places the current graphics window behind all other windows */
void
winpush ()
{
#if PLATFORM_WIN32
    IGL_CHECKINITV ();
    SetWindowPos (IGL_CTX()->wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
    XWindowChanges xwc;

    IGL_CHECKINITV ();
    xwc.stack_mode = BottomIf;
    XConfigureWindow (igl->appInstance, IGL_CTX ()->wnd, CWStackMode, &xwc);
#endif
}


/* DESC: winpop - moves the current graphics window in front of all other windows */
void
winpop ()
{
#if PLATFORM_WIN32
    IGL_CHECKINITV ();
    SetWindowPos (IGL_CTX()->wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
    XWindowChanges xwc;

    IGL_CHECKINITV ();
    xwc.stack_mode = TopIf;
    XConfigureWindow (igl->appInstance, IGL_CTX ()->wnd, CWStackMode, &xwc);
#endif
}


/* DESC: winmove - moves the current graphics window by its lower-left corner */
void
winmove (long orgx, long orgy)
{
    igl_windowT *wptr;


    IGL_CHECKINITV ();

    wptr = IGL_CTX ();
    /* convert lower-left (IrisGL/OpenGL) to upper-left (Win32) coords */
    wptr->x = orgx;
    wptr->y = orgy - wptr->height;

#if PLATFORM_WIN32
    SetWindowPos (wptr->wnd, HWND_TOPMOST, wptr->x, wptr->y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
#else
    XMoveWindow (igl->appInstance, wptr->wnd, wptr->x, wptr->y);
#endif
}


/* DESC: wintitle - adds a title bar to the current graphics window */
void
wintitle (String name)
{
    long wndStyle;
    igl_windowT *wptr;


    IGL_CHECKINITV ();
    if (name == NULL)
    {
        return;
    }

    wptr = IGL_CTX ();
    strcpy (wptr->title, name);

#if PLATFORM_WIN32
    wndStyle = GetWindowLong (wptr->wnd, GWL_STYLE);
    if (wndStyle & WS_CAPTION)
    {
        /* wintitle("") clears the window title. does it clear WS_CAPTION? */
        SetWindowText (wptr->wnd, wptr->title);
    }
    /* TBD: what if we don't have a caption (e.g. swinopen()). do we add it? */
    else
    {
    }
#else
    XStoreName (igl->appInstance, wptr->wnd, wptr->title);
#endif
}


/* DESC: ??? (gets the window id at specified coordinates) */
long
winat (short x, short y)
{
    int i;
    igl_windowT *wptr;


    IGL_CHECKINIT (-1);

#if 0
    /* first, process active window */
    i = _igl_findWindowByHandle (GetActiveWindow());
    if (i != -1)
    {
        wptr = &igl->openWindows[i];
        if (wptr->wnd != IGL_NULLWND && x >= wptr->x && y >= wptr->y &&
            x <= (wptr->x + wptr->width) && y <= (wptr->y + wptr->height))
        {
            return (i);
        }
    }
#endif

    /* not found, try inactive */
    for (i=0; i < IGL_MAXWINDOWS; i++)
    {
        wptr = &igl->openWindows[i];
        if (wptr->wnd != IGL_NULLWND && x >= wptr->x && y >= wptr->y &&
            x <= (wptr->x + wptr->width) && y <= (wptr->y + wptr->height))
        {
            return (i);
        }
    }

    return (-1);
}


/* DESC: ??? (gets the window id at specified coordinates) */
long
gl_winat (short x, short y)
{
    return (winat(x, y));
}


/* DESC: keepaspect - specifies the aspect ratio of a graphics window */
void
keepaspect (int x, int y)
{
    if (x > 0 && x <= 32767 && y > 0 && y <= 32767)
    {
        igl->winAspectX = x;
        igl->winAspectY = y;
    }
}


/* DESC: getsize - returns the size of a graphics window */
void
getsize (long *x, long *y)
{
    IGL_CHECKINITV ();

    if (x != NULL)
    {
        *x = IGL_CTX()->width;
    }
    if (y != NULL)
    {
        *y = IGL_CTX()->height;
    }
}


/* DESC: getorigin - returns the position of a graphics window */
void
getorigin (long *x, long *y)
{
    IGL_CHECKINITV ();

    if (x != NULL)
    {
        *x = IGL_CTX()->x;
    }
    if (y != NULL)
    {
        *y = IGL_CTX()->y;
    }
}


/* DESC: gconfig - reconfigures the GL modes of the current window */
void
gconfig ()
{
    igl_windowT *wptr;
    int format, windowChanged = 0;
#if PLATFORM_WIN32      // TBD
    PIXELFORMATDESCRIPTOR pfd;


    IGL_CHECKINITV ();

    reshapeviewport ();
    wptr = IGL_CTX ();

    /* retrieve window's pixel format */
    format = GetPixelFormat (wptr->hdc);
    if (format <= 0)
    {
        return;
    }
    DescribePixelFormat (wptr->hdc, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);


    /* set by RGBmode()/cmode()/singlebuffer/doublebuffer()/stereobuffer()/monobuffer()/onemap()/multimap() */
    /* TBD: what do we do with stereobuffer()/monobuffer()s? */
    if (wptr->flags != wptr->newFlags)
    {
        wptr->flags = wptr->newFlags;
        wptr->multimapIndex = 0;
        windowChanged = 1;
    }

    /* set by acsize() */
    if (wptr->acPlanes != pfd.cAccumBits)
    {
        pfd.cAccumBits = wptr->acPlanes;
        windowChanged = 1;
    }

    /* set by stensize() */
    if (wptr->stenPlanes != pfd.cStencilBits)
    {
        pfd.cStencilBits = wptr->stenPlanes;
        windowChanged = 1;
    }

    /* set by zbsize() */
    if (wptr->zbPlanes != pfd.cDepthBits)
    {
        pfd.cDepthBits = wptr->zbPlanes;
        windowChanged = 1;
    }

    /* set by RGBsize() */
    if (wptr->rgbPlanes != pfd.cColorBits)
    {
        pfd.cColorBits = wptr->rgbPlanes;
        windowChanged = 1;
    }


    /* TBD */
    /* set by mssize() */
    /* set by overlay() */
    /* set by underlay() */


    /**************************************
        process the changes
    **************************************/
    format = ChoosePixelFormat (wptr->hdc, &pfd);
    SetPixelFormat (wptr->hdc, format, &pfd);

    /* correct wptr's settings if pixel format differs */
    DescribePixelFormat (wptr->hdc, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    wptr->acPlanes = pfd.cAccumBits;
    wptr->stenPlanes = pfd.cStencilBits;
    wptr->zbPlanes = pfd.cDepthBits;
    wptr->rgbPlanes = pfd.cColorBits;

    /* adjust double/fake singlebuffering */
    glDrawBuffer ((wptr->flags & IGL_WFLAGS_DOUBLEBUFFER) ? GL_BACK : GL_FRONT);
#else
    IGL_CHECKINITV ();

    reshapeviewport ();
    wptr = IGL_CTX ();

    if (wptr->flags != wptr->newFlags)
    {
        wptr->flags = wptr->newFlags;
        windowChanged = 1;
    }
    glDrawBuffer ((wptr->flags & IGL_WFLAGS_DOUBLEBUFFER) ? GL_BACK : GL_FRONT);
#endif

    /* this has to be done here to get the first redraw properly (?) */
    qenter (REDRAW, (short)igl->currentWindow);
}


/* DESC: getgconfig - gets the size of a buffer or a state in the current buffer configuration */
long
getgconfig (long buffer)
{
    int buf[2];
    igl_windowT *wptr;


    IGL_CHECKINIT (0);

    wptr = IGL_CTX ();
    switch (buffer)
    {
        case GC_BITS_CMODE:
            return ((wptr->flags & IGL_WFLAGS_RGBA) ? 0 : IGL_MAXCOLORS);

        case GC_BITS_RED:
            glGetIntegerv (GL_RED_BITS, buf);
            return (buf[0]);

        case GC_BITS_GREEN:
            glGetIntegerv (GL_GREEN_BITS, buf);
            return (buf[0]);

        case GC_BITS_BLUE:
            glGetIntegerv (GL_BLUE_BITS, buf);
            return (buf[0]);

        case GC_BITS_ALPHA:
            glGetIntegerv (GL_ALPHA_BITS, buf);
            return (buf[0]);

        case GC_BITS_ZBUFFER:
            glGetIntegerv (GL_DEPTH_BITS, buf);
            return (buf[0]);

        case GC_ZMIN:
        case GC_ZMAX:
            glGetIntegerv (GL_DEPTH_RANGE, buf);
            return ((buffer == GC_ZMIN) ? buf[0] : buf[1]);

        case GC_BITS_STENCIL:
            glGetIntegerv (GL_STENCIL_BITS, buf);
            return (buf[0]);

        case GC_BITS_ACBUF:
            return (wptr->acPlanes);

        case GC_MS_SAMPLES:
        case GC_BITS_MS_ZBUFFER:
        case GC_MS_ZMIN:
        case GC_MS_ZMAX:
        case GC_BITS_MS_STENCIL:
            /* multisample mode not yet supported */
            return (0);

        case GC_STEREO:
            return (wptr->flags & IGL_WFLAGS_STEREOBUFFER);

        case GC_DOUBLE:
            return (wptr->flags & IGL_WFLAGS_DOUBLEBUFFER);

        default:
            return (0);
    }
}


/* DESC: getplanes - returns the number of available bitplanes */
long
getplanes ()
{
    IGL_CHECKINIT (-1);
    /* color-index mode has a fixed palette of 4096 (2^12) entries */
    return ((IGL_CTX()->flags & IGL_WFLAGS_RGBA) ? IGL_CTX()->rgbPlanes : 12);
}


/* DESC: fullscrn - allows a program write to the entire screen */
void
fullscrn ()
{
    igl->api->fullscrn ();
}

void
iglExec_fullscrn ()
{
    IGL_CHECKINITV ();

    /* need an SGI to test this */
    viewport (0, getgdesc (GD_XPMAX)-1, 0, getgdesc (GD_YPMAX)-1);
    ortho2 (0, 0, XMAXSCREEN, YMAXSCREEN);
}


/* DESC: endfullscrn - ends full-screen mode */
void
endfullscrn ()
{
    igl->api->endfullscrn ();
}

void
iglExec_endfullscrn ()
{
    long w, h;


    IGL_CHECKINITV ();

    /* need an SGI to test this */
    getsize (&w, &h);
    w --; h --;
    viewport (0, (Screencoord)w, 0, (Screencoord)h);
    ortho2 (0, 0, (Coord)w, (Coord)h);
}


/* DESC: foreground - prevents a graphical process from being put into the background */
void
foreground ()
{
    /* do we need this? (man page says "useful for debugging on...") */
}


/* DESC: fudge - specifies fudge values that are added to a graphics window */
void
fudge (long xfudge, long yfudge)
{
    /* do we need "interior window borders"? */
}


/* DESC: getwscrn - returns the screen upon which the current window appears */
long
getwscrn ()
{
    /* we're always on screen 0, i guess */
    return (0);
}


/* DESC: noborder - specifies a window without any borders */
void
noborder ()
{
    igl->windowFlags |= IGL_WFLAGS_NOBORDER;
}


/* DESC: noport - specifies that a program does not need screen space */
void
noport ()
{
    /* TBD: hmmm... what do we do here? now we create a hidden window on winopen()... */
    igl->windowFlags |= IGL_WFLAGS_NOPORT;
}


