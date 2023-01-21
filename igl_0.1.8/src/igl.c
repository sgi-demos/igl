#include "igl.h"
#include "iglcmn.h"
#include "iglobj.h"



/* globals */
igl_globalsT *igl;

extern int
main (int argc, char **argv);


#if PLATFORM_WIN32
#if 0
____________________________ WIN32 common stuph ____________________________
() {}
#endif
int APIENTRY
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, char *cmdLine, int windowState)
{
    int ret = 0, queryWindowId;
    RECT r;


    /***************************************
        init (TBD: move to greset())!!!
    ****************************************/
    igl = (igl_globalsT *)malloc (sizeof(igl_globalsT));
    FCN_EXIT (igl == NULL, -1,
        {
            /* TBD: error report */
        });

    /* app-related */
    memset (igl, 0, sizeof(igl_globalsT));
    igl->appInstance = hInstance;
    /* window-related */
    igl->winWidth = IGL_WINDOWWIDTH;
    igl->winHeight = IGL_WINDOWHEIGHT;
    /* color-related */
    igl->colorPalette[BLACK] = RGB (0, 0, 0);
    igl->colorPalette[RED] = RGB (255, 0, 0);
    igl->colorPalette[GREEN] = RGB (0, 255, 0);
    igl->colorPalette[YELLOW] = RGB (255, 255, 0);
    igl->colorPalette[BLUE] = RGB (0, 0, 255);
    igl->colorPalette[MAGENTA] = RGB (255, 0, 255);
    igl->colorPalette[CYAN] = RGB (0, 255, 255);
    igl->colorPalette[WHITE] = RGB (255, 255, 255);
    /* vertex-related */
    igl->curveSegments = 30;
    /* matrix-related */
    igl->matrixMode = MSINGLE;
    /* light-related */
    {
        igl_lmMaterialDefT *matPtr = &igl->materialDefs[0];
        igl_lmLightDefT *lightPtr = &igl->lightDefs[0];
        igl_lmLmodelDefT *lmPtr = &igl->lmodelDefs[0];


        matPtr->ambient[0] = matPtr->ambient[1] = matPtr->ambient[2] = 0.2f;
        matPtr->ambient[3] = 1.0f;
        matPtr->diffuse[0] = matPtr->diffuse[1] = matPtr->diffuse[2] = 0.8f;
        matPtr->diffuse[3] = 1.0f;
        matPtr->specular[3] = 1.0f;
        matPtr->emission[3] = 1.0f;

        lightPtr->ambient[3] = 1.0f;
        lightPtr->position[2] = 1.0f;
        lightPtr->spotDirection[2] = -1.0f;
        lightPtr->spotLight[1] = 180.0f;

        lmPtr->ambient[0] = lmPtr->ambient[1] = lmPtr->ambient[2] = 0.2f;
        lmPtr->ambient[3] = 1.0f;
        lmPtr->attenuation[0] = 1.0f;
    }
    /* texture-related */
    igl->tevDefs[0].tvMode = GL_MODULATE;
    /* object-related */
    FCN_EXIT (igl_initApiList () != 0, -1, {});

    /* open the hidden query window - used for getgdesc() */
    noport ();
    queryWindowId = winopen ("");
    if (queryWindowId != -1)
    {
        memcpy (&igl->queryWindow, &igl->openWindows[queryWindowId], sizeof (igl_windowT));
        memset (&igl->openWindows[queryWindowId], 0, sizeof (igl_windowT));

        GetClientRect (igl->queryWindow.wnd, &r);
        igl->winOffsetX = igl->queryWindow.width - r.right;
        igl->winOffsetY = igl->queryWindow.height - r.bottom;
    }
    igl->currentWindow = -1;


    /***************************************
        process
    ****************************************/
    /* TBD: parse cmdline to argc && argv */
    /* td->argc = ...; td->argv = ...; */
    ret = main (0, NULL);


    /***************************************
        cleanup
    ****************************************/
exit:
    /* delete all open windows etc. */
    gexit ();

    /* delete globals */
    if (igl != NULL)
    {
        free (igl);
    }

    return (ret);
}
#else       /* PLATFORM_WIN32 */
#if 0
____________________________ X11 common stuph ____________________________
() {}
#endif
int
igl_X11main (int argc, char **argv)
{
    int ret = 0, queryWindowId;
    int tmp[2];
    int aidx, attribs[64];
    XVisualInfo *vi;


    /***************************************
        init (TBD: move to greset())!!!
    ****************************************/
    igl = (igl_globalsT *)malloc (sizeof (igl_globalsT));
    FCN_EXIT (igl == NULL, -1,
        {
            printf ("malloc() failed!\n");
        });

    /* app-related */
    memset (igl, 0, sizeof (igl_globalsT));
    igl->appInstance = XOpenDisplay (NULL);
    FCN_EXIT (igl->appInstance == NULL, -1,
        {
            printf ("XOpenDisplay() failed!\n");
        });
#ifdef _DEBUG
    XSynchronize (igl->appInstance, 1);
#endif


    /* setup GL visuals */
    FCN_EXIT (!glXQueryExtension (igl->appInstance, &tmp[0], &tmp[1]), -1,
        {
            printf ("glXQueryExtension() failed!\n");
        });

    aidx = 0;
    attribs[aidx++] = GLX_RGBA;
    attribs[aidx++] = GLX_DOUBLEBUFFER;
    attribs[aidx++] = GLX_DEPTH_SIZE;
    attribs[aidx++] = 16;
#if 0
    attribs[aidx++] = GLX_RED_SIZE;
    attribs[aidx++] = 4;
    attribs[aidx++] = GLX_GREEN_SIZE;
    attribs[aidx++] = 4;
    attribs[aidx++] = GLX_BLUE_SIZE;
    attribs[aidx++] = 4;
    attribs[aidx++] = GLX_ALPHA_SIZE;
    attribs[aidx++] = 4;
    attribs[aidx++] = GLX_STENCIL_SIZE;
    attribs[aidx++] = 1;
    attribs[aidx++] = GLX_ACCUM_RED_SIZE;
    attribs[aidx++] = 16;
    attribs[aidx++] = GLX_ACCUM_BLUE_SIZE;
    attribs[aidx++] = 16;
    attribs[aidx++] = GLX_ACCUM_GREEN_SIZE;
    attribs[aidx++] = 16;
    attribs[aidx++] = GLX_ACCUM_ALPHA_SIZE;
    attribs[aidx++] = 16;
    if (wptr->flags & IGL_WFLAGS_STEREOBUFFER)
    {
        attribs[aidx++] = GLX_STEREO;
    }
#endif
    attribs[aidx++] = None;
    vi = glXChooseVisual (igl->appInstance, DefaultScreen (igl->appInstance), attribs);
    FCN_EXIT (vi == NULL, -1,
        {
            printf ("glXChooseVisual () failed!");
        });
    memcpy (&igl->visualInfo, vi, sizeof (XVisualInfo));
    XFree (vi);
    vi = NULL;


    /* window-related */
    igl->currentWindow = -1;
    igl->winWidth = IGL_WINDOWWIDTH;
    igl->winHeight = IGL_WINDOWHEIGHT;
    /* color-related */
    igl->colorPalette[BLACK] = RGB (0, 0, 0);
    igl->colorPalette[RED] = RGB (255, 0, 0);
    igl->colorPalette[GREEN] = RGB (0, 255, 0);
    igl->colorPalette[YELLOW] = RGB (255, 255, 0);
    igl->colorPalette[BLUE] = RGB (0, 0, 255);
    igl->colorPalette[MAGENTA] = RGB (255, 0, 255);
    igl->colorPalette[CYAN] = RGB (0, 255, 255);
    igl->colorPalette[WHITE] = RGB (255, 255, 255);
    /* vertex-related */
    igl->curveSegments = 30;
    /* matrix-related */
    igl->matrixMode = MSINGLE;
    /* light-related */
    {
        igl_lmMaterialDefT *matPtr = &igl->materialDefs[0];
        igl_lmLightDefT *lightPtr = &igl->lightDefs[0];
        igl_lmLmodelDefT *lmPtr = &igl->lmodelDefs[0];


        matPtr->ambient[0] = matPtr->ambient[1] = matPtr->ambient[2] = 0.2f;
        matPtr->ambient[3] = 1.0f;
        matPtr->diffuse[0] = matPtr->diffuse[1] = matPtr->diffuse[2] = 0.8f;
        matPtr->diffuse[3] = 1.0f;
        matPtr->specular[3] = 1.0f;
        matPtr->emission[3] = 1.0f;

        lightPtr->ambient[3] = 1.0f;
        lightPtr->position[2] = 1.0f;
        lightPtr->spotDirection[2] = -1.0f;
        lightPtr->spotLight[1] = 180.0f;

        lmPtr->ambient[0] = lmPtr->ambient[1] = lmPtr->ambient[2] = 0.2f;
        lmPtr->ambient[3] = 1.0f;
        lmPtr->attenuation[0] = 1.0f;
    }
    /* texture-related */
    igl->tevDefs[0].tvMode = GL_MODULATE;
    /* object-related */
    FCN_EXIT (igl_initApiList () != 0, -1, {});

    /* open the hidden query window - used for getgdesc() */
    noport ();
    queryWindowId = winopen ("");
    if (queryWindowId != -1)
    {
        memcpy (&igl->queryWindow, &igl->openWindows[queryWindowId], sizeof (igl_windowT));
        memset (&igl->openWindows[queryWindowId], 0, sizeof (igl_windowT));
    }
    igl->currentWindow = -1;


    /* run the real main() */
    ret = main (argc, argv);


exit:
    gexit ();

    if (igl != NULL)
    {
        free (igl);
    }

    exit (ret);   /* TBD: upon return(), we dump core. on exit() we don't */
}
#endif      /* PLATFORM_WIN32 */




