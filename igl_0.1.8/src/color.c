#include "igl.h"
#include "iglcmn.h"


/* DESC: color, colorf - sets the color index in the current draw mode */
void
color (Colorindex c)
{
    igl->api->color (c);
}

void
iglExec_color (Colorindex c)
{
    unsigned long clr;
    igl_windowT *wptr;


    IGL_CHECKINITV ();
    wptr = IGL_CTX ();
    if (wptr->flags & IGL_WFLAGS_RGBA)
    {
        return;
    }

    if ((wptr->flags & IGL_WFLAGS_MULTIMAP) && c < IGL_MAXMULTIMAPCOLORS)
    {
        igl->colorIndex = c;
        clr = igl->colorPalette[c + wptr->multimapIndex*IGL_MAXMULTIMAPCOLORS];
        glColor3ubv ((unsigned char *)&clr); /* [0] = r, [1] = g, [2] = b, [3] = alpha (ignored) */
        glClearColor ((unsigned char)GetRValue(clr)/255.0f, (unsigned char)GetGValue(clr)/255.0f,
            (unsigned char)GetBValue(clr)/255.0f, 1.0f);
    }
    else if (!(wptr->flags & IGL_WFLAGS_MULTIMAP) && c < IGL_MAXCOLORS)
    {
        igl->colorIndex = c;
        clr = igl->colorPalette[c];
        glColor3ubv ((unsigned char *)&clr); /* [0] = r, [1] = g, [2] = b, [3] = alpha (ignored) */
        glClearColor ((unsigned char)GetRValue(clr)/255.0f, (unsigned char)GetGValue(clr)/255.0f,
            (unsigned char)GetBValue(clr)/255.0f, 1.0f);
    }
}


/* DESC: color, colorf - sets the color index in the current draw mode */
void
colorf (float c)
{
    igl->api->color ((Colorindex)c);
}


/* DESC: getcolor - returns the current color */
long
getcolor ()
{
    IGL_CHECKINIT (-1);
    return ((IGL_CTX()->flags & IGL_WFLAGS_RGBA) ? -1 : igl->colorIndex);
}


/* DESC: mapcolor - changes a color map entry */
void
mapcolor (Colorindex i, short r, short g, short b)
{
    igl->api->mapcolor (i, r, g, b);
}

void
iglExec_mapcolor (Colorindex i, short r, short g, short b)
{
    igl_windowT *wptr;
    IGL_CHECKINITV ();


    wptr = IGL_CTX ();
    if (i > IGL_MAXCOLORS || ((wptr->flags & IGL_WFLAGS_MULTIMAP) && i > IGL_MAXMULTIMAPCOLORS))
    {
        return;
    }
    if (wptr->flags & IGL_WFLAGS_MULTIMAP)
    {
        i += wptr->multimapIndex*IGL_MAXMULTIMAPCOLORS;
    }
    igl->colorPalette[i] = RGB (r, g, b);
}


/* DESC: getmcolor - gets a copy of the RGB values for a color map entry */
void
getmcolor (Colorindex i, short *r, short *g, short *b)
{
    igl_windowT *wptr;
    IGL_CHECKINITV ();

    wptr = IGL_CTX ();
    if (i > IGL_MAXCOLORS || ((wptr->flags & IGL_WFLAGS_MULTIMAP) && i > IGL_MAXMULTIMAPCOLORS) ||
        r == NULL || g == NULL || b == NULL)
    {
        return;
    }

    if (wptr->flags & IGL_WFLAGS_MULTIMAP)
    {
        i += wptr->multimapIndex*IGL_MAXMULTIMAPCOLORS;
    }
    *r = GetRValue (igl->colorPalette[i]);
    *g = GetGValue (igl->colorPalette[i]);
    *b = GetBValue (igl->colorPalette[i]);
}


/* DESC: setshade - obsolete routine */
void
setshade (Colorindex shade)
{
    /* same as color() */
    igl->api->color (shade);
}


/* DESC: getshade - obsolete routine */
long
getshade ()
{
    /* same as getcolor() */
    return (igl->colorIndex);
}


/* DESC: cpack - specifies RGBA color with a single packed 32-bit integer */
void
cpack (unsigned long color)
{
    igl->api->cpack (color);
}

void
iglExec_cpack (unsigned long color)
{
    IGL_CHECKINITV ();
    if (IGL_CTX()->flags & IGL_WFLAGS_RGBA)
    {
        /* the color is in ABGR mode */
        glColor4ub (((unsigned char *)&color)[3],
            ((unsigned char *)&color)[2],
            ((unsigned char *)&color)[1],
            ((unsigned char *)&color)[0]);
    }
}


