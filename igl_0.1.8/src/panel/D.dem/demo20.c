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

Actuator *s1, *s2, *b1, *b2, *b3, *b4,
  *b5, *b6, *b7, *b8, *b9, *b10, *rb1, *rb2, *rb3;

Panel
*defpanel1(), *defpanel2(), *defpanel3();

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
	translate(PNL_ACCESS(Puck, s1, x),PNL_ACCESS(Puck, s1, y),0.0);
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
beginscript()
{
  (void) pnl_beginwritescript("panel.script");
}

void
appendscript()
{
  (void) pnl_beginappendscript("panel.script");
}

void
endscript()
{
  (void) pnl_endwritescript();
}

void
readscript()
{
  (void) pnl_beginreadscript("panel.script");
}

void
stopscript()
{
  (void) pnl_endreadscript();
}

Panel
*defpanel1()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel one";
    panel->ppu=50.0;

    s1=pnl_mkact(pnl_puck);
    s1->label="position";
    s1->x=4.0;
    s1->y=0.0;
    s1->minval= -1.0;
    s1->maxval=1.0;
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

    b10=pnl_mkact(pnl_button);
    b10->label="dump state";
    b10->x=1.0;
    b10->y=4.5;
    b10->downfunc=pnl_dumpstate;
    pnl_addact(b10, panel);

    b5=pnl_mkact(pnl_button);
    b5->label="begin script";
    b5->x=1.0;
    b5->y=4.0;
    b5->downfunc=beginscript;
    pnl_addact(b5, panel);

    b6=pnl_mkact(pnl_button);
    b6->label="end script";
    b6->x=1.0;
    b6->y=3.5;
    b6->downfunc=endscript;
    pnl_addact(b6, panel);

    b9=pnl_mkact(pnl_button);
    b9->label="append to script";
    b9->x=1.0;
    b9->y=3.0;
    b9->downfunc=appendscript;
    pnl_addact(b9, panel);

    b7=pnl_mkact(pnl_button);
    b7->label="read script";
    b7->x=1.0;
    b7->y=2.5;
    b7->downfunc=readscript;
    pnl_addact(b7, panel);

    b8=pnl_mkact(pnl_button);
    b8->label="stop reading script";
    b8->x=1.0;
    b8->y=2.0;
    b8->downfunc=stopscript;
    pnl_addact(b8, panel);

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

