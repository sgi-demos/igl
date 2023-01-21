#include "igl.h"
#include "iglcmn.h"




/* DESC: getgpos - gets the current graphics position */
void
getgpos (Coord *fx, Coord *fy, Coord *fz, Coord *fw)
{
    IGL_CHECKINITV ();

    if (fx != NULL)
    {
        *fx = IGL_GPOS (x);
    }
    if (fy != NULL)
    {
        *fy = IGL_GPOS (y);
    }
    if (fz != NULL)
    {
        *fz = IGL_GPOS (z);
    }
    if (fw != NULL)
    {
        *fw = IGL_GPOS (w);
    }
}


#if 0
____________________________ move functions ____________________________
() {}
#endif
/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
move (Coord x, Coord y, Coord z)
{
    igl->api->move (x, y, z);
}

void
iglExec_move (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();

    IGL_GPOS (x) = x;
    IGL_GPOS (y) = y;
    IGL_GPOS (z) = z;
}


/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
movei (Icoord x, Icoord y, Icoord z)
{
    igl->api->move ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
moves (Scoord x, Scoord y, Scoord z)
{
    igl->api->move ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
move2 (Coord x, Coord y)
{
    igl->api->move (x, y, 0.0f);
}


/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
move2i (Icoord x, Icoord y)
{
    igl->api->move ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: move, movei, moves, move2, move2i, move2s - moves the current graphics
         position to a specified point */
void
move2s (Scoord x, Scoord y)
{
    igl->api->move ((Coord)x, (Coord)y, 0.0f);
}


#if 0
____________________________ rmv functions ____________________________
() {}
#endif
/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmv (Coord dx, Coord dy, Coord dz)
{
    igl->api->rmv (dx, dy, dz);
}

void
iglExec_rmv (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKINITV ();

    IGL_GPOS (x) += dx;
    IGL_GPOS (y) += dy;
    IGL_GPOS (z) += dz;
}


/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmvi (Icoord dx, Icoord dy, Icoord dz)
{
    igl->api->rmv ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmvs (Coord dx, Coord dy, Coord dz)
{
    igl->api->rmv ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmv2 (Coord dx, Coord dy)
{
    igl->api->rmv (dx, dy, 0.0f);
}


/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmv2i (Icoord dx, Icoord dy)
{
    igl->api->rmv ((Coord)dx, (Coord)dy, 0.0f);
}


/* DESC: rmv, rmvi, rmvs, rmv2, rmv2i, rmv2s - relative move */
void
rmv2s (Scoord dx, Scoord dy)
{
    igl->api->rmv ((Coord)dx, (Coord)dy, 0.0f);
}


#if 0
____________________________ rpmv functions ____________________________
() {}
#endif
/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmv (Coord dx, Coord dy, Coord dz)
{
    igl->api->rpmv (dx, dy, dz);
}

void
iglExec_rpmv (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKINITV ();

    glBegin (GL_POLYGON);
        glVertex3f (IGL_GPOS(x), IGL_GPOS(y), IGL_GPOS(z));
        IGL_GPOS (x) += dx;
        IGL_GPOS (y) += dy;
        IGL_GPOS (z) += dz;
}


/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmvi (Icoord dx, Icoord dy, Icoord dz)
{
    igl->api->rpmv ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmvs (Coord dx, Coord dy, Coord dz)
{
    igl->api->rpmv ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmv2 (Coord dx, Coord dy)
{
    igl->api->rpmv (dx, dy, 0.0f);
}


/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmv2i (Icoord dx, Icoord dy)
{
    igl->api->rpmv ((Coord)dx, (Coord)dy, 0.0f);
}


/* DESC: rpmv, rpmvi, rpmvs, rpmv2, rpmv2i, rpmv2s - relative polygon move */
void
rpmv2s (Scoord dx, Scoord dy)
{
    igl->api->rpmv ((Coord)dx, (Coord)dy, 0.0f);
}


#if 0
____________________________ draw functions ____________________________
() {}
#endif
/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
draw (Coord x, Coord y, Coord z)
{
    igl->api->draw (x, y, z);
}

void
iglExec_draw (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();

    glBegin (GL_LINES);
        glVertex3f (IGL_GPOS(x), IGL_GPOS(y), IGL_GPOS(z));
        glVertex3f (x, y, z);
    glEnd ();

    IGL_GPOS (x) = x;
    IGL_GPOS (y) = y;
    IGL_GPOS (z) = z;
}


/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
drawi (Icoord x, Icoord y, Icoord z)
{
    igl->api->draw ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
draws (Scoord x, Scoord y, Scoord z)
{
    igl->api->draw ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
draw2 (Coord x, Coord y)
{
    igl->api->draw (x, y, 0.0f);
}


/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
draw2i (Icoord x, Icoord y)
{
    igl->api->draw ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: draw, drawi, draws, draw2, draw2i, draw2s - draws a line */
void
draw2s (Scoord x, Scoord y)
{
    igl->api->draw ((Coord)x, (Coord)y, 0.0f);
}



#if 0
____________________________ rdr functions ____________________________
() {}
#endif
/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdr (Coord dx, Coord dy, Coord dz)
{
    igl->api->rdr (dx, dy, dz);
}

void
iglExec_rdr (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKINITV ();

    glBegin (GL_LINES);
        glVertex3f (IGL_GPOS(x), IGL_GPOS(y), IGL_GPOS(z));
        IGL_GPOS (x) += dx; IGL_GPOS (y) += dy; IGL_GPOS(z) += dz;
        glVertex3f (IGL_GPOS(x), IGL_GPOS(y), IGL_GPOS(z));
    glEnd ();
}


/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdri (Icoord dx, Icoord dy, Icoord dz)
{
    igl->api->rdr ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdrs (Scoord dx, Scoord dy, Scoord dz)
{
    igl->api->rdr ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdr2 (Coord dx, Coord dy)
{
    igl->api->rdr (dx, dy, 0.0f);
}


/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdr2i (Icoord dx, Icoord dy)
{
    igl->api->rdr ((Coord)dx, (Coord)dy, 0.0f);
}


/* DESC: rdr, rdri, rdrs, rdr2, rdr2i, rdr2s - relative draw */
void
rdr2s (Scoord dx, Scoord dy)
{
    igl->api->rdr ((Coord)dx, (Coord)dy, 0.0f);
}


#if 0
____________________________ rpdr functions ____________________________
() {}
#endif
/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdr (Coord dx, Coord dy, Coord dz)
{
    igl->api->rpdr (dx, dy, dz);
}

void
iglExec_rpdr (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKINITV ();

    glVertex3f (IGL_GPOS(x), IGL_GPOS(y), IGL_GPOS(z));
    IGL_GPOS (x) += dx; IGL_GPOS (y) += dy; IGL_GPOS(z) += dz;
}


/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdri (Icoord dx, Icoord dy, Icoord dz)
{
    igl->api->rpdr ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdrs (Scoord dx, Scoord dy, Scoord dz)
{
    igl->api->rpdr ((Coord)dx, (Coord)dy, (Coord)dz);
}


/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdr2 (Coord dx, Coord dy)
{
    igl->api->rpdr (dx, dy, 0.0f);
}


/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdr2i (Icoord dx, Icoord dy)
{
    igl->api->rpdr ((Coord)dx, (Coord)dy, 0.0f);
}


/* DESC: rpdr, rpdri, rpdrs, rpdr2, rpdr2i, rpdr2s - relative polygon draw */
void
rpdr2s (Scoord dx, Scoord dy)
{
    igl->api->rpdr ((Coord)dx, (Coord)dy, 0.0f);
}


#if 0
____________________________ splf functions ____________________________
() {}
#endif
/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splf (long n, Coord parray[][3], Colorindex iarray[])
{
    igl->api->splf (n, parray, iarray);
}

void
iglExec_splf (long n, Coord parray[][3], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex3fv (&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = parray[0][0];
    IGL_GPOS (y) = parray[0][1];
    IGL_GPOS (z) = parray[0][2];
}


/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splfi (long n, Icoord parray[][3], Colorindex iarray[])
{
    igl->api->splfi (n, parray, iarray);
}

void
iglExec_splfi (long n, Icoord parray[][3], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex3iv ((GLint *)&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = (float)parray[0][0];
    IGL_GPOS (y) = (float)parray[0][1];
    IGL_GPOS (z) = (float)parray[0][2];
}


/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splfs (long n, Scoord parray[][3], Colorindex iarray[])
{
    igl->api->splfs (n, parray, iarray);
}

void
iglExec_splfs (long n, Scoord parray[][3], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex3sv (&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = (float)parray[0][0];
    IGL_GPOS (y) = (float)parray[0][1];
    IGL_GPOS (z) = (float)parray[0][2];
}


/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splf2 (long n, Coord parray[][2], Colorindex iarray[])
{
    igl->api->splf2 (n, parray, iarray);
}

void
iglExec_splf2 (long n, Coord parray[][2], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex2fv (&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = parray[0][0];
    IGL_GPOS (y) = parray[0][1];
}


/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splf2i (long n, Icoord parray[][2], Colorindex iarray[])
{
    igl->api->splf2i (n, parray, iarray);
}

void
iglExec_splf2i (long n, Icoord parray[][2], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex2iv ((GLint *)&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = (float)parray[0][0];
    IGL_GPOS (y) = (float)parray[0][1];
}


/* DESC: splf, splfi, splfs, splf2, splf2i, splf2s - draws a shaded filled polygon */
void
splf2s (long n, Scoord parray[][2], Colorindex iarray[])
{
    igl->api->splf2s (n, parray, iarray);
}

void
iglExec_splf2s (long n, Scoord parray[][2], Colorindex iarray[])
{
    /* TBD: this only functions in color-map mode */
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            color (iarray[i]);
            glVertex2sv (&parray[i][0]);
        }
    glEnd ();

    IGL_GPOS (x) = (float)parray[0][0];
    IGL_GPOS (y) = (float)parray[0][1];
}


