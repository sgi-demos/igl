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

Actuator *scroll;
Panel *defpanel();

main() 
{
  foreground();
  noport();
  winopen("demo");
  
  doublebuffer();
  gconfig();
  
  defpanel();
  
  for (;;) {
    pnl_dopanel();
    swapbuffers();
  }
}

Panel 
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord x, y, d=1.0, dl=0.5;
  int i,j;

  p=pnl_mkpanel();
  p->label="scrolling";

  scroll=a=pnl_mkact(pnl_scroll);
  a->w=5-PNL_DIM_1;
  a->h=8-PNL_DIM_1;
  pnl_addact(a, p);

  y=0;
  for (i=0; i<4; i++) {
    x=0;
    for (j=0; j<10; j++) {
      a=pnl_mkact(pnl_slider);
      a->x=x; x+=a->w+PNL_DIM_1;
      a->y=y;
      pnl_addsubact(a, scroll);
    }
    y+=a->h+PNL_DIM_1;
  }

  pnl_fixact(scroll);

  return p;
}

