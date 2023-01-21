#include "igl.h"
#include "iglcmn.h"




/* DESC: curson, cursoff - control cursor visibility by window */
void
curson ()
{
    IGL_CHECKINITV ();
    SHOWCURSOR ();
}


/* DESC: curson, cursoff - control cursor visibility by window */
void
cursoff ()
{
    IGL_CHECKINITV ();
    HIDECURSOR ();
}


/* DESC: curstype - defines the type and/or size of cursor */
void
curstype (long type)
{
    /* not needed? */

    /*
       type     expects one of five values that describe the cursor:
                C16X1: the default, a 16x16 bitmap cursor of no more than one color.
                C16X2: a 16x16 bitmap cursor of no more than three colors.
                C32X1: a 32x32 bitmap cursor of no more than one color.
                C32X2: a 32x32 bitmap cursor of no more than three colors.
                CCROSS: a cross-hair cursor.
    */
}


/* DESC: clkon, clkoff - control keyboard click */
void
clkon ()
{
}


/* DESC: clkon, clkoff - control keyboard click */
void
clkoff ()
{
}


/* DESC: lampon, lampoff - control the keyboard display lights */
void
lampon (Byte lamps)
{
    /* we don't need this =) */
}


/* DESC: lampon, lampoff - control the keyboard display lights */
void
lampoff (Byte lamps)
{
    /* we don't need this =) */
}

/* DESC: ringbell - rings the keyboard bell */
void
ringbell ()
{
    /* we don't need this =) */
}


/* DESC: getbutton - returns the state of a button */
Boolean
getbutton (Device dev)
{
    IGL_CHECKINIT (FALSE);
    if (dev > IGL_MAXDEVICES || !ISBUTTON(dev))
    {
        return (FALSE);
    }

    return (igl->devices[dev]);
}


/* DESC: getvaluator - returns the current state of a valuator */
long
getvaluator (Device dev)
{
    IGL_CHECKINIT (0);
    if (dev > IGL_MAXDEVICES || !ISVALUATOR(dev))
    {
        return (0);
    }

    return (igl->devices[dev]);
}


/* DESC: setvaluator - assigns an initial value and a range to a valuator */
void
setvaluator (Device v, short init, short vmin, short vmax)
{
    IGL_CHECKINITV ();
    if (!ISVALUATOR(v))
    {
        return;
    }

    igl->devices[v] = init;
    /* what about vmin/vmax? should we count them in? */
}


/* DESC: getdev - reads a list of valuators at one time */
void
getdev (long n, Device devs[], short vals[])
{
    int i;


    IGL_CHECKINITV ();
    if (n <= 0 || n > 128)
    {
        return;
    }

    for (i=0; i < n; i++)
    {
        if (devs[i] > IGL_MAXDEVICES)
        {
            vals[i] = 0;
        }
        else
        {
            vals[i] = (short)(igl->devices[devs[i]]);
        }
    }
}


/* DESC: qdevice - queues a device */
void
qdevice (Device dev)
{
    IGL_CHECKINITV ();

    if (dev < IGL_MAXDEVICES)
    {
        igl->deviceQueue[dev] = 1;
    }
}


/* DESC: unqdevice - disables the specified device from making entries in the event queue */
void
unqdevice (Device dev)
{
    IGL_CHECKINITV ();

    if (dev < IGL_MAXDEVICES)
    {
        igl->deviceQueue[dev] = 0;
    }
}


/* DESC: isqueued - returns whether the specified device is enabled for queuing */
Boolean
isqueued (Device dev)
{
    IGL_CHECKINIT (FALSE);

    return ((dev < IGL_MAXDEVICES) ? igl->deviceQueue[dev] : FALSE);
}


/* DESC: qenter - creates an event queue entry */
void
qenter (Device dev, short val)
{
    IGL_CHECKINITV ();
    if (igl->numQueuedEvents+1 < IGL_MAXDEVQENTRIES)
    {
        igl->eventQueue[igl->numQueuedEvents*2] = dev;
        igl->eventQueue[igl->numQueuedEvents*2+1] = val;
        igl->numQueuedEvents ++;
    }
}


/* DESC: qtest - checks the contents of the event queue */
long
qtest ()
{
    IGL_CHECKINIT (0);

    return ((igl->numQueuedEvents > 0) ? igl->eventQueue[0] : 0);
}


/* DESC: qreset - empties the event queue */
void
qreset ()
{
    IGL_CHECKINITV ();

    memset (igl->deviceQueue, 0, IGL_MAXDEVICES*sizeof(short));
    memset (igl->eventQueue, 0, IGL_MAXDEVQENTRIES*2*sizeof(long));
    igl->numQueuedEvents = 0;
}


/* DESC: blkqread - reads multiple entries from the queue */
long
blkqread (short *data, short n)
{
    int numEvents, pendingEvents;


    IGL_CHECKINIT (0);
    if (data == NULL || n == 0 || igl->numQueuedEvents == 0)
    {
        return (0);
    }

    numEvents = (n > igl->numQueuedEvents) ? igl->numQueuedEvents : n;
    memcpy (data, igl->eventQueue, numEvents*sizeof(short)*2);

    pendingEvents = igl->numQueuedEvents - numEvents;
    if (pendingEvents > 0)
    {
        memmove (igl->eventQueue, &igl->eventQueue[numEvents*2], pendingEvents*sizeof(short)*2);
    }
    igl->numQueuedEvents = pendingEvents;

    return (numEvents*2);
}


/* DESC: qread - reads the first entry in the event queue */
long
qread (short *data)
{
    long ret;
    short queue[2];


    IGL_CHECKINIT (0);

    /* qread() blocks until something is present in the queue! */
    while (1)
    {
        /*
            we must block, but not window messages (WM_XXX)!
            otherwise, blkqread() would never return anything
            (since we handle device events in WndProc())
        */
        IGL_MSG_PROCESS

        ret = blkqread (queue, 1);
        if (ret != 0)
        {
            break;
        }
    }

    *data = queue[1];
    return (queue[0]);
}


/* DESC: tie - ties two valuators to a button */
void
tie (Device b, Device v1, Device v2)
{
    IGL_CHECKINITV ();

    /*
        tie() conditions:
        1. b has to be a button
        2. v1 has to be a valuator OR a NULLDEV
        3. v2 has to be a valuator OR a NULLDEV
        4. if v1 is NULLDEV, v2 must be too (untie)
        5. v2 can be NULLDEV even if v1 is not
    */
    if (!ISBUTTON(b) || (!ISVALUATOR(v1) && v1 != NULLDEV) ||
        (!ISVALUATOR(v2) && v2 != NULLDEV) ||
        (v1 == NULLDEV && v2 != NULLDEV))
    {
        return;
    }

    igl->tiedValuators[b*2] = v1;
    igl->tiedValuators[b*2+1] = v2;
}


