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

main() 
{
  foreground();
  noport();
  winopen("demo");
  defpanel();

  for (;;) {
    pnl_dopanel();
    swapbuffers();
  }
}

void updatevaldisplay(a)
Actuator *a;
{
  Actuator *target=(Actuator *)a->u;

  sprintf(target->label, "%f", a->val);
  pnl_fixact(target);
}

defpanel()
{
  Panel *p;
  Actuator *a, *b;
  Dial *dd;
  float x, y;
  
  p=pnl_mkpanel();
  p->label="two dials";
  
  b=pnl_mkact(pnl_label);
  b->label=(char *)pnl_alloc(20);
  b->x=x+1;
  b->y=y-1;
  pnl_addact(b, p);

  a=pnl_mkact(pnl_dial);
  a->label="normal";
  a->x=x;
  a->y=y;
  a->w=a->h=3;
  a->activefunc=updatevaldisplay;
  a->u=(char *)b;
  pnl_addact(a, p);
  
  x+=a->w+PNL_DIM_1;
  
  b=pnl_mkact(pnl_label);
  b->label=(char *)pnl_alloc(20);
  b->x=x+1;
  b->y=y-1;
  pnl_addact(b, p);

  a=pnl_mkact(pnl_dial);
  a->label="wrap";
  a->x=x;
  a->y=y;
  a->w=a->h=3;
  a->activefunc=updatevaldisplay;
  a->u=(char *)b;
  dd=(Dial *)a->data;
  dd->mode|=PNL_DM_WRAP;
/*dd->winds=1.0;		/* so it goes around exactly once */
  pnl_addact(a, p);
}

