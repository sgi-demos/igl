#include "igl.h"
#include "iglcmn.h"
#include "iglobj.h"
#include <stdarg.h>


/*********************************************************************
 to add a function named MYFUNC, change the following:

 iglobj.h:
	- add an enum to igl_opcodeT named OP_MYFUNC
	- declare functions iglExec_myfunc () and iglObj_myfunc ()

 iglcmn.h:
	- add myfunc to igl_apiList struct

 object.c:
	- add case OP_MYFUNC to _igl_callFunc () and optional case OP_MYFUNC to _igl_deleteObjectFcn ()
	- define function iglObj_myfunc (), which adds to the object (see e.g. iglObj_clear())

 <appropriate .c file>:
	- define function myfunc (), which runs igl->api->myfunc
	- define function iglExec_myfunc (), which does the job
 *********************************************************************/



#define IGL_OBJTAG_RANGE    2147483647      /* INT_MAX or MAX_INT */
#define IGL_OBJDEF_TAG      1
#define IGL_OBJDEF_FCN      2

#define IGL_CHECKOBJ(_ret)                  \
igl_objectT *obj;                           \
igl_objFcnT *fcn;                           \
if (igl->currentObjIdx == -1)               \
{                                           \
    return (_ret);                          \
}                                           \
obj = &igl->objects[igl->currentObjIdx];

#define IGL_CHECKOBJV()                     \
igl_objectT *obj;                           \
igl_objFcnT *fcn;                           \
if (igl->currentObjIdx == -1)               \
{                                           \
    return;                                 \
}                                           \
obj = &igl->objects[igl->currentObjIdx];

#define IGLFCN(_op)                         \
fcn = _igl_addObjectFcn (obj, _op);         \
if (fcn == NULL)                            \
{                                           \
    return (-1);                            \
}

#define IGLFCNV(_op)                        \
fcn = _igl_addObjectFcn (obj, _op);         \
if (fcn == NULL)                            \
{                                           \
    return;                                 \
}


typedef union
{
    char c;
    unsigned char uc;
    short s;
    unsigned short us;
    int i;
    unsigned int ui;
    float f;
    void *v;
} _igl_objFcnParamU;

typedef struct
{
    char type;
    int opcode;
    _igl_objFcnParamU params[10];
} igl_objFcnT;

typedef struct
{
    char type;
    int id;
} igl_objTagT;


#if PLATFORM_WIN32
static void
_igl_printf (char *fmt, ...)
{
    va_list ap;
    char str[256+1];


    va_start (ap, fmt);
    _vsnprintf (str, 256, fmt, ap);
    OutputDebugString (str);
    va_end (ap);
}
#else
#   define _igl_printf printf
#endif
static void
_igl_dumpObject (igl_objectT *obj)
{
    char *ptr;
    igl_objTagT *tagPtr;
    igl_objFcnT *fcnPtr;
    int idx;


    _igl_printf ("obj ptr=%p id=%d chunkSize=%li chunk=%p beginPtr=%p curPtr=%p endPtr=%p\n",
        obj, obj->id, obj->chunkSize, obj->chunk, obj->beginPtr, obj->curPtr, obj->endPtr);
    if (obj->chunk == NULL)
    {
        _igl_printf ("\n");
        return;
    }

    idx = 0;
    ptr = obj->chunk;
    while (1)
    {
        switch (*ptr)
        {
            case IGL_OBJDEF_TAG:
                tagPtr = (igl_objTagT *)ptr;
                _igl_printf ("\t[%d]TAG id=%d\n", idx, tagPtr->id);
                idx ++;
                ptr += sizeof (igl_objTagT);
            break;

            case IGL_OBJDEF_FCN:
                fcnPtr = (igl_objFcnT *)ptr;
                _igl_printf ("\t[%d]FCN opcode=%d\n", idx, fcnPtr->opcode);
                idx ++;
                ptr += sizeof (igl_objFcnT);
            break;
        }

        if (ptr > obj->endPtr)
        {
            _igl_printf ("\n");
            break;
        }
    }
}


#define FREEPARAM(_idx) if (fcn->params[_idx].v != NULL) free (fcn->params[_idx].v);
static void
_igl_deleteObjectFcn (igl_objFcnT *fcn)
{
    switch (fcn->opcode)
    {
        case OP_CALLFUNC:
        case OP_CLIPPLANE:
            FREEPARAM (2);
        break;

        case OP_FOGVERTEX:
        case OP_LCHARSTR:
        case OP_POLF:
        case OP_POLFI:
        case OP_POLFS:
        case OP_POLF2:
        case OP_POLF2I:
        case OP_POLF2S:
        case OP_POLY:
        case OP_POLYI:
        case OP_POLYS:
        case OP_POLY2:
        case OP_POLY2I:
        case OP_POLY2S:
        case OP_PWLCURVE:
        case OP_CRVN:
        case OP_RCRVN:
            FREEPARAM (1);
        break;

        case OP_C4F:
        case OP_C4I:
        case OP_C4S:
        case OP_V4F:
        case OP_N3F:
        case OP_T4F:
        case OP_LOADMATRIX:
        case OP_MULTMATRIX:
            FREEPARAM (0);
        break;

        case OP_NURBSCURVE:
            FREEPARAM (1);
            FREEPARAM (3);
        break;

        case OP_NURBSSURFACE:
            FREEPARAM (1);
            FREEPARAM (3);
            FREEPARAM (6);
        break;

        case OP_SPLF:
        case OP_SPLFI:
        case OP_SPLFS:
        case OP_SPLF2:
        case OP_SPLF2I:
        case OP_SPLF2S:
            FREEPARAM (1);
            FREEPARAM (2);
        break;

        default:
        break;
    }
}


static igl_objectT *
_igl_findObject (Object id, int *idx)
{
    int i;


    for (i=0; i < igl->numAllocatedObjects; i++)
    {
        if (igl->objects[i].id == id)
        {
            if (idx != NULL)
            {
                *idx = i;
            }
            return (&igl->objects[i]);
        }
    }

    if (idx != NULL)
    {
        *idx = -1;
    }
    return (NULL);
}


static igl_objectT *
_igl_allocObject (int *idx)
{
    igl_objectT *objects;


    /* try to find an "empty" object in the list */
    _igl_findObject (0, idx);
    if (*idx == -1)
    {
        /* if we don't have room, realloc the list */
        if (igl->numObjects + 1 >= igl->numAllocatedObjects)
        {
            /* remember old ptr in case of realloc() failure */
            objects = igl->objects;

            igl->objects = (igl_objectT *)realloc (igl->objects,
                (igl->numAllocatedObjects + 100)*sizeof (igl_objectT));
            if (igl->objects == NULL)
            {
                igl->objects = objects;
                return (NULL);
            }

            igl->numAllocatedObjects += 100;
        }

        igl->numObjects ++;
        *idx = igl->numObjects-1;
    }
    else
    {
        igl->numObjects ++;
    }

    return (&igl->objects[*idx]);
}


static void
_igl_deleteObject (igl_objectT *obj)
{
    char *ptr;


    if (obj->chunk != NULL)
    {
        ptr = obj->beginPtr + sizeof (igl_objTagT);
        while (ptr != obj->endPtr)
        {
            switch (*ptr)
            {
                case IGL_OBJDEF_TAG:
                    ptr += sizeof (igl_objTagT);
                break;

                case IGL_OBJDEF_FCN:
                    _igl_deleteObjectFcn ((igl_objFcnT *)ptr);
                    ptr += sizeof (igl_objFcnT);
                break;
            }
        }

        free (obj->chunk);
        obj->chunk = NULL;
    }
    obj->id = 0;
    obj->chunkSize = 0;
    obj->beginPtr = obj->curPtr = obj->endPtr = NULL;
    igl->numObjects --;
}


