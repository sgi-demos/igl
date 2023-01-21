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

void
_addmenuitem(a, val)
     Actuator *a;
     float val;
{
  Menu *ad=(Menu *)a->data;
}

void
_deletemmenuitem(a, sa)
     Actuator *a;
     Actuator *sa;
{
  Menu *ad=(Menu *)a->data;
}

/*
 * newvalmenu
 *
 * logic:
 *   if no entries, return
 *   if justdown, unset any picked items
 *   if !active, set selected item (if any) inactive, and call its newvalfunc
 *   pick new current item
 *   if hit again on current item, or on its selection, call its newvalfunc,
 *   else
 *     set old current item inactive
 *     call its newvalfunc
 *     if (newitem not null)
 *       set newitem active
 *       call its newvalfunc
 *     update old item (to new item)
 *     set new val for menu
 *
 */

void
_newvalmenu(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  Menu *ad=(Menu *)a->data;
  Actuator *sa, *sb;

  if (!a->al) return;

  if (pnl_justdown) {
    a->ca=NULL;
    pnl_setdirty(a);
  }

  if (!a->active) {
    if (a->ca) {
      a->ca->active=FALSE;
      pnl_newvalact(a->ca,p,x-a->x,y-a->y);
      return;				/* should this... */  /* XXX */
    }
    pnl_setdirty(a);			/*    ...be here? */
  }
  
  if ((y>a->y+a->h-ad->th)||(y<a->y)||x<a->x||x>a->x+a->w) sa=NULL;
  else 
    for (sb=sa=a->al;sa;sa=sa->next)
      if (sa->pickfunc(sa, a->p, x-a->x, y-a->y)) break;

  if (a->ca) /* in the same menu choice */
    if (sa==a->ca||a->ca->pickfunc(a->ca, a->p, x-a->x, y-a->y)) {
      pnl_newvalact(a->ca,p,x-a->x,y-a->y);
      return;
    }

  if (a->ca) {
    a->ca->active=FALSE;
    pnl_newvalact(a->ca,p,x-a->x,y-a->y);
  }
  
  if (sa) {
    sa->active=TRUE;
    pnl_newvalact(sa,p,x-a->x,y-a->y);
  }
  
  a->ca=sa;

  if (a->ca) 
    for (a->val=a->na-1,sa=a->al;sa!=a->ca;a->val-=1,sa=sa->next);
  else
    a->val= -1;
}

void
_newvalmenuitem(a,p,x,y)
     Actuator *a;
     Panel *p;
     Coord x, y;
{
  if (!a->active) a->val=0.0;
  else 		  a->val=1.0;
  pnl_setdirty(a);
}

void
_fixmenu(a)
     Actuator *a;
{
  Actuator *sa;
  Menu *ad=(Menu *)a->data;

  for (sa=a->al;sa;sa=sa->next) {
    pnl_fixact(sa);
    a->w=MAX(a->w, sa->w);
  }
}

void
_addmenu(a)
     Actuator *a;
{
  Actuator *sa;
  Menu *ad=(Menu *)a->data;

  if (a->label) a->h+=ad->th;
  a->w=MAX(a->w, pnl_strwidth(a->p, a->label)+2*PNL_DIM_2);
}

void
_drawmenu(a, p)
Actuator *a;
Panel *p;
{
  Coord y;
  Menu *ad=(Menu *)a->data;
  Actuator *sa;

  for (sa=a->al;sa;sa=sa->next)
    sa->dirtycnt=MAX(sa->dirtycnt, a->dirtycnt);

  pushmatrix();
  translate(a->x,a->y,0.0);

  if (a->dirtycnt) {
    color(pnl_background_color);
    rectf(0.0,0.0,a->w,a->h);
    if (!a->beveled) {
      color(pnl_black_color);
      rect(0.0,0.0,a->w,a->h);
    }
 
    if (a->label) {
      color(pnl_label_color);
      move2(0.0, a->h-ad->th);
      rdr2(a->w, 0.0);
      cmov2((a->w-a->lw)/2.0, a->h-a->lh);
      charstr(a->label);
    }

    pushmatrix();
    translate(-a->x,-a->y,0.0);
    if (a->beveled) pnl_drawbevel(a, p);
    popmatrix();
  }

  for (sa=a->al;sa;sa=sa->next) pnl_drawact(sa, a->p);

  popmatrix();
}

void
_addsubmenu(sa, a)
     Actuator *sa, *a;
{
  Menu *ad=(Menu *)a->data;
  Actuator *sb, *sc;
  Coord y;

  a->na++;
  sa->pa=a;

  sa->next=a->al;
  a->al=sa;

  a->w=MAX(a->w,sa->w+2*PNL_DIM_2);
  a->h+=sa->h+PNL_DIM_2;
  y=PNL_DIM_2;

  sa->x=PNL_DIM_2;

  for (sb=a->al;sb;sb=sb->next) {
    sb->w=a->w-2*PNL_DIM_2;
    sb->y=y;
    y+=sb->h+PNL_DIM_2;
    pnl_fixact(sb);
  }
}

void
pnl_menu(a)
Actuator *a;
{
  Menu *ad;

  a->type=PNL_MENU;

  a->data = (char *) pnl_alloc(sizeof(Menu));
  a->datasize = sizeof(Menu);
  ad=(Menu *)a->data;
  ad->th=PNL_MENU_TITLE_HEIGHT;

  a->h=PNL_DIM_2;
  a->w=PNL_MENU_WIDTH;

  a->labeltype=PNL_LABEL_CENTER;

  a->addfunc=_addmenu;
  a->fixfunc=_fixmenu;
  a->addsubfunc=_addsubmenu; /* to add a sub act to this act */
  a->newvalfunc=_newvalmenu;
  a->drawfunc=_drawmenu;
}

void
pnl_menu_item(a)
Actuator *a;
{
  pnl_wide_button(a);

  a->type=PNL_MENU_ITEM;
  a->newvalfunc=_newvalmenuitem;
}

