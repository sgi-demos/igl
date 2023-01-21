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

Panel *defpanel1(), *defpanel2(), *defpanel3(), *defpanel4();

Coord xpos, ypos;

Colorindex col;

main() 
{
  Actuator *a;
  
  foreground();
  winopen("demo");
  doublebuffer();
  gconfig();
  
  ortho2(-1.0,1.0,-1.0,1.0);
  
  defpanel4();
  
  for (;;) {
    pnl_dopanel();
    
    pushmatrix();
    translate(xpos,ypos,0.0);
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

void cleanexit() { exit(0); }

void
pushbutton(a)
Actuator *a;
{
  ((Actuator *)a->u)->val=a->val;
  pnl_fixact((Actuator *)a->u);
}

void
setxpos(a)
Actuator *a;
{
  xpos=a->val;
}

void
setypos(a)
Actuator *a;
{
  ypos=a->val;
}

void
setcol(a)
Actuator *a;
{
  col=(Colorindex)a->u;
}

void
makepanel(a)
Actuator *a;
{
  Panel *p;

  p= (*(Panel *(*)())a->u)();
  pnl_addpanel(p);
}

void
deletepanel(a)
Actuator *a;
{
  pnl_delpanel(a->p);
}

Panel
*defpanel1()
{
Actuator *a, *b;
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel one";
    panel->ppu=50.0;

    a=pnl_mkact(pnl_hslider);
    a->label="x position";
    a->x=1.0;
    a->y=0.0;
    a->minval= -1.0;
    a->maxval=1.0;
    a->activefunc=setxpos;
    pnl_addact(a, panel);

    b=pnl_mkact(pnl_toggle_button);
    b->label="I push myself";
    b->x=1.0;
    b->y=2.0;
    pnl_addact(b, panel);

    a=pnl_mkact(pnl_toggle_button);
    a->label="push me";
    a->x=1.0;
    a->y=1.5;
    a->u=(char *)b;
    a->downfunc=pushbutton;
    pnl_addact(a, panel);

    a=pnl_mkact(pnl_typein);
    a->x=4.0;
    a->y=2.0;
    a->labeltype=PNL_LABEL_TOP;
    a->label="a typein window";
    PNL_ACCESS(Typein,a,str)="Edit This String";
    pnl_addact(a,panel);

    a=pnl_mkact(pnl_button);
    a->x=1.0;
    a->y=4.0;
    a->label="delete";
    a->downfunc=deletepanel;
    pnl_addact(a, panel);

    return panel;
}

Panel
*defpanel2()
{
Actuator *a;
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel two";
    panel->ppu=50.0;

    a=pnl_mkact(pnl_vslider);
    a->label="y position";
    a->x=0.0;
    a->y=0.0;
    a->minval= -1.0;
    a->maxval=1.0;
    a->activefunc=setypos;
    pnl_addact(a, panel);
  
    a=pnl_mkact(pnl_button);
    a->label="dumppanel";
    a->x=1.0;
    a->y=1.5;
    a->downfunc=(void (*)())pnl_dumppanel;
    pnl_addact(a, panel);

    a=pnl_mkact(pnl_button);
    a->label="exit";
    a->x=1.0;
    a->y=1.0;
    a->upfunc=cleanexit;
    pnl_addact(a, panel);

    a=pnl_mkact(pnl_button);
    a->x=1.0;
    a->y=4.0;
    a->label="delete";
    a->downfunc=deletepanel;
    pnl_addact(a, panel);

    return panel;
}

Panel
*defpanel3()
{
Actuator *a;
Panel *panel;

    panel=pnl_mkpanel();
    panel->label="panel three";
    panel->ppu=50.0;

    a=pnl_mkact(pnl_radio_button);
    a->label="red";
    a->val=1.0;
    a->x=1.0;
    a->y=2.0;
    a->u=(char *)RED;
    a->downfunc=setcol;
    pnl_addact(a, panel);
    setcol(a);

    a=pnl_mkact(pnl_radio_button);
    a->label="green";
    a->x=1.0;
    a->y=1.5;
    a->u=(char *)GREEN;
    a->downfunc=setcol;
    pnl_addact(a, panel);

    a=pnl_mkact(pnl_radio_button);
    a->label="cyan";
    a->x=1.0;
    a->y=1.0;
    a->u=(char *)CYAN;
    a->downfunc=setcol;
    pnl_addact(a, panel);

    a=pnl_mkact(pnl_button);
    a->x=1.0;
    a->y=4.0;
    a->label="delete";
    a->downfunc=deletepanel;
    pnl_addact(a, panel);

    return panel;
}

Panel
*defpanel4()
{
  Coord y,dy;
  Panel *panel;
  Actuator *a;

  y=0; dy=0.5;

  panel=pnl_mkpanel();
  panel->label="Panels";

  a=pnl_mkact(pnl_button);
  a->y=y;
  a->label="panel 1";
  a->u=(char *)defpanel1;
  a->downfunc=makepanel;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_button);
  a->y=y;
  a->label="panel 2";
  a->u=(char *)defpanel2;
  a->downfunc=makepanel;
  pnl_addact(a, panel);
  y+=dy;

  a=pnl_mkact(pnl_button);
  a->y=y;
  a->label="panel 3";
  a->u=(char *)defpanel3;
  a->downfunc=makepanel;
  pnl_addact(a, panel);
  y+=dy;

  return panel;
}
  