static int
_igl_reallocObjectChunk (igl_objectT *obj, int newSize)
{
    int curDiff, endDiff;
    char *chunk = obj->chunk;


    curDiff = obj->curPtr - obj->beginPtr;
    endDiff = obj->endPtr - obj->beginPtr;

    obj->chunk = realloc (obj->chunk, newSize);
    if (obj->chunk == NULL)
    {
        obj->chunk = chunk;
        return (-1);
    }
    obj->beginPtr = obj->chunk;
    obj->curPtr = obj->beginPtr + curDiff;
    obj->endPtr = obj->beginPtr + endDiff;
    obj->chunkSize = newSize;

    return (0);
}


static igl_objTagT *
_igl_findObjectTag (igl_objectT *obj, Tag t)
{
    igl_objTagT *tagPtr;
    char *ptr;


    if (t == STARTTAG)
    {
        return ((igl_objTagT *)obj->beginPtr);
    }
    else if (t == ENDTAG)
    {
        return ((igl_objTagT *)obj->endPtr);
    }

    ptr = obj->beginPtr + sizeof (igl_objTagT);
    while (ptr != obj->endPtr)
    {
        switch (*ptr)
        {
            case IGL_OBJDEF_TAG:
                tagPtr = (igl_objTagT *)ptr;
                if (tagPtr->id == t)
                {
                    return (tagPtr);
                }
                ptr += sizeof (igl_objTagT);
            break;

            case IGL_OBJDEF_FCN:
                ptr += sizeof (igl_objFcnT);
            break;
        }
    }

    return (NULL);
}


static void
_igl_addObjectTag (igl_objectT *obj, Tag t, Boolean addCurrent)
{
    igl_objTagT *tag;


    /* check whether we need to resize object's chunk (as we must insert a tag) */
    if ((obj->endPtr + sizeof (igl_objTagT)) >= (obj->chunk + obj->chunkSize))
    {
        if (_igl_reallocObjectChunk (obj, obj->chunkSize + igl->objChunkSize) != 0)
        {
            return;
        }
    }

    memmove (obj->curPtr + sizeof (igl_objTagT), obj->curPtr, obj->endPtr + sizeof (igl_objTagT) - obj->curPtr);
    obj->endPtr += sizeof (igl_objTagT);
    /* create a new tag in the middle */
    tag = (igl_objTagT *)obj->curPtr;
    tag->type = IGL_OBJDEF_TAG;
    tag->id = t;

    obj->curPtr += sizeof (igl_objTagT);

#if 0
    _igl_printf ("_igl_addObjectTag (%d)\n", t);
    _igl_dumpObject (obj);
#endif
}


static void
_igl_deleteObjectTag (igl_objectT *obj, igl_objTagT *tag)
{
    char *srcPtr;


    srcPtr = (char *)tag + sizeof (igl_objTagT);
    memmove ((char *)tag, srcPtr, obj->endPtr + sizeof (igl_objTagT) - srcPtr);
    obj->endPtr -= sizeof (igl_objTagT);
    obj->curPtr = obj->endPtr;

#if 0
    _igl_printf ("_igl_deleteObjectTag (%d)\n", tag->id);
    _igl_dumpObject (obj);
#endif
}


static igl_objFcnT *
_igl_addObjectFcn (igl_objectT *obj, int opcode)
{
    igl_objFcnT *fcn;


    /* check whether we need to resize object's chunk (as we must insert a tag) */
    if ((obj->endPtr + sizeof (igl_objFcnT)) >= (obj->chunk + obj->chunkSize))
    {
        if (_igl_reallocObjectChunk (obj, obj->chunkSize + igl->objChunkSize) != 0)
        {
            return (NULL);
        }
    }

    memmove (obj->curPtr + sizeof (igl_objFcnT), obj->curPtr, obj->endPtr + sizeof (igl_objTagT) - obj->curPtr);
    obj->endPtr += sizeof (igl_objFcnT);
    /* create a new tag in the middle */
    fcn = (igl_objFcnT *)obj->curPtr;
    fcn->type = IGL_OBJDEF_FCN;
    fcn->opcode = opcode;

    obj->curPtr += sizeof (igl_objFcnT);

#if 0
    _igl_printf ("_igl_addObjectFcn (%d)\n", opcode);
    _igl_dumpObject (obj);
#endif

    return (fcn);
}


