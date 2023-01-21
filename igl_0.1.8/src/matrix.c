#include "igl.h"
#include "iglcmn.h"




/* DESC: mmode - sets the current matrix mode */
void
mmode (short mode)
{
    igl->api->mmode (mode);
}

void
iglExec_mmode (short mode)
{
    IGL_CHECKINITV ();
    if (mode != MSINGLE && mode != MVIEWING && mode != MPROJECTION && mode != MTEXTURE)
    {
        return;
    }

    /* when entering/leaving MSINGLE, all matrix stacks are reset */
    if ((igl->matrixMode == MSINGLE && mode != MSINGLE) || (igl->matrixMode != MSINGLE && mode == MSINGLE))
    {
        glMatrixMode (GL_MODELVIEW);
        while (glGetError () == GL_NO_ERROR)
        {
            glPopMatrix ();
        }
        glLoadIdentity ();

        glMatrixMode (GL_PROJECTION);
        while (glGetError () == GL_NO_ERROR)
        {
            glPopMatrix ();
        }
        glLoadIdentity ();

        glMatrixMode (GL_TEXTURE);
        while (glGetError () == GL_NO_ERROR)
        {
            glPopMatrix ();
        }
        glLoadIdentity ();
    }

    igl->matrixMode = mode;
    switch (mode)
    {
        case MVIEWING:
            glMatrixMode (GL_MODELVIEW);
            break;

        case MSINGLE:
        case MPROJECTION:
            glMatrixMode (GL_PROJECTION);
            break;

        case MTEXTURE:
            glMatrixMode (GL_TEXTURE);
            break;
    }
}


/* DESC: getmmode - returns the current matrix mode */
long
getmmode ()
{
    IGL_CHECKINIT (MSINGLE);
    return (igl->matrixMode);
}


/* DESC: loadmatrix - loads a transformation matrix */
void
loadmatrix (Matrix m)
{
    igl->api->loadmatrix (m);
}

void
iglExec_loadmatrix (Matrix m)
{
    IGL_CHECKINITV ();
    glLoadMatrixf ((GLfloat *)m);
}


/* DESC: getmatrix - returns a copy of a transformation matrix */
void
getmatrix (Matrix m)
{
    IGL_CHECKINITV ();

    switch (igl->matrixMode)
    {
        case MVIEWING:
            glGetFloatv (GL_MODELVIEW_MATRIX, (GLfloat *)m);
            break;

        case MSINGLE:
        case MPROJECTION:
            glGetFloatv (GL_PROJECTION_MATRIX, (GLfloat *)m);
            break;

        case MTEXTURE:
            glGetFloatv (GL_TEXTURE_MATRIX, (GLfloat *)m);
            break;
    }
}


/* DESC: multmatrix - premultiplies the current transformation matrix */
void
multmatrix (Matrix m)
{
    igl->api->multmatrix (m);
}

void
iglExec_multmatrix (Matrix m)
{
    IGL_CHECKINITV ();
    glMultMatrixf ((GLfloat *)m);
}


/* DESC: pushmatrix - pushes down the transformation matrix stack */
void
pushmatrix ()
{
    igl->api->pushmatrix ();
}

void
iglExec_pushmatrix ()
{
    IGL_CHECKINITV ();
    /* TBD: man page says: "should not be called when mmode is MPROJECTION or MTEXTURE" */
    glPushMatrix ();
}


/* DESC: pushmatrix - pops the transformation matrix stack */
void
popmatrix ()
{
    igl->api->popmatrix ();
}

void
iglExec_popmatrix ()
{
    IGL_CHECKINITV ();
    /* TBD: man page says: "should not be called when mmode is MPROJECTION or MTEXTURE" */
    glPopMatrix ();
}


/* DESC: rotate, rot - rotates the current matrix */
void
rot (float amount, char angle)
{
    igl->api->rot (amount, angle);
}

void
iglExec_rot (float amount, char angle)
{
    IGL_CHECKINITV ();

    if (angle == 'x')
    {
        glRotatef (amount, 1, 0, 0);
    }
    else if (angle == 'y')
    {
        glRotatef (amount, 0, 1, 0);
    }
    else
    {
        glRotatef (amount, 0, 0, 1);
    }
}


/* DESC: rotate, rot - rotates the current matrix */
void
rotate (Angle amount, char angle)
{
    igl->api->rotate (amount, angle);
}

