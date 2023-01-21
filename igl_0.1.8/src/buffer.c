#include "igl.h"
#include "iglcmn.h"




#if 0
____________________________ buffer functions ____________________________
() {}
#endif
/* DESC: clear - clears the viewport */
void
clear ()
{
    igl->api->clear ();
}

void
iglExec_clear ()
{
    IGL_CHECKINITV ();
    glClear (GL_COLOR_BUFFER_BIT);
}


/* DESC: cmode - sets color map mode as the current mode */
void
cmode ()
{
    IGL_CHECKINITV ();
    IGL_CTX()->newFlags &= ~IGL_WFLAGS_RGBA;
}


/* DESC: RGBmode - sets a rendering and display mode that bypasses the color map */
void
RGBmode ()
{
    IGL_CHECKINITV ();
    IGL_CTX()->newFlags |= IGL_WFLAGS_RGBA;
}


/* DESC: RGBsize - specifies the number of bit planes desired to comprise each single-color component buffer */
void
RGBsize (long planes)
{
    IGL_CHECKINITV ();
    IGL_CTX()->rgbPlanes = (unsigned char)planes;
}


/* DESC: getdisplaymode - returns the current display mode */
long
getdisplaymode ()
{
    igl_windowT *wptr;


    IGL_CHECKINIT (DMRGB);

    wptr = IGL_CTX ();
    if (wptr->flags & IGL_WFLAGS_RGBA)
    {
        return ((wptr->flags & IGL_WFLAGS_DOUBLEBUFFER) ? DMRGBDOUBLE : DMRGB);
    }
    else
    {
        return ((wptr->flags & IGL_WFLAGS_DOUBLEBUFFER) ? DMDOUBLE : DMSINGLE);
    }
}


/* DESC: lsetdepth - sets the depth range */
void
lsetdepth (long near_, long far_)
{
    igl->api->lsetdepth (near_, far_);
}

void
iglExec_lsetdepth (long near_, long far_)
{
    IGL_CHECKINITV ();
    glDepthRange ((float)near_, (float)far_);
}


/* DESC: setdepth - obsolete routine (use lsetdepth) */
void
setdepth (Screencoord near_, Screencoord far_)
{
    igl->api->lsetdepth ((long)near_, (long)far_);
}


/* DESC: getdepth - obsolete routine */
void
getdepth (Screencoord *near_, Screencoord *far_)
{
    igl->api->getdepth (near_, far_);
}

void
iglExec_getdepth (Screencoord *near_, Screencoord *far_)
{
    int d[2];


    IGL_CHECKINITV ();
    glGetIntegerv (GL_DEPTH_RANGE, d);
    if (near_ != NULL)
    {
        *near_ = (Screencoord)d[0];
    }
    if (far_ != NULL)
    {
        *far_ = (Screencoord)d[1];
    }
}


/* DESC: singlebuffer - writes and displays all bitplanes */
void
singlebuffer ()
{
    IGL_CHECKINITV ();
    /* TBD: since flipping from single to double doesn't work, we don't do anything here */
    IGL_CTX()->newFlags &= ~IGL_WFLAGS_DOUBLEBUFFER;
}


/* DESC: doublebuffer - sets the display mode to double buffer mode */
void
doublebuffer ()
{
    IGL_CHECKINITV ();
    IGL_CTX()->newFlags |= IGL_WFLAGS_DOUBLEBUFFER;
}


/* DESC: swapbuffers - exchanges the front and back buffers of the normal framebuffer */
void
swapbuffers ()
{
    IGL_CHECKINITV ();
#if 1
    if (IGL_CTX()->flags & IGL_WFLAGS_DOUBLEBUFFER)
    {
        SWAPBUFFERS (IGL_CTX ());
    }
#else
    {
        int i;

        for (i=0; i < IGL_MAXWINDOWS; i++)
        {
            if (igl->openWindows[i].wnd != NULL && (igl->openWindows[i].flags & IGL_WFLAGS_DOUBLEBUFFER))
            {
                SWAPBUFFERS (&igl->openWindows[i]);
            }
        }
    }
#endif
}


/* DESC: backbuffer, frontbuffer - enable and disable drawing to the back or front buffer */
void
backbuffer (Boolean enable)
{
    igl->api->backbuffer (enable);
}