static void
_igl_callFunc (igl_objFcnT *fcn)
{
    switch (fcn->opcode)
    {
        case OP_FULLSCRN:
            iglExec_fullscrn ();
        break;

        case OP_ENDFULLSCRN:
            iglExec_endfullscrn ();
        break;

        case OP_CLEAR:
            iglExec_clear ();
        break;

        case OP_LSETDEPTH:
            iglExec_lsetdepth (fcn->params[0].i, fcn->params[1].i);
        break;

        case OP_GETDEPTH:
            iglExec_getdepth ((Screencoord *)fcn->params[0].v, (Screencoord *)fcn->params[1].v);
        break;

        case OP_BACKBUFFER:
            iglExec_backbuffer (fcn->params[0].i);
        break;

        case OP_FRONTBUFFER:
            iglExec_frontbuffer (fcn->params[0].i);
        break;

        case OP_LEFTBUFFER:
            iglExec_leftbuffer (fcn->params[0].i);
        break;

        case OP_RIGHTBUFFER:
            iglExec_rightbuffer (fcn->params[0].i);
        break;

        case OP_BACKFACE:
            iglExec_backface (fcn->params[0].i);
        break;

        case OP_FRONTFACE:
            iglExec_frontface (fcn->params[0].i);
        break;

        case OP_DEPTHCUE:
            iglExec_depthcue (fcn->params[0].i);
        break;

        case OP_LSHADERANGE:
            iglExec_lshaderange (fcn->params[0].us, fcn->params[1].us, fcn->params[2].i, fcn->params[3].i);
        break;

        case OP_ZBUFFER:
            iglExec_zbuffer (fcn->params[0].i);
        break;

        case OP_ZFUNCTION:
            iglExec_zfunction (fcn->params[0].i);
        break;

        case OP_ZCLEAR:
            iglExec_zclear ();
        break;

        case OP_CZCLEAR:
            iglExec_czclear (fcn->params[0].ui, fcn->params[1].i);
        break;

        case OP_SCLEAR:
            iglExec_sclear (fcn->params[0].ui);
        break;

        case OP_STENCIL:
            iglExec_stencil (fcn->params[0].i, fcn->params[1].ui, fcn->params[2].i, fcn->params[3].ui,
                fcn->params[4].i, fcn->params[5].i, fcn->params[6].i);
        break;

        case OP_ACBUF:
            iglExec_acbuf (fcn->params[0].i, fcn->params[1].f);
        break;

        case OP_SHADEMODEL:
            iglExec_shademodel (fcn->params[0].i);
        break;

        case OP_COLOR:
            iglExec_color (fcn->params[0].us);
        break;

        case OP_MAPCOLOR:
            iglExec_mapcolor (fcn->params[0].us, fcn->params[1].s, fcn->params[2].s, fcn->params[3].s);
        break;

        case OP_CPACK:
            iglExec_cpack (fcn->params[0].ui);
        break;

        case OP_RGBCOLOR:
            iglExec_RGBcolor (fcn->params[0].s, fcn->params[1].s, fcn->params[2].s);
        break;

        case OP_AFUNCTION:
            iglExec_afunction (fcn->params[0].i, fcn->params[1].i);
        break;

        case OP_BLENDFUNCTION:
            iglExec_blendfunction (fcn->params[0].i, fcn->params[1].i);
        break;

        case OP_DITHER:
            iglExec_dither (fcn->params[0].i);
        break;

        case OP_FOGVERTEX:
            iglExec_fogvertex (fcn->params[0].i, (float *)fcn->params[0].v);
        break;

        case OP_LOGICOP:
            iglExec_logicop (fcn->params[0].i);
        break;

        case OP_C4F:
            iglExec_c4f ((float *)fcn->params[0].v);
        break;

        case OP_C4I:
            iglExec_c4i ((int *)fcn->params[0].v);
        break;

        case OP_C4S:
            iglExec_c4s ((short *)fcn->params[0].v);
        break;

        case OP_MMODE:
            iglExec_mmode (fcn->params[0].s);
        break;

        case OP_LOADMATRIX:
            iglExec_loadmatrix (fcn->params[0].v);
        break;

        case OP_MULTMATRIX:
            iglExec_multmatrix (fcn->params[0].v);
        break;

        case OP_PUSHMATRIX:
            iglExec_pushmatrix ();
        break;

        case OP_POPMATRIX:
            iglExec_popmatrix ();
        break;

        case OP_ROT:
            iglExec_rot (fcn->params[0].f, fcn->params[1].c);
        break;

        case OP_ROTATE:
            iglExec_rotate (fcn->params[0].s, fcn->params[1].c);
        break;

        case OP_TRANSLATE:
            iglExec_translate (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_SCALE:
            iglExec_scale (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_POLARVIEW:
            iglExec_polarview (fcn->params[0].f, fcn->params[1].s, fcn->params[2].s, fcn->params[3].s);
        break;

        case OP_CLIPPLANE:
            iglExec_clipplane (fcn->params[0].i, fcn->params[1].i, (float *)fcn->params[2].v);
        break;

        case OP_ORTHO:
            iglExec_ortho (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f,
                fcn->params[4].f, fcn->params[5].f);
        break;

        case OP_ORTHO2:
            iglExec_ortho2 (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f);
        break;

        case OP_WINDOW:
            iglExec_window (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f,
                fcn->params[4].f, fcn->params[5].f);
        break;

        case OP_PERSPECTIVE:
            iglExec_perspective (fcn->params[0].s, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f);
        break;

        case OP_VIEWPORT:
            iglExec_viewport (fcn->params[0].s, fcn->params[1].s, fcn->params[2].s, fcn->params[3].s);
        break;

        case OP_GETVIEWPORT:
            iglExec_getviewport (fcn->params[0].v, fcn->params[1].v, fcn->params[2].v, fcn->params[3].v);
        break;

        case OP_RESHAPEVIEWPORT:
            iglExec_reshapeviewport ();
        break;

        case OP_PUSHVIEWPORT:
            iglExec_pushviewport ();
        break;

        case OP_POPVIEWPORT:
            iglExec_popviewport ();
        break;

        case OP_LOOKAT:
            iglExec_lookat (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f,
                fcn->params[4].f, fcn->params[5].f, fcn->params[6].s);
        break;

        case OP_SCRMASK:
            iglExec_scrmask (fcn->params[0].s, fcn->params[1].s, fcn->params[2].s, fcn->params[3].s);
        break;

        case OP_GETSCRMASK:
            iglExec_getscrmask (fcn->params[0].v, fcn->params[1].v, fcn->params[2].v, fcn->params[3].v);
        break;

        case OP_POLF:
            iglExec_polf (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLFI:
            iglExec_polfi (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLFS:
            iglExec_polfs (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLF2:
            iglExec_polf2 (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLF2I:
            iglExec_polf2i (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLF2S:
            iglExec_polf2s (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLY:
            iglExec_poly (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLYI:
            iglExec_polyi (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLYS:
            iglExec_polys (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLY2:
            iglExec_poly2 (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLY2I:
            iglExec_poly2i (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLY2S:
            iglExec_poly2s (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_POLYMODE:
            iglExec_polymode (fcn->params[0].i);
        break;

        case OP_POLYSMOOTH:
            iglExec_polysmooth (fcn->params[0].i);
        break;

        case OP_PMV:
            iglExec_pmv (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_PDR:
            iglExec_pdr (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_PCLOS:
            iglExec_pclos ();
        break;

        case OP_PNT:
            iglExec_pnt (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_PNTSIZEF:
            iglExec_pntsizef (fcn->params[0].f);
        break;

        case OP_PNTSMOOTH:
            iglExec_pntsmooth (fcn->params[0].ui);
        break;

        case OP_RECT:
            iglExec_rect (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f);
        break;

        case OP_RECTF:
            iglExec_rectf (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].f);
        break;

        case OP_V4F:
            iglExec_v4f ((float *)fcn->params[0].v);
        break;

        case OP_ARC:
            iglExec_arc (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].s,
                fcn->params[4].s);
        break;

        case OP_ARCF:
            iglExec_arcf (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f, fcn->params[3].s,
                fcn->params[4].s);
        break;

        case OP_CIRC:
            iglExec_circ (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_CIRCF:
            iglExec_circf (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_BGNTMESH:
            iglExec_bgntmesh ();
        break;

        case OP_BGNCLOSEDLINE:
            iglExec_bgnclosedline ();
        break;

        case OP_BGNCURVE:
            iglExec_bgncurve ();
        break;

        case OP_ENDCURVE:
            iglExec_endcurve ();
        break;

        case OP_NURBSCURVE:
            iglExec_nurbscurve (fcn->params[0].i, fcn->params[1].v, fcn->params[2].i, fcn->params[3].v,
                fcn->params[4].i, fcn->params[5].i);
        break;

        case OP_BGNSURFACE:
            iglExec_bgnsurface ();
        break;

        case OP_ENDSURFACE:
            iglExec_endsurface ();
        break;

        case OP_NURBSSURFACE:
            iglExec_nurbssurface (fcn->params[0].i, fcn->params[1].v, fcn->params[2].i, fcn->params[3].v,
                fcn->params[4].i, fcn->params[5].i, fcn->params[6].v, fcn->params[7].i, fcn->params[8].i,
                fcn->params[9].i);
        break;

        case OP_PWLCURVE:
            iglExec_pwlcurve (fcn->params[0].i, fcn->params[1].v, fcn->params[2].i, fcn->params[3].i);
        break;

        case OP_SETNURBSPROPERTY:
            iglExec_setnurbsproperty (fcn->params[0].i, fcn->params[1].f);
        break;

        case OP_BGNLINE:
            iglExec_bgnline ();
        break;

        case OP_BGNPOINT:
            iglExec_bgnpoint ();
        break;

        case OP_ENDPOINT:
            iglExec_endpoint ();
        break;

        case OP_BGNPOLYGON:
            iglExec_bgnpolygon ();
        break;

        case OP_BGNQSTRIP:
            iglExec_bgnqstrip ();
        break;

        case OP_CRVN:
            iglExec_crvn (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_RCRVN:
            iglExec_rcrvn (fcn->params[0].i, fcn->params[1].v);
        break;

        case OP_CURVEPRECISION:
            iglExec_curveprecision (fcn->params[0].i);
        break;

        case OP_NMODE:
            iglExec_nmode (fcn->params[0].i);
        break;

        case OP_N3F:
            iglExec_n3f ((float *)fcn->params[0].v);
        break;

        case OP_LINESMOOTH:
            iglExec_linesmooth (fcn->params[0].ui);
        break;

        case OP_LINEWIDTHF:
            iglExec_linewidthf (fcn->params[0].f);
        break;

        case OP_DEFLINESTYLE:
            iglExec_deflinestyle (fcn->params[0].s, fcn->params[1].us);
        break;

        case OP_SETLINESTYLE:
            iglExec_setlinestyle (fcn->params[0].s);
        break;

        case OP_LSREPEAT:
            iglExec_lsrepeat (fcn->params[0].i);
        break;

        case OP_LMBIND:
            iglExec_lmbind (fcn->params[0].s, fcn->params[1].s);
        break;

        case OP_LMCOLOR:
            iglExec_lmcolor (fcn->params[0].i);
        break;

        case OP_MOVE:
            iglExec_move (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_RMV:
            iglExec_rmv (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_RPMV:
            iglExec_rpmv (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_DRAW:
            iglExec_draw (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_RDR:
            iglExec_rdr (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_RPDR:
            iglExec_rpdr (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_SPLF:
            iglExec_splf (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_SPLFI:
            iglExec_splfi (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_SPLFS:
            iglExec_splfs (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_SPLF2:
            iglExec_splf2 (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_SPLF2I:
            iglExec_splf2i (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_SPLF2S:
            iglExec_splf2s (fcn->params[0].i, fcn->params[1].v, fcn->params[2].v);
        break;

        case OP_CALLOBJ:
            iglExec_callobj (fcn->params[0].i);
        break;

        case OP_CALLFUNC:
#ifdef MACHINE_X86   /* WARNING: this only works on X86 machines (Win32 only for now) */
        {
            long i, l;


            /* push the parameters to the stack (in reverse order!) */
            for (i=(long)fcn->params[1].i-1; i >= 0; i--)
            {
                l = ((long *)fcn->params[2].v)[i];
                __asm
                {
                    mov eax, l
                    push eax
                }
            }
            /* push nargs */
            l = (long)fcn->params[1].i;
            __asm
            {
                mov eax, l
                push eax
            }

            /* call the function */
            l = (long)fcn->params[0].v;
            __asm   call l

            /* clean up the stack remains using ebx (NOTE: +1 is for nargs) */
            for (i=0; i < (long)fcn->params[1].i+1; i++)
            {
                __asm   pop ebx
            }
        }
#endif
        break;

        case OP_TEVBIND:
            iglExec_tevbind (fcn->params[0].i, fcn->params[1].i);
        break;

        case OP_TEXBIND:
            iglExec_texbind (fcn->params[0].i, fcn->params[1].i);
        break;

        case OP_T4F:
            iglExec_t4f ((float *)fcn->params[0].v);
        break;

        case OP_CMOV:
            iglExec_cmov (fcn->params[0].f, fcn->params[1].f, fcn->params[2].f);
        break;

        case OP_LCHARSTR:
            iglExec_lcharstr (fcn->params[0].i, (char *)fcn->params[1].v);
        break;

        case OP_PUSHATTRIBUTES:
            iglExec_pushattributes ();
        break;

        case OP_POPATTRIBUTES:
            iglExec_popattributes ();
        break;

        case OP_SETPATTERN:
            iglExec_setpattern (fcn->params[0].s);
        break;

        default:
        break;
    }
}


#define API(_fcn)                       \
igl->execApi->_fcn = iglExec_##_fcn;    \
igl->objApi->_fcn = iglObj_##_fcn;

int
igl_initApiList ()
{
    int ret = 0;


    igl->execApi = malloc (sizeof (igl_apiList));
    igl->objApi = malloc (sizeof (igl_apiList));
    igl->objects = (igl_objectT *)malloc (100 * sizeof (igl_objectT));
    FCN_EXIT (igl->execApi == NULL || igl->objApi == NULL || igl->objects == NULL, -1, {});

    memset (igl->execApi, 0, sizeof (igl_apiList));
    memset (igl->objApi, 0, sizeof (igl_apiList));
    memset (igl->objects, 0, 100 * sizeof (igl_objectT));

    igl->numAllocatedObjects = 100;
    igl->numObjects = 0;
    igl->currentObjIdx = -1;
    igl->objChunkSize = IGL_DEFOBJCHUNKSIZE;    /* 1020 bytes was IRIX default */


    igl->api = igl->execApi;
    /* initialize both tables, execution and object (display list in OpenGL dialect) */
    API (fullscrn);
    API (endfullscrn);
    API (clear);
    API (lsetdepth);
    API (getdepth);
    API (backbuffer);
    API (frontbuffer);
    API (leftbuffer);
    API (rightbuffer);
    API (backface);
    API (frontface);
    API (depthcue);
    API (lshaderange);
    API (zbuffer);
    API (zfunction);
    API (zclear);
    API (czclear);
    API (sclear);
    API (stencil);
    API (acbuf);
    API (shademodel);
    API (color);
    API (mapcolor);
    API (cpack);
    API (RGBcolor);
    API (afunction);
    API (blendfunction);
    API (dither);
    API (fogvertex);
    API (logicop);
    API (c4f);
    API (c4i);
    API (c4s);
    API (mmode);
    API (loadmatrix);
    API (multmatrix);
    API (pushmatrix);
    API (popmatrix);
    API (rot);
    API (rotate);
    API (translate);
    API (scale);
    API (polarview);
    API (clipplane);
    API (ortho);
    API (ortho2);
    API (window);
    API (perspective);
    API (viewport);
    API (getviewport);
    API (reshapeviewport);
    API (pushviewport);
    API (popviewport);
    API (lookat);
    API (scrmask);
    API (getscrmask);
    API (polf);
    API (polfi);
    API (polfs);
    API (polf2);
    API (polf2i);
    API (polf2s);
    API (poly);
    API (polyi);
    API (polys);
    API (poly2);
    API (poly2i);
    API (poly2s);
    API (polymode);
    API (polysmooth);
    API (pmv);
    API (pdr);
    API (pclos);
    API (pnt);
    API (pntsizef);
    API (pntsmooth);
    API (rect);
    API (rectf);
    API (v4f);
    API (arc);
    API (arcf);
    API (circ);
    API (circf);
    API (bgntmesh);
    API (bgnclosedline);
    API (bgncurve);
    API (endcurve);
    API (nurbscurve);
    API (bgnsurface);
    API (endsurface);
    API (nurbssurface);
    API (pwlcurve);
    API (setnurbsproperty);
    API (bgnline);
    API (bgnpoint);
    API (endpoint);
    API (bgnpolygon);
    API (bgnqstrip);
    API (crvn);
    API (rcrvn);
    API (curveprecision);
    API (nmode);
    API (n3f);
    API (linesmooth);
    API (linewidthf);
    API (deflinestyle);
    API (setlinestyle);
    API (lsrepeat);
    API (lmbind);
    API (lmcolor);
    API (move);
    API (rmv);
    API (rpmv);
    API (draw);
    API (rdr);
    API (rpdr);
    API (splf);
    API (splfi);
    API (splfs);
    API (splf2);
    API (splf2i);
    API (splf2s);
    API (callobj);
    API (tevbind);
    API (texbind);
    API (t4f);
    API (cmov);
    API (lcharstr);
    API (pushattributes);
    API (popattributes);
    API (setpattern);

exit:
    return (ret);
}


#if 0
_____________________________________________________
(){}
#endif
Boolean
isobj (Object id)
{
    return ((id > 0 && _igl_findObject (id, NULL) != NULL) ? TRUE : FALSE);
}


Object
genobj ()
{
    Object id = 1;  /* is 0 a valid object id? */


    while (1)
    {
        if (_igl_findObject (id, NULL) == NULL)
        {
            /* not found in the list, this will be ok */
            return (id);
        }

        id ++;
        if (id >= IGL_OBJTAG_RANGE)
        {
            break;
        }
    }

    return (-1);
}


void
makeobj (Object id)
{
    igl_objectT *obj;
    igl_objTagT *tag;


    /* should we allow makeobj() if no windows are created yet? */
    IGL_CHECKINITV ();

    /* makeobj() cannot be used within makeobj() */
    if (id <= 0 || igl->currentObjIdx != -1)
    {
        return;
    }

    obj = _igl_findObject (id, &igl->currentObjIdx);
    if (obj != NULL)
    {
        /* already exists -> will be overwritten */
        _igl_deleteObject (obj);
    }
    else
    {
        obj = _igl_allocObject (&igl->currentObjIdx);
    }

    if (obj == NULL || igl->currentObjIdx == -1)
    {
        return;
    }

    obj->id = id;

    /* allocate the first chunk for this object */
    obj->chunk = (char *)malloc (igl->objChunkSize);
    memset (obj->chunk, 0, igl->objChunkSize);
    obj->chunkSize = igl->objChunkSize;
    igl->api = igl->objApi;

    /* add start and end tags to the object */
    obj->beginPtr = obj->chunk;
    tag = (igl_objTagT *)obj->beginPtr;
    tag->type = IGL_OBJDEF_TAG;
    tag->id = STARTTAG;

    obj->endPtr = obj->beginPtr + sizeof (igl_objTagT);
    tag = (igl_objTagT *)obj->endPtr;
    tag->type = IGL_OBJDEF_TAG;
    tag->id = ENDTAG;

    obj->curPtr = obj->endPtr;
#if 0
    _igl_printf ("makeobj (%d)\n", id);
    _igl_dumpObject (obj);
#endif
}


void
closeobj ()
{
    igl_objectT *obj;


    if (igl->currentObjIdx != -1)
    {
        obj = &igl->objects[igl->currentObjIdx];
        obj->curPtr = obj->beginPtr + sizeof (igl_objTagT);
        igl->currentObjIdx = -1;
        igl->api = igl->execApi;
    }
}


void
delobj (Object id)
{
    igl_objectT *obj;


    if (id <= 0)
    {
        return;
    }

    obj = _igl_findObject (id, NULL);
    if (obj != NULL)
    {
        _igl_deleteObject (obj);
    }
}


void
editobj (Object id)
{
    igl_objectT *obj;


    if (igl->currentObjIdx != -1 || id <= 0)
    {
        return;
    }

    obj = _igl_findObject (id, &igl->currentObjIdx);
    if (obj != NULL)
    {
        obj->curPtr = obj->endPtr;
        igl->api = igl->objApi;
    }
}


Object
getopenobj ()
{
    return (igl->currentObjIdx != -1 ? igl->objects[igl->currentObjIdx].id : -1);
}


void
callobj (Object id)
{
    igl->api->callobj (id);
}

void
iglExec_callobj (Object id)
{
    char *ptr;
    igl_objectT *obj;


    if (id <= 0)
    {
        return;
    }

    obj = _igl_findObject (id, NULL);
    if (obj == NULL)
    {
        return;
    }

    ptr = obj->beginPtr + sizeof (igl_objTagT);
    while (ptr != obj->endPtr)
    {
        switch (*ptr)
        {
            case IGL_OBJDEF_TAG:
                ptr += sizeof (igl_objTagT);
            break;

            case IGL_OBJDEF_FCN:
                _igl_callFunc ((igl_objFcnT *)ptr);
                ptr += sizeof (igl_objFcnT);
            break;
        }
    }
}


#if 0
_____________________________________________________
(){}
#endif
Tag
gentag ()
{
    int id = 1;


    if (igl->currentObjIdx == -1)
    {
        return (-1);
    }

    while (1)
    {
        if (_igl_findObjectTag (&igl->objects[igl->currentObjIdx], id) == NULL)
        {
            return (id);
        }

        id ++;
        if (id >= IGL_OBJTAG_RANGE)
        {
            break;
        }
    }

    return (-1);
}


void
maketag (Tag t)
{
    igl_objectT *obj;


    if (igl->currentObjIdx == -1 || t == STARTTAG || t == ENDTAG)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    if (_igl_findObjectTag (obj, t) != NULL)
    {
        /* tag exists within current object, nothing to do here */
        return;
    }
    _igl_addObjectTag (obj, t, TRUE);
}


void
deltag (Tag t)
{
    igl_objTagT *tag;
    igl_objectT *obj;


    if (igl->currentObjIdx == -1 || t == STARTTAG || t == ENDTAG)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    tag = _igl_findObjectTag (obj, t);
    if (tag != NULL)
    {
        _igl_deleteObjectTag (obj, tag);
    }
}


Boolean
istag (Tag t)
{
    if (igl->currentObjIdx == -1)
    {
        return (FALSE);
    }
    else if (t == STARTTAG || t == ENDTAG ||
        _igl_findObjectTag (&igl->objects[igl->currentObjIdx], t) != NULL)
    {
        return (TRUE);
    }

    return (FALSE);
}


void
newtag (Tag newtg, Tag oldtg, Offset offset)
{
    int count;
    igl_objectT *obj;
    igl_objTagT *tag;
    char *ptr;


    /* note: oldtg can be STARTTAG */
    if (igl->currentObjIdx == -1 || newtg == STARTTAG || newtg == ENDTAG || oldtg == ENDTAG)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    if (_igl_findObjectTag (obj, newtg) != NULL)
    {
        /* newtg already exists, bail out */
        return;
    }
    tag = _igl_findObjectTag (obj, oldtg);
    if (tag == NULL)
    {
        /* oldtg doesn't exist, bail out */
        return;
    }

    count = 0;
    ptr = (char *)tag + sizeof (igl_objTagT);
    while (ptr != obj->endPtr && count != offset)
    {
        switch (*ptr)
        {
            case IGL_OBJDEF_TAG:
                ptr += sizeof (igl_objTagT);
            break;

            case IGL_OBJDEF_FCN:
                count ++;
                if (count != offset)
                {
                    ptr += sizeof (igl_objFcnT);
                }
            break;
        }
    }

    /* new tag placeholder wasn't found */
    if (count != offset)
    {
        return;
    }

    obj->curPtr = ptr;
    _igl_addObjectTag (obj, newtg, FALSE);
}


#if 0
_____________________________________________________
(){}
#endif
void
objinsert (Tag t)
{
    igl_objectT *obj;
    igl_objTagT *tag;


    if (igl->currentObjIdx == -1 || t == ENDTAG)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    tag = _igl_findObjectTag (obj, t);
    if (tag == NULL)
    {
        return;
    }

    obj->curPtr = ((char *)tag + sizeof (igl_objTagT));
}


void
objdelete (Tag tag1, Tag tag2)
{
    igl_objectT *obj;
    igl_objTagT *t1, *t2;
    int size;


    if (igl->currentObjIdx == -1)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    t1 = _igl_findObjectTag (obj, tag1);
    if (t1 == NULL)
    {
        return;
    }
    t2 = _igl_findObjectTag (obj, tag2);
    if (t2 == NULL || ((char *)t2 < (char *)t1) || ((char *)t2 - (char *)t1 == sizeof (igl_objTagT)))
    {
        return;
    }

    size = obj->endPtr + sizeof (igl_objTagT) - (char *)t2;
    memmove ((char *)t1 + sizeof (igl_objTagT), t2, size);
    /*
        (char *)t2 - (char *)t1 - sizeof (igl_objTagT) is the amount
        of bytes for which we moved t2 towards t1. so we must move endPtr
        for the same amount
    */
    obj->endPtr -= ((char *)t2 - (char *)t1 - sizeof (igl_objTagT));
    obj->curPtr = obj->endPtr;
#if 0
    _igl_printf ("objdelete (%d, %d)\n", tag1, tag2);
    _igl_dumpObject (obj);
#endif
}


void
objreplace (Tag t)
{
    igl_objectT *obj;
    igl_objTagT *tag1, *tag2;
    Tag t1, t2;
    char *ptr;


    if (igl->currentObjIdx == -1 || t == ENDTAG)
    {
        return;
    }

    obj = &igl->objects[igl->currentObjIdx];
    tag1 = _igl_findObjectTag (obj, t);
    if (tag1 == NULL)
    {
        return;
    }

    ptr = (char *)tag1 + sizeof (igl_objTagT);
    tag2 = NULL;
    while (tag2 == NULL)
    {
        switch (*ptr)
        {
            case IGL_OBJDEF_TAG:
                tag2 = (igl_objTagT *)ptr;
            break;

            case IGL_OBJDEF_FCN:
                ptr += sizeof (igl_objFcnT);
            break;
        }
    }

    t1 = tag1->id;
    t2 = tag2->id;
    objdelete (t1, t2);
    objinsert (t1);
}


void
chunksize (long chunk)
{
    if (chunk > 0)
    {
        igl->objChunkSize = chunk;
    }
}


void
compactify (Object id)
{
    int newSize;
    igl_objectT *obj;


    if (id <= 0)
    {
        return;
    }
    obj = _igl_findObject (id, NULL);
    if (obj == NULL || obj->chunkSize <= 0)
    {
        return;
    }

    /* compact the chunk to fit tags/ops from STARTTAG to ENDTAG */
    newSize = obj->endPtr + sizeof (igl_objTagT) - obj->beginPtr;
    if (obj->chunkSize > newSize)
    {
        _igl_reallocObjectChunk (obj, newSize);
    }
}


void
callfunc (void (*fctn)(), long nargs, ...)
{
    int i;
    va_list ap;
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CALLFUNC);


    fcn->params[0].v = (void *)fctn;
    fcn->params[1].i = (int)nargs;
    fcn->params[2].v = malloc (nargs * sizeof (long));
    if (fcn->params[2].v == NULL)
    {
        fcn->type = 0;
        fcn->opcode = OP_NONE;
        return;
    }

    va_start (ap, nargs);
    for (i=0; i < nargs; i++)
    {
        ((long *)fcn->params[2].v)[i] = va_arg (ap, long);
    }
    va_end (ap);
}


#if 0
_____________________________________________________
(){}
#endif
void
iglObj_fullscrn ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_FULLSCRN);
}

void
iglObj_endfullscrn ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ENDFULLSCRN);
}

void
iglObj_clear ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CLEAR);
}

void
iglObj_lsetdepth (long near_, long far_)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LSETDEPTH);
    fcn->params[0].i = near_;
    fcn->params[1].i = far_;
}

void
iglObj_getdepth (Screencoord *near_, Screencoord *far_)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_GETDEPTH);
    fcn->params[0].v = near_;
    fcn->params[1].v = far_;
}

void
iglObj_backbuffer (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BACKBUFFER);
    fcn->params[0].i = enable;
}

void
iglObj_frontbuffer (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_FRONTBUFFER);
    fcn->params[0].i = enable;
}

void
iglObj_leftbuffer (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LEFTBUFFER);
    fcn->params[0].i = enable;
}

void
iglObj_rightbuffer (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RIGHTBUFFER);
    fcn->params[0].i = enable;
}

void
iglObj_backface (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BACKFACE);
    fcn->params[0].i = enable;
}

void
iglObj_frontface (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_FRONTFACE);
    fcn->params[0].i = enable;
}

void
iglObj_depthcue (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_DEPTHCUE);
    fcn->params[0].i = enable;
}

void
iglObj_lshaderange (Colorindex lowin, Colorindex highin, long znear, long zfar)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LSHADERANGE);
    fcn->params[0].us = lowin;
    fcn->params[1].us = highin;
    fcn->params[2].i = znear;
    fcn->params[3].i = zfar;
}

void
iglObj_zbuffer (Boolean enable)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ZBUFFER);
    fcn->params[0].i = enable;
}

void
iglObj_zfunction (long func)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ZFUNCTION);
    fcn->params[0].i = func;
}

void
iglObj_zclear ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ZCLEAR);
}

void
iglObj_czclear (unsigned long cval, long zval)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CZCLEAR);
    fcn->params[0].ui = cval;
    fcn->params[1].i = zval;
}

void
iglObj_sclear (unsigned long sval)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SCLEAR);
    fcn->params[0].ui = sval;
}