void
iglExec_rotate (Angle amount, char angle)
{
    IGL_CHECKINITV ();

    /* NOTE: amount is an integer specified in tenths of degrees */
    if (angle == 'x')
    {
        glRotatef (amount*0.1f, 1, 0, 0);
    }
    else if (angle == 'y')
    {
        glRotatef (amount*0.1f, 0, 1, 0);
    }
    else
    {
        glRotatef (amount*0.1f, 0, 0, 1);
    }
}


/* DESC: translate - translates the current matrix */
void
translate (Coord x, Coord y, Coord z)
{
    igl->api->translate (x, y, z);
}

void
iglExec_translate (Coord x, Coord y, Coord z)
{
    IGL_CHECKINITV ();
    glTranslatef (x, y, z);
}


/* DESC: scale - scales and mirrors the current matrix */
void
scale (float x, float y, float z)
{
    igl->api->scale (x, y, z);
}

void
iglExec_scale (float x, float y, float z)
{
    IGL_CHECKINITV ();
    glScalef (x, y, z);
}


/* DESC: polarview - defines the viewer's position in polar coordinates */
void
polarview (Coord dist, Angle azim, Angle inc, Angle twist)
{
    igl->api->polarview (dist, azim, inc, twist);
}

void
iglExec_polarview (Coord dist, Angle azim, Angle inc, Angle twist)
{
    IGL_CHECKINITV ();

    glTranslatef (0, 0, -dist);
    /*
        this is crappy. man page states that angles are in tenths of degrees, so
        we should divide by 10 to get angles in degrees. however, the IrisGL to
        OpenGL porting guide multiplies angles by 10. i believe the guide to be
        buggy, since the rotate() function's angle is divided by 10 (and it seems logical).
    */
    glRotatef (-twist*0.1f, 0, 0, 1);
    glRotatef (-inc*0.1f, 1, 0, 0);
    glRotatef (-azim*0.1f, 0, 0, 1);
}


/* DESC: clipplane - specify a plane against which all geometry is clipped */
void
clipplane (long index, long mode, float *params)
{
    igl->api->clipplane (index, mode, params);
}

void
iglExec_clipplane (long index, long mode, float *params)
{
    IGL_CHECKINITV ();

    /* TBD: OpenGL supports 0..GL_MAX_CLIP_PLANES, IrisGL 0..5 - so what do we do? */
    if (index < 0 || index > 5 || (mode == CP_DEFINE && params == NULL) || igl->matrixMode == MSINGLE)
    {
        return;
    }

    if (mode == CP_OFF)
    {
        glDisable (GL_CLIP_PLANE0+index);
    }
    else if (mode == CP_ON)
    {
        glEnable (GL_CLIP_PLANE0+index);
    }
    else if (mode == CP_DEFINE)
    {
        glClipPlane (GL_CLIP_PLANE0+index, (GLdouble *)params);
    }
}


#if 0
____________________________ projection/viewport functions ____________________________
() {}
#endif
/* DESC: ortho, ortho2 - define an orthographic projection transformation */
void
ortho (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    igl->api->ortho (left, right, bottom, top, near_, far_);
}

void
iglExec_ortho (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    int cm;


    IGL_CHECKINITV ();

    /*
        in MSINGLE, ortho() loads the matrix onto the matrix stack, otherwise
        it changes the projection matrix
    */
    if (igl->matrixMode == MSINGLE || igl->matrixMode == MPROJECTION)
    {
        glLoadIdentity ();
        glOrtho ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top,
            (GLdouble)near_, (GLdouble)far_);
    }
    else
    {
        /* in other modes, only MPROJECTION matrix is touched! */
        glGetIntegerv (GL_MATRIX_MODE, &cm);
        glMatrixMode (GL_PROJECTION);

        glLoadIdentity ();
        glOrtho ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top,
            (GLdouble)near_, (GLdouble)far_);

        glMatrixMode (cm);
    }
}


/* DESC: ortho, ortho2 - define an orthographic projection transformation */
void
ortho2 (Coord left, Coord right, Coord bottom, Coord top)
{
    igl->api->ortho2 (left, right, bottom, top);
}

void
iglExec_ortho2 (Coord left, Coord right, Coord bottom, Coord top)
{
    int cm;


    IGL_CHECKINITV ();

    if (igl->matrixMode == MSINGLE || igl->matrixMode == MPROJECTION)
    {
        glLoadIdentity ();
        gluOrtho2D ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top);
    }
    else
    {
        /* in other modes, only MPROJECTION matrix is touched! */
        glGetIntegerv (GL_MATRIX_MODE, &cm);
        glMatrixMode (GL_PROJECTION);

        glLoadIdentity ();
        gluOrtho2D ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top);

        glMatrixMode (cm);
    }
}


/* DESC: window - defines a perspective projection transformation */
void
window (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    igl->api->window (left, right, bottom, top, near_, far_);
}

