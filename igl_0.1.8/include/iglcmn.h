#ifndef IGLCMN_H
#define IGLCMN_H


#include <GL/glu.h> /* for GLUnurbsObj */



#ifdef __cplusplus
extern "C" {
#endif

#define FCN_EXIT(condition_, ret_, action_) \
{                                           \
    if (condition_)                         \
    {                                       \
        ret = ret_;                         \
        action_;                            \
        goto exit;                          \
    }                                       \
}


/*******************************************
    igl_windowT
********************************************/
#define IGL_WFLAGS_NONE             0x00
#define IGL_WFLAGS_FULLSCREEN       0x01
#define IGL_WFLAGS_DOUBLEBUFFER     0x02
#define IGL_WFLAGS_RGBA             0x04
#define IGL_WFLAGS_STEREOBUFFER     0x08
#define IGL_WFLAGS_NOBORDER         0x10
#define IGL_WFLAGS_NOPORT           0x20
#define IGL_WFLAGS_MULTIMAP         0x40

#define IGL_WINDOWCLASS             "IGL"

#define IGL_WLAYER_NORMALDRAW       0
#define IGL_WLAYER_UNDERDRAW        1
#define IGL_WLAYER_OVERDRAW         2
#define IGL_WLAYER_PUPDRAW          3
#define IGL_WLAYER_CURSORDRAW       4
#define IGL_WLAYER_MARKER           (IGL_WLAYER_CURSORDRAW+1)
typedef struct
{
    HWND wnd;
    HWND parentWnd;
    HDC hdc;
    HGLRC hrcs[IGL_WLAYER_MARKER];
    short currentLayer;

    char title[256];
    int x, y;
    int width, height;
    int minWidth, minHeight;
    int maxWidth, maxHeight;
    int aspectX, aspectY;

    /* graphics position related */
    float gposx, gposy, gposz, gposw;

    /* character position related */
    float cposx, cposy, cposz;

    long flags;         /* IGL_WFLAGS_XXX */
    long newFlags;      /* IGL_WFLAGS_XXX before gconfig() */
    long style;         /* window style before fullscrn() */

    /* text/font related */
    unsigned int fontBase;

    /* color related */
    unsigned char multimapIndex;

    /* gconfig() related */
    unsigned char acPlanes;      /* accumulation buffer planes */
    unsigned char stenPlanes;    /* stencil buffer planes */
    unsigned char msPlanes;      /* multisample buffer planes */
    unsigned char zbPlanes;      /* Z-buffer buffer planes */
    unsigned char rgbPlanes;     /* RGB planes */
} igl_windowT;


/*******************************************
    lmdefs
********************************************/
#define IGL_LMMATERIALFLAGS_NONE            0x00
#define IGL_LMMATERIALFLAGS_AMBIENT         0x01
#define IGL_LMMATERIALFLAGS_COLORINDEXES    0x02
#define IGL_LMMATERIALFLAGS_DIFFUSE         0x04
#define IGL_LMMATERIALFLAGS_EMISSION        0x08
#define IGL_LMMATERIALFLAGS_SHININESS       0x10
#define IGL_LMMATERIALFLAGS_SPECULAR        0x20
typedef struct
{
    unsigned short flags;
    float ambient[4];
    float colorIndexes[4];
    float diffuse[4];
    float emission[4];
    float shininess;
    float specular[4];
} igl_lmMaterialDefT;

#define IGL_LMLIGHTFLAGS_NONE           0x00
#define IGL_LMLIGHTFLAGS_AMBIENT        0x01
#define IGL_LMLIGHTFLAGS_LCOLOR         0x02
#define IGL_LMLIGHTFLAGS_POSITION       0x04
#define IGL_LMLIGHTFLAGS_SPOTDIRECTION  0x08
#define IGL_LMLIGHTFLAGS_SPOTLIGHT      0x10
typedef struct
{
    unsigned short flags;
    float ambient[4];
    float lcolor[4];
    float position[4];
    float spotDirection[4];
    float spotLight[2];

    short lightTarget;                  /* GL_LIGHTx to which this lightDef is bound */
} igl_lmLightDefT;

#define IGL_LMMODELFLAGS_NONE           0x00
#define IGL_LMMODELFLAGS_AMBIENT        0x01
#define IGL_LMMODELFLAGS_ATTENUATION    0x02
#define IGL_LMMODELFLAGS_ATTENUATION2   0x04
#define IGL_LMMODELFLAGS_LOCALVIEWER    0x08
#define IGL_LMMODELFLAGS_TWOSIDE        0x10
typedef struct
{
    unsigned short flags;
    float ambient[4];
    float attenuation[2];
    float attenuation2;
    float localViewer;
    float twoSide;
} igl_lmLmodelDefT;


#define IGL_TEVDEFFLAGS_NONE            0x00
#define IGL_TEVDEFFLAGS_COLOR           0x01
#define IGL_TEVDEFFLAGS_COMPONENT       0x02
typedef struct
{
    int tvMode;
    unsigned short flags;
    float blendColor[4];    /* TV_COLOR specifies it, used with TV_BLEND */
    float component;
} igl_tevDefT;


#define IGL_TEXDEFFLAGS_NONE            0x0000
#define IGL_TEXDEFFLAGS_MINFILTER       0x0001
#define IGL_TEXDEFFLAGS_MAGFILTER       0x0002
#define IGL_TEXDEFFLAGS_MAGFILTERALPHA  0x0004
#define IGL_TEXDEFFLAGS_MAGFILTERCOLOR  0x0008
#define IGL_TEXDEFFLAGS_WRAPS           0x0010
#define IGL_TEXDEFFLAGS_WRAPT           0x0020
#define IGL_TEXDEFFLAGS_WRAPR           0x0040
#define IGL_TEXDEFFLAGS_MIPMAPFILTER    0x0080
#define IGL_TEXDEFFLAGS_CONTROLPOINT    0x0100  /* LOD and scale */
#define IGL_TEXDEFFLAGS_CLAMP           0x0200
#define IGL_TEXDEFFLAGS_DETAIL          0x0400
#define IGL_TEXDEFFLAGS_TILE            0x0800
#define IGL_TEXDEFFLAGS_BICUBICFILTER   0x1000
#define IGL_TEXDEFFLAGS_3D              0x2000
typedef struct
{
    unsigned short flags;
    float minFilter;
    float magFilter;
    float magFilterAlpha;
    float magFilterColor;
    float wrap;
    float wrapS;
    float wrapT;
    float wrapR;
    int internalFormat;
    int externalFormat;
    float mipmapFilter[8];
    float lod;
    float scale;
    float clamp;
    float detail[5];
    float tile[4];
    float bicubicFilter[2];
} igl_texDefT;


#define IGL_MENUITEMFLAGS_NONE          0x00
#define IGL_MENUITEMFLAGS_DELIMITER     0x01
#define IGL_MENUITEMFLAGS_GRAYED        0x02
#define IGL_MENUITEMFLAGS_CHECKED       0x04
#define IGL_MENUITEMFLAGS_UNCHECKED     0x08
typedef struct
{
    char title[256];
    short id;
    short flags;
    short dontExecuteRoutine;
    int (*routine)(int, ...);
    long submenu;
#if PLATFORM_X11
    int y;
#endif
} igl_menuItemT;

#define IGL_MAXMENUITEMS    100
typedef struct
{
    HMENU hmenu;
    char title[256];
    int (*routine)(int, ...);
    igl_menuItemT items[IGL_MAXMENUITEMS];
    int numItems;
#if PLATFORM_X11
    int width, height, itemHeight, titlePosX;
    Boolean hasCheckboxes;
    GC textGC;          /* GC for displaying text */
    GC backgroundGC;    /* GC for background color */
    GC borderWhiteGC;   /* GC for menu surrounding border */
    GC borderGrayGC;    /* GC for menu surrounding border */
    Pixmap itemUnchecked, itemChecked;
#else
    HBITMAP itemUnchecked, itemChecked;
#endif
} igl_menuT;


typedef struct
{
    Object id;
    long chunkSize;
    char *chunk;
    char *beginPtr, *curPtr, *endPtr;
} igl_objectT;

typedef struct
{
    void (*fullscrn) ();
    void (*endfullscrn) ();
    void (*clear) ();
    void (*lsetdepth) (long near_, long far_);
    void (*getdepth) (Screencoord *near_, Screencoord *far_);
    void (*backbuffer) (Boolean enable);
    void (*frontbuffer) (Boolean enable);
    void (*leftbuffer) (Boolean enable);
    void (*rightbuffer) (Boolean enable);
    void (*backface) (Boolean enable);
    void (*frontface) (Boolean enable);
    void (*depthcue) (Boolean enable);
    void (*lshaderange) (Colorindex lowin, Colorindex highin, long znear, long zfar);
    void (*zbuffer) (Boolean enable);
    void (*zfunction) (long func);
    void (*zclear) ();
    void (*czclear) (unsigned long cval, long zval);
    void (*sclear) (unsigned long sval);
    void (*stencil) (long enable, unsigned long ref, long func, unsigned long mask, long fail, long pass, long zpass);
    void (*acbuf) (long op, float value);
    void (*shademodel) (int mode);
    void (*color) (Colorindex c);
    void (*mapcolor) (Colorindex i, short r, short g, short b);
    void (*cpack) (unsigned long color);
    void (*RGBcolor) (short r, short g, short b);
    void (*afunction) (long ref, long func);
    void (*blendfunction) (long sfactor, long dfactor);
    void (*dither) (long mode);
    void (*fogvertex) (long mode, float *params);
    void (*logicop) (long opcode);
    void (*c4f) (float cv[4]);
    void (*c4i) (int cv[4]);
    void (*c4s) (short cv[4]);
    void (*mmode) (short mode);
    void (*loadmatrix) (Matrix m);
    void (*multmatrix) (Matrix m);
    void (*pushmatrix) ();
    void (*popmatrix) ();
    void (*rot) (float amount, char angle);
    void (*rotate) (Angle amount, char angle);
    void (*translate) (Coord x, Coord y, Coord z);
    void (*scale) (float x, float y, float z);
    void (*polarview) (Coord dist, Angle azim, Angle inc, Angle twist);
    void (*clipplane) (long index, long mode, float *params);
    void (*ortho) (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_);
    void (*ortho2) (Coord left, Coord right, Coord bottom, Coord top);
    void (*window) (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_);
    void (*perspective) (Angle fovy, float aspect, Coord near_, Coord far_);
    void (*viewport) (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top);
    void (*getviewport) (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top);
    void (*reshapeviewport) ();
    void (*pushviewport) ();
    void (*popviewport) ();
    void (*lookat) (Coord vx, Coord vy, Coord vz, Coord px, Coord py, Coord pz, Angle twist);
    void (*scrmask) (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top);
    void (*getscrmask) (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top);
    void (*polf) (long n, const Coord parray[][3]);
    void (*polfi) (long n, const Icoord parray[][3]);
    void (*polfs) (long n, const Scoord parray[][3]);
    void (*polf2) (long n, const Coord parray[][2]);
    void (*polf2i) (long n, const Icoord parray[][2]);
    void (*polf2s) (long n, const Scoord parray[][2]);
    void (*poly) (long n, const Coord parray[][3]);
    void (*polyi) (long n, const Icoord parray[][3]);
    void (*polys) (long n, const Scoord parray[][3]);
    void (*poly2) (long n, const Coord parray[][2]);
    void (*poly2i) (long n, const Icoord parray[][2]);
    void (*poly2s) (long n, const Scoord parray[][2]);
    void (*polymode) (long mode);
    void (*polysmooth) (long mode);
    void (*pmv) (Coord x, Coord y, Coord z);
    void (*pdr) (Coord x, Coord y, Coord z);
    void (*pclos) ();
    void (*pnt) (Coord x, Coord y, Coord z);
    void (*pntsizef) (float n);
    void (*pntsmooth) (unsigned long mode);
    void (*rect) (Coord a, Coord b, Coord c, Coord d);
    void (*rectf) (Coord a, Coord b, Coord c, Coord d);
    void (*v4f) (float vector[4]);
    void (*arc) (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle);
    void (*arcf) (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle);
    void (*circ) (Coord x, Coord y, Coord radius);
    void (*circf) (Coord x, Coord y, Coord radius);
    void (*bgntmesh) ();
    void (*bgnclosedline) ();
    void (*bgncurve) ();
    void (*endcurve) ();
    void (*nurbscurve) (long knotCount, const double *knotList, long offset, const double *ctlArray, long order, long type);
    void (*bgnsurface) ();
    void (*endsurface) ();
    void (*nurbssurface) (long scount, const double *sknot, long tcount, const double *tknot, long soffset, long toffset, const double *ctlArray, long sorder, long torder, long type);
    void (*pwlcurve) (long n, double *dataArray, long byteSize, long type);
    void (*setnurbsproperty) (long property, float value);
    void (*bgnline) ();
    void (*bgnpoint) ();
    void (*endpoint) ();
    void (*bgnpolygon) ();
    void (*bgnqstrip) ();
    void (*crv) (Coord points[4][3]);
    void (*crvn) (long n, Coord points[][3]);
    void (*rcrv) (Coord points[4][4]);
    void (*rcrvn) (long n, Coord points[][4]);
    void (*curveprecision) (short n);
    void (*nmode) (long mode);
    void (*n3f) (float *vector);
    void (*linesmooth) (unsigned long mode);
    void (*linewidthf) (float width);
    void (*deflinestyle) (short n, Linestyle ls);
    void (*setlinestyle) (short n);
    void (*lsrepeat) (long factor);
    void (*lmbind) (short target, short index);
    void (*lmcolor) (long mode);
    void (*move) (Coord x, Coord y, Coord z);
    void (*rmv) (Coord dx, Coord dy, Coord dz);
    void (*rpmv) (Coord dx, Coord dy, Coord dz);
    void (*draw) (Coord x, Coord y, Coord z);
    void (*rdr) (Coord dx, Coord dy, Coord dz);
    void (*rpdr) (Coord dx, Coord dy, Coord dz);
    void (*splf) (long n, Coord parray[][3], Colorindex iarray[]);
    void (*splfi) (long n, Icoord parray[][3], Colorindex iarray[]);
    void (*splfs) (long n, Scoord parray[][3], Colorindex iarray[]);
    void (*splf2) (long n, Coord parray[][2], Colorindex iarray[]);
    void (*splf2i) (long n, Icoord parray[][2], Colorindex iarray[]);
    void (*splf2s) (long n, Scoord parray[][2], Colorindex iarray[]);
    void (*callobj) (Object id);
    void (*tevbind) (long target, long index);
    void (*texbind) (long target, long index);
    void (*t4f) (float vector[4]);
    void (*cmov) (Coord x, Coord y, Coord z);
    void (*lcharstr) (long type, void *str);
    void (*pushattributes) ();
    void (*popattributes) ();
    void (*defpattern) (short n, short size, unsigned short mask[]);
    void (*setpattern) (short index);
} igl_apiList;

extern int
igl_initApiList ();


/*******************************************
    igl_globalsT
********************************************/
#define IGL_MAXWINDOWS          256         /* max open windows per process */
#define IGL_WINDOWWIDTH         640         /* default window width for winopen() */
#define IGL_WINDOWHEIGHT        480         /* default window height for winopen() */
#define IGL_MAXCOLORS           4096        /* max colors available for color[f]() palette */
#define IGL_MAXMULTIMAPCOLORS   256         /* max colors available in each multimap palette (16 palettes) */
#define IGL_MAXLINESTYLES       32          /* max linestyles available */
#define IGL_MAXPATTERNS         32          /* max patterns available */
#define IGL_MAXPOLYVERTICES     256         /* max vertices in a polygon */
#define IGL_MAXCURVESEGMENTS    256         /* max curve segments for crv() and crvn() */
#define IGL_MAXLMDEFS           64          /* max material/light/lightmodel definitions */
#define IGL_MAXDEVICES          548         /* max devices on IrisGL - the last one is VIDEO */
#define IGL_MAXDEVQENTRIES      101         /* max entries in device queue */
#define IGL_MAXTIEDVALUATORS    IGL_MAXDEVICES /* max tie()'d valuators */
#define IGL_MAXTEVDEFS          10          /* max tevdef()'s */
#define IGL_MAXTEXDEFS          10          /* max texdef()'s */
#define IGL_MAXMENUS            20
#define IGL_DEFOBJCHUNKSIZE     1020
typedef struct
{
    /* app related */
    HINSTANCE appInstance;  /* set by WinMain() on Win32, set by XOpenDisplay() on UNIX */
    int initialized;        /* since some things cannot be inited before WGL, we have a helper */
#if PLATFORM_X11
    XVisualInfo visualInfo;
#endif

    /* window related */
    igl_windowT openWindows[IGL_MAXWINDOWS];
    igl_windowT queryWindow;    /* needed for getgdesc() */
    int winOffsetX, winOffsetY; /* border surrounding client rectangle */
    int currentWindow;
    int winPosx, winPosy;
    int winWidth, winHeight;
    int winMinWidth, winMinHeight;
    int winMaxWidth, winMaxHeight;
    int winAspectX, winAspectY;
    long windowFlags; /* reset on winopen() */

    /* color related */
    unsigned long colorPalette[IGL_MAXCOLORS];
    unsigned short colorIndex;

    /* vertex related */
    Linestyle lineStyles[IGL_MAXLINESTYLES];
    short lineStyleIndex;
    GLubyte patterns[IGL_MAXPATTERNS][1024];  /* IGL_MAXPATTERNS of 32x32 in size */
    short patternIndex;
    GLUnurbsObj *nurbsCurve;
    short curveSegments;

    /* matrix related */
    short matrixMode;

    /* light related */
    igl_lmMaterialDefT materialDefs[IGL_MAXLMDEFS];
    short materialIndex;                        /* current materialDef */
    igl_lmLightDefT lightDefs[IGL_MAXLMDEFS];
    short lightIndex;                           /* current lightDef */
    igl_lmLmodelDefT lmodelDefs[IGL_MAXLMDEFS];
    short lmodelIndex;                          /* current lightmodelDef */

    /* device related */
    long devices[IGL_MAXDEVICES];               /* list of devices */
    short deviceQueue[IGL_MAXDEVICES];          /* list of queued devices */
    short eventQueue[IGL_MAXDEVQENTRIES*2];     /* list of queued events */
    int numQueuedEvents;
    Device tiedValuators[IGL_MAXTIEDVALUATORS*2];
    int numTiedValuators;

    /* texture related */
    igl_tevDefT tevDefs[IGL_MAXTEVDEFS];
    igl_texDefT texDefs[IGL_MAXTEXDEFS];

    /* menu related */
    igl_menuT menus[IGL_MAXMENUS];

    /* object related */
    igl_objectT *objects;
    long objChunkSize;
    int currentObjIdx;
    long numObjects;
    long numAllocatedObjects;
    igl_apiList *execApi;
    igl_apiList *objApi;
    igl_apiList *api;   /* equals either execApi or objApi */
} igl_globalsT;

extern igl_globalsT *igl;


#if PLATFORM_WIN32
#   define IGL_NULLWND      NULL
#   define IGL_NULLMENU     NULL
#else
#   define IGL_NULLWND      0
#   define IGL_NULLMENU     0
#endif
#define IGL_CHECKINIT(_ret)                                                                     \
{                                                                                               \
    if (igl->currentWindow == -1 || igl->openWindows[igl->currentWindow].wnd == IGL_NULLWND)    \
    {                                                                                           \
        return (_ret);                                                                          \
    }                                                                                           \
}
#define IGL_CHECKINITV()                                                                        \
{                                                                                               \
    if (igl->currentWindow == -1 || igl->openWindows[igl->currentWindow].wnd == IGL_NULLWND)    \
    {                                                                                           \
        return;                                                                                 \
    }                                                                                           \
}


/* returns current window context (igl_windowT *) */
#define IGL_CTX() (&igl->openWindows[igl->currentWindow])

/* returns current HRC of current window */
#define IGL_HRC(_wptr) (_wptr->hrcs[_wptr->currentLayer])

/* references current window's graphical position (gpos)/character position (cpos) */
#define IGL_GPOS(_n) (igl->openWindows[igl->currentWindow].gpos##_n)
#define IGL_CPOS(_n) (igl->openWindows[igl->currentWindow].cpos##_n)


#if PLATFORM_X11
#   define RGB(r,g,b) ((unsigned long)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#   define GetRValue(_rgb) ((BYTE)(_rgb))
#   define GetGValue(_rgb) ((BYTE)(((WORD)(_rgb)) >> 8))
#   define GetBValue(_rgb) ((BYTE)((_rgb) >> 16))
#endif
/* Win32 API programmers forgot this - get alpha value from an unsigned long */
#define GetAValue(_rgb) ((BYTE)(((WORD)(_rgb)) >> 24))


/* these are needed for gsync() i guess */
typedef BOOL (*WGLSWAPINTERVALEXT) (int interval);
typedef int (*WGLGETSWAPINTERVALEXT) ();

WGLSWAPINTERVALEXT wglSwapIntervalEXT;
WGLGETSWAPINTERVALEXT wglGetSwapIntervalEXT;

#ifdef __cplusplus
}
#endif

#endif  /* IGLCMN_H */


