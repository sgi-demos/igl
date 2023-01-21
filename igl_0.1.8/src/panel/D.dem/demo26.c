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

Panel
*defpanel();

main()
{
  Actuator *a;
  Panel *panel;

  foreground();
  noport();
  winopen("demo");

  doublebuffer();
  gconfig();

  ortho2(-1.0,1.0,-1.0,1.0);

  panel=defpanel();

  for (;;) {
    pnl_dopanel();
    swapbuffers();
  }
}

/* note, default position moves down after creating bigbuttons... */

#define MKBIGBUTTON()	\
  a=pnl_mkact(pnl_wide_button);	\
  a->w=1.0-PNL_DIM_1;		\
  a->h=1.0-PNL_DIM_1;		\
  a->x=x;			\
  a->y=(y-=d);			\
  pnl_addact(a, p)

/* ...and to the right after creating these dials */

#define MKDIAL()       	   	\
  a=pnl_mkact(pnl_dial);	   	\
  a->w=a->h*=1.5;	   	\
  a->x=x;		   	\
  a->y=y;		   	\
  x+=a->w+(1.6667*PNL_DIM_1);	\
  pnl_addact(a, p)

Panel
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord y, x, d=1.0;

  p=pnl_mkpanel();
  p->label="demo";
  p->ppu=50.0;

  x=0.0;
  y=0.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x=1.0;
  y=1.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x=2.0;
  y=1.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x=3.0;
  y=1.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x=4.0;
  y=1.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x=5.0;
  y=0.0;

  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();
  MKBIGBUTTON();

  x= 0.0;
  y= -6.5;

  MKDIAL();
  MKDIAL();
  MKDIAL();
  MKDIAL();

  x= 0.0;
  y= -8.0;

  MKDIAL();
  MKDIAL();
  MKDIAL();
  MKDIAL();

  return p;
}

