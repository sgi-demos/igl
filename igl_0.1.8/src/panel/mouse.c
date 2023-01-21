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
#include <stdio.h>
#include <gl.h>
#include <panel.h>

void _newvalmouse(a,p,x,y)
Actuator *a;
Panel *p;
Coord x, y;
{
  Mouse *ad=(Mouse *)a->data;

  ad->x=pnl_mx;
  ad->y=pnl_my;

  if (a->active) a->val=1.0;
  else		 a->val=0.0;
}

void
_addmouse(a, p)
Actuator *a;
Panel *p;
{
  if (pnl_mouse_act) {
    (void) fprintf(stderr, "libpanel: warning, duplicate pnl_mouse actuator\n");
    return;
  }
  a->p=NULL;
  a->data=(char *)pnl_alloc(sizeof(Mouse));
  pnl_mouse_act=a;
}

void
pnl_mouse(a)
Actuator *a;
{
    a->type=PNL_MOUSE;

/*     a->automatic=TRUE; */
    a->visible=FALSE;
    a->newvalfunc=_newvalmouse;
    a->addfunc=_addmouse;
    a->drawfunc=NULL;
}

