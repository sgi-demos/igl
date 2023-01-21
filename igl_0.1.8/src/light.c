#include "igl.h"
#include "iglcmn.h"




#if 0
____________________________ IGL helpers ____________________________
() {}
#endif
static void
_igl_setLmDefaults (short deftype, void *ptr)
{
    static igl_lmMaterialDefT defaultMaterialDef =
    {
        IGL_LMMATERIALFLAGS_NONE,
        { 0.2f, 0.2f, 0.2f, 1.0f },     /* ambient */
        { 0.0f, 127.5f, 255.0f, 1.0f }, /* colorIndexes */
        { 0.8f, 0.8f, 0.8f, 1.0f },     /* diffuse (+ alpha) */
        { 0.0f, 0.0f, 0.0f, 1.0f },     /* emission */
        0.0f,                           /* shininess */
        { 0.0f, 0.0f, 0.0f, 1.0f }      /* specular */
    };
    static igl_lmLightDefT defaultLightDef =
    {
        IGL_LMLIGHTFLAGS_NONE,
        { 0.0f, 0.0f, 0.0f, 1.0f },     /* ambient */
        { 1.0f, 1.0f, 1.0f, 1.0f },     /* lcolor */
        { 0.0f, 0.0f, 1.0f, 0.0f },     /* position */
        { 0.0f, 0.0f, -1.0f, 0.0f },    /* spotDirection */
        { 0.0f, 180.0f },               /* spotLight */
        -1
    };
    static igl_lmLmodelDefT defaultLmodelDef =
    {
        IGL_LMMODELFLAGS_NONE,
        { 0.2f, 0.2f, 0.2f, 1.0f },     /* ambient */
        { 1.0f, 0.0f },                 /* attenuation */
        0.0f,                           /* attenuation2 */
        0.0f,                           /* localViewer */
        0.0f                            /* twoSide */
    };


    switch (deftype)
    {
        case DEFMATERIAL:
            memcpy (ptr, &defaultMaterialDef, sizeof (igl_lmMaterialDefT));
        break;

        case DEFLIGHT:
            memcpy (ptr, &defaultLightDef, sizeof (igl_lmLightDefT));
        break;

        case DEFLMODEL:
            memcpy (ptr, &defaultLmodelDef, sizeof (igl_lmLmodelDefT));
        break;
    }
}


