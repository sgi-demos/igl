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

Actuator *s1, *s2, *b1, *b2, *b3, *b4, *rb1, *rb2, *rb3;

Panel
*defpanel1(), *defpanel2(), *defpanel3();

Coord posx=0.0, posy=0.0, savx=0.0, savy=0.0;

main() 
{
Actuator *a;
Panel *panel1;
Panel *panel2;
short col=RED;

    foreground();
    winopen("demo");
    doublebuffer();
    gconfig();

    ortho2(-1.0,1.0,-1.0,1.0);

    defpanel1();
    defpanel2();
    defpanel3();

    for (;;) {
        a=pnl_dopanel();
	if (a==b1) exit(0);
	if (a==b4) pnl_dumppanel();
	if (a==b2) {
	    b3->val=b2->val;
	    b3->dirtycnt=2;
	}
	if (a==rb1) col=CYAN;
	if (a==rb2) col=GREEN;
	if (a==rb3) col=RED;

	pushmatrix();
	translate(posx, posy, 0.0);
	color(BLACK);
	clear();
	color(col);
	drawit();
	swapbuffers();
	popmatrix();
    }
}


drawit()
{
    rectf(-.10,-.10,.10,.10);
}


void
deltapos(a)
Actuator *a;
{
    posx=savx+PNL_ACCESS(Puck, a, x);
    posy=savy+PNL_ACCESS(Puck, a, y);
}

void
bakepos(a)
Actuator *a;
{
    savx=posx;
    savy=posy;
}

Panel
*defpanel1()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel one";
    panel->ppu=50.0;

    s1=pnl_mkact(pnl_floating_puck);
    s1->label="position";
    s1->x=4.0;
    s1->y=0.0;
    s1->minval= -1.0;
    s1->maxval=1.0;
    s1->activefunc=deltapos;
    s1->upfunc=bakepos;
    pnl_addact(s1, panel);

    b2=pnl_mkact(pnl_toggle_button);
    b2->label="push me";
    b2->x=1.0;
    b2->y=1.5;
    pnl_addact(b2, panel);

    b3=pnl_mkact(pnl_toggle_button);
    b3->label="I push myself";
    b3->x=1.0;
    b3->y=2.0;
    pnl_addact(b3, panel);

    return panel;
}

Panel
*defpanel2()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel two";
    panel->ppu=50.0;

    b4=pnl_mkact(pnl_button);
    b4->label="dumppanel";
    b4->x=1.0;
    b4->y=1.5;
    pnl_addact(b4, panel);

    b1=pnl_mkact(pnl_button);
    b1->label="exit";
    b1->x=1.0;
    b1->y=1.0;
    pnl_addact(b1, panel);

    return panel;
}

Panel
*defpanel3()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel three";
    panel->ppu=50.0;

    rb3=pnl_mkact(pnl_radio_button);
    rb3->label="red";
    rb3->x=1.0;
    rb3->y=2.0;
    rb3->val=1.0;
    pnl_addact(rb3, panel);

    rb2=pnl_mkact(pnl_radio_button);
    rb2->label="green";
    rb2->x=1.0;
    rb2->y=1.5;
    pnl_addact(rb2, panel);

    rb1=pnl_mkact(pnl_radio_button);
    rb1->label="cyan";
    rb1->x=1.0;
    rb1->y=1.0;
    pnl_addact(rb1, panel);

    return panel;
}

