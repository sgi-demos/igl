#include "igl.h"
#include "iglcmn.h"



#if 0
____________________________ IGL helpers ____________________________
() {}
#endif
static void
_igl_parseTevDef (long index, long np, float *props)
{
    int i;
    igl_tevDefT *tvptr;
    GLboolean endParse = FALSE;


    tvptr = &igl->tevDefs[index];
    tvptr->tvMode = TV_MODULATE;    /* this is the default mode */

    for (i=0; !endParse; i++)
    {
        switch ((int)props[i])
        {
            /* these four are mutually exclusive */
            case TV_BLEND:
            case TV_DECAL:
            case TV_ALPHA:
            case TV_MODULATE:
                tvptr->tvMode = (int)props[i];
            break;

            case TV_COLOR:
                tvptr->flags |= IGL_TEVDEFFLAGS_COLOR;
                memcpy (tvptr->blendColor, &props[i+1], 4*sizeof(float));
                i += 4;
            break;

            case TV_COMPONENT_SELECT:
                tvptr->flags |= IGL_TEVDEFFLAGS_COMPONENT;
                tvptr->component = props[i+1];
                i ++;
            break;

            default:
            break;
        }

        /* if np is specified as non-zero, we parse np fields, otherwise we grep for TV_NULL */
        endParse = ((np > 0 && i == np) || props[i] == TV_NULL);
    }
}


static void
_igl_setTevDef (long index)
{
    igl_tevDefT *tvptr = &igl->tevDefs[0];


    switch (tvptr->tvMode)
    {
        case TV_BLEND:
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
        break;

        case TV_DECAL:
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        break;

/* TBD: what to do with this one? */
        case TV_ALPHA:
        break;

        /* if none specified, TV_MODULATE is the default */
        case TV_MODULATE:
        default:
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;
    }

    if (tvptr->flags & IGL_TEVDEFFLAGS_COLOR)
    {
        glTexEnvfv (GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, tvptr->blendColor);
    }
    if (tvptr->flags & IGL_TEVDEFFLAGS_COMPONENT)
    {
    }
}