void
iglExec_backbuffer (Boolean enable)
{
    int cb;


    IGL_CHECKINITV ();
    if (!(IGL_CTX()->flags & IGL_WFLAGS_DOUBLEBUFFER))
    {
        return;
    }

    /* NOTE: we only support FRONT and BACK buffers!!!! */
    glGetIntegerv (GL_DRAW_BUFFER, &cb);
    if (enable)
    {
        /* backbuffer already set? -> nothing to do */
        if (cb == GL_BACK || cb == GL_FRONT_AND_BACK)
        {
            return;
        }
        else if (cb == GL_FRONT)
        {
            glDrawBuffer (GL_FRONT_AND_BACK);
        }
        else if (cb == GL_NONE)
        {
            glDrawBuffer (GL_BACK);
        }
    }
    else
    {
        if (cb == GL_BACK)
        {
            glDrawBuffer (GL_NONE);
        }
        else if (cb == GL_FRONT_AND_BACK)
        {
            glDrawBuffer (GL_FRONT);
        }
    }
}


/* DESC: backbuffer, frontbuffer - enable and disable drawing to the back or front buffer */
void
frontbuffer (Boolean enable)
{
    igl->api->frontbuffer (enable);
}

void
iglExec_frontbuffer (Boolean enable)
{
    int cb;


    IGL_CHECKINITV ();
    if (!(IGL_CTX()->flags & IGL_WFLAGS_DOUBLEBUFFER))
    {
        return;
    }

    /* NOTE: we only support FRONT and BACK buffers!!!! */
    glGetIntegerv (GL_DRAW_BUFFER, &cb);
    if (enable)
    {
        /* frontbuffer already set? -> nothing to do */
        if (cb == GL_FRONT || cb == GL_FRONT_AND_BACK)
        {
            return;
        }
        else if (cb == GL_BACK)
        {
            glDrawBuffer (GL_FRONT_AND_BACK);
        }
        else if (cb == GL_NONE)
        {
            glDrawBuffer (GL_FRONT);
        }
    }
    else
    {
        if (cb == GL_FRONT)
        {
            glDrawBuffer (GL_NONE);
        }
        else if (cb == GL_FRONT_AND_BACK)
        {
            glDrawBuffer (GL_BACK);
        }
    }
}


/* DESC: leftbuffer, rightbuffer - enables and disables drawing to the left/right buffer */
void
leftbuffer (Boolean enable)
{
    igl->api->leftbuffer (enable);
}

void
iglExec_leftbuffer (Boolean enable)
{
    int cb;


    IGL_CHECKINITV ();
    glGetIntegerv (GL_DRAW_BUFFER, &cb);

    if (enable)
    {
        if (cb == GL_LEFT || cb == GL_FRONT || cb == GL_FRONT_LEFT || cb == GL_BACK ||
            cb == GL_BACK_LEFT || cb == GL_FRONT_AND_BACK)
        {
            return;
        }

        switch (cb)
        {
            case GL_NONE:
                cb = GL_LEFT;
                break;

            case GL_FRONT_RIGHT:
                cb = GL_FRONT;
                break;

            case GL_BACK_RIGHT:
                cb = GL_BACK;
                break;

            case GL_RIGHT:
                cb = GL_FRONT_AND_BACK;
                break;

            default:
                break;
        }
        glDrawBuffer (cb);
    }
    else
    {
        if (cb == GL_NONE || cb == GL_FRONT_RIGHT || cb == GL_BACK_RIGHT ||
            cb == GL_FRONT_AND_BACK)
        {
            return;
        }

        switch (cb)
        {
            case GL_LEFT:
            case GL_FRONT_LEFT:
            case GL_BACK_LEFT:
                cb = GL_NONE;
                break;

            case GL_FRONT:
                cb = GL_FRONT_RIGHT;
                break;

            case GL_BACK:
                cb = GL_BACK_RIGHT;
                break;

            case GL_FRONT_AND_BACK:
                cb = GL_RIGHT;
                break;

            default:
                break;
        }
        glDrawBuffer (cb);
    }
}


/* DESC: leftbuffer, rightbuffer - enables and disables drawing to the left/right buffer */
void
rightbuffer (Boolean enable)
{
    igl->api->rightbuffer (enable);
}