void
iglExec_window (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    int cm;


    IGL_CHECKINITV ();

    if (igl->matrixMode == MSINGLE || igl->matrixMode == MPROJECTION)
    {
        glLoadIdentity ();
        glFrustum ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top,
            (GLdouble)near_, (GLdouble)far_);
    }
    else
    {
        /* in other modes, only MPROJECTION matrix is touched! */
        glGetIntegerv (GL_MATRIX_MODE, &cm);
        glMatrixMode (GL_PROJECTION);

        glLoadIdentity ();
        glFrustum ((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top,
            (GLdouble)near_, (GLdouble)far_);

        glMatrixMode (cm);
    }
}


/* DESC: perspective - defines a perspective projection transformation */
void
perspective (Angle fovy, float aspect, Coord near_, Coord far_)
{
    igl->api->perspective (fovy, aspect, near_, far_);
}

void
iglExec_perspective (Angle fovy, float aspect, Coord near_, Coord far_)
{
    int cm;


    IGL_CHECKINITV ();

    if (igl->matrixMode == MSINGLE || igl->matrixMode == MPROJECTION)
    {
        glLoadIdentity ();
        gluPerspective ((GLdouble)fovy, (GLdouble)aspect, (GLdouble)near_, (GLdouble)far_);
    }
    else
    {
        /* in other modes, only MPROJECTION matrix is touched! */
        glGetIntegerv (GL_MATRIX_MODE, &cm);
        glMatrixMode (GL_PROJECTION);

        glLoadIdentity ();
        gluPerspective ((GLdouble)fovy, (GLdouble)aspect, (GLdouble)near_, (GLdouble)far_);

        glMatrixMode (cm);
    }
}


/* DESC: viewport - allocates a rectangular area of the window for an image */
void
viewport (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    igl->api->viewport (left, right, bottom, top);
}

void
iglExec_viewport (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    long width, height;


    IGL_CHECKINITV ();

    width = right-left;
    height = top-bottom;
    if (width <= 0 || height <= 0)
    {
        return;
    }

    glViewport (left, bottom, width, height);
    /* viewport() sets the scrmask/scissor region to match the viewport. thus, we can disable it. */
    glDisable (GL_SCISSOR_TEST);
}


/* DESC: getviewport - gets a copy of the dimensions of the current viewport */
void
getviewport (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    igl->api->getviewport (left, right, bottom, top);
}

void
iglExec_getviewport (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    int cviewport[4];


    IGL_CHECKINITV ();
    if (left == NULL || right == NULL || bottom == NULL || top == NULL)
    {
        return;
    }

    glGetIntegerv (GL_VIEWPORT, cviewport);

    *left = cviewport[0];
    *right = cviewport[0] + cviewport[2];
    *bottom = cviewport[1];
    *top = cviewport[1] + cviewport[3];
}


/* DESC: reshapeviewport - sets the viewport to the dimensions of the current graphics window */
void
reshapeviewport ()
{
    igl->api->reshapeviewport ();
}

void
iglExec_reshapeviewport ()
{
    IGL_CHECKINITV ();
    glViewport (0, 0, IGL_CTX()->width-1, IGL_CTX()->height-1);
}


/* DESC: pushviewport - pushes down the viewport stack */
void
pushviewport ()
{
    igl->api->pushviewport ();
}

void
iglExec_pushviewport ()
{
    IGL_CHECKINITV ();
    glPushAttrib (GL_VIEWPORT_BIT);
}


/* DESC: popviewport - pops the viewport stack */
void
popviewport ()
{
    igl->api->popviewport ();
}

void
iglExec_popviewport ()
{
    IGL_CHECKINITV ();
    glPopAttrib ();
}


/* DESC: lookat - defines a viewing transformation */
void
lookat (Coord vx, Coord vy, Coord vz, Coord px, Coord py, Coord pz, Angle twist)
{
    igl->api->lookat (vx, vy, vz, px, py, pz, twist);
}

void
iglExec_lookat (Coord vx, Coord vy, Coord vz, Coord px, Coord py, Coord pz, Angle twist)
{
    IGL_CHECKINITV ();

    /* TBD: we're missing IrisGL->OpenGL conversion here methinks */
    glRotatef ((float)-twist*0.1f, 0, 0, 1);
    gluLookAt (vx, vy, vz, px, py, pz, 0, 1, 0);
}


/* DESC: scrmask - defines a rectangular screen clipping mask */
void
scrmask (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    igl->api->scrmask (left, right, bottom, top);
}