void
iglObj_stencil (long enable, unsigned long ref, long func, unsigned long mask, long fail, long pass, long zpass)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_STENCIL);
    fcn->params[0].i = enable;
    fcn->params[1].ui = ref;
    fcn->params[2].i = func;
    fcn->params[3].ui = mask;
    fcn->params[4].i = fail;
    fcn->params[5].i = pass;
    fcn->params[6].i = zpass;
}

void
iglObj_acbuf (long op, float value)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ACBUF);
    fcn->params[0].i = op;
    fcn->params[1].f = value;
}

void
iglObj_shademodel (int mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SHADEMODEL);
    fcn->params[0].i = mode;
}

void
iglObj_color (Colorindex c)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_COLOR);
    fcn->params[0].us = c;
}

void
iglObj_mapcolor (Colorindex i, short r, short g, short b)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_MAPCOLOR);
    fcn->params[0].us = i;
    fcn->params[1].s = r;
    fcn->params[2].s = g;
    fcn->params[3].s = b;
}

void
iglObj_cpack (unsigned long color)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CPACK);
    fcn->params[0].ui = color;
}

void
iglObj_RGBcolor (short r, short g, short b)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RGBCOLOR);
    fcn->params[0].s = r;
    fcn->params[1].s = g;
    fcn->params[2].s = b;
}

