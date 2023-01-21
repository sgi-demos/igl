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

Actuator *menu, *submenu1, *submenu2, *submenu3,
  *amenuitem, *bmenuitem, *cmenuitem;
  
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

  p=pnl_mkpanel();
  p->label="menus";

  menu=a=pnl_mkact(pnl_icon_menu);
  a->label="a nested menu";
  pnl_addact(menu, p);

  submenu1=a=pnl_mkact(pnl_sub_menu);
  a->label="menu 1";
  pnl_addsubact(submenu1, menu);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="first choice";
  pnl_addsubact(a, submenu1);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="a big problem";
  pnl_addsubact(a, submenu1);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="the end";
  pnl_addsubact(a, submenu1);

  submenu2=a=pnl_mkact(pnl_sub_menu);
  a->label="another menu";
  pnl_addsubact(submenu2, menu);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="oneish";
  pnl_addsubact(a, submenu2);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="twoish";
  pnl_addsubact(a, submenu2);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="threeish";
  pnl_addsubact(a, submenu2);

  submenu3=a=pnl_mkact(pnl_sub_menu);
  a->label="thirdly";
  pnl_addsubact(submenu3, menu);

  amenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="1";
  pnl_addsubact(a, submenu3);

  bmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="2";
  pnl_addsubact(a, submenu3);

  cmenuitem=a=pnl_mkact(pnl_menu_item);
  a->label="3";
  pnl_addsubact(a, submenu3);

  pnl_fixact(menu);

  return p;
}