void
iglExec_scrmask (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    long width, height;


    IGL_CHECKINITV ();

    width = right-left;
    height = top-bottom;
    if (width <= 0 || height <= 0)
    {
        return;
    }

    glScissor (left, bottom, width, height);
    glEnable (GL_SCISSOR_TEST);
}


/* DESC: getscrmask - returns the current screen mask */
void
getscrmask (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    igl->api->getscrmask (left, right, bottom, top);
}

void
iglExec_getscrmask (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    int cscrmask[4];


    IGL_CHECKINITV ();
    if (left == NULL || right == NULL || bottom == NULL || top == NULL)
    {
        return;
    }

    /* if GL_SCISSOR_TEST is disabled, we get the entire window box */
    glGetIntegerv (GL_SCISSOR_BOX, cscrmask);

    *left = cscrmask[0];
    *right = cscrmask[0] + cscrmask[2];
    *bottom = cscrmask[1];
    *top = cscrmask[1] + cscrmask[3];
}


/* DESC: gl_invertmat - invert a matrix??? */
/* NOTE: ripped from Mesa */
void
gl_invertmat (Matrix m, Matrix out)
{
    GLdouble *_tmp;
    GLdouble wtmp[4][8];
    GLdouble m0, m1, m2, m3, s;
    GLdouble *r0, *r1, *r2, *r3;
#define SWAP_ROWS(a, b) { _tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m, r, c) (m)[(c)][(r)]


    IGL_CHECKINITV ();

    r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

    r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
        r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
        r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
        r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
        r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
        r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
        r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
        r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
        r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
        r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
        r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
        r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

    /* choose pivot - or die */
    if (fabs(r3[0]) > fabs(r2[0]))
    {
        SWAP_ROWS (r3, r2);
    }
    if (fabs(r2[0]) > fabs(r1[0]))
    {
        SWAP_ROWS (r2, r1);
    }
    if (fabs(r1[0]) > fabs(r0[0]))
    {
        SWAP_ROWS (r1, r0);
    }
    if (r0[0] == 0)
    {
        return;
    }

    /* eliminate first variable     */
    m1 = r1[0] / r0[0];
    m2 = r2[0] / r0[0];
    m3 = r3[0] / r0[0];
    s = r0[1];
    r1[1] -= m1 * s;
    r2[1] -= m2 * s;
    r3[1] -= m3 * s;
    s = r0[2];
    r1[2] -= m1 * s;
    r2[2] -= m2 * s;
    r3[2] -= m3 * s;
    s = r0[3];
    r1[3] -= m1 * s;
    r2[3] -= m2 * s;
    r3[3] -= m3 * s;
    s = r0[4];
    if (s != 0.0)
    {
        r1[4] -= m1 * s;
        r2[4] -= m2 * s;
        r3[4] -= m3 * s;
    }
    s = r0[5];
    if (s != 0.0)
    {
        r1[5] -= m1 * s;
        r2[5] -= m2 * s;
        r3[5] -= m3 * s;
    }
    s = r0[6];
    if (s != 0.0)
    {
        r1[6] -= m1 * s;
        r2[6] -= m2 * s;
        r3[6] -= m3 * s;
    }
    s = r0[7];
    if (s != 0.0)
    {
        r1[7] -= m1 * s;
        r2[7] -= m2 * s;
        r3[7] -= m3 * s;
    }

    /* choose pivot - or die */
    if (fabs(r3[1]) > fabs(r2[1]))
    {
        SWAP_ROWS (r3, r2);
    }
    if (fabs(r2[1]) > fabs(r1[1]))
    {
        SWAP_ROWS (r2, r1);
    }
    if (r1[1] == 0)
    {
        return;
    }

    /* eliminate second variable */
    m2 = r2[1] / r1[1];
    m3 = r3[1] / r1[1];
    r2[2] -= m2 * r1[2];
    r3[2] -= m3 * r1[2];
    r2[3] -= m2 * r1[3];
    r3[3] -= m3 * r1[3];
    s = r1[4];
    if (0.0 != s)
    {
        r2[4] -= m2 * s;
        r3[4] -= m3 * s;
    }
    s = r1[5];
    if (0.0 != s)
    {
        r2[5] -= m2 * s;
        r3[5] -= m3 * s;
    }
    s = r1[6];
    if (0.0 != s)
    {
        r2[6] -= m2 * s;
        r3[6] -= m3 * s;
    }
    s = r1[7];
    if (0.0 != s)
    {
        r2[7] -= m2 * s;
        r3[7] -= m3 * s;
    }

    /* choose pivot - or die */
    if (fabs(r3[2]) > fabs(r2[2]))
    {
        SWAP_ROWS (r3, r2);
    }
    if (r2[2] == 0)
    {
        return;
    }

    /* eliminate third variable */
    m3 = r3[2] / r2[2];
    r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
        r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

    /* last check */
    if (r3[3] == 0)
    {
        return;
    }

    s = 1.0 / r3[3];        /* now back substitute row 3 */
    r3[4] *= s;
    r3[5] *= s;
    r3[6] *= s;
    r3[7] *= s;

    m2 = r2[3];            /* now back substitute row 2 */
    s = 1.0 / r2[2];
    r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
    m1 = r1[3];
    r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
        r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
    m0 = r0[3];
    r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
        r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

    m1 = r1[2];            /* now back substitute row 1 */
    s = 1.0 / r1[1];
    r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
        r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
    m0 = r0[2];
    r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
        r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

    m0 = r0[1];            /* now back substitute row 0 */
    s = 1.0 / r0[0];
    r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
        r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

    MAT(out, 0, 0) = (float)r0[4];
    MAT(out, 0, 1) = (float)r0[5], MAT(out, 0, 2) = (float)r0[6];
    MAT(out, 0, 3) = (float)r0[7], MAT(out, 1, 0) = (float)r1[4];
    MAT(out, 1, 1) = (float)r1[5], MAT(out, 1, 2) = (float)r1[6];
    MAT(out, 1, 3) = (float)r1[7], MAT(out, 2, 0) = (float)r2[4];
    MAT(out, 2, 1) = (float)r2[5], MAT(out, 2, 2) = (float)r2[6];
    MAT(out, 2, 3) = (float)r2[7], MAT(out, 3, 0) = (float)r3[4];
    MAT(out, 3, 1) = (float)r3[5], MAT(out, 3, 2) = (float)r3[6];
    MAT(out, 3, 3) = (float)r3[7];

#undef MAT
#undef SWAP_ROWS
}


