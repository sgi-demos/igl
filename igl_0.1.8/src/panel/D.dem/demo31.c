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

Actuator *icon, *menu, *amenuitem, *bmenuitem, *cmenuitem;
  
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

void
printfunc(a)
Actuator *a;
{
#if 0
  switch (pnl_funcmode) {
  case PNL_FCNM_NONE:
    fprintf (stdout,"no function mode\n, actuator %s",
	   (a->label?a->label:"<no label>"));
    break;
  case PNL_FCNM_DOWN:
    fprintf (stdout,"downfunc, actuator %s\n", (a->label?a->label:"<no label>"));
    break;
  case PNL_FCNM_ACTIVE:
    fprintf (stdout,"activefunc, actuator %s\n", (a->label?a->label:"<no label>"));
    break;
  case PNL_FCNM_UP:
    fprintf (stdout,"upfunc, actuator %s\n", (a->label?a->label:"<no label>"));
    break;
  }
#endif 0
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
  a->downfunc=a->activefunc=a->upfunc=printfunc;
  pnl_addact(icon, p);

  menu=a=pnl_mkact(pnl_menu);
  a->label="a menu";
  a->downfunc=a->activefunc=a->upfunc=printfunc;
  pnl_addsubact(menu, icon);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="first choice";
  a->downfunc=a->activefunc=a->upfunc=printfunc;
  pnl_addsubact(a, menu);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="a big problem";
  a->downfunc=a->activefunc=a->upfunc=printfunc;
  pnl_addsubact(a, menu);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="the end";
  a->downfunc=a->activefunc=a->upfunc=printfunc;
  pnl_addsubact(a, menu);

  pnl_fixact(icon);

  return p;
}

