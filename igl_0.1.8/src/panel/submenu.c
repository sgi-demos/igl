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
#include <panel.h>

void
_addsubsubmenu(sa, a)
     Actuator *sa, *a;
{
  if (!a->ca) {
    a->na++;
    sa->pa=a;
  } else (*a->ca->addsubfunc)(sa, a->ca);  /* a->ca is the submenu's menu */
}

void
_add_submenu(a)
     Actuator *a;
{
  Actuator *sa;
  extern void _addicon();

  _addicon(a);

  sa=pnl_mkact(pnl_menu);
  sa->label=a->label;
  pnl_addsubact(sa, a);

  a->al=sa;

  a->addsubfunc=_addsubsubmenu;  /* subsequent calls to pnl_addsubact will */
				 /* add sub acts to the menu, not the icon */
}

void
_fixsubmenu(a)
     Actuator *a;
{
  Icon *ad=(Icon *)a->data;
  extern void _fixicon();

  _fixicon(a);
  ad->xopen=a->x+ad->wstowed/3.0;
}

void
pnl_sub_menu(a)
Actuator *a;
{
  pnl_icon(a);

  a->type=PNL_SUB_MENU;
  a->fixfunc=_fixsubmenu;
  a->addfunc=_add_submenu;
}