#if 0
____________________________ misc IrisGL stuph ____________________________
() {}
#endif
/* DESC: pushattributes - pushes down the attribute stack */
void
pushattributes ()
{
    igl->api->pushattributes ();
}

void
iglExec_pushattributes ()
{
    IGL_CHECKINITV ();
    glPushAttrib (GL_ALL_ATTRIB_BITS);
}


/* DESC: popattributes - pops the attribute stack */
void
popattributes ()
{
    igl->api->popattributes ();
}

void
iglExec_popattributes ()
{
    IGL_CHECKINITV ();
    glPopAttrib ();
}


/* DESC: ? */
void
sginap (long nap)
{
    sleep (nap);
}


/* memset the (old) UNIX way */
#if PLATFORM_WIN32
void
bzero (void *ptr, int len)
{
    memset (ptr, 0, len);
}
#endif


/* DESC: glcompat - controls compatibility modes */
void
glcompat (long mode, long value)
{
}


/* DESC: getgdesc - gets graphics system description */
long
getgdesc (long inquiry)
{
    igl_windowT *wptr;
    int buf[2];


    /*
        NOTE: getgdesc() needs an open window, meaning that all queries
        executed BEFORE a window is created fail miserably. the problem
        lies in OpenGL, which can only be initialized after the window
        has been created. to solve the problem, IGL creates a hidden
        window in WinMain()/igl_X11main() to retrieve info from there...
    */
    wptr = &igl->queryWindow;
    if (wptr->wnd == IGL_NULLWND)
    {
        return (0);
    }


    switch (inquiry)
    {
#if PLATFORM_WIN32
        case GD_XMMAX:
        case GD_YMMAX:
        case GD_XPMAX:
        case GD_YPMAX:
        {
            DEVMODE devMode;


            if (EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, &devMode))
            {
                /* we assume 72dpi = 72pixels/25.4mm */
                switch (inquiry)
                {
                    case GD_XMMAX:
                        return ((long)(devMode.dmPelsWidth*25.4f/72.0f));
                    break;

                    case GD_YMMAX:
                        return ((long)(devMode.dmPelsHeight*25.4f/72.0f));
                    break;

                    case GD_XPMAX:
                        return (devMode.dmPelsWidth);
                    break;

                    case GD_YPMAX:
                    default:
                        return (devMode.dmPelsHeight);
                    break;
                }
            }
            else
            {
                return (0);
            }
        }
        break;

#else
        case GD_XMMAX:
            return (DisplayWidthMM (igl->appInstance, DefaultScreen (igl->appInstance)));
        break;

        case GD_XPMAX:
            return (DisplayWidth (igl->appInstance, DefaultScreen (igl->appInstance)));
        break;

        case GD_YMMAX:
            return (DisplayHeightMM (igl->appInstance, DefaultScreen (igl->appInstance)));
        break;

        case GD_YPMAX:
            return (DisplayHeight (igl->appInstance, DefaultScreen (igl->appInstance)));
        break;
#endif

        case GD_ZMIN:
        case GD_ZMAX:
            glGetIntegerv (GL_DEPTH_RANGE, buf);
            return ((inquiry == GD_ZMIN) ? buf[0] : buf[1]);
        break;

        case GD_BITS_ACBUF_HW:
            /*
                i don't think we have have an accelerated accumulation buffer if we're not
                on an SGI machine. tried glAccum() and it's slow as hell.
            */
            return (0);
        break;

        case GD_BITS_ACBUF:
            return (wptr->acPlanes);
        break;

        case GD_BITS_CURSOR:
            /* TBD: what do we return here? */
            return (0);
        break;

        case GD_BITS_NORM_DBL_ALPHA:
            /* TBD: what do we return here? */
            return (wptr->acPlanes);
        break;

        case GD_BITS_NORM_DBL_CMODE:
        case GD_BITS_NORM_SNG_CMODE:
        case GD_BITS_NORM_DBL_MMAP:
        case GD_BITS_NORM_SNG_MMAP:
            /* IGL_MAXCOLORS is 4096 = 2^12, so we have 12 bits for (double-buffered) colormap mode */
            return (12);
        break;

        case GD_BITS_NORM_DBL_RED:
        case GD_BITS_NORM_SNG_RED:
            glGetIntegerv (GL_RED_BITS, buf);
            return (buf[0]);
        break;

        case GD_BITS_NORM_DBL_GREEN:
        case GD_BITS_NORM_SNG_GREEN:
            glGetIntegerv (GL_GREEN_BITS, buf);
            return (buf[0]);
        break;

        case GD_BITS_NORM_DBL_BLUE:
        case GD_BITS_NORM_SNG_BLUE:
            glGetIntegerv (GL_BLUE_BITS, buf);
            return (buf[0]);
        break;

        case GD_BITS_NORM_SNG_ALPHA:
            glGetIntegerv (GL_ALPHA_BITS, buf);
            return (buf[0]);
        break;

        case GD_BITS_NORM_ZBUFFER:
            glGetIntegerv (GL_DEPTH_BITS, buf);
            return (buf[0]);
        break;

        case GD_BITS_OVER_SNG_CMODE:
            /* no layer support yet */
            return (0);
        break;

        case GD_BITS_PUP_SNG_CMODE:
        case GD_BITS_UNDR_SNG_CMODE:
            /* no layer support yet */
            return (0);
        break;

        case GD_BITS_STENCIL:
            glGetIntegerv (GL_STENCIL_BITS, buf);
            return (buf[0]);
        break;

        case GD_AFUNCTION:
        case GD_AFUNCTION_MODES:
            /* yes, IGL/OpenGL support alpha functions + they support all modes */
            return (1);
        break;

        case GD_ALPHA_OVERUNDER:
            /* TBD */
            return (0);
        break;

        case GD_BLEND:
            /* i guess we support blending in all framebuffers */
            return (1);
        break;

        case GD_BLENDCOLOR:
            return (0);
        break;

        case GD_CIFRACT:
            return (1);
        break;

        case GD_CLIPPLANES:
            /* IrisGL supports 6 clipplanes. TBD: we should rely on GL_MAX_CLIP_PLANES just in case */
            return (6);
        break;

        case GD_CROSSHAIR_CINDEX:
            return (WHITE);
        break;

        case GD_MUXPIPES:
            return (0);
        break;

        case GD_DBBOX:
            /* dial buttons are not available */
            return (0);
        break;

        case GD_DITHER:
            return (1);
        break;

        case GD_FOGVERTEX:
        case GD_FOGPIXEL:
            return (1);
        break;

        case GD_FRAMEGRABBER:
            /* Live Video Digitizer isn't available */
            return (0);
        break;

        case GD_LIGHTING_ATT2:
        case GD_LIGHTING_SPOTLIGHT:
        case GD_LIGHTING_TWOSIDE:
            return (1);
        break;

        case GD_LINESMOOTH_CMODE:
        case GD_LINESMOOTH_RGB:
            /* in IGL colormap is simulated through RGB, so we say yes to colormap line smooting */
            return (1);
        break;

        case GD_LOGICOP:
            return (1);
        break;

        case GD_MULTISAMPLE:
            /* hmm... is multisampling available in OpenGL? GL extensions perhaps? */
            return (0);
        break;

        case GD_NBLINKS:
            return (0);
        break;

        case GD_NMMAPS:
            return (16);
        break;

        case GD_NSCRNS:
            return (1); /* number of screens on X11 systems is probably more than 1 */
        break;

        case GD_NURBS_ORDER:
            /* is maximum order of nurbs curves/surfaces defined in OpenGL? */
            return (10);
        break;

        case GD_NVERTEX_POLY:
            return (GD_NOLIMIT);
        break;

        case GD_OVERUNDER_SHARED:
            /* overlay and underlay planes are not shared */
            return (0);
        break;

        case GD_PATSIZE_64:
            /* 64x64 pattern/stipple sizes aren't supported */
            return (0);
        break;

        case GD_PNTSMOOTH_CMODE:
        case GD_PNTSMOOTH_RGB:
            /* point smoothing is available */
            return (1);
        break;

        case GD_POLYMODE:
            return (1);
        break;

        case GD_POLYSMOOTH:
            return (1);
        break;

        case GD_PUP_TO_OVERUNDER:
            return (0);
        break;

        case GD_READSOURCE:
        case GD_READSOURCE_ZBUFFER:
            /* readsource() isn't implemented yet */
            return (0);
        break;

        case GD_SCRBOX:
            /* scrbox() isn't implemented yet */
            return (0);
        break;

        case GD_SCRNTYPE:
            return (GD_SCRNTYPE_WM);    /* TBD: on X11 we might be in GD_SCRNTYPE_NOWM mode */
        break;

        case GD_STEREO:
            return (0);
        break;

        case GD_STEREO_IN_WINDOW:
            /* not yet implemented */
            return (0);
        break;

        case GD_SUBPIXEL_LINE:
        case GD_SUBPIXEL_PNT:
        case GD_SUBPIXEL_POLY:
            return (1);
        break;

        case GD_TEXTPORT:
            return (0);
        break;

        case GD_TEXTURE:
            return (1);
        break;

        case GD_TEXTURE_3D:
            /* not yet */
            return (0);
        break;

        case GD_TEXTURE_DETAIL:
        case GD_TEXTURE_LUT:
        case GD_TEXTURE_SHARP:
            return (0);
        break;

        case GD_TIMERHZ:
            /* TBD: get display refresh rate */
            return (60);
        break;

        case GD_TRIMCURVE_ORDER:
            return (3);
        break;

        case GD_WSYS:
            return (GD_WSYS_4S);
        break;

        case GD_ZDRAW_GEOM:
        case GD_ZDRAW_PIXELS:
            return (1);
        break;

        default:
            return (0);
        break;
    }
}


