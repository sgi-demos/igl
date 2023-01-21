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

Actuator *icon, *menu, *aslider, *bslider, *cslider;
  
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

  p=pnl_mkpanel();
  p->label="menus";

  icon=a=pnl_mkact(pnl_icon);
  pnl_addact(icon, p);

  menu=a=pnl_mkact(pnl_menu);
  a->label="a menu";
  pnl_addsubact(menu, icon);

  aslider=a=pnl_mkact(pnl_hslider);
  a->label="first choice";
  a->labeltype=PNL_LABEL_RIGHT;
  pnl_addsubact(a, menu);

  bslider=a=pnl_mkact(pnl_hslider);
  a->label="a big problem";
  a->labeltype=PNL_LABEL_RIGHT;
  pnl_addsubact(a, menu);

  cslider=a=pnl_mkact(pnl_hslider);
  a->label="the end";
  a->labeltype=PNL_LABEL_RIGHT;
  pnl_addsubact(a, menu);

  pnl_fixact(icon);

  return p;
}