static void
_igl_parseTexDef (long index, long np, float *props)
{
    int i;
    igl_texDefT *txptr;
    GLboolean endParse = FALSE;


    txptr = &igl->texDefs[index];
    memset (txptr, 0, sizeof(igl_texDefT));
    txptr->internalFormat = 0;  /* which is the default? */
    txptr->externalFormat = TX_PACK_8;

    for (i=0; !endParse; i++)
    {
        switch ((int)props[i])
        {
            case TX_FAST_DEFINE:
                break;

            case TX_MINFILTER:
                txptr->flags |= IGL_TEXDEFFLAGS_MINFILTER;
                txptr->minFilter = props[i+1];
                i ++;
                break;

            case TX_MAGFILTER:
                txptr->flags |= IGL_TEXDEFFLAGS_MAGFILTER;
                txptr->magFilter = props[i+1];
                i ++;
                break;

            case TX_MAGFILTER_ALPHA:
                txptr->flags |= IGL_TEXDEFFLAGS_MAGFILTERALPHA;
                txptr->magFilterAlpha = props[i+1];
                i ++;
                break;

            case TX_MAGFILTER_COLOR:
                txptr->flags |= IGL_TEXDEFFLAGS_MAGFILTERCOLOR;
                txptr->magFilterColor = props[i+1];
                i ++;
                break;

            case TX_WRAP:
                txptr->flags |= (IGL_TEXDEFFLAGS_WRAPS | IGL_TEXDEFFLAGS_WRAPT | IGL_TEXDEFFLAGS_WRAPR);
                txptr->wrapS = txptr->wrapT = txptr->wrapR = props[i+1];
                i ++;
                break;

            case TX_WRAP_S:
                txptr->flags |= IGL_TEXDEFFLAGS_WRAPS;
                txptr->wrapS = props[i+1];
                i ++;
                break;

            case TX_WRAP_T:
                txptr->flags |= IGL_TEXDEFFLAGS_WRAPT;
                txptr->wrapT = props[i+1];
                i ++;
                break;

            case TX_WRAP_R:
                txptr->flags |= IGL_TEXDEFFLAGS_WRAPR;
                txptr->wrapR = props[i+1];
                i ++;
                break;

            case TX_INTERNAL_FORMAT:
                txptr->internalFormat = (int)props[i+1];
                i ++;
                break;

            case TX_EXTERNAL_FORMAT:
                txptr->externalFormat = (int)props[i+1];
                i ++;
                break;

            case TX_MIPMAP_FILTER_KERNEL:
                txptr->flags |= IGL_TEXDEFFLAGS_MIPMAPFILTER;
                memcpy (txptr->mipmapFilter, &props[i+1], 8*sizeof(float));
                i += 8;
                break;

            case TX_CONTROL_POINT:
                txptr->flags |= IGL_TEXDEFFLAGS_CONTROLPOINT;
                txptr->lod = props[i+1];
                txptr->scale = props[i+2];
                i += 2;
                break;

            case TX_CONTROL_CLAMP:
                txptr->flags |= IGL_TEXDEFFLAGS_CLAMP;
                txptr->clamp = props[i+1];
                i ++;
                break;

            case TX_DETAIL:
                txptr->flags |= IGL_TEXDEFFLAGS_DETAIL;
                memcpy (txptr->detail, &props[i+1], 5*sizeof(float));
                i += 5;
                break;

            case TX_TILE:
                txptr->flags |= IGL_TEXDEFFLAGS_TILE;
                memcpy (txptr->tile, &props[i+1], 4*sizeof(float));
                i += 4;
                break;

            case TX_BICUBIC_FILTER:
                txptr->flags |= IGL_TEXDEFFLAGS_BICUBICFILTER;
                memcpy (txptr->bicubicFilter, &props[i+1], 2*sizeof(float));
                i += 2;
                break;

            default:
                break;
        }

        /* if np is specified as non-zero, we parse np fields, otherwise we grep for TX_NULL */
        endParse = ((np > 0 && i == np) || props[i] == TX_NULL);
    }
}


