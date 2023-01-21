#include "igl.h"
#include "iglcmn.h"




/* DESC: getcpos - returns the current character position */
void
getcpos (short *ix, short *iy)
{
    IGL_CHECKINITV ();
    if (ix != NULL)
    {
        *ix = (short)IGL_CPOS (x);
    }
    if (iy != NULL)
    {
        *iy = (short)IGL_CPOS (y);
    }
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmov (Coord x, Coord y, Coord z)
{
    igl->api->cmov (x, y, z);
}

void
iglExec_cmov (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();

    IGL_CPOS (x) = x;
    IGL_CPOS (y) = y;
    IGL_CPOS (z) = z;
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmovi (Icoord x, Icoord y, Icoord z)
{
    igl->api->cmov ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmovs (Scoord x, Scoord y, Scoord z)
{
    igl->api->cmov ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmov2 (Coord x, Coord y)
{
    igl->api->cmov (x, y, 0.0f);
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmov2i (Icoord x, Icoord y)
{
    igl->api->cmov ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: cmov, cmovi, cmovs, cmov2, cmov2i, cmov2s - updates the current character position */
void
cmov2s (Scoord x, Scoord y)
{
    igl->api->cmov ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: charstr, lcharstr - draws a string of characters */
void
charstr (String str)
{
    igl->api->lcharstr (STR_B, str);
}


/* DESC: charstr, lcharstr - draws a string of characters */
void
lcharstr (long type, void *str)
{
    igl->api->lcharstr (type, str);
}

void
iglExec_lcharstr (long type, void *str)
{
    IGL_CHECKINITV ();
    /* only ASCII supported for now! */
    if (type != STR_B)
    {
        return;
    }

#if 0
    glRasterPos3f (IGL_CPOS(x), IGL_CPOS(y), IGL_CPOS(z));
#else
    /* this seems better, at least for EP */
    glRasterPos3f (IGL_CPOS(x)+0.05f, IGL_CPOS(y), IGL_CPOS(z));
#endif
    glListBase (IGL_CTX()->fontBase-32);
    glCallLists (strlen(str), GL_UNSIGNED_BYTE, str);
}


/* DESC: strwidth, lstrwidth - returns the width of the specified text string */
long
strwidth (String str)
{
    return (lstrwidth(STR_B, str));
}


long
lstrwidth (long type, String str)
{
    IGL_CHECKINIT (0);
    /* only ASCII supported for now! */
    if (type != STR_B)
    {
        return (0);
    }

#if PLATFORM_WIN32
    {
        SIZE sz;


        GetTextExtentPoint32 (IGL_CTX ()->hdc, str, strlen (str), &sz);
        return (sz.cx + 4);
    }
#else
    return (strlen (str)*8 + 1);
#endif
}


/* DESC: getheight - returns the maximum character height in the current raster font */
long
getheight ()
{
    IGL_CHECKINIT (0);

#if PLATFORM_WIN32
    {
        TEXTMETRIC tm;
        HFONT font;


        font = GetCurrentObject (IGL_CTX ()->hdc, OBJ_FONT);
        GetTextMetrics (IGL_CTX ()->hdc, &tm);

        return (tm.tmHeight);
    }
#else
    return (16);
#endif
}


/* DESC: font - selects a raster font for drawing text strings */
void
font (short fntnum)
{
    /* TBD */
}


/* DESC: getfont - returns the current raster font number */
long
getfont ()
{
    /* TBD - not until font() is done */
    return (0);
}


/* DESC: getdescender - returns the character characteristics */
long
getdescender ()
{
#if PLATFORM_WIN32
    {
        TEXTMETRIC tm;
        HFONT font;


        font = GetCurrentObject (IGL_CTX ()->hdc, OBJ_FONT);
        GetTextMetrics (IGL_CTX ()->hdc, &tm);

        return (tm.tmDescent + 1);
    }
#else
    /* this seems to be a good approximation */
    return (4);
#endif
}