/* DESC: gl_invertmat - invert a matrix??? */
void
gl_invert4d (Matrix out, Matrix m)
{
    gl_invertmat (m, out);
}


/* DESC: mapw - maps a point on the screen into a line in 3-D world coordinates */
void
mapw (Object obj, Screencoord x, Screencoord y, Coord *wx1, Coord *wy1, Coord *wz1, 
      Coord *wx2, Coord *wy2, Coord *wz2)
{
    GLdouble modelMatrix[16], projMatrix[16], tx, ty, tz;
    GLint viewport[4];


    IGL_CHECKINITV ();
    if (wx1 == NULL || wy1 == NULL || wz1 == NULL || wx2 == NULL || wy2 == NULL || wz2 == NULL)
    {
        return;
    }

    glPushMatrix ();
    glPushAttrib (GL_VIEWPORT_BIT);

    callobj (obj);
    glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv (GL_VIEWPORT, viewport);

    glPopAttrib ();
    glPopMatrix ();

    gluUnProject (x, viewport[3]-y-1, 0, modelMatrix, projMatrix, viewport, &tx, &ty, &tz);
    *wx1 = *wx2 = (GLfloat)tx;
    *wy1 = *wy2 = (GLfloat)ty;
    *wz1 = *wz2 = (GLfloat)tz;
}


/* DESC: mapw2 - maps a point on the screen into 2-D world coordinates */
void
mapw2 (Object obj, Screencoord x, Screencoord y, Coord *wx, Coord *wy)
{
    GLdouble modelMatrix[16], projMatrix[16], tx, ty, tz;
    GLint viewport[4];


    IGL_CHECKINITV ();
    if (wx == NULL || wy == NULL)
    {
        return;
    }

    glPushMatrix ();
    glPushAttrib (GL_VIEWPORT_BIT);

    callobj (obj);
    glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv (GL_VIEWPORT, viewport);

    glPopAttrib ();
    glPopMatrix ();

    gluUnProject (x, viewport[3]-y-1, 0, modelMatrix, projMatrix, viewport, &tx, &ty, &tz);
    *wx = (GLfloat)tx;
    *wy = (GLfloat)ty;
}


/* DESC: screenspace - map world space to absolute screen coordinates */
void
screenspace ()
{
    long xmin, ymin, xmax, ymax;


    IGL_CHECKINITV ();

    getorigin (&xmin, &ymin);
    xmax = getgdesc (GD_XPMAX);
    ymax = getgdesc (GD_YPMAX);

    viewport ((short)-xmin, (short)(xmax-xmin), (short)-ymin, (short)(ymax-ymin));
    ortho2 (-0.5f, xmax+0.5f, -0.5f, ymax+0.5f);
}