/* DESC: wmpack - specifies RGBA writemask with a single packed integer */
void
wmpack (unsigned long color)
{
    /*
        OpenGL doesn't support changes to individual bits - color mask can be either
        enabled ot disabled with glColorMask(). note that color is in ABGR mode
    */
    glColorMask (((unsigned char *)&color)[3] != 0 ? GL_TRUE : GL_FALSE,
        ((unsigned char *)&color)[2] != 0 ? GL_TRUE : GL_FALSE,
        ((unsigned char *)&color)[1] != 0 ? GL_TRUE : GL_FALSE,
        ((unsigned char *)&color)[0] != 0 ? GL_TRUE : GL_FALSE);
}


/* DESC: RGBcolor - sets the current color in RGB mode */
void
RGBcolor (short r, short g, short b)
{
    igl->api->RGBcolor (r, g, b);
}

void
iglExec_RGBcolor (short r, short g, short b)
{
    IGL_CHECKINITV ();
    if (IGL_CTX()->flags & IGL_WFLAGS_RGBA)
    {
        glColor3s (r, g, b);
    }
}


/* DESC: gRGBcolor - gets the current RGB color values */
void
gRGBcolor (short *r, short *g, short *b)
{
    float color[4];


    IGL_CHECKINITV ();

    if ((IGL_CTX()->flags & IGL_WFLAGS_RGBA) && r != NULL && g != NULL && b != NULL)
    {
        glGetFloatv (GL_CURRENT_COLOR, color);

        /* TBD: is this type of clamp ok? Mesa does this a bit differently */
        *r = (short)(color[0] * 32767.0f);
        *g = (short)(color[1] * 32767.0f);
        *b = (short)(color[2] * 32767.0f);
    }
}


/* DESC: afunction - specify alpha test function */
void
afunction (long ref, long func)
{
    igl->api->afunction (ref, func);
}

void
iglExec_afunction (long ref, long func)
{
    int oglFunc;


    IGL_CHECKINITV ();

    switch (func)
    {
        case AF_LESS:
            oglFunc = GL_LESS;
            break;

        case AF_EQUAL:
            oglFunc = GL_EQUAL;
            break;

        case AF_LEQUAL:
            oglFunc = GL_LEQUAL;
            break;

        case AF_GREATER:
            oglFunc = GL_GREATER;
            break;

        case AF_NOTEQUAL:
            oglFunc = GL_NOTEQUAL;
            break;

        case AF_GEQUAL:
            oglFunc = GL_GEQUAL;
            break;

        case AF_ALWAYS:
            oglFunc = GL_ALWAYS;
            break;

        case AF_NEVER:
            oglFunc = GL_NEVER;
            break;

        default:
            return;
    }

    /* ref is 0-255 in IrisGL, 0.0f-1.0f in OpenGL */
    glAlphaFunc (oglFunc, (float)ref/255.0f);
}


/* DESC: blendcolor - specifies a constant color for blending */
void
blendcolor (float r, float g, float b, float a)
{
    IGL_CHECKINITV ();

    /* does OpenGL support blend color specification? */
}


/* DESC: blendfunction - computes a blended color value for a pixel */
void
blendfunction (long sfactor, long dfactor)
{
    igl->api->blendfunction (sfactor, dfactor);
}

void
iglExec_blendfunction (long sfactor, long dfactor)
{
    int osf, odf;


    IGL_CHECKINITV ();

    /* oh the ugliness... */
    if (sfactor == BF_ZERO) osf = GL_ZERO;
    else if (sfactor == BF_ONE) osf = GL_ONE;
    else if (sfactor == BF_DC) osf = GL_DST_COLOR;
    else if (sfactor == BF_MDC) osf = GL_ONE_MINUS_DST_COLOR;
    else if (sfactor == BF_SA) osf = GL_SRC_ALPHA;
    else if (sfactor == BF_MSA) osf = GL_ONE_MINUS_SRC_ALPHA;
    else if (sfactor == BF_DA) osf = GL_DST_ALPHA;
    else if (sfactor == BF_MDA) osf = GL_ONE_MINUS_DST_ALPHA;
#if 0   /* should we support this? */
    else if (sfactor == BF_MIN_SA_MDA) osf = GL_ONE;
    else if (sfactor == BF_CC) osf = GL_ONE;
    else if (sfactor == BF_MCC) osf = GL_ONE;
    else if (sfactor == BF_CA) osf = GL_ONE;
    else if (sfactor == BF_MCA) osf = GL_ONE;
    else if (sfactor == BF_MIN) osf = GL_ONE;
    else if (sfactor == BF_MAX) osf = GL_ONE;
#endif
    else osf = GL_DST_COLOR;

    if (dfactor == BF_ZERO) odf = GL_ZERO;
    else if (dfactor == BF_ONE) odf = GL_ONE;
    else if (dfactor == BF_SC) odf = GL_SRC_COLOR;
    else if (dfactor == BF_MSC) odf = GL_ONE_MINUS_SRC_COLOR;
    else if (dfactor == BF_SA) odf = GL_SRC_ALPHA;
    else if (dfactor == BF_MSA) odf = GL_ONE_MINUS_SRC_ALPHA;
    else if (dfactor == BF_DA) odf = GL_DST_ALPHA;
    else if (dfactor == BF_MDA) odf = GL_ONE_MINUS_DST_ALPHA;
#if 0   /* should we support this? */
    else if (dfactor == BF_CC) odf = GL_ONE;
    else if (dfactor == BF_MCC) odf = GL_ONE;
    else if (dfactor == BF_CA) odf = GL_ONE;
    else if (dfactor == BF_MCA) odf = GL_ONE;
#endif
    else odf = GL_SRC_COLOR;


    if (sfactor == GL_ONE && dfactor == GL_ZERO)
    {
        /* is this how do we disable blending with IrisGL? */
        glDisable (GL_BLEND);
    }
    else
    {
        glEnable (GL_BLEND);
        glBlendFunc (osf, odf);
        /* man page says to force logicop(LO_SRC) = glLogicOp(GL_COPY) */
        glLogicOp (GL_COPY);
    }
}