void
iglExec_rightbuffer (Boolean enable)
{
    int cb;


    IGL_CHECKINITV ();
    glGetIntegerv (GL_DRAW_BUFFER, &cb);

    if (enable)
    {
        if (cb == GL_RIGHT || cb == GL_FRONT || cb == GL_FRONT_RIGHT || cb == GL_BACK ||
            cb == GL_BACK_RIGHT || cb == GL_FRONT_AND_BACK)
        {
            return;
        }

        switch (cb)
        {
            case GL_NONE:
                cb = GL_RIGHT;
                break;

            case GL_FRONT_LEFT:
                cb = GL_FRONT;
                break;

            case GL_BACK_LEFT:
                cb = GL_BACK;
                break;

            case GL_LEFT:
                cb = GL_FRONT_AND_BACK;
                break;

            default:
                break;
        }
        glDrawBuffer (cb);
    }
    else
    {
        if (cb == GL_NONE || cb == GL_FRONT_LEFT || cb == GL_BACK_LEFT ||
            cb == GL_FRONT_AND_BACK)
        {
            return;
        }

        switch (cb)
        {
            case GL_RIGHT:
            case GL_FRONT_RIGHT:
            case GL_BACK_RIGHT:
                cb = GL_NONE;
                break;

            case GL_FRONT:
                cb = GL_FRONT_LEFT;
                break;

            case GL_BACK:
                cb = GL_BACK_LEFT;
                break;

            case GL_FRONT_AND_BACK:
                cb = GL_LEFT;
                break;

            default:
                break;
        }
        glDrawBuffer (cb);
    }
}


/* DESC: monobuffer - configures the framebuffer for monoscopic viewing */
void
monobuffer ()
{
    IGL_CHECKINITV ();
    IGL_CTX()->newFlags &= ~IGL_WFLAGS_STEREOBUFFER;
}


/* DESC: stereobuffer - configures the framebuffer for stereoscopic viewing */
void
stereobuffer ()
{
    IGL_CHECKINITV ();
    IGL_CTX()->newFlags |= IGL_WFLAGS_STEREOBUFFER;
}


/* DESC: getbuffer - indicates which buffers are enabled for writing */
long
getbuffer ()
{
    int cb, zb;
    long ret = 0;


    IGL_CHECKINIT (0);

    glGetIntegerv (GL_DEPTH_TEST, &zb);
    glGetIntegerv (GL_DRAW_BUFFER, &cb);

    if (zb == 1)
    {
        ret = DRAWZBUFFER;
    }

    switch (cb)
    {
        case GL_FRONT:
            ret |= FRNTBUFFER;
            break;

        case GL_BACK:
            ret |= BCKBUFFER;
            break;

        case GL_FRONT_AND_BACK:
            ret |= (BCKBUFFER | FRNTBUFFER);
            break;
    }

    return (ret);
}


/* DESC: backface - turns backfacing polygon removal on and off */
void
backface (Boolean enable)
{
    igl->api->backface (enable);
}

void
iglExec_backface (Boolean enable)
{
    int cm;


    IGL_CHECKINITV ();

    glGetIntegerv (GL_CULL_FACE_MODE, &cm);
    if (enable && cm == GL_FRONT)
    {
        glCullFace (GL_FRONT_AND_BACK);
    }
    else if (!enable && cm == GL_FRONT_AND_BACK)
    {
        glCullFace (GL_FRONT);
    }
}


/* DESC: getbackface - returns whether backfacing polygons will appear */
long
getbackface ()
{
    int cm;


    IGL_CHECKINIT (0);
    glGetIntegerv (GL_CULL_FACE_MODE, &cm);

    return (cm == GL_BACK || cm == GL_FRONT_AND_BACK);
}


/* DESC: frontface - turns frontfacing polygon removal on and off */
void
frontface (Boolean enable)
{
    igl->api->frontface (enable);
}

void
iglExec_frontface (Boolean enable)
{
    int cm;


    IGL_CHECKINITV ();

    glGetIntegerv (GL_CULL_FACE_MODE, &cm);
    if (enable && cm == GL_BACK)
    {
        glCullFace (GL_FRONT_AND_BACK);
    }
    else if (!enable && cm == GL_FRONT_AND_BACK)
    {
        glCullFace (GL_BACK);
    }
}


/* DESC: depthcue - turns depth-cue mode on and off */
void
depthcue (Boolean enable)
{
    igl->api->depthcue (enable);
}

