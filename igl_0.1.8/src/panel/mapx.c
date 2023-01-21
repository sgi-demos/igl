/*****************************************************************
    Panel Library/Electropaint Copyright (c) 1986 David A. Tristram.
    Electropaint (TM) is a Registered U.S. Trademark of Tristram Visual.
    Tristram Visual can be contacted at www.tristram.com.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
*****************************************************************/
#include <gl.h>
#include <device.h>
#include <panel.h>

#define LOWCOLOR    0
#define HIGHCOLOR   1023
#define BASHR	    0
#define BASHG	    0
#define BASHB	    0
#define MAXRANGE    1024

Panel
*defpanel();

Colorindex range, bash=LOWCOLOR, lastbash=LOWCOLOR;
float rangef, bashf=(float)LOWCOLOR;
short *savr, *savg, *savb, bashr,  bashg,  bashb;

Actuator *index_slider, *range_slider;

main() 
{
    savr=(short *)malloc(sizeof(Colorindex)*MAXRANGE);
    savg=(short *)malloc(sizeof(Colorindex)*MAXRANGE);
    savb=(short *)malloc(sizeof(Colorindex)*MAXRANGE);

    foreground();
    noport();
    winopen("mapo");

    doublebuffer();
    gconfig();

    defpanel();

    for (;;) {
        pnl_dopanel();
	swapbuffers();
    }
}

void
savecolors(start, end)
Colorindex start, end;
{
int temp, i, dir;

    if (start>end) {
	temp=end;
	end=start;
	start=temp;
    }
    for (i=start;i<=end;i++) {
	getmcolor(i, &savr[i], &savg[i], &savb[i]);
    }
}

void
bashcolors(start, end)
Colorindex start, end;
{
int temp, i, dir;

    if (start>end) {
	temp=end;
	end=start;
	start=temp;
    }
    for (i=start;i<=end;i++) {
	mapcolor(i, bashr, bashg, bashb);
    }
}

void
restorecolors(start, end)
Colorindex start, end;
{
int temp, i, dir;

    if (start>end) {
	temp=end;
	end=start;
	start=temp;
    }
    for (i=start;i<=end;i++) {
	mapcolor(i, savr[i], savg[i], savb[i]);
    }
}

void
bashmap(a)
Actuator *a;
{
short tr, tg, tb;

    bashf+=index_slider->val;
    rangef=range_slider->val;
    bashf=MAX((float)LOWCOLOR+rangef, bashf);
    bashf=MIN((float)HIGHCOLOR-rangef, bashf);

    bash=(Colorindex)bashf;
    range=(Colorindex)rangef;
    if (lastbash!=bash) {
	savecolors(bash, bash+range);
	bashcolors(bash, bash+range);
	restorecolors(lastbash, lastbash+range);
        lastbash=bash;
    }
}

void
patchmap(){
}

void
cleanexit()
{
    exit(0);
}

Panel
*defpanel()
{
Panel *p;
Actuator *a;

    p=pnl_mkpanel();
    p->label="mapo";

    a=pnl_mkact(pnl_button);
    a->label="exit";
    a->x=1.0;
    a->y=1.0;
    a->downfunc=cleanexit;
    pnl_addact(a, p);

    a=pnl_mkact(pnl_slider);
    a->label="index";
    a->x=1.0;
    a->y=2.5;
    a->val=0.0;
    a->maxval=  (float)(HIGHCOLOR-LOWCOLOR)/100.0;
    a->minval= -(float)(HIGHCOLOR-LOWCOLOR)/100.0;
    a->activefunc=bashmap;
    index_slider=a;
    pnl_addact(a, p);

    a=pnl_mkact(pnl_slider);
    a->label="range";
    a->x=2.0;
    a->y=2.5;
    a->val=0.0;
    a->maxval=  (float)(HIGHCOLOR-LOWCOLOR)/10.0;
    a->minval= -(float)(HIGHCOLOR-LOWCOLOR)/10.0;
    a->activefunc=bashmap;
    range_slider=a;
    pnl_addact(a, p);
}