static void
_igl_setTexDef (long index)
{
    int oglType;
    igl_texDefT *txptr = &igl->texDefs[index];


    if (txptr->flags & IGL_TEXDEFFLAGS_MINFILTER)
    {
        oglType = -1;

        if (txptr->minFilter == TX_POINT)
        {
            oglType = GL_NEAREST;
        }
        else if (txptr->minFilter == TX_BILINEAR)
        {
            oglType = GL_LINEAR;
        }
        else if (txptr->minFilter == TX_BILINEAR_LEQUAL)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->minFilter == TX_BILINEAR_GEQUAL)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->minFilter == TX_TRILINEAR)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->minFilter == TX_BICUBIC)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->minFilter == TX_MIPMAP_POINT)
        {
            oglType = GL_NEAREST_MIPMAP_NEAREST;
        }
        else if (txptr->minFilter == TX_MIPMAP_LINEAR)
        {
            oglType = GL_NEAREST_MIPMAP_LINEAR;
        }
        else if (txptr->minFilter == TX_MIPMAP_BILINEAR)
        {
            oglType = GL_LINEAR_MIPMAP_NEAREST;
        }
        else if (txptr->minFilter == TX_MIPMAP_TRILINEAR)
        {
            oglType = GL_LINEAR_MIPMAP_LINEAR;
        }
        else if (txptr->minFilter == TX_MIPMAP_QUADLINEAR)
        {
            /* no equivalent in OpenGL */
        }

        if (oglType != -1)
        {
            /* TBD: 3D */
            glTexParameteri ((txptr->flags & IGL_TEXDEFFLAGS_3D) ? GL_TEXTURE_2D : GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER, oglType);
        }
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_MAGFILTER)
    {
        oglType = -1;

        if (txptr->magFilter == TX_POINT)
        {
            oglType = GL_NEAREST;
        }
        else if (txptr->magFilter == TX_BILINEAR)
        {
            oglType = GL_LINEAR;
        }
        else if (txptr->magFilter == TX_BILINEAR_LEQUAL)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->magFilter == TX_TRILINEAR)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->magFilter == TX_BICUBIC)
        {
            /* no equivalent in OpenGL */
        }
        else if (txptr->magFilter == TX_SHARPEN)
        {
            /* no equivalent in OpenGL */
        }

        if (oglType != -1)
        {
            /* TBD: 3D */
            glTexParameteri ((txptr->flags & IGL_TEXDEFFLAGS_3D) ? GL_TEXTURE_2D : GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER, oglType);
        }
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_MAGFILTERALPHA)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_MAGFILTERCOLOR)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_WRAPS)
    {
        oglType = -1;

        if (txptr->wrapS == TX_REPEAT)
        {
            oglType = GL_REPEAT;
        }
        else if (txptr->wrapS == TX_CLAMP)
        {
            oglType = GL_CLAMP;
        }
        else if (txptr->wrapS == TX_SELECT)
        {
            /* no equivalent in OpenGL */
        }

        if (oglType != -1)
        {
            /* TBD: 3D */
            glTexParameteri ((txptr->flags & IGL_TEXDEFFLAGS_3D) ? GL_TEXTURE_2D : GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S, oglType);
        }
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_WRAPT)
    {
        oglType = -1;

        if (txptr->wrapT == TX_REPEAT)
        {
            oglType = GL_REPEAT;
        }
        else if (txptr->wrapT == TX_CLAMP)
        {
            oglType = GL_CLAMP;
        }
        else if (txptr->wrapT == TX_SELECT)
        {
            /* no equivalent in OpenGL */
        }

        if (oglType != -1)
        {
            /* TBD: 3D */
            glTexParameteri ((txptr->flags & IGL_TEXDEFFLAGS_3D) ? GL_TEXTURE_2D : GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T, oglType);
        }
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_WRAPR)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_MIPMAPFILTER)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_CONTROLPOINT)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_CLAMP)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_DETAIL)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_TILE)
    {
        /* no equivalent in OpenGL */
    }

    if (txptr->flags & IGL_TEXDEFFLAGS_BICUBICFILTER)
    {
        /* no equivalent in OpenGL */
    }
}


#if 0
____________________________ tev|tex functions ____________________________
() {}
#endif
/* DESC: tevdef - defines a texture mapping environment */
void
tevdef (long index, long np, float *props)
{
    IGL_CHECKINITV ();
    if (index <= 0 || index > IGL_MAXTEVDEFS || props == NULL)
    {
        return;
    }

    _igl_parseTevDef (index, np, props);
}


/* DESC: tevbind -  selects a texture environment */
void
tevbind (long target, long index)
{
    igl->api->tevbind (target, index);
}

void
iglExec_tevbind (long target, long index)
{
    IGL_CHECKINITV ();
    if (index <= 0 || index > IGL_MAXTEVDEFS || target != TV_ENV0)
    {
        return;
    }

    _igl_setTevDef (index);
}


