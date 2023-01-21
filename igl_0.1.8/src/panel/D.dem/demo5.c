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

Actuator *sy, *sx, *sd, *b1, *b2, *b3, *b4, *rb1, *rb2, *rb3;

Panel
*defpanel1(), *defpanel2(), *defpanel3();

main() 
{
Actuator *a;
short col=RED;

    foreground();
    winopen("demo");
    doublebuffer();
    gconfig();

    ortho2(-1.0,1.0,-1.0,1.0);

    defpanel1();
    defpanel2();
    defpanel3();

    pnl_needredraw();

    drawit(col);
    swapbuffers();
    drawit(col);

    for (;;) {
        a=pnl_dopanel();
	if (pnl_userredraw()||a) {
redo:
	    if (a==b1) exit(0);
	    if (a==b4) pnl_dumppanel();
	    if (a==rb1) col=CYAN;
	    if (a==rb2) col=GREEN;
	    if (a==rb3) col=RED;

	    drawit(col);
	    swapbuffers();

	    if (a=pnl_dopanel()) goto redo;

	    drawit(col);
	    swapbuffers();
	    drawit(col);
	    swapbuffers();
	}
	swapbuffers();
    }
}


drawit(col)
Colorindex col;
{
Coord x,y;

    color(BLACK);
    clear();
    color(col);

    for (x= -1.0;x<1.0;x+=sd->val)
    for (y= -1.0;y<1.0;y+=sd->val) {
	move2(sx->val,sy->val);
	draw2(x,y);
    }
}


Panel
*defpanel1()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="position";

    sy=pnl_mkact(pnl_vslider);
    sy->label="y";
    sy->x=0.0;
    sy->y=0.0;
    sy->minval= -1.0;
    sy->maxval=1.0;
    pnl_addact(sy, panel);

    sx=pnl_mkact(pnl_hslider);
    sx->label="x position i said - x position";
    sx->x=0.0;
    sx->y=1.0;
    sx->minval= -1.0;
    sx->maxval=1.0;
    pnl_addact(sx, panel);
}

Panel
*defpanel2()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="frob it";

    sd=pnl_mkact(pnl_vslider);
    sd->label="density";
    sd->x=1.0;
    sd->y=0.0;
    sd->minval=0.01;
    sd->maxval=0.2;
    sd->val=0.1;
    pnl_addact(sd, panel);
}

Panel
*defpanel3()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel three";

    b1=pnl_mkact(pnl_button);
    b1->label="exit";
    b1->x=1.0;
    b1->y=1.0;
    pnl_addact(b1, panel);

    rb3=pnl_mkact(pnl_radio_button);
    rb3->label="red";
    rb3->x=1.0;
    rb3->y=3.0;
    rb3->val=1.0;
    pnl_addact(rb3, panel);

    rb2=pnl_mkact(pnl_radio_button);
    rb2->label="green";
    rb2->x=1.0;
    rb2->y=2.5;
    pnl_addact(rb2, panel);

    rb1=pnl_mkact(pnl_radio_button);
    rb1->label="cyan";
    rb1->x=1.0;
    rb1->y=2.0;
    pnl_addact(rb1, panel);

    pnl_endgroup(panel);
}