void
iglObj_afunction (long ref, long func)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_AFUNCTION);
    fcn->params[0].i = ref;
    fcn->params[1].i = func;
}

void
iglObj_blendfunction (long sfactor, long dfactor)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BLENDFUNCTION);
    fcn->params[0].i = sfactor;
    fcn->params[1].i = dfactor;
}

void
iglObj_dither (long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_DITHER);
    fcn->params[0].i = mode;
}

void
iglObj_fogvertex (long mode, float *params)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_FOGVERTEX);
    fcn->params[0].i = mode;
    fcn->params[1].v = (float *)malloc (5*sizeof (float));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, params,
            (mode == FG_VTX_LIN || mode == FG_PIX_LIN) ? 5*sizeof (float) : 4*sizeof (float));
    }
}

void
iglObj_logicop (long opcode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LOGICOP);
    fcn->params[0].i = opcode;
}

void
iglObj_c4f (float cv[4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_C4F);
    fcn->params[0].v = (float *)malloc (4*sizeof (float));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, cv, 4*sizeof (float));
    }
}

void
iglObj_c4i (int cv[4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_C4I);
    fcn->params[0].v = (int *)malloc (4*sizeof (int));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, cv, 4*sizeof (int));
    }
}

void
iglObj_c4s (short cv[4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_C4S);
    fcn->params[0].v = (short *)malloc (4*sizeof (short));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, cv, 4*sizeof (short));
    }
}