static void
_igl_parseLmMaterial (short index, short np, float *lmdefs)
{
    int i, cp;
    igl_lmMaterialDefT *lmptr;
    GLboolean endParse = FALSE;


    lmptr = &igl->materialDefs[index];
    _igl_setLmDefaults (DEFMATERIAL, lmptr);

    if (lmdefs == NULL)
    {
        return;
    }


    cp = 0;     /* counted props */
    for (i=0; !endParse; i++)
    {
        switch ((int)lmdefs[i])
        {
            case ALPHA: /* = 4th parameter to diffuse */
                lmptr->flags |= IGL_LMMATERIALFLAGS_DIFFUSE;
                lmptr->diffuse[3] = lmdefs[i+1];
                i ++;
            break;

            case AMBIENT:
                lmptr->flags |= IGL_LMMATERIALFLAGS_AMBIENT;
                memcpy (&lmptr->ambient[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case COLORINDEXES:
                lmptr->flags |= IGL_LMMATERIALFLAGS_COLORINDEXES;
                /* ignored when in RGBA mode! */
                memcpy (&lmptr->colorIndexes[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case DIFFUSE:
                lmptr->flags |= IGL_LMMATERIALFLAGS_DIFFUSE;
                memcpy (&lmptr->diffuse[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case EMISSION:
                lmptr->flags |= IGL_LMMATERIALFLAGS_EMISSION;
                memcpy (&lmptr->emission[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case SHININESS:
                lmptr->flags |= IGL_LMMATERIALFLAGS_SHININESS;
                lmptr->shininess = lmdefs[i+1];
                i ++;
            break;

            case SPECULAR:
                lmptr->flags |= IGL_LMMATERIALFLAGS_SPECULAR;
                memcpy (&lmptr->specular[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;
        }

        cp ++;
        endParse = ((np > 0 && cp == np) || lmdefs[i] == LMNULL);
    }
}


void
_igl_setLmMaterial (short target, short index)
{
    float props[4];
    unsigned long rgb;
    int ogltype = (target == MATERIAL) ? GL_FRONT : GL_BACK;
    igl_lmMaterialDefT *lmptr = &igl->materialDefs[index];


    if (IGL_CTX ()->flags & IGL_WFLAGS_RGBA)
    {
        if (lmptr->flags & IGL_LMMATERIALFLAGS_AMBIENT)
        {
            glMaterialfv (ogltype, GL_AMBIENT, lmptr->ambient);
        }
        if (lmptr->flags & IGL_LMMATERIALFLAGS_DIFFUSE)
        {
            glMaterialfv (ogltype, GL_DIFFUSE, lmptr->diffuse);
        }
        if (lmptr->flags & IGL_LMMATERIALFLAGS_EMISSION)
        {
            glMaterialfv (ogltype, GL_EMISSION, lmptr->emission);
        }
        if (lmptr->flags & IGL_LMMATERIALFLAGS_SPECULAR)
        {
            glMaterialfv (ogltype, GL_SPECULAR, lmptr->specular);
        }
    }
    else
    {
        if (lmptr->flags & IGL_LMMATERIALFLAGS_COLORINDEXES)
        {
            /* IGL has fake colormap mode, so we adjust RGB ambient/diffuse/specular */
            props[3] = 1.0f;

#define BTF(_b) ((2.0f*(_b) + 1.0f) * (1.0f/255.0f))        /* byte to float (-128..+127 to -1..+1) */
            rgb = igl->colorPalette[(long)lmptr->colorIndexes[0]];
            props[0] = BTF (GetRValue (rgb));
            props[1] = BTF (GetBValue (rgb));
            props[2] = BTF (GetGValue (rgb));
            glMaterialfv (ogltype, GL_AMBIENT, props);

            rgb = igl->colorPalette[(long)lmptr->colorIndexes[1]];
            props[0] = BTF (GetRValue (rgb));
            props[1] = BTF (GetBValue (rgb));
            props[2] = BTF (GetGValue (rgb));
            glMaterialfv (ogltype, GL_DIFFUSE, props);

            rgb = igl->colorPalette[(long)lmptr->colorIndexes[2]];
            props[0] = BTF (GetRValue (rgb));
            props[1] = BTF (GetBValue (rgb));
            props[2] = BTF (GetGValue (rgb));
            glMaterialfv (ogltype, GL_SPECULAR, props);
#undef BTF
        }
    }

    if (lmptr->flags & IGL_LMMATERIALFLAGS_SHININESS)
    {
        glMaterialf (ogltype, GL_SHININESS, lmptr->shininess);
    }
}


static void
_igl_parseLmLight (short index, short np, float *lmdefs)
{
    int i, cp;
    short lightTarget;
    igl_lmLightDefT *lmptr;
    GLboolean endParse = FALSE;


    lmptr = &igl->lightDefs[index];
    lightTarget = lmptr->lightTarget;
    _igl_setLmDefaults (DEFLIGHT, lmptr);
    lmptr->lightTarget = lightTarget;

    if (lmdefs == NULL)
    {
        return;
    }


    cp = 0;     /* counted props */
    for (i=0; !endParse; i++)
    {
        switch ((int)lmdefs[i])
        {
            case AMBIENT:
                lmptr->flags |= IGL_LMLIGHTFLAGS_AMBIENT;
                memcpy (&lmptr->ambient[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case LCOLOR:
                lmptr->flags |= IGL_LMLIGHTFLAGS_LCOLOR;
                memcpy (&lmptr->lcolor[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case POSITION:
                lmptr->flags |= IGL_LMLIGHTFLAGS_POSITION;
                memcpy (&lmptr->position[0], &lmdefs[i+1], 4*sizeof(float));
                i += 4;
            break;

            case SPOTDIRECTION:
                lmptr->flags |= IGL_LMLIGHTFLAGS_SPOTDIRECTION;
                memcpy (&lmptr->spotDirection[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case SPOTLIGHT:
                lmptr->flags |= IGL_LMLIGHTFLAGS_SPOTLIGHT;
                memcpy (&lmptr->spotLight[0], &lmdefs[i+1], 2*sizeof(float));
                i += 2;
            break;
        }

        cp ++;
        endParse = ((np > 0 && cp == np) || lmdefs[i] == LMNULL);
    }
}


void
_igl_setLmLight (short target, short index)
{
    int oglLight;
    igl_lmLightDefT *lmptr = &igl->lightDefs[index];


    oglLight = GL_LIGHT0 + (target - LIGHT0);

    
    if (IGL_CTX ()->flags & IGL_WFLAGS_RGBA)
    {
        if (lmptr->flags & IGL_LMLIGHTFLAGS_AMBIENT)
        {
            glLightfv (oglLight, GL_AMBIENT, lmptr->ambient);
            /* TBD: */
//            glLightfv (oglLight, GL_DIFFUSE, {1.0f, 1.0f, 1.0f, 1.0f});
//            glLightfv (oglLight, GL_SPECULAR, {1.0f, 1.0f, 1.0f, 1.0f});
        }
        if (lmptr->flags & IGL_LMLIGHTFLAGS_LCOLOR)
        {
            /* TBD: no equivalent in OpenGL? */
            glLightfv (oglLight, GL_DIFFUSE, lmptr->lcolor);
        }
        if (lmptr->flags & IGL_LMLIGHTFLAGS_SPOTDIRECTION)
        {
            glLightfv (oglLight, GL_SPOT_DIRECTION, lmptr->spotDirection);
        }
        if (lmptr->flags & IGL_LMLIGHTFLAGS_SPOTLIGHT)
        {
            glLightf (oglLight, GL_SPOT_EXPONENT, lmptr->spotLight[0]);
            glLightf (oglLight, GL_SPOT_CUTOFF, lmptr->spotLight[1]);
        }
    }

    if (lmptr->flags & IGL_LMLIGHTFLAGS_POSITION)
    {
        glLightfv (oglLight, GL_POSITION, lmptr->position);
    }

    glEnable (oglLight);
}


static void
_igl_parseLmLightModel (short index, short np, float *lmdefs)
{
    int i, cp;
    igl_lmLmodelDefT *lmptr;
    GLboolean endParse = FALSE;


    lmptr = &igl->lmodelDefs[index];
    _igl_setLmDefaults (DEFLMODEL, lmptr);

    if (lmdefs == NULL)
    {
        return;
    }


    cp = 0;
    for (i=0; !endParse; i++)
    {
        switch ((int)lmdefs[i])
        {
            case AMBIENT:
                lmptr->flags |= IGL_LMMODELFLAGS_AMBIENT;
                memcpy (&lmptr->ambient[0], &lmdefs[i+1], 3*sizeof(float));
                i += 3;
            break;

            case ATTENUATION:
                lmptr->flags |= IGL_LMMODELFLAGS_ATTENUATION;
                memcpy (&lmptr->attenuation[0], &lmdefs[i+1], 2*sizeof(float));
                i += 2;
            break;

            case ATTENUATION2:
                lmptr->flags |= IGL_LMMODELFLAGS_ATTENUATION2;
                lmptr->attenuation2 = lmdefs[i+1];
                i ++;
            break;

            case LOCALVIEWER:
                lmptr->flags |= IGL_LMMODELFLAGS_LOCALVIEWER;
                lmptr->localViewer = (lmdefs[i+1] == 1.0f) ? 1.0f : 0.0f;
                i ++;
            break;

            case TWOSIDE:
                lmptr->flags |= IGL_LMMODELFLAGS_TWOSIDE;
                lmptr->twoSide = (lmdefs[i+1] == 1.0f) ? 1.0f : 0.0f;
                i ++;
            break;
        }

        cp ++;
        endParse = ((np > 0 && cp == np) || lmdefs[i] == LMNULL);
    }
}


void
_igl_setLmLightModel (short target, short index)
{
    int i;
    float lightPosition[4];
    igl_lmLmodelDefT *lmptr = &igl->lmodelDefs[index];


    if (IGL_CTX ()->flags & IGL_WFLAGS_RGBA)
    {
        if (lmptr->flags & IGL_LMMODELFLAGS_AMBIENT)
        {
            glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmptr->ambient);
        }
        if (lmptr->flags & IGL_LMMODELFLAGS_TWOSIDE)
        {
            glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, lmptr->twoSide);
        }

        /*
            NOTE: ATTENUATION[2] specifies the attenuation factors associated with ALL
            non-infinite light sources - those whose w-coordinate differs from 0.0.
            as OpenGL doesn't support this with one API call, we change all affected lights.
        */
        if ((lmptr->flags & IGL_LMMODELFLAGS_ATTENUATION) || (lmptr->flags & IGL_LMMODELFLAGS_ATTENUATION2))
        {
            for (i=0; i < MAXLIGHTS; i++)
            {
                glGetLightfv (GL_LIGHT0+i, GL_POSITION, lightPosition);
                if (lightPosition[4] == 0.0f)   /* infinite light source */
                {
                    continue;
                }

                if (lmptr->flags & IGL_LMMODELFLAGS_ATTENUATION)
                {
                    glLightf (GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lmptr->attenuation[0]);
                    glLightf (GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lmptr->attenuation[1]);
                }
                if (lmptr->flags & IGL_LMMODELFLAGS_ATTENUATION2)
                {
                    glLightf (GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, lmptr->attenuation2);
                }
            }
        }
    }

    if (lmptr->flags & IGL_LMMODELFLAGS_LOCALVIEWER)
    {
        glLightModelf (GL_LIGHT_MODEL_LOCAL_VIEWER, lmptr->localViewer);
    }
}


#if 0
____________________________ light functions ____________________________
() {}
#endif
/* DESC: lmdef - defines or modifies a material, light source, or lighting model */
void
lmdef (short deftype, short index, short np, float *props)
{
    IGL_CHECKINITV ();
    if (index <= 0 || index >= IGL_MAXLMDEFS)
    {
        return;
    }


    /* NOTE: if we're changing a currently bound definition, changes take effect immediately (lmbind!) */
    switch (deftype)
    {
        case DEFMATERIAL:
            _igl_parseLmMaterial (index, np, props);
            if (igl->materialIndex == index)
            {
                _igl_setLmMaterial (MATERIAL, index);
            }
        break;

        case DEFLIGHT:
            _igl_parseLmLight (index, np, props);
            if (igl->lightIndex == index)
            {
                _igl_setLmLight (igl->lightDefs[index].lightTarget, index);
            }
        break;

        case DEFLMODEL:
            _igl_parseLmLightModel (index, np, props);
            if (igl->lmodelIndex == index)
            {
                _igl_setLmLightModel (LMODEL, index);
            }
        break;
    }
}


/* DESC: lmbind - selects a new material, light source, or lighting model */
void
lmbind (short target, short index)
{
    igl->api->lmbind (target, index);
}

void
iglExec_lmbind (short target, short index)
{
    IGL_CHECKINITV ();

    /* lighting can be bound only in MVIEWING mode */
    if (igl->matrixMode != MVIEWING || index < 0 || index > IGL_MAXLMDEFS)
    {
        return;
    }

    /* index=0 disables lighting when target is MATERIAL or LMODEL! */
    if (index == 0 && (target == MATERIAL || target == LMODEL))
    {
        glDisable (GL_LIGHTING);
        return;
    }

    switch (target)
    {
        case MATERIAL:
        case BACKMATERIAL:
            _igl_setLmMaterial (target, index);
            if (target == MATERIAL)
            {
                igl->materialIndex = index;
            }
        break;

        case LIGHT0:
        case LIGHT1:
        case LIGHT2:
        case LIGHT3:
        case LIGHT4:
        case LIGHT5:
        case LIGHT6:
        case LIGHT7:
            _igl_setLmLight (target, index);
            igl->lightIndex = index;
            igl->lightDefs[index].lightTarget = target;
        break;

        case LMODEL:
            _igl_setLmLightModel (target, index);
            igl->lmodelIndex = index;
        break;
    }

    /* if both MATERIAL and LMODEL indexes are non-zero, we are enlightened */
    if (igl->materialIndex != 0 && igl->lmodelIndex != 0)
    {
        glEnable (GL_LIGHTING);
    }
}


/* DESC: lmcolor - change the effect of color commands while lighting is active */
void
lmcolor (long mode)
{
    igl->api->lmcolor (mode);
}

void
iglExec_lmcolor (long mode)
{
    IGL_CHECKINITV ();


    /* is this correct? manual states it affects c()/cpack()/RGBcolor() functions only... */
    if (!IGL_CTX ()->flags & IGL_WFLAGS_RGBA)
    {
        return;
    }
    if (mode == LMC_NULL)
    {
        glDisable (GL_COLOR_MATERIAL);
        return;
    }


    /* lmcolor() doesn't change properties to BACKMATERIAL */
    switch (mode)
    {
        case LMC_COLOR: /* nothing to do for color? */
        break;

        case LMC_EMISSION:
            glColorMaterial (GL_FRONT, GL_EMISSION);
        break;

        case LMC_AMBIENT:
            glColorMaterial (GL_FRONT, GL_AMBIENT);
        break;

        case LMC_DIFFUSE:
            glColorMaterial (GL_FRONT, GL_DIFFUSE);
        break;

        case LMC_SPECULAR:
            glColorMaterial (GL_FRONT, GL_SPECULAR);
        break;

        case LMC_AD:
            glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        break;
    }    

    glEnable (GL_COLOR_MATERIAL);
}