void
iglExec_depthcue (Boolean enable)
{
    int cm;


    IGL_CHECKINITV ();

    glGetIntegerv (GL_FOG_MODE, &cm);

    /* (un)set the linear fog (used for depth-cueing) */
    if (enable && cm != GL_LINEAR)
    {
        glFogi (GL_FOG_MODE, GL_LINEAR);
        if (!glIsEnabled(GL_FOG))
        {
            glEnable (GL_FOG);
        }
    }
    else if (!enable && cm == GL_LINEAR)
    {
        glFogi (GL_FOG_MODE, GL_EXP);
        if (glIsEnabled(GL_FOG))
        {
            glDisable (GL_FOG);
        }
    }
}


/* DESC: getdcm - indicates whether depth-cue mode is on or off */
Boolean
getdcm ()
{
    int cm;


    IGL_CHECKINIT (FALSE);

    glGetIntegerv (GL_FOG_MODE, &cm);
    return (cm == GL_LINEAR && glIsEnabled(GL_FOG));
}


/* DESC: lshaderange - sets range of color indices used for depth-cueing */
void
lshaderange (Colorindex lowin, Colorindex highin, long znear, long zfar)
{
    igl->api->lshaderange (lowin, highin, znear, zfar);
}

void
iglExec_lshaderange (Colorindex lowin, Colorindex highin, long znear, long zfar)
{
    int clr;
    int fogClr[4];


    IGL_CHECKINITV ();
    glFogi (GL_FOG_START, znear);
    glFogi (GL_FOG_END, zfar);

    /* TBD: hmm... WTF do we do with lowin/highin? is this ok in RGBA mode? */
    clr = (highin+lowin)/2;
    fogClr[0] = GetRValue (clr);
    fogClr[1] = GetGValue (clr);
    fogClr[2] = GetBValue (clr);
    fogClr[3] = GetAValue (clr);
    glFogiv (GL_FOG_COLOR, fogClr);
}


/* DESC: shaderange - obsolete routine */
void
shaderange (Colorindex lowin, Colorindex highin, Screencoord z1, Screencoord z2)
{
    igl->api->lshaderange (lowin, highin, (long)z1, (long)z2);
}


#if 0
____________________________ Z-buffer functions ____________________________
() {}
#endif
/* DESC: zbuffer - enable or disable z-buffer operation in the current framebuffer */
void
zbuffer (Boolean enable)
{
    igl->api->zbuffer (enable);
}

void
iglExec_zbuffer (Boolean enable)
{
    IGL_CHECKINITV ();

    if (enable)
    {
        glEnable (GL_DEPTH_TEST);
    }
    else
    {
        glDisable (GL_DEPTH_TEST);
    }
}


/* DESC: getzbuffer - returns whether z-buffering is on or off */
Boolean
getzbuffer ()
{
    IGL_CHECKINIT (FALSE);
    return (glIsEnabled (GL_DEPTH_TEST));
}


/* DESC: zfunction - specifies the function used for z-buffer comparison by the current framebuffer */
void
zfunction (long func)
{
    igl->api->zfunction (func);
}

void
iglExec_zfunction (long func)
{
    int oglFunc;


    IGL_CHECKINITV ();

    switch (func)
    {
        case ZF_LESS:
            oglFunc = GL_LESS;
            break;

        case ZF_EQUAL:
            oglFunc = GL_EQUAL;
            break;

        case ZF_LEQUAL:
            oglFunc = GL_LEQUAL;
            break;

        case ZF_GREATER:
            oglFunc = GL_GREATER;
            break;

        case ZF_NOTEQUAL:
            oglFunc = GL_NOTEQUAL;
            break;

        case ZF_GEQUAL:
            oglFunc = GL_GEQUAL;
            break;

        case ZF_ALWAYS:
            oglFunc = GL_ALWAYS;
            break;

        case ZF_NEVER:
            oglFunc = GL_NEVER;
            break;

        default:
            return;
    }
    glDepthFunc (oglFunc);
}


/* DESC: zsource - selects the source for z-buffering comparisons */
void
zsource (long src)
{
    /* not supported in OpenGL */
}


/* DESC: zdraw - enables or disables drawing to the z-buffer */
void
zdraw (Boolean enable)
{
    /* no equivalent on OpenGL, so we just enable Z-buffer */
    zbuffer (enable);
}


