#include "igl.h"
#include "iglcmn.h"




#if 0
____________________________ polf functions ____________________________
() {}
#endif
/* DESC: polf, polfi, polfs, polf2, polf2i, polf2s - draws a filled polygon */
void
polf (long n, const Coord parray[][3])
{
    igl->api->polf (n, parray);
}

void
iglExec_polf (long n, const Coord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex3fv (&parray[i][0]);
        }
    glEnd ();
}


void
polfi (long n, const Icoord parray[][3])
{
    igl->api->polfi (n, parray);
}

void
iglExec_polfi (long n, const Icoord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex3iv ((GLint *)&parray[i][0]);
        }
    glEnd ();
}


void
polfs (long n, const Scoord parray[][3])
{
    igl->api->polfs (n, parray);
}

void
iglExec_polfs (long n, const Scoord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex3sv (&parray[i][0]);
        }
    glEnd ();
}


void
polf2 (long n, const Coord parray[][2])
{
    igl->api->polf2 (n, parray);
}

void
iglExec_polf2 (long n, const Coord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex2fv (&parray[i][0]);
        }
    glEnd ();
}


void
polf2i (long n, const Icoord parray[][2])
{
    igl->api->polf2i (n, parray);
}

void
iglExec_polf2i (long n, const Icoord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex2iv ((GLint *)&parray[i][0]);
        }
    glEnd ();
}


void
polf2s (long n, const Scoord parray[][2])
{
    igl->api->polf2s (n, parray);
}

void
iglExec_polf2s (long n, const Scoord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_POLYGON);
        for (i=0; i < n; i ++)
        {
            glVertex2sv (&parray[i][0]);
        }
    glEnd ();
}


#if 0
____________________________ poly functions ____________________________
() {}
#endif
/* DESC: poly, polyi, polys, poly2, poly2i, poly2s - outlines a polygon */
void
poly (long n, const Coord parray[][3])
{
    igl->api->poly (n, parray);
}

void
iglExec_poly (long n, const Coord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex3fv (&parray[i][0]);
        }
    glEnd ();
}


void
polyi (long n, const Icoord parray[][3])
{
    igl->api->polyi (n, parray);
}

void
iglExec_polyi (long n, const Icoord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex3iv ((GLint *)&parray[i][0]);
        }
    glEnd ();
}


void
polys (long n, const Scoord parray[][3])
{
    igl->api->polys (n, parray);
}

void
iglExec_polys (long n, const Scoord parray[][3])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex3sv (&parray[i][0]);
        }
    glEnd ();
}


void
poly2 (long n, const Coord parray[][2])
{
    igl->api->poly2 (n, parray);
}

void
iglExec_poly2 (long n, const Coord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex2fv (&parray[i][0]);
        }
    glEnd ();
}


void
poly2i (long n, const Icoord parray[][2])
{
    igl->api->poly2i (n, parray);
}

void
iglExec_poly2i (long n, const Icoord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex2iv ((GLint *)&parray[i][0]);
        }
    glEnd ();
}


void
poly2s (long n, const Scoord parray[][2])
{
    igl->api->poly2s (n, parray);
}

void
iglExec_poly2s (long n, const Scoord parray[][2])
{
    int i;


    IGL_CHECKINITV ();
    if (n < 2 || n > IGL_MAXPOLYVERTICES)
    {
        return;
    }

    glBegin (GL_LINE_LOOP);
        for (i=0; i < n; i ++)
        {
            glVertex2sv (&parray[i][0]);
        }
    glEnd ();
}


/* DESC: polymode - control the rendering of polygons */
void
polymode (long mode)
{
    igl->api->polymode (mode);
}