/* DESC: dither - controls the dithering of pixels */
void
dither (long mode)
{
    igl->api->dither (mode);
}

void
iglExec_dither (long mode)
{
    IGL_CHECKINITV ();

    if (mode == DT_OFF)
    {
        glDisable (GL_DITHER);
    }
    else
    {
        glEnable (GL_DITHER);
    }
}


/* DESC: fogvertex - specify atmospheric fogging effects */
void
fogvertex (long mode, float *params)
{
    igl->api->fogvertex (mode, params);
}

void
iglExec_fogvertex (long mode, float *params)
{
    float fogClr[4];
    float odensity;


    IGL_CHECKINITV ();

    if (params == NULL && mode != FG_OFF && mode != FG_ON)
    {
        return;
    }

    switch (mode)
    {
        case FG_OFF:
            glDisable (GL_FOG);
            break;

        case FG_ON:
            glEnable (GL_FOG);
            break;

        case FG_VTX_EXP:
        case FG_PIX_EXP:
            glFogi (GL_FOG_MODE, GL_EXP);
            glHint (GL_FOG_HINT, (mode == FG_VTX_EXP) ? GL_FASTEST : GL_NICEST);
            break;

        case FG_VTX_LIN:
        case FG_PIX_LIN:
            glFogi (GL_FOG_MODE, GL_LINEAR);
            glHint (GL_FOG_HINT, (mode == FG_VTX_LIN) ? GL_FASTEST : GL_NICEST);
            break;

        case FG_VTX_EXP2:
        case FG_PIX_EXP2:
            glFogi (GL_FOG_MODE, GL_EXP2);
            glHint (GL_FOG_HINT, (mode == FG_VTX_EXP2) ? GL_FASTEST : GL_NICEST);
            break;
    }

    /* these expect four params[] - density, r, g, b */
    if (mode == FG_VTX_EXP || mode == FG_PIX_EXP || mode == FG_VTX_EXP2 || mode == FG_PIX_EXP2)
    {
        odensity = (mode == FG_VTX_EXP || FG_PIX_EXP) ?
            -params[0]*(float)log (1/255) : params[0]*(float)sqrt (1/255);
        glFogf (GL_FOG_DENSITY, odensity);

        /* GL_FOG_COLOR requires RGBA color */
        memcpy (fogClr, &params[1], 3*sizeof(float));
        fogClr[3] = 0;
        glFogfv (GL_FOG_COLOR, fogClr);
    }
    /* and these five - near, far, r, g, b */
    else if (mode == FG_VTX_LIN || mode == FG_PIX_LIN)
    {
        glFogf (GL_FOG_START, params[0]);
        glFogf (GL_FOG_END, params[1]);

        /* GL_FOG_COLOR requires RGBA color */
        memcpy (fogClr, &params[2], 3*sizeof(float));
        fogClr[3] = 0;
        glFogfv (GL_FOG_COLOR, fogClr);
    }
}


/* DESC: logicop - specifies a logical operation for pixel writes */
void
logicop (long opcode)
{
    igl->api->logicop (opcode);
}