/* DESC: zwritemask - specifies a write mask for the z-buffer of the current framebuffer */
void
zwritemask (unsigned long mask)
{
    /* not supported in OpenGL */
}


/* DESC: zclear - initializes the z-buffer of the current framebuffer */
void
zclear ()
{
    igl->api->zclear ();
}

void
iglExec_zclear ()
{
    IGL_CHECKINITV ();
    glClear (GL_DEPTH_BUFFER_BIT);
}


/* DESC: czclear - clears the color bitplanes and the z-buffer simultaneously */
void
czclear (unsigned long cval, long zval)
{
    igl->api->czclear (cval, zval);
}

void
iglExec_czclear (unsigned long cval, long zval)
{
    float cclear[4], dclear; /* current clear and depth colors */
    float cvalf[4], zvalf;   /* cval and zval converted to floats */


    IGL_CHECKINITV ();

    glGetFloatv (GL_COLOR_CLEAR_VALUE, cclear);
    glGetFloatv (GL_DEPTH_CLEAR_VALUE, &dclear);

    /* cval is in ABGR mode */
    cvalf[0] = (unsigned char)GetAValue (cval) / 255.0f;
    cvalf[1] = (unsigned char)GetBValue (cval) / 255.0f;
    cvalf[2] = (unsigned char)GetGValue (cval) / 255.0f;
    cvalf[3] = (unsigned char)GetRValue (cval) / 255.0f;
    zvalf = (float)zval/LONG_MAX;


    /* if colors differ from currently set ones, set them */
    if (memcmp (cvalf, cclear, 4*sizeof(float)) != 0)
    {
        glClearColor (cvalf[0], cvalf[1], cvalf[2], cvalf[3]);
    }
    if (zvalf != dclear)
    {
        glClearDepth (zvalf);
    }

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* restore colors */
    if (memcmp (cvalf, cclear, 4*sizeof(float)) != 0)
    {
        glClearColor (cclear[0], cclear[1], cclear[2], cclear[3]);
    }
    if (zvalf != dclear)
    {
        glClearDepth (dclear);
    }
}


/* DESC: zbsize - specifies the number of bit planes desired to comprise the Z buffer */
void
zbsize (long planes)
{
    IGL_CHECKINITV ();
    if (planes != 0 && planes != 8 && planes != 16 && planes != 24 && planes != 32)
    {
        return;
    }

    IGL_CTX ()->zbPlanes = (unsigned char)planes;
}


#if 0
_________________________ stencil buffer functions _________________________
() {}
#endif
/* DESC: sclear - clear the stencil planes to a specified value */
void
sclear (unsigned long sval)
{
    igl->api->sclear (sval);
}

void
iglExec_sclear (unsigned long sval)
{
    int cclear;


    IGL_CHECKINITV ();


    glGetIntegerv (GL_STENCIL_CLEAR_VALUE, &cclear);
    if ((unsigned long)cclear != sval)
    {
        glClearStencil (sval);
    }

    glClear (GL_STENCIL_BUFFER_BIT);

    if ((unsigned long)cclear != sval)
    {
        glClearStencil (cclear);
    }
}


/* DESC: stencil - alter the operating parameters of the stencil */
void
stencil (long enable, unsigned long ref, long func, unsigned long mask, long fail, long pass, long zpass)
{
    igl->api->stencil (enable, ref, func, mask, fail, pass, zpass);
}

void
iglExec_stencil (long enable, unsigned long ref, long func, unsigned long mask, long fail, long pass, long zpass)
{
    int ofunc, ofail;


    IGL_CHECKINITV ();

    if (enable)
    {
        glEnable (GL_STENCIL_TEST);
        switch (func)
        {
            case SF_NEVER:
                ofunc = GL_NEVER;
                break;

            case SF_LESS:
                ofunc = GL_LESS;
                break;

            case SF_EQUAL:
                ofunc = GL_EQUAL;
                break;

            case SF_LEQUAL:
                ofunc = GL_LEQUAL;
                break;

            case SF_GREATER:
                ofunc = GL_GREATER;
                break;

            case SF_NOTEQUAL:
                ofunc = GL_NOTEQUAL;
                break;

            case SF_GEQUAL:
                ofunc = GL_GEQUAL;
                break;

            case SF_ALWAYS:
                ofunc = GL_ALWAYS;
                break;

            default:
                return;
        }
        glStencilFunc (ofunc, ref, mask);

        switch (fail)
        {
            case ST_KEEP:
                ofail = GL_KEEP;
                break;

            case ST_ZERO:
                ofail = GL_ZERO;
                break;

            case ST_REPLACE:
                ofail = GL_REPLACE;
                break;

            case ST_INCR:
                ofail = GL_INCR;
                break;

            case ST_DECR:
                ofail = GL_DECR;
                break;

            case ST_INVERT:
                ofail = GL_INVERT;
                break;

            default:
                return;
        }
        glStencilOp (ofail, pass, zpass);
    }
    else
    {
        glDisable (GL_STENCIL_TEST);
    }
}