/* DESC: texdef2d - convert a 2-dimensional image into a texture */
void
texdef2d (long index, long nc, long width, long height, unsigned long *image, long np, float *props)
{
    int type, ointernalFormat, oFormat;
    igl_texDefT *txptr;


    /* TBD: texdef2d() should call texdef3d() width depth of 1 */

    IGL_CHECKINITV ();
    if (index <= 0 || index > IGL_MAXTEXDEFS || image == NULL || props == NULL ||
        width <= 0 || height <= 0 || nc < 1 || nc > 4)
    {
        return;
    }

    /* parse */
    _igl_parseTexDef (index, np, props);
    txptr = &igl->texDefs[index];

    switch (nc)
    {
        case 2:
            /* should be intensity + alpha */
            oFormat = GL_LUMINANCE_ALPHA;
        break;

        case 3:
//            oFormat = GL_BGR_EXT;
            oFormat = GL_RGB;
        break;

        case 4:
            /* TBD: we need ABGR */
            oFormat = GL_RGBA;
        break;

        case 1:
        default:
            /* should be intensity */
            oFormat = GL_LUMINANCE;
        break;
    }

    switch (txptr->externalFormat)
    {
        case TX_PACK_16:
            type = GL_UNSIGNED_SHORT;
        break;

        case TX_PACK_8:
        default:
            type = GL_UNSIGNED_BYTE;
        break;
    }

    switch (txptr->internalFormat)
    {
        case TX_I_12A_4:
            ointernalFormat = GL_LUMINANCE12_ALPHA4;   /* intensity 12 + alpha 4 not supported? */
        break;

        case TX_IA_8:
            ointernalFormat = GL_LUMINANCE8_ALPHA8;    /* intensity 8 + alpha 8 not supported? */
        break;

        case TX_RGB_5:
            ointernalFormat = GL_RGB5;
        break;

        case TX_RGBA_4:
            ointernalFormat = GL_RGBA4;
        break;

        case TX_IA_12:
            ointernalFormat = GL_LUMINANCE12_ALPHA12;   /* intensity 12 + alpha 12 not supported? */
        break;

        case TX_RGBA_8:
            ointernalFormat = GL_RGBA8;
        break;

        case TX_RGBA_12:
            ointernalFormat = GL_RGBA12;
        break;

        case TX_RGB_12:
            ointernalFormat = GL_RGB12;
        break;

        case TX_I_16:
            ointernalFormat = GL_INTENSITY16;
        break;

        default:
            ointernalFormat = 1;
        break;
    }

    glBindTexture (GL_TEXTURE_2D, index);
    glTexImage2D (GL_TEXTURE_2D, 0, ointernalFormat, width, height, 0, oFormat, type, image);
}


/* DESC: texdef3d - convert a 3-dimensional image into a texture */
void
texdef3d (long index, long nc, long width, long height, long depth, unsigned long *image, long np,
          float *props)
{
    int type;
    igl_texDefT *txptr;


    IGL_CHECKINITV ();
    if (index <= 0 || index > IGL_MAXTEXDEFS || image == NULL || props == NULL ||
        width <= 0 || height <= 0 || depth <= 0 || nc < 1 || nc > 4)
    {
        return;
    }


    /* parse */
    _igl_parseTexDef (index, np, props);
    txptr = &igl->texDefs[index];
    txptr->flags |= IGL_TEXDEFFLAGS_3D;

    switch (nc)
    {
        case 2:
            type = GL_UNSIGNED_SHORT;
        break;

        case 4:
            type = GL_UNSIGNED_INT;
        break;

        case 1:
        default:
            type = GL_UNSIGNED_BYTE;
        break;
    }
#if 0
    glBindTexture (GL_TEXTURE_3D, index);
    /* TBD: default type should probably be GL_BGRA_EXT. calculate which type is used! */
    glTexImage2D (GL_TEXTURE_3D, 0, nc, width, height, depth, 0, GL_RGBA, type, image);
#endif
}


/* DESC: texbind -  selects a texture function */
void
texbind (long target, long index)
{
    igl->api->texbind (target, index);
}

void
iglExec_texbind (long target, long index)
{
    igl_texDefT *txptr;


    IGL_CHECKINITV ();
    if ((target != TX_TEXTURE_0 && target != TX_TEXTURE_DETAIL && target != TX_TEXTURE_IDLE) ||
        index < 0 || index > IGL_MAXTEXDEFS || igl->matrixMode == MSINGLE)
    {
        return;
    }

    if (index == 0)
    {
        glBindTexture (GL_TEXTURE_2D, 0);
        glDisable (GL_TEXTURE_2D);

        /* TBD: bind && disable GL_TEXTURE_3D as well? */
        return;
    }


    _igl_setTexDef (index);
    txptr = &igl->texDefs[index];
    if (txptr->flags & IGL_TEXDEFFLAGS_3D)
    {
    }
    else
    {
        glEnable (GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, index);
    }
}