void
iglExec_logicop (long opcode)
{
    int glop;


    IGL_CHECKINITV ();

    switch (opcode)
    {
        case LO_ZERO:
            glop = GL_CLEAR;
            break;

        case LO_ONE:
            glop = GL_SET;
            break;

        case LO_SRC:
            glop = GL_COPY;
            break;

        case LO_NSRC:
            glop = GL_COPY_INVERTED;
            break;

        case LO_DST:
            glop = GL_NOOP;
            break;

        case LO_NDST:
            glop = GL_INVERT;
            break;

        case LO_AND:
            glop = GL_AND;
            break;

        case LO_NAND:
            glop = GL_NAND;
            break;

        case LO_OR:
            glop = GL_OR;
            break;

        case LO_NOR:
            glop = GL_NOR;
            break;

        case LO_XOR:
            glop = GL_XOR;
            break;

        case LO_XNOR:
            glop = GL_EQUIV;
            break;

        case LO_ANDR:
            glop = GL_AND_REVERSE;
            break;

        case LO_ANDI:
            glop = GL_AND_INVERTED;
            break;

        case LO_ORR:
            glop = GL_OR_REVERSE;
            break;

        case LO_ORI:
            glop = GL_OR_INVERTED;
            break;

        default:
            return;
    }

    glLogicOp (glop);
    if (opcode != LO_SRC)
    {
        /* man page says to force blending to (BF_ONE, BF_ZERO) = (GL_ONE, GL_ZERO) */
        glBlendFunc (GL_ONE, GL_ZERO);
    }
}


/* DESC: onemap - organizes the color map as one large map */
void
onemap ()
{
    IGL_CHECKINITV ();
    IGL_CTX ()->newFlags &= ~IGL_WFLAGS_MULTIMAP;
}


/* DESC: multimap - organizes the color map as a number of smaller maps */
void
multimap ()
{
    IGL_CHECKINITV ();
    IGL_CTX ()->newFlags |= IGL_WFLAGS_MULTIMAP;
}


/* DESC: getcmmode - returns the current color map mode */
Boolean
getcmmode ()
{
    IGL_CHECKINIT (TRUE);   /* TRUE means onemap() */
    return ((IGL_CTX ()->flags & IGL_WFLAGS_MULTIMAP) ? FALSE : TRUE);
}


/* DESC: setmap - selects one of the small color maps provided by multimap mode */
void
setmap (short mapnum)
{
    IGL_CHECKINITV ();
    if ((IGL_CTX ()->flags & IGL_WFLAGS_MULTIMAP) && mapnum >= 0 && mapnum < 16)
    {
        IGL_CTX ()->multimapIndex = (unsigned char)mapnum;
    }
}


/* DESC: getmap - returns the number of the current color map */
long
getmap ()
{
    IGL_CHECKINIT (0);
    return ((IGL_CTX ()->flags & IGL_WFLAGS_MULTIMAP) ? (long)IGL_CTX ()->multimapIndex : 0);
}


#if 0
____________________________ c functions ____________________________
() {}
#endif
/* DESC: c3f, c3i, c3s, c4f, c4i, c4s - sets the RGB (or RGBA) values for the current color vector */
void
c4f (float cv[4])
{
    igl->api->c4f (cv);
}

void
iglExec_c4f (float cv[4])
{
    IGL_CHECKINITV ();
    if (IGL_CTX()->flags & IGL_WFLAGS_RGBA)
    {
        glColor4fv (cv);
    }
}


void
c4i (int cv[4])
{
    igl->api->c4i (cv);
}

void
iglExec_c4i (int cv[4])
{
    IGL_CHECKINITV ();
    if (IGL_CTX()->flags & IGL_WFLAGS_RGBA)
    {
        glColor4iv (cv);
    }
}


void
c4s (short cv[4])
{
    igl->api->c4s (cv);
}

void
iglExec_c4s (short cv[4])
{
    IGL_CHECKINITV ();
    if (IGL_CTX()->flags & IGL_WFLAGS_RGBA)
    {
        glColor4sv (cv);
    }
}


void
c3f (float cv[3])
{
    float cv4[4];


    cv4[0] = cv[0];
    cv4[1] = cv[1];
    cv4[2] = cv[2];
    cv4[3] = 1.0f;
    igl->api->c4f (cv4);
}

void
c3i (int cv[3])
{
    int cv4[4];


    cv4[0] = cv[0];
    cv4[1] = cv[1];
    cv4[2] = cv[2];
    cv4[3] = 2147483647;    /* should we use MAX_INT? */
    igl->api->c4i (cv4);
}


void
c3s (short cv[3])
{
    short cv4[4];


    cv4[0] = cv[0];
    cv4[1] = cv[1];
    cv4[2] = cv[2];
    cv4[3] = 32767;
    igl->api->c4s (cv4);
}


#if 0
____________________________ pixel functions ____________________________
() {}
#endif
#if 0
void
pixmode (long mode, long value)
{
    int validValue;
    IGL_CHECKINITV ();


    validValue = 0;
    switch (mode)
    {
        case PM_SHIFT:
            validValue = (value == 0 || value == -1 || value == 1 || value == -4 || value == 4 ||
                value == -8 || value == 8);
            break;

        case PM_EXPAND:
            validValue = (value == 0 || value == 1);
            break;
    }
}
#endif