void
iglExec_polymode (long mode)
{
    IGL_CHECKINITV ();

    if (mode == PYM_FILL)
    {
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (mode == PYM_POINT)
    {
        glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
    }
    /* PYM_HOLLOW not supported by OpenGL */
    else if (mode == PYM_LINE || mode == PYM_HOLLOW)
    {
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    }
}


/* DESC: polysmooth - specify antialiasing of polygons */
void
polysmooth (long mode)
{
    igl->api->polysmooth (mode);
}

void
iglExec_polysmooth (long mode)
{
    IGL_CHECKINITV ();

    if (mode == PYSM_OFF)
    {
        glDisable (GL_POLYGON_SMOOTH);
    }
    /* PYSM_SHRINK not supported in OpenGL */
    else if (mode == PYSM_ON || mode == PYSM_SHRINK)
    {
        glEnable (GL_POLYGON_SMOOTH);
    }
}


#if 0
____________________________ pmv functions ____________________________
() {}
#endif
/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmv (Coord x, Coord y, Coord z)
{
    igl->api->pmv (x, y, z);
}

void
iglExec_pmv (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();
    glBegin (GL_POLYGON);
        glVertex3f (x, y, z);
}


/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmvi (Icoord x, Icoord y, Icoord z)
{
    igl->api->pmv ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmvs (Scoord x, Scoord y, Scoord z)
{
    igl->api->pmv ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmv2 (Coord x, Coord y)
{
    igl->api->pmv (x, y, 0.0f);
}

/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmv2i (Icoord x, Icoord y)
{
    igl->api->pmv ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: pmv, pmvi, pmvs, pmv2, pmv2i, pmv2s - specifies the first point of a polygon */
void
pmv2s (Scoord x, Scoord y)
{
    igl->api->pmv ((Coord)x, (Coord)y, 0.0f);
}


#if 0
____________________________ pdr functions ____________________________
() {}
#endif
/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdr (Coord x, Coord y, Coord z)
{
    igl->api->pdr (x, y, z);
}

void
iglExec_pdr (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();
    glVertex3f (x, y, z);
}


/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdri (Icoord x, Icoord y, Icoord z)
{
    igl->api->pdr ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdrs (Scoord x, Scoord y, Scoord z)
{
    igl->api->pdr ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdr2 (Coord x, Coord y)
{
    igl->api->pdr (x, y, 0.0f);
}


/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdr2i (Coord x, Coord y)
{
    igl->api->pdr ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: pdr, pdri, pdrs, pdr2, pdr2i, pdr2s - specifies the next point of a polygon */
void
pdr2s (Coord x, Coord y)
{
    igl->api->pdr ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: pclos - closes a filled polygon */
void
pclos ()
{
    igl->api->pclos ();
}

void
iglExec_pclos ()
{
    IGL_CHECKINITV ();
    glEnd ();
}


/* DESC: spclos - obsolete routine */
void
spclos ()
{
    igl->api->pclos ();
}


#if 0
____________________________ pnt functions ____________________________
() {}
#endif
/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnt (Coord x, Coord y, Coord z)
{
    igl->api->pnt (x, y, z);
}

void
iglExec_pnt (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();
    glBegin (GL_POINTS);
        glVertex3f (x, y, z);
    glEnd ();
}


/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnti (Icoord x, Icoord y, Icoord z)
{
    igl->api->pnt ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnts (Scoord x, Scoord y, Scoord z)
{
    igl->api->pnt ((Coord)x, (Coord)y, (Coord)z);
}


/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnt2 (Coord x, Coord y)
{
    igl->api->pnt (x, y, 0.0f);
}


/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnt2i (Icoord x, Icoord y)
{
    igl->api->pnt ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: pnt, pnti, pnts, pnt2, pnt2i, pnt2s - draws a point */
void
pnt2s (Scoord x, Scoord y)
{
    igl->api->pnt ((Coord)x, (Coord)y, 0.0f);
}


/* DESC: pntsize, pntsizef - specifies size of points */
void
pntsizef (float n)
{
    igl->api->pntsizef (n);
}

void
iglExec_pntsizef (float n)
{
    IGL_CHECKINITV ();
    glPointSize (n);
}


/* DESC: pntsize, pntsizef - specifies size of points */
void
pntsize (short n)
{
    igl->api->pntsizef ((float)n);
}


/* DESC: pntsmooth - specify antialiasing of points */
void
pntsmooth (unsigned long mode)
{
    igl->api->pntsmooth (mode);
}

void
iglExec_pntsmooth (unsigned long mode)
{
    IGL_CHECKINITV ();

    if (mode == SMP_OFF)
    {
        glDisable (GL_POINT_SMOOTH);
    }
    else if (mode & SMP_ON)
    {
        glEnable (GL_POINT_SMOOTH);

        if (mode & SMP_SMOOTHER)
        {
            glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
        }
        else
        {
            glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
        }
    }
}


#if 0
____________________________ rect functions ____________________________
() {}
#endif
/* DESC: rect, recti, rects - outlines a rectangular region */
void
rect (Coord a, Coord b, Coord c, Coord d)
{
    igl->api->rect (a, b, c, d);
}

void
iglExec_rect (Coord a, Coord b, Coord c, Coord d)
{
    IGL_CHECKINITV ();
    glBegin (GL_LINE_LOOP);
        glVertex2f (a, b);
        glVertex2f (c, b);
        glVertex2f (c, d);
        glVertex2f (a, d);
    glEnd ();
}


void
recti (Icoord a, Icoord b, Icoord c, Icoord d)
{
    igl->api->rect ((Coord)a, (Coord)b, (Coord)c, (Coord)d);
}


void
rects (Scoord a, Scoord b, Scoord c, Scoord d)
{
    igl->api->rect ((Coord)a, (Coord)b, (Coord)c, (Coord)d);
}


/* DESC: rectf, rectfi, rectfs - fills a rectangular area */
void
rectf (Coord a, Coord b, Coord c, Coord d)
{
    igl->api->rectf (a, b, c, d);
}

void
iglExec_rectf (Coord a, Coord b, Coord c, Coord d)
{
    IGL_CHECKINITV ();
    glRectf (a, b, c, d);
}


void
rectfi (Icoord a, Icoord b, Icoord c, Icoord d)
{
    igl->api->rectf ((Coord)a, (Coord)b, (Coord)c, (Coord)d);
}


void
rectfs (Scoord a, Scoord b, Scoord c, Scoord d)
{
    igl->api->rectf ((Coord)a, (Coord)b, (Coord)c, (Coord)d);
}


#if 0
____________________________ v functions ____________________________
() {}
#endif
/* DESC: v2d, v2f, v2i, v2s, v3d, v3f, v3i, v3s, v4d, v4f, v4i, v4s -
         transfers a 2-D, 3-D, or 4-D vertex to the graphics pipe */
void
v4f (float vector[4])
{
    igl->api->v4f (vector);
}

void
iglExec_v4f (float vector[4])
{
    IGL_CHECKINITV ();
    glVertex4fv (vector);
}


void
v4d (double vector[4])
{
    float vector4[4];


    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->v4f (vector4);
}


void
v4i (int vector[4])
{
    float vector4[4];


    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->v4f (vector4);
}


void
v4s (short vector[4])
{
    float vector4[4];


    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->v4f (vector4);
}


void
v2f (float vector[2])
{
    float vector4[4];

    vector4[0] = vector[0];
    vector4[1] = vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v2d (double vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v2i (int vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v2s (short vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v3f (float vector[3])
{
    float vector4[4];

    vector4[0] = vector[0];
    vector4[1] = vector[1];
    vector4[2] = vector[2];
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v3d (double vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v3i (int vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


void
v3s (short vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->v4f (vector4);
}


#if 0
____________________________ arc functions ____________________________
() {}
#endif
/*
    NOTE on arc()'s in OpenGL:
    1. angles are in degrees (IrisGL - tenths of degrees)
    2. start angle is measured from positive y-axis (IrisGL - positive x-axis)
    3. arc is drawn clockwise (IrisGL - counterclockwise)
*/

/* DESC: arc, arci, arcs - draw a circular arc */
void
arc (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arc (x, y, radius, startAngle, endAngle);
}

void
iglExec_arc (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    GLUquadricObj *a;


    IGL_CHECKINITV ();
    if (radius <= 0)
    {
        return;
    }

    a = gluNewQuadric ();
    if (a != NULL)
    {
        glPushMatrix ();
        glTranslatef (x, y, 0);
        gluPartialDisk (a, radius, radius, 1, 1,
            (double)startAngle/10.0 + 90.0, (double)endAngle/10.0 - 360.0);
        gluDeleteQuadric (a);
        glPopMatrix ();
    }
}


/* DESC: arc, arci, arcs - draw a circular arc */
void
arci (Icoord x, Icoord y, Icoord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arc ((Coord)x, (Coord)y, (Coord)radius, startAngle, endAngle);
}


/* DESC: arc, arci, arcs - draw a circular arc */
void
arcs (Scoord x, Scoord y, Scoord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arc ((Coord)x, (Coord)y, (Coord)radius, startAngle, endAngle);
}


/* DESC: arcf, arcfi, arcfs - draw a filled circular arc */
void
arcf (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arcf (x, y, radius, startAngle, endAngle);
}

void
iglExec_arcf (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    GLUquadricObj *a;


    IGL_CHECKINITV ();
    if (radius <= 0)
    {
        return;
    }

    a = gluNewQuadric ();
    if (a != NULL)
    {
        glPushMatrix ();
        glTranslatef (x, y, 0);
        gluPartialDisk (a, 0, radius, 1, 1,
            (double)startAngle/10.0 + 90.0, (double)endAngle/10.0 - 360.0);
        gluDeleteQuadric (a);
        glPopMatrix ();
    }
}


/* DESC: arcf, arcfi, arcfs - draw a filled circular arc */
void
arcfi (Icoord x, Icoord y, Icoord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arcf ((Coord)x, (Coord)y, (Coord)radius, startAngle, endAngle);
}


/* DESC: arcf, arcfi, arcfs - draw a filled circular arc */
void
arcfs (Scoord x, Scoord y, Scoord radius, Angle startAngle, Angle endAngle)
{
    igl->api->arcf ((Coord)x, (Coord)y, (Coord)radius, startAngle, endAngle);
}


#if 0
____________________________ circ functions ____________________________
() {}
#endif
/* DESC: circ, circi, circs - outlines a circle */
void
circ (Coord x, Coord y, Coord radius)
{
    igl->api->circ (x, y, radius);
}

void
iglExec_circ (Coord x, Coord y, Coord radius)
{
    GLUquadricObj *c;


    IGL_CHECKINITV ();
    if (radius <= 0)
    {
        return;
    }

    c = gluNewQuadric ();
    if (c != NULL)
    {
        glPushMatrix ();
        glTranslatef (x, y, 0);
        gluDisk (c, radius, radius, 1, 1);
        gluDeleteQuadric (c);
        glPopMatrix ();
    }
}


/* DESC: circ, circi, circs - outlines a circle */
void
circi (Icoord x, Icoord y, Icoord radius)
{
    igl->api->circ ((float)x, (float)y, (float)radius);
}


/* DESC: circ, circi, circs - outlines a circle */
void
circs (Scoord x, Scoord y, Scoord radius)
{
    igl->api->circ ((float)x, (float)y, (float)radius);
}


/* DESC: circf, circfi, circfs - draws a filled circle */
void
circf (Coord x, Coord y, Coord radius)
{
    igl->api->circf (x, y, radius);
}

void
iglExec_circf (Coord x, Coord y, Coord radius)
{
    GLUquadricObj *c;


    IGL_CHECKINITV ();
    if (radius <= 0)
    {
        return;
    }

    c = gluNewQuadric ();
    if (c != NULL)
    {
        glPushMatrix ();
        glTranslatef (x, y, 0);
        gluDisk (c, 0, radius, 1, 1);
        gluDeleteQuadric (c);
        glPopMatrix ();
    }
}


/* DESC: circf, circfi, circfs - draws a filled circle */
void
circif (Icoord x, Icoord y, Icoord radius)
{
    igl->api->circf ((float)x, (float)y, (float)radius);
}


/* DESC: circf, circfi, circfs - draws a filled circle */
void
circsf (Scoord x, Scoord y, Scoord radius)
{
    igl->api->circf ((float)x, (float)y, (float)radius);
}


#if 0
____________________________ mesh functions ____________________________
() {}
#endif
/* DESC: bgntmesh, endtmesh - delimit the vertices of a triangle mesh */
void
bgntmesh ()
{
    igl->api->bgntmesh ();
}

void
iglExec_bgntmesh ()
{
    IGL_CHECKINITV ();
    glBegin (GL_TRIANGLE_STRIP);
}


/* DESC: bgntmesh, endtmesh - delimit the vertices of a triangle mesh */
void
endtmesh ()
{
    igl->api->endpoint ();
}


/* DESC: swaptmesh - toggles the triangle mesh register pointer */
void
swaptmesh ()
{
    /* TBD: can this be a NOP? */
}


/* DESC: bgnclosedline, endclosedline - delimit the vertices of a closed line */
void
bgnclosedline ()
{
    igl->api->bgnclosedline ();
}

void
iglExec_bgnclosedline ()
{
    IGL_CHECKINITV ();
    glBegin (GL_LINE_LOOP);
}


/* DESC: bgnclosedline, endclosedline - delimit the vertices of a closed line */
void
endclosedline ()
{
    igl->api->endpoint ();
}


/* DESC: bgncurve, endcurve - delimit a NURBS curve definition */
void
bgncurve ()
{
    igl->api->bgncurve ();
}

void
iglExec_bgncurve ()
{
    IGL_CHECKINITV ();

    igl->nurbsCurve = gluNewNurbsRenderer ();
    if (igl->nurbsCurve != NULL)
    {
        gluBeginCurve (igl->nurbsCurve);
    }
}


/* DESC: bgncurve, endcurve - delimit a NURBS curve definition */
void
endcurve ()
{
    igl->api->endcurve ();
}

void
iglExec_endcurve ()
{
    IGL_CHECKINITV ();

    if (igl->nurbsCurve != NULL)
    {
        gluEndCurve (igl->nurbsCurve);
        gluDeleteNurbsRenderer (igl->nurbsCurve);
        igl->nurbsCurve = NULL;
    }
}


/* DESC: nurbscurve - controls the shape of a NURBS curve */
void 
nurbscurve (long knotCount, const double *knotList, long offset, const double *ctlArray,
            long order, long type)
{
    igl->api->nurbscurve (knotCount, knotList, offset, ctlArray, order, type);
}

void 
iglExec_nurbscurve (long knotCount, const double *knotList, long offset, const double *ctlArray,
            long order, long type)
{
    int otype;


    IGL_CHECKINITV ();

    if (igl->nurbsCurve != NULL)
    {
        if (type == N_V3D) otype = GL_MAP1_VERTEX_3;
        else if (type == N_V3DR) otype = GL_MAP1_VERTEX_4;
        else return;

#if 0        
        if (type == N_P2D) return;          /* TBD: not supported by OpenGL? */
        else if (type == N_P2DR) return;    /* TBD: not supported by OpenGL? */
#endif

        gluNurbsCurve (igl->nurbsCurve, knotCount, (float *)knotList, offset, (float *)ctlArray,
            order, otype);
    }
}


/* DESC: bgnsurface, endsurface - delimit a NURBS surface definition */
void
bgnsurface ()
{
    igl->api->bgnsurface ();
}

void
iglExec_bgnsurface ()
{
    IGL_CHECKINITV ();

    igl->nurbsCurve = gluNewNurbsRenderer ();
    if (igl->nurbsCurve != NULL)
    {
        gluBeginSurface (igl->nurbsCurve);
    }
}


/* DESC: bgnsurface, endsurface - delimit a NURBS surface definition */
void
endsurface ()
{
    igl->api->endsurface ();
}

void
iglExec_endsurface ()
{
    IGL_CHECKINITV ();

    if (igl->nurbsCurve != NULL)
    {
        gluEndSurface (igl->nurbsCurve);
        gluDeleteNurbsRenderer (igl->nurbsCurve);
        igl->nurbsCurve = NULL;
    }
}


/* nurbssurface - controls the shape of a NURBS surface */
void
nurbssurface (long scount, const double *sknot, long tcount, const double *tknot, long soffset,
              long toffset, const double *ctlArray, long sorder, long torder, long type)
{
    igl->api->nurbssurface (scount, sknot, tcount, tknot, soffset, toffset, ctlArray, sorder, torder, type);
}

void
iglExec_nurbssurface (long scount, const double *sknot, long tcount, const double *tknot, long soffset,
              long toffset, const double *ctlArray, long sorder, long torder, long type)
{
    int otype;


    IGL_CHECKINITV ();

    if (igl->nurbsCurve != NULL)
    {
        if (type == N_V3D) otype = GL_MAP2_VERTEX_3;
        else if (type == N_V3DR) otype = GL_MAP2_VERTEX_4;
        else if (type == N_C4D) otype = GL_MAP2_COLOR_4;
        else if (type == N_T2D) otype = GL_MAP2_TEXTURE_COORD_2;
        else if (type == N_T2DR) otype = GL_MAP2_TEXTURE_COORD_3;
        else return;    /* others (e.g. N_C4DR) not supported */

        gluNurbsSurface (igl->nurbsCurve, scount, (float *)sknot, tcount, (float *)tknot,
            soffset, toffset, (float *)ctlArray, sorder, torder, otype);
    }
}


/* DESC: bgntrim, endtrim - delimit a NURBS surface trimming loop */
void
bgntrim ()
{
    igl->api->bgnsurface ();
}


/* DESC: bgntrim, endtrim - delimit a NURBS surface trimming loop */
void
endtrim ()
{
    igl->api->endsurface ();
}


/* DESC: pwlcurve - describes a piecewise linear trimming curve for NURBS surfaces */
void
pwlcurve (long n, double *dataArray, long byteSize, long type)
{
    igl->api->pwlcurve (n, dataArray, byteSize, type);
}

void
iglExec_pwlcurve (long n, double *dataArray, long byteSize, long type)
{
    IGL_CHECKINITV ();
    if (igl->nurbsCurve != NULL)
    {
        if (type != N_ST)   /* the only one supported by IrisGL */
        {
            return;
        }

        gluPwlCurve (igl->nurbsCurve, n, (float *)dataArray, byteSize, GLU_MAP1_TRIM_2);
    }
}


/* DESC: setnurbsproperty - sets a property for the display of trimmed NURBS surfaces */
void
setnurbsproperty (long property, float value)
{
    igl->api->setnurbsproperty (property, value);
}

void
iglExec_setnurbsproperty (long property, float value)
{
    IGL_CHECKINITV ();

    if (igl->nurbsCurve != NULL)
    {
        if (property == N_ERRORCHECKING)
        {
            /* not supported in OpenGL? */
        }
        else if (property == N_PIXEL_TOLERANCE)
        {
            gluNurbsProperty (igl->nurbsCurve, GLU_SAMPLING_TOLERANCE, value);
        }
        else if (property == N_DISPLAY)
        {
            if (value == N_FILL)
            {
                gluNurbsProperty (igl->nurbsCurve, GLU_DISPLAY_MODE, GLU_FILL);
            }
            else if (value == N_OUTLINE_POLY)
            {
                gluNurbsProperty (igl->nurbsCurve, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
            }
            else if (value == N_OUTLINE_PATCH)
            {
                gluNurbsProperty (igl->nurbsCurve, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
            }
        }
        else if (property == N_CULLING)
        {
            gluNurbsProperty (igl->nurbsCurve, GLU_CULLING, value);
        }
    }
}


/* DESC: bgnline, endline - delimit the vertices of a line */
void
bgnline ()
{
    igl->api->bgnline ();
}

void
iglExec_bgnline ()
{
    IGL_CHECKINITV ();
    glBegin (GL_LINE_STRIP);
}


/* DESC: bgnline, endline - delimit the vertices of a line */
void
endline ()
{
    igl->api->endpoint ();
}


/* DESC: bgnpoint, endpoint - delimit the interpretation of vertex routines as points */
void
bgnpoint ()
{
    igl->api->bgnpoint ();
}

void
iglExec_bgnpoint ()
{
    IGL_CHECKINITV ();
    glBegin (GL_POINTS);
}


/* DESC: bgnpoint, endpoint - delimit the interpretation of vertex routines as points */
void
endpoint ()
{
    igl->api->endpoint ();
}

void
iglExec_endpoint ()
{
    IGL_CHECKINITV ();
    glEnd ();
}


/* DESC: bgnpolygon, endpolygon - delimit the vertices of a polygon */
void
bgnpolygon ()
{
    igl->api->bgnpolygon ();
}

void
iglExec_bgnpolygon ()
{
    IGL_CHECKINITV ();
    glBegin (GL_POLYGON);
}


/* DESC: bgnpolygon, endpolygon - delimit the vertices of a polygon */
void
endpolygon ()
{
    igl->api->endpoint ();
}


/* DESC: bgnqstrip, endqstrip - delimit the vertices of a quadrilateral strip */
void
bgnqstrip ()
{
    igl->api->bgnqstrip ();
}

void
iglExec_bgnqstrip ()
{
    IGL_CHECKINITV ();
    glBegin (GL_QUAD_STRIP);
}


/* DESC: bgnqstrip, endqstrip - delimit the vertices of a quadrilateral strip */
void
endqstrip ()
{
    igl->api->endpoint ();
}


/* DESC: crvn - draws a series of curve segments */
void
crvn (long n, Coord points[][3])
{
    igl->api->crvn (n, points);
}

void
iglExec_crvn (long n, Coord points[][3])
{
    int i, j;


    /* TBD: only splines are supported for now!!!! */
    IGL_CHECKINITV ();
    if (n < 4)
    {
        return;
    }

    glEnable (GL_MAP1_VERTEX_3);
    for (j=0; j < n; j+=4)
    {
        glMap1f (GL_MAP1_VERTEX_3, 0, 1, 3, 4, points[j]);
        glBegin (GL_LINE_STRIP);
            for (i=0; i <= igl->curveSegments; i++)
            {
                glEvalCoord1f ((GLfloat)i/30.0f);
            }
        glEnd ();
    }
}


/* DESC: crv - draws a curve */
void
crv (Coord points[4][3])
{
    igl->api->crvn (4, points);
}


/* DESC: rcrvn - draws a series of curve segments */
void
rcrvn (long n, Coord points[][4])
{
    igl->api->rcrvn (n, points);
}

void
iglExec_rcrvn (long n, Coord points[][4])
{
    int i, j;


    /* TBD: only splines are supported for now!!!! */
    IGL_CHECKINITV ();
    if (n < 4)
    {
        return;
    }

    glEnable (GL_MAP1_VERTEX_3);
    for (j=0; j < n; j+=4)
    {
        glMap1f (GL_MAP1_VERTEX_3, 0, 1, 4, 4, points[j]);
        glBegin (GL_LINE_STRIP);
            for (i=0; i <= igl->curveSegments; i++)
            {
                glEvalCoord1f ((GLfloat)i/30.0f);
            }
        glEnd ();
    }
}


/* DESC: rcrv - draws a rational curve */
void
rcrv (Coord points[4][4])
{
    igl->api->rcrvn (4, points);
}


/* DESC: curveprecision - sets number of line segments used to draw a curve segment */
void
curveprecision (short n)
{
    igl->api->curveprecision (n);
}

void
iglExec_curveprecision (short n)
{
    IGL_CHECKINITV ();
    if (n < 0 || n > IGL_MAXCURVESEGMENTS)
    {
        return;
    }
    igl->curveSegments = n;
}


#if 0
____________________________ misc functions ____________________________
() {}
#endif
/* DESC: nmode - specify renormalization of normals */
void
nmode (long mode)
{
    igl->api->nmode (mode);
}

void
iglExec_nmode (long mode)
{
    IGL_CHECKINITV ();

    /* TBD: check correctness of code with specification */
    if (mode == NAUTO)
    {
        glDisable (GL_NORMALIZE);
    }
    else if (mode == NNORMALIZE)
    {
        glEnable (GL_NORMALIZE);
    }
}


/* DESC: n3f - specifies a normal */
void
n3f (float *vector)
{
    igl->api->n3f (vector);
}

void
iglExec_n3f (float *vector)
{
    IGL_CHECKINITV ();
    glNormal3fv (vector);
}


/* DESC: normal - obsolete routine */
void
normal (Coord *narray)
{
    igl->api->n3f (narray);
}


/* DESC: linesmooth - specify antialiasing of lines */
void
linesmooth (unsigned long mode)
{
    igl->api->linesmooth (mode);
}

void
iglExec_linesmooth (unsigned long mode)
{
    IGL_CHECKINITV ();

    if (mode == SML_OFF)
    {
        glDisable (GL_LINE_SMOOTH);
    }
    else if (mode & SML_ON)
    {
        glEnable (GL_LINE_SMOOTH);

        /* is this a good approximation? */
        if ((mode & SML_SMOOTHER) || (mode & SML_END_CORRECT))
        {
            glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
        }
        else
        {
            glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
        }
    }
}


/* DESC: smoothline - obsolete routine */
void
smoothline (long mode)
{
    igl->api->linesmooth ((unsigned long)mode);
}


/* DESC: linewidth, linewidthf - specifies width of lines */
void
linewidthf (float width)
{
    igl->api->linewidthf (width);
}

void
iglExec_linewidthf (float width)
{
    IGL_CHECKINITV ();
    glLineWidth (width);
}


/* DESC: linewidth, linewidthf - specifies width of lines */
void
linewidth (short width)
{
    igl->api->linewidthf ((float)width);
}


/* DESC: getlwidth - returns the current linewidth */
long
getlwidth ()
{
    int lw;


    IGL_CHECKINIT (1);

    glGetIntegerv (GL_LINE_WIDTH, &lw);
    return (lw);
}


/* DESC: deflinestyle - defines a linestyle */
void
deflinestyle (short n, Linestyle ls)
{
    igl->api->deflinestyle (n, ls);
}

void
iglExec_deflinestyle (short n, Linestyle ls)
{
    IGL_CHECKINITV ();

    /* 0 cannot be changed! (solid line) */
    if (n <= 0 || n > IGL_MAXLINESTYLES)
    {
        return;
    }

    igl->lineStyles[n] = ls;
}


/* DESC: setlinestyle - selects a linestyle pattern */
void
setlinestyle (short n)
{
    igl->api->setlinestyle (n);
}

void
iglExec_setlinestyle (short n)
{
    IGL_CHECKINITV ();

    if (n < 0 || n > IGL_MAXLINESTYLES)
    {
        return;
    }

    if (n == 0)
    {
        /* solid line */
        glDisable (GL_LINE_STIPPLE);
    }
    else
    {
        glEnable (GL_LINE_STIPPLE);
        glLineStipple (1, (unsigned short)igl->lineStyles[n]);
    }
    igl->lineStyleIndex = n;
}


/* DESC: getlstyle - returns the current linestyle */
long
getlstyle ()
{
    IGL_CHECKINIT (0);
    return (igl->lineStyleIndex);
}


/* DESC: lsbackup - controls whether the ends of a line segment are colored */
void
lsbackup (Boolean b)
{
    /* not supported by OpenGL */
    IGL_CHECKINITV ();
}


/* DESC: getlsbackup - returns the state of linestyle backup mode */
Boolean
getlsbackup ()
{
    /* not supported by OpenGL */
    return (FALSE);
}


/* DESC: lsrepeat - sets a repeat factor for the current linestyle */
void
lsrepeat (long factor)
{
    igl->api->lsrepeat (factor);
}

void
iglExec_lsrepeat (long factor)
{
    IGL_CHECKINITV ();
    if (factor > 0 && factor < 256)
    {
        glLineStipple (factor, (unsigned short)igl->lineStyles[igl->lineStyleIndex]);
    }
}


/* DESC: getlsrepeat - returns the linestyle repeat count */
long
getlsrepeat ()
{
    int lsr;


    IGL_CHECKINIT (1);

    glGetIntegerv (GL_LINE_STIPPLE_REPEAT, &lsr);
    return (lsr);
}


/* DESC: defpattern - defines patterns */
void
defpattern (short n, short size, unsigned short mask[])
{
    int i;
    GLubyte *omask;

     
    if (n <= 0 || n >= IGL_MAXPATTERNS)
    {
        return;
    }
    omask = igl->patterns[n];

    /* OpenGL only allows one pattern size (32x32 pixels), so we resize */
    switch (size)
    {
        case 16:
            memset (omask, 0, 1024 * sizeof (GLubyte));
            for (i=0; i < 256; i++)
            {
                omask[i] = (GLubyte)mask[i];
            }
        break;

        case 32:
            for (i=0; i < 1024; i++)
            {
                omask[i] = (GLubyte)mask[i];
            }
        break;

        default:
        break;
    }
}


/* DESC: setpattern - selects a pattern for filling polygons and rectangles */
void
setpattern (short index)
{
    igl->api->setpattern (index);
}

void
iglExec_setpattern (short index)
{
    if (index <= 0 || index >= IGL_MAXPATTERNS)
    {
        return;
    }

    if (index == 0)
    {
        glDisable (GL_POLYGON_STIPPLE);
    }
    else
    {
        glPolygonStipple (igl->patterns[index]);
        glEnable (GL_POLYGON_STIPPLE);
    }
    igl->patternIndex = index;
}


/* DESC: getpattern - returns the index of the current pattern */
long
getpattern ()
{
    return (igl->patternIndex);
}


/* DESC: concave - allows the system to draw concave polygons */
void
concave (Boolean enable)
{
    /* not supported in OpenGL - should be done with gluTess functions */
}


