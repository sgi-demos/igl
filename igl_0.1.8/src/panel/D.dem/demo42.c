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

Panel *defpanel1(), *defpanel2(), *defpanel3(), *defpanel4();
Panel *panel1, *panel2, *panel3;

main() 
{
  Actuator *a;
  short col=RED;
  
  foreground();
  winopen("demo");
  doublebuffer();
  gconfig();
  
  ortho2(-1.0,1.0,-1.0,1.0);
  
  panel1=defpanel1();
  panel2=defpanel2();
  panel3=defpanel3();
  defpanel4();
  
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
    translate(s1->val,s2->val,0.0);
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
togglepanelvisibility(a)
Actuator *a;
{
  ((Panel *)a->u)->visible=(a->val==1.0);
  pnl_fixpanel((Panel *)a->u);
}

void
togglepanelselectability(a)
Actuator *a;
{
  ((Panel *)a->u)->selectable=(a->val==1.0);
  pnl_fixpanel((Panel *)a->u);
}

void
togglebell(a)
Actuator *a;
{
  pnl_panel_bell=(a->val==1.0);
}

Panel
*defpanel1()
{
Actuator *a;
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel one";
    panel->ppu=50.0;

    s1=pnl_mkact(pnl_hslider);
    s1->label="y position";
    s1->x=1.0;
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

    a=pnl_mkact(pnl_typein);
    a->x=4.0;
    a->y=2.0;
    a->labeltype=PNL_LABEL_TOP;
    a->label="a typein window";
    PNL_ACCESS(Typein,a,str)="Edit This String";
    pnl_addact(a,panel);

    return panel;
}

Panel
*defpanel2()
{
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel two";
    panel->ppu=50.0;

    s2=pnl_mkact(pnl_vslider);
    s2->label="x position";
    s2->x=0.0;
    s2->y=0.0;
    s2->minval= -1.0;
    s2->maxval=1.0;
    pnl_addact(s2, panel);

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

Panel
*defpanel4()
{
  Coord x,y,dy;
  Panel *panel;
  Actuator *a;

  x=0; y=0; dy=0.5;

  panel=pnl_mkpanel();
  panel->label="Panels";

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 3";
  a->val=1;
  a->u=(char *)panel3;
  a->downfunc=togglepanelvisibility;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_label);
  a->label="visibility";
  a->x=x-2.5;
  a->y=y;
  pnl_addact(a, panel);

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 2";
  a->val=1;
  a->u=(char *)panel2;
  a->downfunc=togglepanelvisibility;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 1";
  a->val=1;
  a->u=(char *)panel1;
  a->downfunc=togglepanelvisibility;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 3";
  a->val=1;
  a->u=(char *)panel3;
  a->downfunc=togglepanelselectability;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_label);
  a->label="selectability";
  a->x=x-2.5;
  a->y=y;
  pnl_addact(a, panel);

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 2";
  a->val=1;
  a->u=(char *)panel2;
  a->downfunc=togglepanelselectability;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="panel 1";
  a->val=1;
  a->u=(char *)panel1;
  a->downfunc=togglepanelselectability;
  pnl_addact(a, panel);
  y+=dy;

  y+=dy;

  a=pnl_mkact(pnl_toggle_button);
  a->x=x;
  a->y=y;
  a->label="bell";
  a->labeltype=PNL_LABEL_LEFT;
  a->val=1;
  a->u=(char *)panel1;
  a->downfunc=togglebell;
  pnl_addact(a, panel);
  y+=dy;
  return panel;
}
  
