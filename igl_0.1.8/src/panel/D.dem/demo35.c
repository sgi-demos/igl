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

Actuator *menu, *amenuitem, *bmenuitem, *cmenuitem;
  
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

Actuator
*defframe(p)
   Panel *p;
{
  Actuator *frame, *a;
  Coord x, y, d=1.0, dl=0.5;

  frame=pnl_mkact(pnl_frame);
  pnl_addact(frame, p);

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
  pnl_addsubact(a, frame);

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
  pnl_addsubact(a, frame);

  a=pnl_mkact(pnl_slider);
  a->x=x; x+=a->w+PNL_DIM_1;
  pnl_addsubact(a, frame);

  return frame;
}

Panel 
*defpanel()
{
  Actuator *a;
  Panel *p;
  Coord x, y, d=1.0, dl=0.5;
  float sf;

  p=pnl_mkpanel();
  p->label="frames";

  a=defframe(p);
  a->label="frame one";
  a->x=x;
  a->scalefactor=sf=1.0;
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  a=defframe(p);
  a->label="frame two";
  a->x=x;
  a->scalefactor=(sf*=0.66);
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  a=defframe(p);
  a->label="frame three";
  a->x=x;
  a->scalefactor=(sf*=0.66);
  pnl_fixact(a);
  x+=a->w+PNL_DIM_1;

  return p;
}