void
iglObj_mmode (short mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_MMODE);
    fcn->params[0].s = mode;
}

void
iglObj_loadmatrix (Matrix m)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LOADMATRIX);
    fcn->params[0].v = (Matrix *)malloc (sizeof (Matrix));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, m, sizeof (Matrix));
    }
}

void
iglObj_multmatrix (Matrix m)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_MULTMATRIX);
    fcn->params[0].v = (Matrix *)malloc (sizeof (Matrix));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, m, sizeof (Matrix));
    }
}

void
iglObj_pushmatrix ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PUSHMATRIX);
}

void
iglObj_popmatrix ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POPMATRIX);
}

void
iglObj_rot (float amount, char angle)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ROT);
    fcn->params[0].f = amount;
    fcn->params[1].c = angle;
}

void
iglObj_rotate (Angle amount, char angle)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ROTATE);
    fcn->params[0].s = amount;
    fcn->params[1].c = angle;
}

void
iglObj_translate (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_TRANSLATE);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_scale (float x, float y, float z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SCALE);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_polarview (Coord dist, Angle azim, Angle inc, Angle twist)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLARVIEW);
    fcn->params[0].f = dist;
    fcn->params[1].s = azim;
    fcn->params[2].s = inc;
    fcn->params[3].s = twist;
}