/* DESC: texgen - specify automatic generation of texture coordinates */
#define TXGL(_a) if (coord == TX_##_a) { oglType=GL_##_a; oglTex=GL_TEXTURE_GEN_##_a; }
void
texgen (long coord, long mode, float *params)
{
    int oglType = -1, oglTex = -1;


    IGL_CHECKINITV ();


    /* is this pretty or what? */
    TXGL(S)
    else TXGL(T)
    else TXGL(R)
    else TXGL(Q)

    if (oglType == -1)
    {
        return;
    }


    if (mode == TG_ON)
    {
        glEnable (oglTex);
        return;
    }
    else if (mode == TG_OFF)
    {
        glDisable (oglTex);
        return;
    }


    if (mode == TG_CONTOUR)
    {
        glTexGenf (oglType, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGenfv (oglType, GL_OBJECT_PLANE, params);
    }
    else if (mode == TG_LINEAR)
    {
        glTexGenf (oglType, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv (oglType, GL_EYE_PLANE, params);
    }
    else if (mode == TG_SPHEREMAP)
    {
        glTexGenf (oglType, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    }
}
#undef TXGL


#if 0
____________________________ t functions ____________________________
() {}
#endif
/* DESC: t2d, t2f, t2i, t2s, t3d, t3f, t3i, t3s, t4d, t4f, t4i, t4s - specifies a texture coordinate tuple */
void
t4f (float vector[4])
{
    igl->api->t4f (vector);
}

void
iglExec_t4f (float vector[4])
{
    IGL_CHECKINITV ();
    glTexCoord4fv (vector);
}


void
t4s (short vector[4])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->t4f (vector4);
}


void
t4i (long vector[4])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->t4f (vector4);
}


void
t4d (double vector[4])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = (float)vector[3];

    igl->api->t4f (vector4);
}


/* DESC: t2d, t2f, t2i, t2s, t3d, t3f, t3i, t3s, t4d, t4f, t4i, t4s - specifies a texture coordinate tuple */
void
t2s (short vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t2i (long vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t2f (float vector[2])
{
    float vector4[4];

    vector4[0] = vector[0];
    vector4[1] = vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t2d (double vector[2])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = 0.0f;
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


/* DESC: t2d, t2f, t2i, t2s, t3d, t3f, t3i, t3s, t4d, t4f, t4i, t4s - specifies a texture coordinate tuple */
void
t3s (short vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t3i (long vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t3f (float vector[3])
{
    float vector4[4];

    vector4[0] = vector[0];
    vector4[1] = vector[1];
    vector4[2] = vector[2];
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


void
t3d (double vector[3])
{
    float vector4[4];

    vector4[0] = (float)vector[0];
    vector4[1] = (float)vector[1];
    vector4[2] = (float)vector[2];
    vector4[3] = 1.0f;

    igl->api->t4f (vector4);
}


#if 0
____________________________ misc functions ____________________________
() {}
#endif
/* DESC: subtexload - load part or all of a texture defined with TX_FASTDEFINE */
void
subtexload (long target, long id, float s0, float s1, float t0, float t1, long numwords, unsigned long *texture,
            unsigned long flags)
{
    IGL_CHECKINITV ();
    /* flags=0 or 1 mean RGBA? */
    if (target != TX_TEXTURE_0 || (flags != 0 && flags != 1))
    {
        return;
    }

    /* not sure this is correct */
    glTexSubImage2D (GL_TEXTURE_2D, 0, (int)s0, (int)t0, (int)(s0+s1), (int)(t0+t1),
        GL_RGBA, GL_UNSIGNED_INT, texture);
}


/* DESC: fbsubtexload - load part or all of a texture defined with TX_FASTDEFINE */
void
fbsubtexload (long x, long y, long target, long id, float s0, float s1, float t0, float t1,
              unsigned long flags)
{
    /* nothing to do here? */
}


/* DESC: istexloaded - returns whether the texture with the passed id is resident in texture memory */
long
istexloaded (long target, long id)
{
    IGL_CHECKINIT (0);
    if (target != TX_TEXTURE_0 || id <= 0 || id > IGL_MAXTEXDEFS)
    {
        return (0);
    }

    /* hmm... is this correct? */
//    return (igl->texDefs[id].image != NULL);
    return (0);
}


