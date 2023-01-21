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
  Actuator *a, *icon, *menu, *amenuitem, *bmenuitem, *cmenuitem;
  Panel *p;
  Coord x, y, d=1.0, dl=0.5;

  p=pnl_mkpanel();
  p->label="icons";

  icon=a=pnl_mkact(pnl_icon);
  a->x=x;
  pnl_addact(icon, p);

  menu=a=pnl_mkact(pnl_menu);
  a->label="menu 1";
  pnl_addsubact(menu, icon);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="first choice";
  pnl_addsubact(a, menu);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="a big problem";
  pnl_addsubact(a, menu);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="the end";
  pnl_addsubact(a, menu);

  pnl_fixact(icon);
  x+=icon->w+PNL_DIM_1;

  icon=a=pnl_mkact(pnl_icon);
  a->x=x;
  pnl_addact(icon, p);

  menu=a=pnl_mkact(pnl_menu);
  a->label="another menu";
  pnl_addsubact(menu, icon);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="oneish";
  pnl_addsubact(a, menu);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="twoish";
  pnl_addsubact(a, menu);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="threeish";
  pnl_addsubact(a, menu);

  pnl_fixact(icon);
  x+=icon->w+PNL_DIM_1;

  icon=a=pnl_mkact(pnl_icon);
  a->x=x;
  pnl_addact(icon, p);

  menu=a=pnl_mkact(pnl_menu);
  a->label="thirdly";
  pnl_addsubact(menu, icon);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="1";
  pnl_addsubact(a, menu);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="2";
  pnl_addsubact(a, menu);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="3";
  pnl_addsubact(a, menu);

  pnl_fixact(icon);
  x+=icon->w+PNL_DIM_1;

  return p;
}