void
iglObj_clipplane (long index, long mode, float *params)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CLIPPLANE);
    fcn->params[0].i = index;
    fcn->params[1].i = mode;
    fcn->params[2].v = (float *)malloc (4*sizeof (float));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, params, 4*sizeof (float));
    }
}

void
iglObj_ortho (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ORTHO);
    fcn->params[0].f = left;
    fcn->params[1].f = right;
    fcn->params[2].f = bottom;
    fcn->params[3].f = top;
    fcn->params[4].f = near_;
    fcn->params[5].f = far_;
}

void
iglObj_ortho2 (Coord left, Coord right, Coord bottom, Coord top)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ORTHO2);
    fcn->params[0].f = left;
    fcn->params[1].f = right;
    fcn->params[2].f = bottom;
    fcn->params[3].f = top;
}

void
iglObj_window (Coord left, Coord right, Coord bottom, Coord top, Coord near_, Coord far_)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_WINDOW);
    fcn->params[0].f = left;
    fcn->params[1].f = right;
    fcn->params[2].f = bottom;
    fcn->params[3].f = top;
    fcn->params[4].f = near_;
    fcn->params[5].f = far_;
}

void
iglObj_perspective (Angle fovy, float aspect, Coord near_, Coord far_)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PERSPECTIVE);
    fcn->params[0].s = fovy;
    fcn->params[1].f = aspect;
    fcn->params[2].f = near_;
    fcn->params[3].f = far_;
}

void
iglObj_viewport (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_VIEWPORT);
    fcn->params[0].s = left;
    fcn->params[1].s = right;
    fcn->params[2].s = bottom;
    fcn->params[3].s = top;
}

void
iglObj_getviewport (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_GETVIEWPORT);
    fcn->params[0].v = left;
    fcn->params[1].v = right;
    fcn->params[2].v = bottom;
    fcn->params[3].v = top;
}

void
iglObj_reshapeviewport ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RESHAPEVIEWPORT);
}

void
iglObj_pushviewport ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PUSHVIEWPORT);
}

void
iglObj_popviewport ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POPVIEWPORT);
}

void
iglObj_lookat (Coord vx, Coord vy, Coord vz, Coord px, Coord py, Coord pz, Angle twist)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LOOKAT);
    fcn->params[0].f = vx;
    fcn->params[1].f = vy;
    fcn->params[2].f = vz;
    fcn->params[3].f = px;
    fcn->params[4].f = py;
    fcn->params[5].f = pz;
    fcn->params[6].s = twist;
}

void
iglObj_scrmask (Screencoord left, Screencoord right, Screencoord bottom, Screencoord top)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SCRMASK);
    fcn->params[0].s = left;
    fcn->params[1].s = right;
    fcn->params[2].s = bottom;
    fcn->params[3].s = top;
}

void
iglObj_getscrmask (Screencoord *left, Screencoord *right, Screencoord *bottom, Screencoord *top)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_GETSCRMASK);
    fcn->params[0].v = left;
    fcn->params[1].v = right;
    fcn->params[2].v = bottom;
    fcn->params[3].v = top;
}

void
iglObj_polf (long n, const Coord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLF);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Coord));
    }
}

void
iglObj_polfi (long n, const Icoord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLFI);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Icoord));
    }
}

void
iglObj_polfs (long n, const Scoord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLFS);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Scoord));
    }
}

void
iglObj_polf2 (long n, const Coord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLF2);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Icoord));
    }
}

void
iglObj_polf2i (long n, const Icoord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLF2I);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Icoord));
    }
}

void
iglObj_polf2s (long n, const Scoord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLF2S);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Scoord));
    }
}

void
iglObj_poly (long n, const Coord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLY);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Coord));
    }
}

void
iglObj_polyi (long n, const Icoord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLYI);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Icoord));
    }
}

void
iglObj_polys (long n, const Scoord parray[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLYS);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Scoord));
    }
}

void
iglObj_poly2 (long n, const Coord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLY2);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Icoord));
    }
}

void
iglObj_poly2i (long n, const Icoord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLY2I);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Icoord));
    }
}

void
iglObj_poly2s (long n, const Scoord parray[][2])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLY2S);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Scoord));
    }
}

void
iglObj_polymode (long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLYMODE);
    fcn->params[0].i = mode;
}

void
iglObj_polysmooth (long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POLYSMOOTH);
    fcn->params[0].i = mode;
}

void
iglObj_pmv (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PMV);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_pdr (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PDR);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_pclos ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PCLOS);
}

void
iglObj_pnt (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PNT);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_pntsizef (float n)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PNTSIZEF);
    fcn->params[0].f = n;
}

void
iglObj_pntsmooth (unsigned long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PNTSMOOTH);
    fcn->params[0].ui = mode;
}

void
iglObj_rect (Coord a, Coord b, Coord c, Coord d)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RECT);
    fcn->params[0].f = a;
    fcn->params[1].f = b;
    fcn->params[2].f = c;
    fcn->params[3].f = d;
}

void
iglObj_rectf (Coord a, Coord b, Coord c, Coord d)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RECTF);
    fcn->params[0].f = a;
    fcn->params[1].f = b;
    fcn->params[2].f = c;
    fcn->params[3].f = d;
}

void
iglObj_v4f (float vector[4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_V4F);
    fcn->params[0].v = (float *)malloc (4*sizeof (float));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, vector, 4*sizeof (float));
    }
}

void
iglObj_arc (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ARC);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = radius;
    fcn->params[3].s = startAngle;
    fcn->params[4].s = endAngle;
}

void
iglObj_arcf (Coord x, Coord y, Coord radius, Angle startAngle, Angle endAngle)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ARCF);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = radius;
    fcn->params[3].s = startAngle;
    fcn->params[4].s = endAngle;
}

void
iglObj_circ (Coord x, Coord y, Coord radius)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CIRC);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = radius;
}

void
iglObj_circf (Coord x, Coord y, Coord radius)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CIRCF);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = radius;
}

void
iglObj_bgntmesh ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNTMESH);
}

void
iglObj_bgnclosedline ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNCLOSEDLINE);
}