/* DESC: stensize - specify the number of bit planes desired to comprise the stencil buffer */
void
stensize (long planes)
{
    IGL_CHECKINITV ();
    if (planes < 0 || planes > 8)
    {
        return;
    }

    IGL_CTX ()->stenPlanes = (unsigned char)planes;
}


#if 0
______________________ accumulation buffer functions _______________________
() {}
#endif
/* DESC: acbuf - operate on the accumulation buffer */
void
acbuf (long op, float value)
{
    igl->api->acbuf (op, value);
}

void
iglExec_acbuf (long op, float value)
{
    IGL_CHECKINITV ();
    switch (op)
    {
        case AC_CLEAR:
            glClear (GL_ACCUM_BUFFER_BIT);
            break;

        case AC_ACCUMULATE:
            glAccum (GL_ACCUM, value);
            break;

        case AC_CLEAR_ACCUMULATE:
            glAccum (GL_LOAD, value);
            break;

        case AC_RETURN:
            glAccum (GL_RETURN, value);
            break;

        case AC_MULT:
            glAccum (GL_MULT, value);
            break;

        case AC_ADD:
            glAccum (GL_ADD, value);
            break;
    }
}


/* DESC: acsize - specify the number of bitplanes per color component in the accumulation buffer */
void
acsize (long planes)
{
    IGL_CHECKINITV ();
    if (planes != 0 && planes != 16)
    {
        return;
    }

    IGL_CTX ()->acPlanes = (unsigned char)planes;
}


#if 0
______________________________ misc functions ______________________________
() {}
#endif
/* DESC: finish - blocks until the Geometry Pipeline is empty */
void
finish ()
{
    IGL_CHECKINITV ();
    glFinish ();
}


/* DESC: gflush - flushs the remote GL client buffer */
void
gflush ()
{
    IGL_CHECKINITV ();
    glFlush ();
}


/* DESC: gsync -  waits for a vertical retrace period */
void
gsync ()
{
    int cv;


    IGL_CHECKINITV ();

    if (wglGetSwapIntervalEXT != NULL && wglSwapIntervalEXT != NULL)
    {
        /* if vsync is not set, set it */
        /* TBD: how do we disable vsync on IrisGL? */
        cv = wglGetSwapIntervalEXT ();

#if 0   /* TBD: causes access violation on Win32! */
        if (cv == 0)
        {
            wglSwapIntervalEXT (1);
        }
#endif
    }
}


/* DESC: swapinterval - defines a minimum time between buffer swaps */
void
swapinterval (short interval)
{
    IGL_CHECKINITV ();

    if (interval > 0 && wglSwapIntervalEXT != NULL)
    {
#if 0   /* TBD: causes access violation on Win32! */
        wglSwapIntervalEXT (interval);
#endif
    }
}


/* DESC: shademodel - selects the shading model */
void
shademodel (int mode)
{
    igl->api->shademodel (mode);
}

void
iglExec_shademodel (int mode)
{
    IGL_CHECKINITV ();
    glShadeModel (mode == FLAT ? GL_FLAT : GL_SMOOTH);
}


/* DESC: getsm - returns the current shading model */
long
getsm ()
{
    int sm;


    IGL_CHECKINIT (GOURAUD);

    glGetIntegerv (GL_SHADE_MODEL, &sm);
    return (sm == GL_FLAT ? FLAT : GOURAUD);
}


/* DESC: subpixel - controls the placement of point, line, and polygon vertices */
void
subpixel (Boolean enable)
{
    /* in OpenGL subpixel is always true */
}




