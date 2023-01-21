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

Actuator *s1, *s2, *b1;

Panel
*defpanel();

main() 
{
Actuator *a;
Panel *panel;

    foreground();
    winopen("demo");
    doublebuffer();
    gconfig();

    ortho2(-1.0,1.0,-1.0,1.0);

    panel=defpanel();

    for (;;) {
        a=pnl_dopanel();
	if (a==b1) {
	    exit(0);
	}
	s2->w=s1->val*10.0;
	s2->dirtycnt=2;
	panel->dirtycnt=2;	/* because we are modifying the layout */
	pushmatrix();
	translate(s1->val,s2->val,0.0);
	color(BLACK);
	clear();
	color(WHITE);
	drawit();
	swapbuffers();
	popmatrix();
    }
}


drawit()
{
    rectf(-.10,-.10,.10,.10);
}


Panel
*defpanel()
{
Panel *panel;

    panel=pnl_mkpanel();

    s1=pnl_mkact(pnl_slider);
    s1->label="slider 1";
    s1->x=0.0;
    s1->y=0.0;
    s1->minval= -1.0;
    s1->maxval=1.0;
    pnl_addact(s1, panel);

    s2=pnl_mkact(pnl_slider);
    s2->label="slider 2";
    s2->x=1.0;
    s2->y=0.0;
    s2->minval= -1.0;
    s2->maxval=1.0;
    pnl_addact(s2, panel);

    b1=pnl_mkact(pnl_button);
    b1->label="button 1";
    b1->x=2.0;
    b1->y=0.0;
    pnl_addact(b1, panel);

    return panel;
}