void
iglObj_bgncurve ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNCURVE);
}

void
iglObj_endcurve ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ENDCURVE);
}

void
iglObj_nurbscurve (long knotCount, const double *knotList, long offset, const double *ctlArray, long order, long type)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_NURBSCURVE);
// TBD: upon malloc(), calculate offset!
    fcn->params[0].i = knotCount;
    fcn->params[1].v = malloc (knotCount*sizeof (double));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, knotList, knotCount*sizeof (double));
    }
    fcn->params[2].i = offset;
    fcn->params[3].v = malloc (order*sizeof (double));
    if (fcn->params[3].v != NULL)
    {
        memcpy (fcn->params[3].v, ctlArray, order*sizeof (double));
    }
    fcn->params[4].i = order;
    fcn->params[5].i = type;
}

void
iglObj_bgnsurface ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNSURFACE);
}

void
iglObj_endsurface ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ENDSURFACE);
}

void
iglObj_nurbssurface (long scount, const double *sknot, long tcount, const double *tknot, long soffset, long toffset, const double *ctlArray, long sorder, long torder, long type)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_NURBSSURFACE);

// TBD: upon malloc(), calculate offsets! (soffset, toffset)
    fcn->params[0].i = scount;
    fcn->params[1].v = malloc (scount*sizeof (double));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, sknot, scount*sizeof (double));
    }
    fcn->params[2].i = tcount;
    fcn->params[3].v = malloc (tcount*sizeof (double));
    if (fcn->params[3].v != NULL)
    {
        memcpy (fcn->params[3].v, tknot, tcount*sizeof (double));
    }
    fcn->params[4].i = soffset;
    fcn->params[5].i = toffset;
    fcn->params[6].v = malloc (sorder*sizeof (double));
    if (fcn->params[6].v != NULL)
    {
        memcpy (fcn->params[6].v, ctlArray, sorder*sizeof (double));
    }
    fcn->params[7].i = sorder;
    fcn->params[8].i = torder;
    fcn->params[9].i = type;
}

void
iglObj_pwlcurve (long n, double *dataArray, long byteSize, long type)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PWLCURVE);
// TBD: upon malloc(), calculate offset! (byteSize)
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*sizeof (double));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, dataArray, n*sizeof (double));
    }
    fcn->params[2].i = byteSize;
    fcn->params[3].i = type;
}

void
iglObj_setnurbsproperty (long property, float value)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SETNURBSPROPERTY);
    fcn->params[0].i = property;
    fcn->params[1].f = value;
}

void
iglObj_bgnline ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNLINE);
}

void
iglObj_bgnpoint ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNPOINT);
}

void
iglObj_endpoint ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_ENDPOINT);
}

void
iglObj_bgnpolygon ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNPOLYGON);
}

void
iglObj_bgnqstrip ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_BGNQSTRIP);
}

void
iglObj_crvn (long n, Coord points[][3])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CRVN);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, points, n*3*sizeof (Coord));
    }
}

void
iglObj_rcrvn (long n, Coord points[][4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RCRVN);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*4*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, points, n*4*sizeof (Coord));
    }
}

void
iglObj_curveprecision (short n)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CURVEPRECISION);
    fcn->params[0].i = n;
}

void
iglObj_nmode (long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_NMODE);
    fcn->params[0].i = mode;
}

void
iglObj_n3f (float *vector)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_N3F);
    fcn->params[0].v = (float *)malloc (3*sizeof (float));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, vector, 3*sizeof (float));
    }
}

void
iglObj_linesmooth (unsigned long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LINESMOOTH);
    fcn->params[0].ui = mode;
}

void
iglObj_linewidthf (float width)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LINEWIDTHF);
    fcn->params[0].f = width;
}

void
iglObj_deflinestyle (short n, Linestyle ls)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_DEFLINESTYLE);
    fcn->params[0].s = n;
    fcn->params[0].us = ls;
}

void
iglObj_setlinestyle (short n)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SETLINESTYLE);
    fcn->params[0].s = n;
}

void
iglObj_lsrepeat (long factor)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LSREPEAT);
    fcn->params[0].i = factor;
}

void
iglObj_lmbind (short target, short index)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LMBIND);
    fcn->params[0].s = target;
    fcn->params[1].s = index;
}

void
iglObj_lmcolor (long mode)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LMCOLOR);
    fcn->params[0].i = mode;
}

void
iglObj_move (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_MOVE);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_rmv (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RMV);
    fcn->params[0].f = dx;
    fcn->params[1].f = dy;
    fcn->params[2].f = dz;
}

void
iglObj_rpmv (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RPMV);
    fcn->params[0].f = dx;
    fcn->params[1].f = dy;
    fcn->params[2].f = dz;
}

void
iglObj_draw (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_DRAW);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_rdr (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RDR);
    fcn->params[0].f = dx;
    fcn->params[1].f = dy;
    fcn->params[2].f = dz;
}

void
iglObj_rpdr (Coord dx, Coord dy, Coord dz)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_RPDR);
    fcn->params[0].f = dx;
    fcn->params[1].f = dy;
    fcn->params[2].f = dz;
}

void
iglObj_splf (long n, Coord parray[][3], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLF);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Coord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_splfi (long n, Icoord parray[][3], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLFI);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Icoord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_splfs (long n, Scoord parray[][3], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLFS);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*3*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*3*sizeof (Scoord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_splf2 (long n, Coord parray[][2], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLF2);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Coord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Coord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_splf2i (long n, Icoord parray[][2], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLF2I);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Icoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Icoord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_splf2s (long n, Scoord parray[][2], Colorindex iarray[])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SPLF2S);
    fcn->params[0].i = n;
    fcn->params[1].v = malloc (n*2*sizeof (Scoord));
    if (fcn->params[1].v != NULL)
    {
        memcpy (fcn->params[1].v, parray, n*2*sizeof (Scoord));
    }
    fcn->params[2].v = malloc (n*sizeof (Colorindex));
    if (fcn->params[2].v != NULL)
    {
        memcpy (fcn->params[2].v, iarray, n*sizeof (Colorindex));
    }
}

void
iglObj_callobj (Object id)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CALLOBJ);
    fcn->params[0].i = id;
}

void
iglObj_tevbind (long target, long index)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_TEVBIND);
    fcn->params[0].i = target;
    fcn->params[1].i = index;
}

void
iglObj_texbind (long target, long index)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_TEXBIND);
    fcn->params[0].i = target;
    fcn->params[1].i = index;
}

void
iglObj_t4f (float vector[4])
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_T4F);
    fcn->params[0].v = (float *)malloc (4*sizeof (float));
    if (fcn->params[0].v != NULL)
    {
        memcpy (fcn->params[0].v, vector, 4*sizeof (float));
    }
}

void
iglObj_cmov (Coord x, Coord y, Coord z)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_CMOV);
    fcn->params[0].f = x;
    fcn->params[1].f = y;
    fcn->params[2].f = z;
}

void
iglObj_lcharstr (long type, void *str)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_LCHARSTR);
    fcn->params[0].i = type;
    fcn->params[1].v = malloc (strlen ((char *)str) + 1);
    if (fcn->params[1].v != NULL)
    {
        strcpy ((char *)fcn->params[1].v, (char *)str);
    }
}

void
iglObj_pushattributes ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_PUSHATTRIBUTES);
}

void
iglObj_popattributes ()
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_POPATTRIBUTES);
}

void
iglObj_setpattern (short index)
{
    IGL_CHECKOBJV ();
    IGLFCNV (OP_SETPATTERN);
    fcn->params[0].s = index;
}



